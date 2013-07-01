
/*! \file MultithreadingAlgorithm.h
    \brief This file contains declaration of abstract class that represents multithreaded genetic algorithms. It also declares paremters' calss for the algorithm.
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

#ifndef __GA_MULTITHREADING_ALGORITHM_H__
#define __GA_MULTITHREADING_ALGORITHM_H__

#include "Platform.h"
#include "Algorithm.h"

namespace Algorithm
{
	/// <summary>This class should be used (directly or by inheritance) by genetic algorithms which use multiple threads for parallel execution of work.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaMultithreadingAlgorithmParams : public GaAlgorithmParams
	{

	private:

		/// <summary>Number of working threads which should be used by genetic algorithm.</summary>
		int _numberOfWorkers;

	public:

		/// <summary>This constructor initializes parameters for user-defined number of working threads.</summary>
		/// <param name="numberOfWorkers">number of working threads.</param>
		GaMultithreadingAlgorithmParams(int numberOfWorkers) : _numberOfWorkers(numberOfWorkers) { }

		/// <summary>Default number of working threads is 1.</summary>
		GaMultithreadingAlgorithmParams() : _numberOfWorkers(1) { }

		/// <summary>More details are given in specification of <see cref="GaParameters::Clone" /> method.
		///
		/// This method is not thread-safe.</summary>
		virtual GaParameters* GACALL Clone() const { return new GaMultithreadingAlgorithmParams( *this ); }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns number of working threads should be used by algorithm.</returns>
		inline int GACALL GetNumberOfWorkers() const { return _numberOfWorkers; }

		/// <summary><c>SetNumberOfWorkers</c> method sets number of working threads which should be used by genetic algorithm.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="workers">number of working threads.</param>
		inline void GACALL SetNumberOfWorkers(int workers) { _numberOfWorkers = workers; }

	};// END CLASS DEFINITION GaMultithreadingAlgorithmParams

	/// <summary>This class implements features to allow parallel execution of some operations of genetic algorithm. It runs multiple threads
	/// which execute work which can be done in parallel, one control thread which controls workers and executes all operations which must be
	/// executed sequentially. 
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class,
	/// but all public method and operators are thread-safe.</summary>
	class GaMultithreadingAlgorithm : public GaBaseAlgorithm
	{

	protected:

		/// <summary>Object for synchronization of working step start.</summary>
		SysSemaphoreObject _workerForkSync;

		/// <summary>Object for synchronization of working step finish.</summary>
		SysSemaphoreObject _workerJoinSync;

		/// <summary>Object for synchronization of worker threads with control thread after end of work step</summary>
		SysEventObject _controlSync;

		/// <summary>Number of worker threads currently executing working section of code.</summary>
		int _workersThreadIn;

		/// <summary>Number of worker threads that have to exit working section before control thread can continue execution.</summary>
		int _workersThreadOut;

		/// <summary>This flag indicates that user has changed some aspect of the algorithm, but algorithm didn't refresh its control structures</summary>
		bool _parametersChange;

		/// <summary>Number of threads which are used by algorithm (working and control threads).</summary>
		int _numberOfThreads;

		/// <summary>This attribute is used for assigning identifiers to working threads when algorithm starts execution.</summary>
		volatile int _workerIdCounter;

		/// <summary>Array of threads which are used by the algorithm (working and control threads).</summary>
		GaThread** _threads;

	public:

		/// <summary>This constructor initializes control structures with user-defined parameters.</summary>
		/// <param name="parameters">reference to algorithm parameters.</param>
		GAL_API
		GaMultithreadingAlgorithm(const GaMultithreadingAlgorithmParams& parameters);

		/// <summary>Destructors releases acquired resources such as threads and synchronization objects and memory used by control structures.</summary>
		GAL_API
		virtual ~GaMultithreadingAlgorithm();

		/// <summary>This method should be called from classes that inherit this class when user changes algorithm parameters.
		/// It updates structures which control multithreading execution.
		///
		/// More details are given in specification of <see cref="GaAlgorithm::SetAlgorithmParameters" /> method.
		///
		/// This method is thread-safe.</summary>
		GAL_API
		virtual void GACALL SetAlgorithmParameters(const GaAlgorithmParams& parameters);

		/// <summary>This method waits for control and working thread to finish their execution.</summary>
		/// <returns>Method returns <c>true</c> if ther were no errors during waiting.</returns>
		GAL_API
		virtual bool GACALL WaitForThreads();

	protected:

		/// <summary>This method is called before workers execute their steps. It is executed in control thread context.</summary>
		virtual void GACALL BeforeWorkers() { }

		/// <summary>After control thread executes <see cref="BeforeWork" /> method, working threads starts execution and steps in this method.
		/// Working threads get their identification numbers which are assigned sequentially to them.</summary>
		/// <param name="workerId">identification number of working thread</param>
		virtual void GACALL WorkStep(int workerId) { }

		/// <summary>This method is called after workers finish their steps. It is executed in control thread context.</summary>
		virtual void GACALL AfterWorkers() { }

		/// <summary>This method starts control and working threads.</summary>
		virtual bool GACALL OnStart() { return StartThreads(); }

		/// <summary>Empty method imeplmentation. Always returns <c>true</c>.</summary>
		virtual bool GACALL OnStop() { return true; }

		/// <summary>Empty method imeplmentation. Always returns <c>true</c>.</summary>
		virtual bool GACALL OnPause() { return true; }

		/// <summary>This method starts control and working threads.</summary>
		virtual bool GACALL OnResume() { return StartThreads(); }

		/// <summary>This method implements work flow of control thread.</summary>
		GAL_API
		virtual void GACALL ControlFlow();

		/// <summary>This method implements work flow of working thread.</summary>
		GAL_API
		virtual void GACALL WorkFlow();

		/// <summary>This method starts control and working threads.</summary>
		/// <returns>Method returs <c>true</c> if all threads are started successafully.</returns>
		GAL_API
		virtual bool GACALL StartThreads();

	private:

		/// <summary>This method is used as wrapper for control flow method and is used when algorithm starts control thread.</summary>
		/// <param name="thread">pointer to control thread.</param>
		/// <param name="params">pointer to parameters which are passed by algorithm to the control thread.</param>
		/// <returns>Returns status of the thread when it finishes its execution.</returns>
		static ThreadFunctionReturn GACALL ControlFlowWrapper(GaThread* thread,
			void* params);

		/// <summary>This method is used as wrapper for working flow method and is used when algorithm starts working thread.</summary>
		/// <param name="thread">pointer to working thread.</param>
		/// <param name="params">pointer to parameters which are passed by algorithm to the working thread.</param>
		/// <returns>Returns status of the thread when it finishes its execution.</returns>
		static ThreadFunctionReturn GACALL WorkFlowWrapper(GaThread* thread,
			void* params);

	};// END CLASS DEFINITION GaMultithreadingAlgorithm

} // Algorithm

#endif // __GA_MULTITHREADING_ALGORITHM_H__
