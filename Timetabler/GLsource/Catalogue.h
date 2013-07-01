
/*! \file Catalogue.h
    \brief This file contains declaration and implementation of catalogue template class used to store genetic operations and other stateless objects.
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

#ifndef __GA_CATALOGUE_H__
#define __GA_CATALOGUE_H__

#include "Platform.h"

#if defined(GAL_STL_EXT_MSVC)

#include <hash_map>
using namespace stdext;

#elif defined(GAL_STL_EXT_STLPORT)

#include <hash_map>

#elif defined(GAL_STL_EXT_GNUC)

#include <ext/hash_map>
using namespace __gnu_cxx;

#endif

#include "Threading.h"
#include <math.h>
#include <cstdlib>

using namespace std;

namespace Common
{
	template <typename T>
	class GaCatalogue;

	/// <summary>This template class manages key and pointer to data of a catalogue entry.
	/// Catalogue entry stores key's string value and pointer to data. Entry stores copied string value of a key. After binding data to an entry,
	/// entry object takes over responsibility for memory occupied by data. Data must be located at heap. 
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	/// <param name="T">type of stored data in the entry.</param>
	template <typename T>
	class GaCatalogueEntry
	{
	friend class GaCatalogue<T>;

	private:

		/// <summary>Pointer to string representing key of the catalogue's entry.</summary>
		char* _name;

		/// <summary>Stores length of string (key) without null-terminating character.</summary>
		int _nameLength;

		/// <summary>Pointer to the data.</summary>
		T* _data;

	public:

		/// <summary>Constructor makes copy of key and stores pointer to copied key and pointer to user specified data.</summary>
		/// <param name="name">key of the entry (null-terminating string).</param>
		/// <param name="data">data which will be stored in the entry.</param>
		GaCatalogueEntry(const char* name,
						T* data)
		{
			if( name )
			{
				_nameLength = (int)strlen( name );

				// copy name
				_name = new char[ _nameLength + 1 ];
				strcpy( _name, name );

				_data = data;

				return;
			}

			_name = NULL;
			_nameLength = 0;
			_data = NULL;
		}

		/// <summary>Frees memory used by the data and the key.</summary>
		~GaCatalogueEntry()
		{
			if( _name )
				delete[] _name;

			if( _data )
				delete _data;
		}

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns pointer to string representing key of the entry.</returns>
		inline const char* GACALL GetName() const { return _name; }

		/// <summary><c>SetName</c> method copies key's string and stores pointer to the copied string.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="name">pointer to string which represents new key.</param>
		void GACALL SetName(const char* name)
		{
			if( name )
			{
				if( _name )
					delete[] _name;

				_nameLength = strlen( name );

				// copy name
				_name = new char[ _nameLength + 1 ];
				strcpy( _name, name );
			}
		}

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns length of the key.</returns>
		inline int GACALL GetNameLength() const { return _nameLength; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference to the data.</returns>
		inline T* GACALL GetData() const { return _data; }

		/// <summary><c>SetData</c> method stores pointer to data.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="data">reference to new data.</param>
		inline void GACALL SetData(T* data) { _data = data; }

	};


	/// <summary><C>GaCatalogue</c> template class stores and manages catalogue (directory) for genetic operations.
	/// Data can be accessed by its key (name). When user adds new data into catalogue, catalogue takes over responsibility for memory
	/// allocated by the data. Data must come from heap. Key/data combination is stored in <see cref="GaCatalogueEntry" /> object.
	/// Duplicates of key in catalogue are not allowed.
	///
	/// All public methods are thread-safe except <c>MakeInstance</c> and <c>FreeInstance</c>. This class has built-in synchronizator so
	/// it is allowed to use <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros with instances of this class.</summary>
	/// <param name="T">type of stored data in catalogue.</param>
	template <typename T>
	class GaCatalogue
	{
		DEFINE_SYNC_CLASS

	private:

		// String comparator
		struct GaStringComparator
		{

		public:

			// Compare
			inline bool GACALL operator()(const char* left, const char* right) const
			{
				#if defined(GAL_STL_EXT_MSVC)

				return strcmp( left, right ) == -1;

				#elif defined(GAL_STL_EXT_GNUC) || defined (GAL_STL_EXT_STLPORT)

				return strcmp( left, right ) == 0;

				#endif
			}

		};

		// Comarator for catalogue's hash map
		class GaCataolgueHashMapComparator
		{

		public:

			// Parameters for hash table
			enum
			{
				bucket_size = 4,
				min_buckets = 8
			};	

			// Construct with default comparator
			GaCataolgueHashMapComparator() : _comparator() { }

			// Construct with _Pred comparator
			GaCataolgueHashMapComparator(GaStringComparator pred) : _comparator( pred ) { }

			// Hash function
			size_t GACALL operator()(const char* key) const
			{
				if( key == NULL )
					return 0;

				size_t t = 0;
				while( *key )
					t += *( key++ );

				ldiv_t rem = ldiv((long)t, 127773);
				rem.rem = 16807 * rem.rem - 2836 * rem.quot;
				if (rem.rem < 0)
					rem.rem += 2147483647;

				return ((size_t)rem.rem);
			}

			// Comparison
			inline bool GACALL operator()(const char* value1, const char* value2) const { return _comparator( value1, value2 ); }

		private:

			// String comparator
			GaStringComparator _comparator;
		};

		#if defined(GAL_STL_EXT_MSVC)

			typedef hash_map<const char*, GaCatalogueEntry<T>*, GaCataolgueHashMapComparator> GaCataolgueHashMap;

		#elif defined(GAL_STL_EXT_GNUC) || defined(GAL_STL_EXT_STLPORT)

			typedef hash_map<const char*, GaCatalogueEntry<T>*, GaCataolgueHashMapComparator, GaCataolgueHashMapComparator> GaCataolgueHashMap;
		
		#endif

		/// <summary>Pointer to global instance of catalogue for type <c>T</c>.</summary>
		GAL_API
		static GaCatalogue<T>* _instance;

		/// <summary>Hash table which stores keys and data of the catalogue.</summary>
		GaCataolgueHashMap _entries;

	public:

		/// <summary>Returns reference to global instance of catalogue for type <c>T</c>.</summary>
		/// <returns>Returns reference to global instance of catalogue for type <c>T</c>.</returns>
		static GaCatalogue<T>& GACALL Instance() { return *_instance; }

		/// <summary>Method makes global instance of catalogue if it wasn't instanced before.
		/// This method must be called before using of global instance of catalogue. All built-in global instances of catalogues are instanced
		/// in <see cref="GaInitialization" /> function.
		///
		/// This method is not thread safe.</summary>
		static void GACALL MakeInstance()
		{
			if( !_instance )
				_instance = new GaCatalogue();
		}

		/// <summary>Frees memory used by global instance of the catalogue. This method must be called before application quitting
		/// application or unloading library. All built-in global instances of catalogues are freed in <see cref="GaFinalize" /> function.
		///
		/// This method is not thread safe.</summary>
		static void GACALL FreeInstance()
		{
			if( _instance )
				delete _instance;
		}

		/// <summary>Initializes empty catalogue</summary>
		GaCatalogue() { }

		/// <summary>Frees memory used by entries (keys and user data).</summary>
		~GaCatalogue()
		{
			for( typename GaCataolgueHashMap::const_iterator it = _entries.begin(); it != (typename GaCataolgueHashMap::const_iterator)_entries.end(); it++ )
				delete it->second;

			_entries.clear();
		}

		/// <summary><c>Register</c> method adds data with specified key (name) to catalogue. If entry with specified key already exists
		/// in catalogue and replace is set to <c>false</c>, insertion will fail because duplicate of keys are not allowed.
		/// When replace is set to <c>true</c>, data in an entry with specified key are replaced or if such entry doesn't exist new entry
		/// is added to catalogue. Key is copied and catalogue object takes over responsibility for memory used by data. Data must be located at heap.
		///
		/// This method is thread-safe.</summary>
		/// <param name="name">key of new or existing entry.</param>
		/// <param name="data">reference to data which will be stored in catalogue</param>
		/// <param name="replace">tells method whether it should replace data in an entry if it exists with specified key.</param>
		/// <returns>Method returns <c>true</c> if data is added or replaced successfully. If data cannot be added it returns <c>false</c>.</returns>
		bool GACALL Register(const char* name,
				T* data,
				bool replace = false)
		{
			// no name - error?
			if( !name )
				return false;

			LOCK_THIS_OBJECT( lock );

			// does it exist?
			
			typename GaCataolgueHashMap::const_iterator it = _entries.find( name );
			bool exists = it != (typename GaCataolgueHashMap::const_iterator)_entries.end();
			if( exists && !replace )
				// exists and no replacing - error
				return false;

			// erease if entry exists and replace is enabled
			if( exists && replace )
			{
				delete it->second;			
				_entries.erase( name );
			}

			// make entry for data and insert to catalogue
			GaCatalogueEntry<T>* new_entry = new GaCatalogueEntry<T>( name, data );
			return _entries.insert( pair<const char*, GaCatalogueEntry<T>*>( name, new_entry ) ).second;
		}

		/// <summary><c>Unregister</c> method removes entry with specified key and frees memory used by it.
		///
		/// This method is thread-safe.</summary>
		/// <param name="name">key of the desired entry.</param>
		/// <returns>Method returns <c>true</c> if entry with specified key is removed successfully.
		/// If entry cannot be found method returns <c>false</c>.</returns>
		bool GACALL Unregister(const char* name)
		{
			LOCK_THIS_OBJECT( lock );

			bool removed = false;

			// exists?
			typename GaCataolgueHashMap::const_iterator it = _entries.find( name );
			if( it != _entries.end() )
			{
				// remove
				delete it->second;
				removed = _entries.erase( name ) > 0;
			}

			return removed;
		}

		/// <summary>This method is thread-safe.</summary>
		/// <param name=name">key of the desired entry.</param>
		/// <returns>Method returns pointer to catalogue entry with specified key. If such entry cannot be found, method returns <c>NULL</c> pointer.</returns>
		inline GaCatalogueEntry<T>* GACALL GetEntry(const char* name) const
		{
			LOCK_THIS_OBJECT( lock );

			// find entry and return pointer to entry
			typename GaCataolgueHashMap::const_iterator it = _entries.find( name );
			return it == _entries.end() ? NULL : it->second;
		}

		/// <summary>This method is thread-safe.</summary>
		/// <param name="name">key of the desired entry.</param>
		/// <returns>Method returns pointer to data of entry with specified key. If such entry cannot be found, method returns <c>NULL</c> pointer.</returns>
		inline T* GACALL GetEntryData(const char* name) const
		{
			LOCK_THIS_OBJECT( lock );
			
			// find entry and return pointer to data
			typename GaCataolgueHashMap::const_iterator it = _entries.find( name );
			return it == _entries.end() ? NULL : it->second->GetData();
		}

		/// <summary>This operator method is thread-safe.</summary>
		/// <param name="name">key of the desired entry.</param>
		/// <returns>Operator returns pointer to data of entry with specified key. If such entry cannot be found, operator returns <c>NULL</c> pointer.</returns>
		inline T* GACALL operator [](const char* name) const { return GetEntryData( name ); }

		// Returns all keys in catalogue
		// This method allocates memory for array of pointers to keys and keys but caller is responsible for it.
		// It returns pointer to array of keys and number of keys for which is memory allocated.
		/// <summary><c>GetKeys</c> method returns all kesy in catalogue.
		/// This method allocates memory for array of pointers to keys and keys but caller is responsible for it.
		///
		/// This method is thread-safe.</summary>
		/// <param name="number">Reference to variable to which number of kesy will be stored.</param>
		/// <returns>Method returns pointer to array of keys.</returns>
		char** GACALL GetKeys(int& number) const
		{
			LOCK_THIS_OBJECT( lock );

			// empty directory
			if( _entries.size() == 0 )
			{
				number = 0;
				return NULL;
			}

			number = _entries.size();
			char** names = new char*[ number ];

			// fill buffer with names
			int i = 0;
			for( typename GaCataolgueHashMap::const_iterator it = _entries.begin(); it != _entries.end(); ++it, i++ )
			{
				names[ i ] = new char[ it->second->GetNameLength() + 1 ];
				strcpy( names[ i ], it->second->GetName() );
			}

			return names;
		}

		/// <summary>This method is thread-safe.</summary>
		/// <returns>Method returns number of entries in catalogue.</returns>
		inline int GACALL GetCount() const
		{
			LOCK_THIS_OBJECT( lock );

			return _entries.size();
		}

		/// <summary><c>IsExist</c> method checks existence of entry with specified key.
		///
		/// This method is thread-safe.</summary>
		/// <param name="name">key of the desired entry.</param>
		/// <returns>Method returns <c>true</c> if entry with specified key exists in catalogue, otherwise return <c>false</c>.</returns>
		inline bool GACALL IsExist(const char* name) const
		{
			LOCK_THIS_OBJECT( lock );

			return _entries.count( name ) > 0;
		}

	};

	#if defined(GAL_API_EXPORTING)

		template <typename T>
		GaCatalogue<T>* GaCatalogue<T>::_instance = NULL;

	#else
	
		#if defined(GAL_PLATFORM_NIX)

			#if !defined(GAL_SYNTAX_INTL) && !defined(GAL_SYNTAX_SUNC)

				template <typename T>
				extern GaCatalogue<T>* GaCatalogue<T>::_instance;

			#endif

		#endif
	
	#endif

} // Common

#endif //__GA_CATALOGUE_H__

