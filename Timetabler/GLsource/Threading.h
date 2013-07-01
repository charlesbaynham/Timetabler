
/*! \file Threading.h
    \brief This file contains declaration of classes and types used to abstract operating system specific threads control.
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

#ifndef __GA_THREADING_H__
#define __GA_THREADING_H__

#include "Platform.h"

#if defined(GAL_PLATFORM_WIN)

	#include <windows.h>

	#if defined(GAL_SYNTAX_MSVC) && !defined(GAL_SYNTAX_INTL)

		#include <intrin.h>

	#endif

#elif defined(GAL_PLATFORM_NIX)

	#include <pthread.h>
	#include <semaphore.h>

	#if defined(GAL_PLATFORM_MACOS)

		#include <stdio.h>
		#include <string.h>

		#include <libkern/OSAtomic.h>
		#include <sys/types.h>
		#include <unistd.h>

	#endif

	#if defined(GAL_PLATFORM_SOL)

		#include <atomic.h>

	#endif

#endif

/// <summary>Contatins classes and types used to abstract operating system specific threads control.</summary>
namespace Threading
{

	#ifdef __GAL_DOCUMENTATION__

		/// <summary>This type defines system specific type for storing threads objects or handles to them.</summary>
		typedef _SYSTEM_OR_COMPILER_SPECIFIC_ SystemThread;

		/// <summary>This type defines system specific type for storing synchronization objects or handles to them.
		/// System specific synchronization is wrapped by <see cref="GaCriticalSection" /> class.</summary>
		typedef _SYSTEM_OR_COMPILER_SPECIFIC_ SysSyncObject;

		/// <summary>This type defines system specific type for storing semaphores objects or handles to them.
		/// Manipulation over semaphores is done by <c>MAKE_SEMAPHORE</c>, <c>FREE_SEMAPHORE</c>, <c>LOCK_SEMAPHORE</c> and 
		/// <c>UNLOCK_SEMAPHORE</c> macros.</summary>
		typedef _SYSTEM_OR_COMPILER_SPECIFIC_ SysSemaphoreObject;

		/// <summary>This type defines system specific type for storing events objects or handles to them.
		/// Manipulation over events is done by <c>MAKE_EVENT</c>, <c>FREE_EVENT</c>, <c>WAIT_FOR_EVENT</c> and 
		/// <c>SIGNAL_EVENT</c> macros.</summary>
		typedef _SYSTEM_OR_COMPILER_SPECIFIC_ SysEventObject;

		/// <summary>This type is used as return value type for functions which are used as threads' entry points.
		/// This type hides system specific types which are used for the purpose. </summary>
		typedef _SYSTEM_OR_COMPILER_SPECIFIC_ ThreadFunctionReturn;

		/// <summary>Variables/objects of this type are used for storing threads' IDs. This type hides system specific
		/// types which are used for the purpose.</summary>
		typedef _SYSTEM_OR_COMPILER_SPECIFIC_ ThreadID;

		/// <summary><c>ATOMIC_INC</c> macro atomically increments <c>VALUE</c> by one and returns new value.</summary>
		/// <param name="VALUE">variable which is incremented.</param>
		#define ATOMIC_INC(VALUE) _SYSTEM_OR_COMPILER_SPECIFIC_

		/// <summary><c>ATOMIC_DEC</c> macro atomically decrements <c>VALUE</c> by one and returns new value.</summary>
		/// <param name="VALUE">variable which is decremented.</param>
		#define ATOMIC_DEC(VALUE) _SYSTEM_OR_COMPILER_SPECIFIC_

		/// <summary><c>SPINLOCK</c> macro defines mechanism of spinlock.</summary>
		/// <param name="LOCK">variable that is useed as lock.</param>
		#define SPINLOCK(LOCK) _SYSTEM_OR_COMPILER_SPECIFIC_

	#endif

	#if defined(GAL_PLATFORM_WIN)

		#if defined(GAL_SYNTAX_MSVC) && !defined(GAL_SYNTAX_INTL)

			#define ATOMIC_INC(VALUE) _InterlockedIncrement( (volatile long*)&VALUE )
			#define ATOMIC_DEC(VALUE) _InterlockedDecrement( (volatile long*)&VALUE )
			#define SPINLOCK(LOCK) while( _InterlockedExchange( (volatile long*)&LOCK, 1 ) )

		#else

			#define ATOMIC_INC(VALUE) InterlockedIncrement( (LONG*)&VALUE )
			#define ATOMIC_DEC(VALUE) InterlockedDecrement( (LONG*)&VALUE )
			#define SPINLOCK(LOCK) while( InterlockedExchange( (LONG*)&LOCK, 1 ) )

		#endif

	#elif defined(GAL_PLATFORM_MACOS)

		#define ATOMIC_INC(VALUE) OSAtomicIncrement32( (int32_t*)&VALUE )
		#define ATOMIC_DEC(VALUE) OSAtomicDecrement32( (int32_t*)&VALUE )
		#define SPINLOCK(LOCK) while( !OSAtomicCompareAndSwap32( 0, 1, (int32_t*)&LOCK ) )

	#elif defined(GAL_PLATFORM_SOL)

		#define ATOMIC_INC(VALUE) atomic_inc_32_nv( (volatile uint32_t*)&VALUE )
		#define ATOMIC_DEC(VALUE) atomic_dec_32_nv( (volatile uint32_t*)&VALUE )
		#define SPINLOCK(LOCK) while( atomic_swap_32( (volatile uint32_t*)&LOCK, 1 ) )

	#elif defined(GAL_PLATFORM_NIX)

		#if defined(GAL_SYNTAX_INTL)

			#define ATOMIC_INC(VALUE) _InterlockedIncrement( (int*)&VALUE )
			#define ATOMIC_DEC(VALUE) _InterlockedDecrement( (int*)&VALUE )
			#define SPINLOCK(LOCK) while( _InterlockedExchange( (int*)&LOCK, 1 ) )

		#else

			#define ATOMIC_INC(VALUE) __sync_add_and_fetch( (long volatile*)&VALUE, 1L )
			#define ATOMIC_DEC(VALUE) __sync_sub_and_fetch( (long volatile*)&VALUE, 1L )
			#define SPINLOCK(LOCK) while( !__sync_val_compare_and_swap( (long volatile*)&LOCK, 0, 1 ) )

		#endif
	
	#endif


	#if defined(GAL_PLATFORM_WIN)
	
		typedef HANDLE SystemThread;
		typedef DWORD ThreadID;
		typedef int ThreadFunctionReturn;

		typedef CRITICAL_SECTION SysSyncObject;
		typedef HANDLE SysSemaphoreObject;
		typedef HANDLE SysEventObject;

	#elif defined(GAL_PLATFORM_NIX)
	
		typedef pthread_t SystemThread;
		typedef pthread_t ThreadID;

		typedef void* ThreadFunctionReturn;

		typedef pthread_mutex_t SysSyncObject;

		#if defined(GAL_PLATFORM_MACOS)

			typedef sem_t* SysSemaphoreObject;
			typedef sem_t* SysEventObject;

		#else

			typedef sem_t SysSemaphoreObject;
			typedef sem_t SysEventObject;

		#endif
	
	#endif

	/// <summary>This function is used to create operating system object for semaphore and to initialize it.</summary>
	/// <param name="semaphoreHandle">variable which will store handle to newly created semaphore.</param>
	/// <param name="maxCount">maximum count of semaphore, value must be greater then 0. On *nix systems this parameter is ignored.</param>
	/// <param name="initialCount">initial count of semaphore, value must be greater or equals to the 0 and less then or equals to <c>maxCount</c>.</param>
	/// <returns>Returns <c>true</c> if the semaphore is successfully created.</returns>
	inline bool MakeSemaphore(SysSemaphoreObject& semaphoreHandle, int maxCount, int initialCount)
	{
		#if defined(GAL_PLATFORM_WIN)

		return ( semaphoreHandle = CreateSemaphore( NULL, initialCount, maxCount, NULL ) ) != NULL;

		#elif defined(GAL_PLATFORM_MACOS)

		static int sem_counter = 0;
		char sem_name[ 40 ];
		strcpy( sem_name, "/tmp/gal_sem" );
		sprintf( sem_name + 12, "_%d", getpid() );
		sprintf( sem_name + strlen( sem_name ), "_%d", ATOMIC_INC( sem_counter ) );

		return ( semaphoreHandle = sem_open( sem_name, O_CREAT, 0644, initialCount ) ) != (sem_t*)SEM_FAILED;

		#elif defined(GAL_PLATFORM_NIX)

		return sem_init( &semaphoreHandle, 0, initialCount ) == 0;

		#endif
	}

	/// <summary>This function is used to free operating system semaphore.</summary>
	/// <param name="semaphoreHandle">reference to variable that holds reference to semaphore.</param>
	/// <returns>Returns <c>true</c> if the semaphore is successfully freed.</returns>
	inline bool DeleteSemaphore(SysSemaphoreObject& semaphoreHandle)
	{
		#if defined(GAL_PLATFORM_WIN)

		return CloseHandle( semaphoreHandle ) == S_OK;

		#elif defined(GAL_PLATFORM_MACOS)

		return sem_destroy( semaphoreHandle ) == 0;

		#elif defined(GAL_PLATFORM_NIX)

		return sem_destroy( &semaphoreHandle ) == 0;
		
		#endif
	}

	/// <summary>This function is used to acquire access to critical section protected by semaphore.</summary>
	/// <param name="semaphoreHandle">reference to variable that holds reference to semaphore.</param>
	/// <returns>Returns <c>true</c> if the semaphore is successfully acquired.</returns>
	inline bool LockSemaphore(SysSemaphoreObject& semaphoreHandle)
	{
		#if defined(GAL_PLATFORM_WIN)

		return WaitForSingleObject( semaphoreHandle, INFINITE ) != WAIT_FAILED;

		#elif defined(GAL_PLATFORM_MACOS)

		return sem_wait( semaphoreHandle ) == 0;

		#elif defined(GAL_PLATFORM_NIX)

		return sem_wait( &semaphoreHandle ) == 0;

		#endif
	}

	/// <summary>This function is used to release access to critical section protected by semaphore.</summary>
	/// <param name="semaphoreHandle">reference to variable that holds reference to semaphore.</param>
	/// <param name="count">amount by which semaphore's count is increased, value must be grater then 0 and sum
	/// of the value and current semaphore's count must be less then or equals to maximal count of semaphore.</param>
	/// <returns>Returns <c>true</c> if the semaphore is successfully released.</returns>
	inline bool UnlockSemaphore(SysSemaphoreObject& semaphoreHandle, int count)
	{
		#if defined(GAL_PLATFORM_WIN)
		
		return ReleaseSemaphore( semaphoreHandle, count, NULL ) != FALSE;

		#elif defined(GAL_PLATFORM_MACOS)

		for( ; count > 0; count-- )
		{
			if( sem_post( semaphoreHandle ) != 0 )
				return false;
		}

		return true;

		#elif defined(GAL_PLATFORM_NIX)

		for( ; count > 0; count-- )
		{
			if( sem_post( &semaphoreHandle ) != 0 )
				return false;
		}

		return true;
		
		#endif
	}

	/// <summary>This function is used to create operating system object for event and to initialize it.</summary>
	/// <param name="eventHandle">reference to variable which will store handle to newly created event.</param>
	/// <param name="intialState">initial state of event. Should be to <c>true</c> if event should be in signaled
	/// state after it is created, otherwise it should be set to <c>false</c>.</param>
	/// <returns>Returns <c>true</c> if the event is successfully created.</returns>
	inline bool MakeEvent(SysEventObject& eventHandle, bool intialState)
	{
		#if defined(GAL_PLATFORM_WIN)

		return ( eventHandle = CreateEvent( NULL, FALSE, intialState, NULL ) ) != NULL;

		#elif defined(GAL_PLATFORM_NIX)

		return MakeSemaphore( eventHandle, 1, intialState ? 1 : 0 );

		#endif
	}

	/// <summary>This function is used to free operating system semaphore.</summary>
	/// <param name="eventHandle">reference to variable that holds reference to event.</param>
	/// <returns>Returns <c>true</c> if the event is successfully freed.</returns>
	inline bool DeleteEvent(SysEventObject& eventHandle)
	{
		#if defined(GAL_PLATFORM_WIN)

		return CloseHandle( eventHandle ) == S_OK;

		#elif defined(GAL_PLATFORM_NIX)

		return DeleteSemaphore( (SysSemaphoreObject&)eventHandle );

		#endif
	}

	/// <summary>This function is used to block calling thread until event reaches signaled state.
	/// When calling thread is released, event is restared to non-signaled state.</summary>
	/// <param name="eventHandle">reference to variable that holds reference to event.</param>
	/// <returns>Returns <c>true</c> if the thread successfully received signal.</returns>
	inline bool WaitForEvent(SysEventObject& eventHandle)
	{
		#if defined(GAL_PLATFORM_WIN)

		return WaitForSingleObject( eventHandle, INFINITE ) != WAIT_FAILED;

		#elif defined(GAL_PLATFORM_NIX)

		return LockSemaphore( (SysSemaphoreObject&)eventHandle );

		#endif
	}

	/// <summary>This function is used to set event to signaled state.</summary>
	/// <param name="eventHandle">reference to variable that holds reference to event.</param>
	/// <returns>Returns <c>true</c> if the event is successfully signaled.</returns>
	inline bool SignalEvent(SysEventObject& eventHandle)
	{
		#if defined(GAL_PLATFORM_WIN)

		return SetEvent( eventHandle ) != FALSE;

		#elif defined(GAL_PLATFORM_NIX)

		return UnlockSemaphore( (SysSemaphoreObject&)eventHandle, 1 );

		#endif
	}

	/// <summary><c>DEFINE_SYNC_CLASS</c> macro inserts members to class which are needed to synchronize access to an object.
	/// Synchronization is don by using <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros.</summary>
	#define DEFINE_SYNC_CLASS	protected: mutable Threading::GaCriticalSection _synchronizator; \
								public: Threading::GaCriticalSection* GACALL GetSynchronizator() const \
											{ return &_synchronizator; }

	/// <summary>Macro is used to acquire access to critical section protected by synchronization object
	/// (<see cref="GaSectionLock" /> and <see cref="GaCriticalSection" />).</summary>
	/// <param name="LOCK_NAME">synchronization object.</param>
	#define LOCK(LOCK_NAME) ( LOCK_NAME ).Lock()

	/// <summary>Macro is used when thread exits critical section and releases access to synchronization object 
	/// (<see cref="GaSectionLock" /> and <see cref="GaCriticalSection" />).</summary>
	/// <param name="LOCK_NAME">synchronization object.</param>
	#define UNLOCK(LOCK_NAME) ( LOCK_NAME ).Unlock()
	
	/// <summary>Macro acquires access to an object with built-in synchronizator and prevents concurrent access.
	/// It instantiate <see cref="GaSectionLock" /> object with name lock and acquire access to the object, when
	/// execution leave the scope in which <c>LOCK_OBJECT</c> is specified, <see cref="GaSectionLock" /> object
	/// is destroyed and access to the locked object is released. Unlocking access to the object before leaving
	/// scope can be done by calling <c>UNLOCK(lock_name)</c> macro.</summary>
	/// <param name="LOCK_NAME">object which is synchronized.</param>
	/// <param name="OBJECT">name of <c>GaSectionLock</c> object.</param>
	#define LOCK_OBJECT(LOCK_NAME, OBJECT) Threading::GaSectionLock LOCK_NAME( ( OBJECT )->GetSynchronizator(), true )

	/// <summary>Macro acquires access to this and prevents concurrent access.
	/// It declares and instantiates <see cref="GaSectionLock" /> object with name lock and acquire access to
	/// this object, when execution leave the scope in which <c>LOCK_OBJECT</c> is specified,
	/// <see cref="GaSectionLock" /> object is destroyed and access to this object is released. Unlocking
	/// access to this before leaving scope can be done by calling <c>UNLOCK(lock_name)</c> macro.</summary>
	/// <param name="LOCK_NAME">name of <c>GaSectionLock</c> object.</param>
	#define LOCK_THIS_OBJECT(LOCK_NAME) Threading::GaSectionLock LOCK_NAME( &this->_synchronizator, true )

	class GaThread;

	/// <summary><c>ThreadFunctionPointer</c> is pointer to function used as thread's entry point. Entry point function must obey restriction of this type:
	/// <br>1. Function must return value of <see cref="ThreadFunctionReturn" /> type.
	/// <br>2. Function must use <c>GACALL</c> calling convention.
	/// <br>3. Function must have two parameters.
	/// <br>4. First parameter must be pointer <see cref="GaThread" /> (<see cref="GaThread" />*).
	/// <br>5. Second parameter must be pointer to <c>void</c> (<c>void</c>*).</summary>
	typedef ThreadFunctionReturn (GACALL *ThreadFunctionPointer)(GaThread*, void*);

	/// <summary><c>GaThreadParameter</c> structure contains information needed to start new thread.
	/// It has pointer to function which is entry point of thread and pointer to parameters which will be passed to the function.
	/// Restrictions of entry point's function of a thread are described by <c>GaThreadFunctionPointer</c> type.</summary>
	struct GaThreadParameter
	{

	public:

		/// <summary>Pointer to entry point's function of thread.</summary>
		ThreadFunctionPointer _functionPointer;

		/// <summary>Pointer to parameters which will be passed to the entry point's function.</summary>
		void* _functionParameters;

	};// END STRUCTURE DEFINITION GaThreadParameter

	/// <summary>This enumeration defines possible states of threads.</summary>
	enum GaThreadStatus
	{
		/// <summary>Thread is running.</summary>
		GATS_RUNNING = 0x1, 
		/// <summary>Thread execution is terminated, but thread object still exists and can be used to restart execution.</summary>
		GATS_STOPPED = 0x2, 
		/// <summary>Thread execution is temporary is suspended.</summary>
		GATS_PAUSED = 0x4, 
		/// <summary>Used for checking running state of thread.</summary>
		GATS_NOT_RUNNING = GATS_STOPPED | GATS_PAUSED
	};

	/// <summary><c>GaCriticalSection</c> class is wrapper class for system synchronization object.
	/// 
	/// This class provides basic synchronization and protection from concurrent access to the objects and resources.
	/// Built-in synchronizators used in other classes are instances of <c>GaCriticalSection</c> class. <c>LOCK</c> and
	/// <c>UNLOCK</c> macros can operate on instances of this class.</summary>
	class GaCriticalSection
	{

	private:

		/// <summary>System specific object used for synchronization of critical section.</summary>
		SysSyncObject _section;

	public:

		/// <summary>Constructor performs system specific initialization of synchronization object if needed.</summary>
		GaCriticalSection()
		{
			#if defined(GAL_PLATFORM_WIN)

			InitializeCriticalSection( (LPCRITICAL_SECTION)&_section );

			#elif defined(GAL_PLATFORM_NIX)
			
			pthread_mutex_init( &_section, NULL );
			
			#endif
		}

		/// <summary>Frees resources used by system synchronization object.</summary>
		~GaCriticalSection()
		{
			#if defined(GAL_PLATFORM_WIN)

			DeleteCriticalSection( (LPCRITICAL_SECTION)&_section );

			#elif defined(GAL_PLATFORM_NIX)
			
			pthread_mutex_destroy( &_section );
			
			#endif
		}

		/// <summary><c>Lock</c> method acquires synchronization object and prevents other thread to access protected section simultaneously.
		/// If another thread had already acquired synchronization object current thread is put to sleep, and is waked when synchronization
		/// object is released. </summary>
		inline void GACALL Lock()
		{
			#if defined(GAL_PLATFORM_WIN)

			EnterCriticalSection( (LPCRITICAL_SECTION)&_section );

			#elif defined(GAL_PLATFORM_NIX)
			
			pthread_mutex_lock( &_section );
			
			#endif
		}

		/// <summary><c>Unlock</c> method release synchronization object and wakes threads which was put to wait for release of the object.</summary>
		inline void GACALL Unlock()
		{
			#if defined(GAL_PLATFORM_WIN)

			LeaveCriticalSection( (LPCRITICAL_SECTION)&_section );

			#elif defined(GAL_PLATFORM_NIX)
			
			pthread_mutex_unlock( &_section );
			
			#endif
		}

	};// END CLASS DEFINITION GaCriticalSection

	/// <summary><c>GaSectionLock</c> class is used for automatic access control with help of <see cref="GaCriticalSection" /> class.
	/// Synchronization object can be automatically acquired when instance of <c>GaSectionLock</c> is created. If synchronization object
	/// is locked by instance of <c>GaSectionLock</c> it is released when the instance goes out of scope. This mechanism provides simple
	/// way of managing critical sections because users don't have to worry about releasing of synchronization object in most cases,
	/// but for more complicated cases <c>LOCK</c> and <c>UNLOCK</c> macros can be used with instances of this class or with 
	/// <see cref="GaCriticalSection" /> class. <c>GaSectionLock</c> is mainly employed by <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c>.
	///
	/// Object of this class shouldn't be used from multiple threads simultaneously.</summary>
	class GaSectionLock
	{

	private:

		/// <summary>Pointer to underlying synchronization object which is managed by this lock.</summary>
		GaCriticalSection* _section;

		/// <summary>Keeps track whether underlying synchronization object is locked by this instance of <c>GaSectionLock</c> class.</summary>
		bool _locked;

	public:

		/// <summary>Constructor associates new instance of this class with underlying synchronization object.</summary>
		/// <param name="section">pointer to underlying synchronization object.</param>
		/// <param name="aquireLock">if this parameter is set to <c>true</c> constructor tries to acquire underlying object immediately.</param>
		GaSectionLock(GaCriticalSection* section,
			bool aquireLock) : _section(section),
			_locked(false)
		{
			if( aquireLock )
				Lock();
		}

		/// <summary>Destructor releases underlying synchronization object if it was acquired by this instance of <c>GaSectionLock</c> class.</summary>
		~GaSectionLock() { Unlock(); }

		/// <summary><c>Lock</c> method locks underlying synchronization object.
		/// If it was already locked by this instance of <c>GaSectionLock</c> class, call has no effect. Details of locking synchronization object
		/// are described in specification of <c>GaCriticalSection::Lock</c> method.</summary>
		inline void GACALL Lock()
		{
			if( !_locked )
			{
				_section->Lock();
				_locked = true;
			}
		}

		/// <summary>Unlock method unlocks underlying synchronization object.
		/// If it wasn't locked by this instance of <c>GaSectionLock</c> class, call has no effect. Details of unlocking synchronization object are
		/// described in specification of <c>GaCriticalSection::Unlock</c> method.</summary>
		inline void GACALL Unlock()
		{
			if( _locked )
			{
				_locked = false;
				_section->Unlock();
			}
		}

	};// END CLASS DEFINITION GaSectionLock

	// Controls of threads
	/// <summary><c>GaThread</c> class controls system threads.
	/// It wraps system specific control of threading. This class has built-in synchronizator so it is allowed to use <c>LOCK_OBJECT</c>
	/// and <c>LOCK_THIS_OBJECT</c> macros with instances of this class.</summary>
	class GaThread
	{
		DEFINE_SYNC_CLASS

	private:

		/// <summary>This attribute holds running status of the thread.</summary>
		GaThreadStatus _status;

		/// <summary>System specific the thread object or handle to it.</summary>
		SystemThread _thread;

		/// <summary>stores identification number of thread if it is running or it is suspended.</summary>
		ThreadID _id;

		/// <summary>User defined information (thread's entry point and custom parameters passed to the thread) needed to start thread.</summary>
		GaThreadParameter _parameters;

	public:

		/// <summary>Initializes thread object and stores thread parameters.
		/// If user specified, thread can be started automatically when object is created.</summary>
		/// <param name="parameters">thread parameters (entry point and pointer to custom parameters passed to the thread).</param>
		/// <param name="started">if this parameter is set to <c>true</c> then thread start execution immediately. </param>
		GAL_API
		GaThread(const GaThreadParameter& parameters,
			bool started);

		/// <summary>Stops thread if it is running and releases system object and closes handles to it.</summary>
		GAL_API
		~GaThread();

		/// <summary>Method starts execution of the thread or resumes execution if it was suspended.
		/// If thread already running or system is unable to start/resume thread call fails and method returns <c>false</c>.
		///
		/// This method is thread-safe.</summary>
		/// <returns>Method returns <c>true</c> if thread is started/resumed successfully. If thread was in running state or
		/// system is unable to start/resume thread method returns <c>false</c>.</returns>
		GAL_API
		bool GACALL Start();

		/// <summary>Method suspends thread execution if it is running.
		/// If thread is stopped or system cannot suspend thread, call fails and method returns <c>false</c>. Pause method is intended
		/// only for debugging purposes.
		///
		/// This method is not implemented on *nix systems.
		///
		/// This method is thread-safe.</summary>
		/// <returns>Method returns <c>true</c> if thread is suspended successfully. If thread was stopped or system couldn't suspend thread
		/// method returns <c>false</c>.</returns>
		GAL_API
		bool GACALL Pause();

		/// <summary>Method stops execution of thread forcibly. Calling this method can cause problems thread cannot release acquired resources
		/// properly and do necessary cleanups.
		///
		///This method is thread-safe.</summary>
		/// <returns>Method returns <c>true</c> if thread is stopped successfully.
		/// If thread was stopped or system couldn't stop thread method returns <c>false</c>.</returns>
		GAL_API
		bool GACALL Abort();

		/// <summary>Suspends execution of thread from which it was called until thread which is managed by this instance of <c>GaThread</c> class
		/// finish execution.
		///
		/// This method is thread-safe.</summary>
		/// <returns>Method returns <c>true</c> if waiting has finished successfully. If there was an error method returns <c>false</c>.</returns>
		GAL_API
		bool GACALL Join();

		/// <summary>This method is thread-safe.</summary>
		/// <returns>Method returns current status of the thread.</returns>
		inline GaThreadStatus GACALL Status()
		{
			LOCK_THIS_OBJECT( lock );
			return _status;
		}

		/// <summary>This method is thread-safe.</summary>
		/// <returns>Method returns identification number of thread if it is running or suspended. If thread is stopped, method returns 0.</returns>
		inline ThreadID GACALL GetId()
		{
			LOCK_THIS_OBJECT( lock );
			return _id;
		}

	private:

		/// <summary>This method is used as entry point of new thread. Operating system calls this method when it starts thread.
		/// After that, when thread enters <c>ThreadFunctionWrapper</c> it calls user entry point which is specified in thread parameters.</summary>
		/// <param name="">pointer to object which manages new thread.</param>
		/// <returns>Method returns result of user defined thread's entry point function when thread finishes its execution.</returns>
		static ThreadFunctionReturn APICALL ThreadFunctionWraper(GaThread* thread);

	};// END CLASS DEFINITION GaThread

} // Threading

#endif // __GA_THREADING_H__
