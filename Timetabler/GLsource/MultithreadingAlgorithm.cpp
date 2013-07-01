
/*! \file MultithreadingAlgorithm.cpp
\brief This file implements methods of abstract class that represents multithreaded genetic algorithms.
*/

/*
* 
* website: N/A
* contact: kataklinger@gmail.com
*
*/

/*
* Genetic Algorithms Library
* Copyright (C) 2007-2012 Mladen Jankovic
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
*/

#include "MultithreadingAlgorithm.h"

namespace Algorithm
{

	// Initialize algorithm
	GaMultithreadingAlgorithm::GaMultithreadingAlgorithm(const GaMultithreadingAlgorithmParams& parameters) : _parametersChange(false),
		_workerIdCounter(0)
	{
		_numberOfThreads = parameters.GetNumberOfWorkers() + 1;

		// make thread pool
		_threads = new GaThread*[ _numberOfThreads ];

		// defalut thread parameters
		GaThreadParameter p;
		p._functionParameters = this;

		// initialize control and worker threads
		for( int i = 0; i < _numberOfThreads; i++ )
		{
			p._functionPointer = !i ? ControlFlowWrapper : WorkFlowWrapper;

			_threads[ i ] = new GaThread( p, false );
		}

		// initialize of semaphores for synchronization of worker threads
		MakeSemaphore( _workerForkSync, _numberOfThreads - 1, 0 );
		MakeSemaphore( _workerJoinSync, _numberOfThreads - 1, 0 );
		MakeEvent( _controlSync, 0 ); 
	}

	// Frees resources
	GaMultithreadingAlgorithm::~GaMultithreadingAlgorithm()
	{
		// free resources used by threads
		if( _threads )
		{
			for( int i = 0; i < _numberOfThreads; i++ )
			{
				if( _threads[ i ] )
					delete _threads[ i ];
			}

			// free thread pool
			delete[] _threads;
		}

		// free resources used by synchronization
		DeleteSemaphore( _workerForkSync );
		DeleteSemaphore( _workerJoinSync );
		DeleteEvent( _controlSync );
	}

	// Sets new parameters for algorithm
	void GaMultithreadingAlgorithm::SetAlgorithmParameters(const GaAlgorithmParams& parameters)
	{
		int newCount = ( (const GaMultithreadingAlgorithmParams&)parameters ).GetNumberOfWorkers();

		// state of algorithm couldnot be changed during parameter change
		BlockStateChange();

		int oldCount = _numberOfThreads - 1;

		// nothing changed?
		if( oldCount == newCount )
		{
			// now it safe to change state of algorithm
			ReleaseStateChange();
			return;
		}

		// if algorithm is running - stop all worker threads
		if( _state == GAS_RUNNING )
		{
			_parametersChange = true;

			// release working threads
			_workersThreadOut = _workersThreadIn = oldCount;
			UnlockSemaphore( _workerForkSync, oldCount );

			// wait for working threads to be closed
			for( int i = 1; i <= oldCount; i++ )
				_threads[ i ]->Join();

			_parametersChange = false;
		}

		// remove synchronization objects
		DeleteSemaphore( _workerForkSync );
		DeleteSemaphore( _workerJoinSync );

		// make new synchronization object
		MakeSemaphore( _workerForkSync, newCount, 0 );
		MakeSemaphore( _workerJoinSync, newCount, 0 );

		// new thread pool
		GaThread** newThreads = new GaThread*[ newCount + 1 ];

		// copy old needed threads
		int lim = min( oldCount, newCount ) + 1;
		for( int i = 0; i < lim; i++ )
			newThreads[ i ] = _threads[ i ];

		if( oldCount < newCount )
		{
			// new threads should be added

			// new worker threads' parameters
			GaThreadParameter p;
			p._functionPointer = WorkFlowWrapper;
			p._functionParameters = this;

			// make new threads
			for( int i = newCount - oldCount + 1; i < newCount; i++ )
				newThreads[ i ] = new GaThread( p, false );
		}
		else
		{
			// threads should be remove

			// free old threads
			for( int i = oldCount - newCount + 1; i < oldCount; i++ )
				delete _threads[ i ];
		}

		// swap pool of threads
		delete[] _threads;
		_threads = newThreads;
		_numberOfThreads = newCount + 1;

		// restart working threads if they were running
		if( _state == GAS_RUNNING )
		{
			for( int i = 1; i <= newCount; i++ )
				_threads[ i ]->Start();
		}

		// now it safe to change state of algorithm
		ReleaseStateChange();
	}


