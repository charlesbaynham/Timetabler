
/*! \file SmartPtr.h
    \brief This file contains declaration and implementation of template classes and datatypes that handles smart pointers used by the library.
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

#ifndef __GA_SMART_PTR_H__
#define __GA_SMART_PTR_H__

#include "Platform.h"
#include "Threading.h"

namespace Common
{
	template <typename T>
	class GaSmartPtr;

	/// <summary><c>GaSmartStorage</c> template class provides reference-counting for smart pointers.
	/// <c>GaSmartStorage</c> objects holds address of used data and number of references (smart pointers) which point to the data.
	/// Object of this class as well as the data are destroyed and memory is freed when there are no more references which points to the data.
	/// Arrays cannot be used with this class. This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros
	/// cannot be used with instances of this class, but all public method and operators are thread-safe. </summary>
	template <typename T>
	class GaSmartStorage
	{

	friend class GaSmartPtr<T>;

	private:

		/// <summary>Holds number of references (smart pointers) to data guarded by this object.</summary>
		mutable int _count;

		/// <summary>Pointer to user data.</summary>
		T* _data;

	public:

		/// <summary><c>MakeInstance</c> makes new reference-counting object for data at address data. This method is thread-safe.</summary>
		/// <param name="data">pointer to data. <c>data</c> cannot be an array.</param>
		/// <returns>Method returns smart pointer which points to data and use newly created object for reference-counting.</returns>
		static GaSmartPtr<T> GACALL MakeInstance(T* data) { return GaSmartPtr<T>( new GaSmartStorage<T>( data ) ); }

		/// <summary><c>MakeInstance</c> makes new reference-counting object for data at address data. This method is thread-safe.</summary>
		/// <param name="data">pointer to data. <c>data</c> cannot be an array.</param>
		/// <returns>Method returns reference to newly created object for reference-counting.</returns>
		static GaSmartStorage<T>* GACALL MakeInstanceDirect(T* data) { return new GaSmartStorage<T>( data ); }

		// Increase count of references which point to this location
		/// <summary>This method increments number of references to the data. <c>AddReference</c> method is called when smart pointer
		/// is set to point to the location. This method is thread-safe.</summary>
		/// <param name="location">pointer to location which is incremented.</param>
		/// <returns>Method returns new number of references which still points to the data.</returns>
		static int GACALL AddReference(GaSmartStorage<T>* location) { return location ? ATOMIC_INC( location->_count ) : 0; }

		// Decrease count of references which point to this location
		/// <summary>This method decrements number of references, if that number reaches 0, the data is destroyed and memory is freed.
		/// <c>RemoveReference</c> method is called when smart pointers are destructed or when set to point to new location.
		/// This method is thread-safe.</summary>
		/// <param name="location">pointer to location which is decremented.</param>
		/// <returns>Method returns new number of references which still points to the data.</returns>
		static int GACALL RemoveReference(GaSmartStorage<T>* location)
		{
			if( location )
			{
				int new_count = ATOMIC_DEC( location->_count );

				// no more references?
				if( !new_count )
					delete location;

				return new_count;
			}

			return 0;
		}

		/// <summary>Initializes object for counting references to data located at <c>data</c> address. </summary>
		/// <param name="data">pointer to user data. <c>data</c> cannot be an array.</param>
		GaSmartStorage(T* data) : _count(0), 
			_data(data) { }

		/// <summary>Destructor is called when there are no more references which points to the data. Destructor frees memory used by the data. </summary>
		~GaSmartStorage()
		{
			if( _data )
				delete _data;
		}

		/// <summary>This method is thread-safe.</summary>
		/// <returns>Method returns pointer to user data.</returns>
		inline T* GACALL GetData() const { return _data; }

		/// <summary>This method is thread-safe.</summary>
		/// <returns>Method returns number of references (smart pointers) which points to this location.</returns>
		inline int GACALL GetCount() const { return _count; }

	};// END CLASS DEFINITION GaSmartStorage

	/// <summary><c>GaSmartPtr</c> template class wraps C++ raw pointers, and takes over responsibility of managing the allocated memory.
	/// Smart pointer holds address of user data and reference to an object which is responsible for counting number of references to data,
	/// when there are no instances of <c>GaSmartPtr</c> pointing to location of the data (reference count of the location reaches 0),
	/// object is destroyed and memory used by the object is freed. Memory management by <c>GaSmartPtr</c> class is thread-safe,
	/// but after dereferencing smart pointer to access the data, it cannot be guaranteed that memory will not be freed if some other thread
	/// changes dereferenced pointer. Implemented smart pointers have some limitations:
	/// <br>1. Dynamically allocated arrays cannot be managed by GaSmartPtr class.
	/// <br>2. Circular references can cause memory leakage.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of
	/// this class, but all public method and operators are thread-safe.</summary>
	/// <param name="T">type of data to which smart pointer references.</param>
	template <typename T>
	class GaSmartPtr
	{

	private:

		/// <summary>Guards smart pointer against concurrent changes.</summary>
		mutable volatile unsigned long _lock;

		/// <summary>Holds address of user data.</summary>
		T* _data;

		/// <summary>Pointer to object which holds reference-count and address of data.</summary>
		GaSmartStorage<T>* _location;

	public:

		/// <summary>This constructor makes new reference to data that are managed by <see cref="GaSmertStorage" />.
		/// Creation of smart pointer and counting of references is thread-safe.</summary>
		/// <param name="storage">reference to object responsible for reference-counting.</param>
		GaSmartPtr(GaSmartStorage<T>* storage) : _lock(0)
		{
			Lock();

			// add new reference to smart location
			GaSmartStorage<T>::AddReference( storage );

			_location = storage;
			_data = _location ? _location->_data : NULL;

			Unlock();
		}

		/// <summary>This constructor make instance of <see cref="GaSmartStorage" /> and binds unmanaged memory to the smart storage.
		/// If the given memory is already managed by this mechanism, it can cause unexpected results.
		/// Creation of smart pointer and counting of references is thread-safe.</summary>
		/// <param name="rawPtr">raw pointer to data.</param>
		GaSmartPtr(T* rawPtr) : _lock(0)
		{
			Lock();

			GaSmartStorage<T>* location = GaSmartStorage<T>::MakeInstanceDirect( rawPtr );
			
			// add new reference to smart location
			GaSmartStorage<T>::AddReference( location );

			_location = location;
			_data = _location ? _location->_data : NULL;

			Unlock();
		}

		/// <summary>Copy constructor makes new reference to data to which <c>ptr</c> points.
		/// Creation of smart pointer and counting of references is thread-safe.</summary>
		/// <param name="ptr">reference to smart pointer which should be copied.</param>
		GaSmartPtr(const GaSmartPtr<T>& ptr) : _lock(0)
		{
			Lock( ptr );

			// add new reference to smart location
			GaSmartStorage<T>::AddReference( ptr._location );

			_location = ptr._location;
			_data = _location ? _location->_data : NULL;

			Unlock( ptr );
		}

		/// <summary>Default constructor, initializes pointer as <c>NULL</c> pointer.
		/// Creation of smart pointer and counting of references is thread-safe.</summary>
		GaSmartPtr() : _location(NULL),
			_data(NULL),
			_lock(0) { }

		/// <summary>Decrements number of references to data. If there is no more references, memory used my data is freed and object is destroyed.
		/// Destruction of smart pointer is thread-safe.</summary>
		~GaSmartPtr()
		{
			Lock();

			GaSmartStorage<T>::RemoveReference( _location );

			Unlock();
		}

		/// <summary>Operator provides access to data to which smart pointer points. This method is thread safe, but it cannot be guaranteed
		/// that memory will not be freed if some other thread changes pointer after address is returned.</summary>
		/// <returns>Operator returns pointer to user data.</returns>
		inline T* GACALL operator ->() const { return _data; }

		/// <summary>Operator provides access to data to which smart pointer points. This method is thread safe, but it cannot be guaranteed
		/// that memory will not be freed if some other thread changes pointer after address is returned.</summary>
		/// <returns>Operator returns reference to user data.</returns>
		inline T& GACALL operator *() const { return *_data; }

		/// <summary>Sets smart pointer to points to same location as <c>rhs</c> pointer.
		/// It also decrements number of references of old smart locationa and increments number of new smart location.
		/// If number of references of old location reached zero, this operator frees used memory. This operator is thread-safe.</summary>
		/// <param name="rhs">smart pointer which holds address to which <c>this</c> pointer should point.</param>
		/// <returns>Operator returns reference to <c>this</c> object.</returns>
		inline GaSmartPtr<T>& GACALL operator =(const GaSmartPtr<T>& rhs)
		{
			if( this != &rhs )
			{
				Lock( rhs );

				if( rhs._location != _location )
				{
					// remove old reference
					GaSmartStorage<T>::RemoveReference( _location );

					// new reference
					GaSmartStorage<T>::AddReference( rhs._location );

					_location = rhs._location;
					_data = _location ? _location->_data : NULL;
				}

				Unlock( rhs );
			}
			return *this;
		}

		/// <summary>Sets smart pointer to points to <c>rhs</c> smart location for storing data.
		/// It also decrements number of references of old smart locationa and increments number of new smart location.
		/// If number of references of old location reached zero, this operator frees used memory. This operator is thread-safe.</summary>
		/// <param name="rhs">reference to smart location to which <c>this</c> pointer should point.</param>
		/// <returns>Operator returns reference to <c>this</c> object.</returns>
		inline GaSmartPtr<T>& GACALL operator =(GaSmartStorage<T>& rhs)
		{
			Lock();

			if( &rhs != _location )
			{
				// remove old reference
				GaSmartStorage<T>::RemoveReference( _location );

				// new reference
				GaSmartStorage<T>::AddReference( &rhs );

				_location = &rhs;
				_data = _location ? _location->_data : NULL;
			}

			Unlock();

			return *this;
		}

		/// <summary>This operator makes new instance of <see cref="GaSmartStorage" /> and binds unmanaged memory to the smart storage and
		/// sets pointer to it. It also decrements number of references of old smart locationa and increments number of new smart location.
		/// If number of references of old location reached zero, this operator frees used memory. This operator is thread-safe.</summary>
		/// <param name="rhs">raw pointer to user data which should be managed.</param>
		/// <returns>Operator returns reference to <c>this</c> object.</returns>
		inline GaSmartPtr<T>& GACALL operator =(T* rhs)
		{
			Lock();

			if( &rhs != _data )
			{
				GaSmartStorage<T>* location = GaSmartStorage<T>::MakeInstanceDirect( rhs );

				// remove old reference
				GaSmartStorage<T>::RemoveReference( _location );

				// new reference
				GaSmartStorage<T>::AddReference( location );

				_location = location;
				_data = _location ? _location->_data : NULL;
			}

			Unlock();

			return *this;
		}

		// Compare to see if two smart pointer points to same safe location.
		/// <summary>Compares two smart pointers to see they points to same data. This operator is thread-safe.</summary>
		/// <param name="rhs">the second smart pointer in the expression.</param>
		/// <returns>Operator returns true if two pointers point to same location.</returns>
		inline bool GACALL operator ==(const GaSmartPtr<T>& rhs) const { return _location == rhs._location; }

		/// <summary>This method is thread-safe.</summary>
		/// <returns>Method returns raw pointer to user data.</returns>
		inline T* GACALL GetRawPtr() const { return _data; }

		/// <summary>Checks pointer against <c>NULL</c> value. This method is thread-safe.</summary>
		/// <returns>Returns <c>true</c> if this is <c>NULL</c> pointer.</returns>
		inline bool GACALL IsNULL() const { return !_data; }

	private:

		/// <summary>This method prevents concurrent changes of smart pointer by locking it.</summary>
		inline void Lock() const
		{
			// sections are very short so spin-lock is used
			SPINLOCK( _lock );
		}

		/// <summary>This method prevents concurrent changes of <c>this</c> and <c>second</c> smart pointers and it is used
		/// when value of one pointer is assigning to another (<c>operator =</c> or copy constructor). Order of pointer locking is
		/// defined by order of addresses of pointers' objects to prevent deadlocks (first is locked pointer which object has lower address).</summary>
		/// <param name="second">the second pointer which is used in expression.</param>
		inline void Lock(const GaSmartPtr<T>& second) const
		{
			// sections are very short so spin-lock is used
			// pointer sare locked in order in which address are ordered to prevent deadlocks
			if( this < &second )
			{
				SPINLOCK( _lock );
				SPINLOCK( second._lock );
			}
			else
			{
				SPINLOCK( second._lock );
				SPINLOCK( _lock );
			}
		}

		/// <summary>This method unlocks changes of smart pointer.</summary>
		inline void Unlock() const { _lock = 0; }

		/// <summary>This method unlocks changes of this and second smart pointers, it is used after assigning of values
		/// from one smart pointer to another is done (<c>operator =</c> or copy constructor).</summary>
		/// <param name="second">the second pointer whic is used in expression.</param>
		inline void Unlock(const GaSmartPtr<T>& second)
		{
			_lock = 0;
			second._lock = 0;
		}

	public:

		/// <summary><c>NullPtr</c> is global constant <c>NULL</c> pointer for <c>T</c> type.</summary>
		static const GaSmartPtr<T> NullPtr;

	};// END CLASS DEFINITION GaSmartPtr
	
	// NULL pointer
	template <typename T>
	const GaSmartPtr<T> GaSmartPtr<T>::NullPtr;
	
} // Common

#endif //__GA_SMART_PTR_H__
