
/*! \file Threading.cpp
    \brief This file contains implementation classes and types used to abstract operating system specific threads control.
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

#include "Threading.h"

namespace Threading
{

	// Initialize thread parameters
	GaThread::GaThread(const GaThreadParameter& parameters,
				   bool started)
	{
		LOCK_THIS_OBJECT( lock );

		_parameters = parameters;

		if( started )
		{
			#if defined(GAL_PLATFORM_WIN)
			
			_thread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunctionWraper, this, 0, &_id );
			
			#elif defined(GAL_PLATFORM_NIX)
			
			if( !pthread_create( &_thread, NULL, (void* (*)( void* ))ThreadFunctionWraper, this ) )
				_id = _thread;
			else
				_id = _thread = 0;
			
			#endif

			// set status of thread
			_status = _thread ? GATS_RUNNING : GATS_STOPPED;

			return;
		}
		
		// stopped status
		_status = GATS_STOPPED;
		_thread = 0;
	}

	// Frees aquired resources.
	GaThread::~GaThread()
	{
		Abort();
	}

	// Starts thread if it is not running.
	// Returns TRUE if thread is started.
	bool GaThread::Start()
	{
		LOCK_THIS_OBJECT( lock );

		// thread not running?
		if( _status == GATS_RUNNING )
			return false;

		switch( _status )
		{
			// thread was stopped
		case GATS_STOPPED:

			#if defined(GAL_PLATFORM_WIN)
			
			_thread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunctionWraper, this, 0, &_id );
			
			#elif defined(GAL_PLATFORM_NIX)
			
			if( !pthread_create( &_thread, NULL, (void* (*)( void* ))ThreadFunctionWraper, this ) )
				_id = _thread;
			else
				_id = _thread = 0;
			
			#endif

			// thread started successafully?
			if( _thread )
				_status = GATS_RUNNING;

			return _thread != 0;

			// thread was paused
		case GATS_PAUSED:

			bool resumed = false;

			#if defined(GAL_PLATFORM_WIN)
			
			resumed = ResumeThread( _thread ) != -1;
			
			#endif
			
			// resumed successafully?
			if( resumed )
				_status = GATS_RUNNING;
			return resumed;

		}
		return false;
	}

	// Temproary pause the execution of the thread and saves current state.
	// Returns TRUE if execution is paused.
	bool GaThread::Pause()
	{
		LOCK_THIS_OBJECT( lock );

		// thread exists and running?
		if( !_thread || _status != GATS_RUNNING )
			return false;

		bool paused = false;

		#if defined(GAL_PLATFORM_WIN)
		
		paused = SuspendThread( _thread ) != -1;
		
		#endif

		// resumed successafully?
		if( paused )
			_status = GATS_PAUSED;

		return paused;
	}

	// Stop the execution of the thread and discarge current state.
	// Returns TRUE if execution is stopped.
	bool GaThread::Abort()
	{
		LOCK_THIS_OBJECT( lock );

		// thread exists and running?
		if( !_thread || _status == GATS_STOPPED )
			return false;

		bool stopped;

		#if defined(GAL_PLATFORM_WIN)
		
		// is thread terminates itself?
		if( GetCurrentThreadId() == _id )
		{
			CloseHandle( _thread );

			_status = GATS_STOPPED;
			_thread = NULL;
			_id = 0;

			ExitThread( 0 );
		}
		else
		{
			stopped = TerminateThread( _thread, 0 ) == TRUE;

			if( stopped )
				CloseHandle( _thread );
		}
		
		#elif defined(GAL_PLATFORM_NIX)
		
		if( !pthread_equal( pthread_self(), _thread ) )
		{
			_status = GATS_STOPPED;
			_id = _thread = 0;
			
			pthread_exit( 0 );
		}
		else
			stopped = !pthread_cancel( _thread );
		
		#endif

		// stopped successafully?
		if( stopped )
		{
			_status = GATS_STOPPED;
			_thread = 0;
			_id = 0;
		}

		return stopped;
	}

	// Wait for thread to finish
	bool GaThread::Join()
	{
		LOCK_THIS_OBJECT( lock );

		SystemThread old = _thread;

		UNLOCK( lock );

		#if defined(GAL_PLATFORM_WIN)

		return WaitForSingleObject( old, INFINITE ) == 0;

		#elif defined(GAL_PLATFORM_NIX)
		
		return !pthread_join( old, NULL );
		
		#endif
	}

	// Wraper for thread function
	ThreadFunctionReturn GaThread::ThreadFunctionWraper(GaThread* thread)
	{
		// run thread's function
		ThreadFunctionReturn ret = thread->_parameters._functionPointer( 
			thread, thread->_parameters._functionParameters );

		// set stopped status
		LOCK_OBJECT( lock, thread );
		thread->_status = GATS_STOPPED;

		return ret;
	}

} // Threading