	// Waits for threads to finish
	bool GaMultithreadingAlgorithm::WaitForThreads()
	{
		for( int i = 0; i <= ( (const GaMultithreadingAlgorithmParams&) GetAlgorithmParameters() ).GetNumberOfWorkers(); i++ )
			_threads[ i ]->Join();

		return true;
	}

	// Control flow of evolution
	void GaMultithreadingAlgorithm::ControlFlow()
	{
		while( 1 )
		{
			// now parameters and state cannot be changed
			BlockParameterChanges();
			BlockStateChange();

			int count = ( (const GaMultithreadingAlgorithmParams&) GetAlgorithmParameters() ).GetNumberOfWorkers();

			if( _state == GAS_RUNNING )
				// execute control step before workers
					BeforeWorkers();

			// release working threads
			_workersThreadOut = _workersThreadIn = count;
			UnlockSemaphore( _workerForkSync, count );

			// wait for working threads to finish the job
			WaitForEvent( _controlSync );

			// still running?
			if( _state != GAS_RUNNING )
			{
				// stop the threads

				// now it safe to change parameters and state
				ReleaseParameterChanages();
				ReleaseStateChange();

				break;
			}

			// execute control step after workers
			AfterWorkers();

			// stop algorithm is criteria is reached
			CheckStopCriteria();

			// now it safe to change parameters and state
			ReleaseStateChange();
			ReleaseParameterChanages();
		}
	}

	// Working flow of evolution
	void GaMultithreadingAlgorithm::WorkFlow()
	{
		// give ID to worker thread
		int workerId = ATOMIC_INC( _workerIdCounter ) - 1;

		while( 1 )
		{
			// wait for command from control thread
			LockSemaphore( _workerForkSync );

			// stop the thread to apply parameter change
			if( _parametersChange )
				break;

			// execute work step if the algorithm is not stopped
			if( _state == GAS_RUNNING )
				WorkStep( workerId );

			// only the last worker will releast the others to continue
			if( !ATOMIC_DEC( _workersThreadIn ) )
				UnlockSemaphore( _workerJoinSync,  _numberOfThreads - 1 );

			// wait for the last worker to reach this point before notifying control thread
			LockSemaphore( _workerJoinSync );

			// the last worker thread to exit notifies control thread that work step is done
			if( !ATOMIC_DEC( _workersThreadOut ) )
				SignalEvent( _controlSync );

			// algorithm is stopped
			if( _state != GAS_RUNNING )
				break;
		}
	}

	// Starts control and working threads.
	// Returns TRUE if all threads are started successafully.
	bool GaMultithreadingAlgorithm::StartThreads()
	{
		_workerIdCounter = 0;

		// start control and worker threads
		for( int i = 0; i <= ( (const GaMultithreadingAlgorithmParams&) GetAlgorithmParameters() ).GetNumberOfWorkers(); i++ )
		{
			if( !_threads[ i ]->Start() )
				return false;
		}

		return true;
	}

	// Wraper method of control flow method for thread starting
	ThreadFunctionReturn GaMultithreadingAlgorithm::ControlFlowWrapper(GaThread* thread,
		void* params)
	{
		( ( GaMultithreadingAlgorithm* ) params )->ControlFlow();

		return 0;
	}

	// Wraper method of work flow method for thread starting
	ThreadFunctionReturn GaMultithreadingAlgorithm::WorkFlowWrapper(GaThread* thread,
		void* params)
	{
		( ( GaMultithreadingAlgorithm* ) params )->WorkFlow();

		return 0;
	}

} // Algorithm
