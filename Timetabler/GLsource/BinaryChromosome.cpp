
/*! \file BinaryChromosome.cpp
    \brief This file contains implementation of binary chromosome classes.
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

#include "GlobalRandomGenerator.h"
#include "BinaryChromosome.h"

namespace Chromosome
{
	namespace Representation
	{

		// Removes part of chromosome's code
		void GaBinaryChromosome::Remove(int start,
			int size)
		{
			// check params
			if( size <= 0 || start < 0 || start > _codeSize || !_codeSize )
				return;

			// all values should be removed?
			if( _codeSize <= size )
			{
				delete[] _bits;
				_bits = NULL;
				_codeSize = 0;
			}
			else
			{
				// memory for new code
				bool* newBits = new bool[ _codeSize - size ];

				int i = 0, j = 0;
				// copy values befor remove point
				for( ; i < start; i++ )
					newBits[ i ] = _bits[ i ];

				// skip removing values
				j = i + size;

				// copy values afte remove point
				for( ; j < _codeSize; i++, j++ )
					newBits[ i ] = _bits[ j ];

				// save new code
				delete[] _bits;
				_bits = newBits;
				_codeSize = _codeSize - size;
			}
		}

		// Inserts information int chromosome's code
		void GaBinaryChromosome::Insert(int start,
			GaCodeValue* data,
			int size)
		{
			// check params
			if( size <= 0 || start < 0 || start > _codeSize || !data )
				return;

			// memory for new code
			bool* newBits = new bool[ _codeSize + size ];

			int i = 0;
			// copy values before insertion point
			for( ; i < start; i++ )
				newBits[ i ] = _bits[ i ];

			int k = i;
			// insert new values
			for( int j = 0; j < size; j++, k++ )
				newBits[ k ] = ( (GaBit*)data )[ j ].GetBitState();

			// copy values after insertion point
			for( ; i < _codeSize; i++, k++ )
				newBits[ k ] = _bits[ i ];

			// save new code
			if( _bits )
				delete[] _bits;
			_bits = newBits;
			_codeSize += size;
		}

		// Swap parts of chromosome's code
		void GaBinaryChromosome::Swap(int start1,
			int size1,
			int start2,
			int size2)
		{
			// check params
			if( start1 < 0 || start1 >= _codeSize || start2 < 0 || start2 >= _codeSize ||
				size1 <= 0 || size2 <= 0 || !_codeSize )
				return;

			// reorder swapping points (start1 is first, start2 is second)
			if( start2 < start1 )
			{
				int t = start2;
				start2 = start1;
				start1 = t;

				t = size2;
				size2 = size1;
				size1 = t;
			}

			// prevent overlapping of swap sequences
			if( start1 + size1 >= start2 )
			{
				int old = start2;
				start2 = start1 + size1;
				size2 -= start2 - old;
			}

			// direction of swapping
			if( size1 < size2 )
			{
				// temp buffer (contains longer swap sequence)
				bool* tmp = new bool[ size2 ];
				memcpy( tmp, _bits + start2, size2 );

				// move shorter swap sequence to new position
				int ss = start2 + size2 - size1;
				for( int i = 0; i < size1; i++ )
					_bits[ ss + i ] = _bits[ start1 + i ];

				// shift values between swap sequences
				for( int i = ss - 1 ; i >= start1 + size2; i-- )
					_bits[ i ] = _bits[ i - ( size2 - size1 ) ];

				// copy longer swap sequence to new position
				for( int i = 0; i < size2; i++ )
					_bits[ start1 + i ] = tmp[ i ];

				// free temp buffer
				delete[] tmp;
			}
			else
			{
				// temp buffer (contains longer swap sequence)
				bool* tmp = new bool[ size1 ];
				memcpy( tmp, _bits + start1, size1 );

				// move shorter swap sequence to new position
				for( int i = 0; i < size1; i++ )
					_bits[ start1 + i ] = _bits[ start2 + i ];

				// shift values between swap sequences
				int ss = start1 + size2;
				for( int i = ss; i < start2 - size1; i++ )
					_bits[ i ] = _bits[ i + ( size1 - size2 ) ];

				// copy longer swap sequence to new position
				for( int i = 0; i < size1; i++ )
					_bits[ start2 + i ] = tmp[ i ];

				// free temp buffer
				delete[] tmp;
			}
		}

		// Randomly change the values of selected part of chromosome's code
		void GaBinaryChromosome::Flip(int start,
			int size)
		{
			// check params
			if( start < 0 || start >= _codeSize || !_codeSize )
				return;

			// flip
			for( int i = 0; i < size; i++ )
				_bits[ start + i ] = GaGlobalRandomBoolGenerator->
				Generate( ( (const GaBinaryChromosomeParams&)_configBlock->GetParameters() ).GetProbabilityOfSetState() );
		}

		// Inverts data in selected part of chromosome's code
		void GaBinaryChromosome::Invert(int start,
			int size)
		{
			// check params
			if( start < 0 || start >= _codeSize || !_codeSize )
				return;

			// invert
			for( int i = 0; i < size; i++ )
				_bits[ start + i ] = !_bits[ start + i ];
		}

		// Makes new buffer for manupulatin parts of chromosome's code
		GaCodeValuesBuffer* GaBinaryChromosome::MakeBuffer(int size) const
		{
			return new GaCodeValuesBuffer( size * sizeof( bool ) );
		}

		// Fills buffer with part of chromosome's code
		void GaBinaryChromosome::FillBuffer(int pos,
			int size,
			GaCodeValuesBuffer& buffer) const
		{
			// check params
			if( size <= 0 || pos < 0 || pos >= _codeSize )
				return;

			// get buffer params
			int s = pos + size < _codeSize ? size : _codeSize - pos;
			bool* bits = (bool*)buffer.GetBuffer();
			int p = buffer.GetPosition();

			// save to buffer
			for( int i = 0; i < s; i++ )
				bits[ p + i ] = _bits[ pos + i ];

			// move buffer pointer
			buffer.Move( s * sizeof( bool ) );
		}

		// Makes chromosome's code from buffer of values
		void GaBinaryChromosome::FromBuffer(const GaCodeValuesBuffer& buffer)
		{
			// remove old code
			if( _bits )
				delete[] _bits;

			// memory for new code
			_bits = new bool[ buffer.GetSize() ];
			_codeSize = buffer.GetPosition();

			bool* bits = (bool*)buffer.GetBuffer();

			// copy from buffer
			for( int i = 0; i < _codeSize; i++ )
				_bits[ i ] = bits[ i ];
		}

		// Initialization of empty chromosome
		GaBinaryChromosome::GaBinaryChromosome(GaChromosomeOperationsBlock* configBlock) : 
			GaDynamicOperationChromosome(configBlock),
			_bits(NULL),
			_codeSize(0),
			_backupBits(NULL),
			_backupSize(0) { }

		// Initialization of chromosome with random code
		GaBinaryChromosome::GaBinaryChromosome(int size,
			GaChromosomeOperationsBlock* configBlock) : GaDynamicOperationChromosome(configBlock),
			_backupBits(NULL),
			_backupSize(0)
		{
			if( size > 0 )
			{
				// copy passed chromosome's code
				_codeSize = size;
				_bits = new bool[ _codeSize ];
				for( int i = 0; i < _codeSize; i++ )
					_bits[ i ] = GaGlobalRandomBoolGenerator->
					Generate( ( (const GaBinaryChromosomeParams&)_configBlock->GetParameters() ).GetProbabilityOfSetState() );
			}
			else
			{
				_codeSize = 0;
				_bits = 0;
			}
		}

		// Copy constructor
		GaBinaryChromosome::GaBinaryChromosome(const GaBinaryChromosome& c,
			bool setupOnly) : GaDynamicOperationChromosome(c, setupOnly),
			_backupBits(NULL),
			_backupSize(0)
		{
			if( !setupOnly )
			{
				// copy chromosome's code
				_codeSize = c._codeSize;
				_bits = new bool[ _codeSize ];
				for( int i = 0; i < _codeSize; i++ )
					_bits[ i ] = c._bits[ i ];
			}
		}

		// Initialization of chromosome
		GaBinaryChromosome::GaBinaryChromosome(bool* code,
			int size,
			GaChromosomeOperationsBlock* configBlock) : GaDynamicOperationChromosome(configBlock),
			_backupBits(NULL),
			_backupSize(0)
		{
			if( size > 0 )
			{
				// copy passed chromosome's code
				_codeSize = size;
				_bits = new bool[ _codeSize ];
				for( int i = 0; i < _codeSize; i++ )
					_bits[ i ] = code[ i ];
			}
			else
			{
				_codeSize = 0;
				_bits = NULL;
			}
		}

		// Frees used memory
		GaBinaryChromosome::~GaBinaryChromosome()
		{
			if( _bits )
				delete[] _bits;

			if( _backupBits )
				delete[] _backupBits;
		}

		// Make new chromosome of this type but with random characteristic
		GaChromosomePtr GaBinaryChromosome::MakeNewFromPrototype() const
		{
			// make chromosome with exact setup
			GaChromosomePtr newPtr = MakeCopy( true );
			GaBinaryChromosome* newChromosome = dynamic_cast<GaBinaryChromosome*>( &( *newPtr ) );

			// generate random chromosome code
			if( _codeSize )
			{
				newChromosome->_bits = new bool[ _codeSize ];
				newChromosome->_codeSize = _codeSize;

				for( int i = 0; i < _codeSize; i++ )
					newChromosome->_bits[ i ] = GaGlobalRandomBoolGenerator->
					Generate( ( (const GaBinaryChromosomeParams&)_configBlock->GetParameters() ).GetProbabilityOfSetState() );
			}

			return newPtr;
		}

		// Sets or clears bit of chromosome's code
		void GaBinaryChromosome::SetBitState(int bit,
			bool state)
		{
			if( bit >= 0 && bit < _codeSize )
				_bits[ bit ] = state;
		}

		// Returns state of selected bit of chromosome's code
		bool GaBinaryChromosome::GetBit(int bit)
		{
			if( bit >= 0 && bit < _codeSize )
				return _bits[ bit ];

			return false;
		}

		// Copy data and setup from given source chromosome
		GaChromosome& GaBinaryChromosome::operator =(const GaChromosome& rhs)
		{
			const GaBinaryChromosome& r = dynamic_cast<const GaBinaryChromosome&>( rhs );
			bool* bits = r._bits;
			_codeSize = r._codeSize;

			// allocate memory for new code
			if( _bits )
				delete[] _bits;
			_bits = new bool[ _codeSize ];

			// copy code
			for( int i = 0; i < _codeSize; i++ )
				_bits[ i ] = bits[ i ];

			return GaDefaultChromosome::operator =( rhs );
		}

		// Compares two chromosomes and returns how much are they simular in percent
		bool GaBinaryChromosome::operator ==(const GaChromosome& c) const
		{
			const GaBinaryChromosome& r = dynamic_cast<const GaBinaryChromosome&>( c );
			bool* bits = r._bits;
			int s = r._codeSize;

			int sim = 0, t = s + _codeSize;

			// get shorter chromosome
			if( s > _codeSize )
				s = _codeSize;

			// compare codes
			for( int i = 0; i < s; i++ )
			{
				if( bits[ i ] == _bits[ i ] )
					sim += 2;
			}

			return sim == t/*( (float)sim / t ) * 100*/;
		}

		// Returns char made based on bit stream
		char GaBinaryChromosome::GetChar(int pos/* = 0*/) const
		{
			if( pos < 0 || pos > _codeSize )
				return 0;

			char res = 0;

			for( int i = 0; pos + i < _codeSize && i < sizeof( char ); i++ )
				res = ( res << 1 ) | (char)_bits[ pos + i ];

			return res;
		}

		// Return array of chars made based on bit stream
		void GaBinaryChromosome::GetChar(char* output,
			int pos/* = 0*/,
			int size/* = 0*/) const
		{
			for( int i = 0; i < size; i++ )
				output[ i ] = GetChar( pos + i * sizeof( char ) );
		}

		// Returns short int made based on bit stream
		short GaBinaryChromosome::GetShort(int pos/* = 0*/) const
		{
			if( pos < 0 || pos > _codeSize )
				return 0;

			short res = 0;

			for( int i = 0; pos + i < _codeSize && i < sizeof( short ); i++ )
				res = ( res << 1 ) | (short)_bits[ pos + i ];

			return res;
		}

		// Return array of short ints made based on bit stream
		void GaBinaryChromosome::GetShort(short* output,
			int pos/* = 0*/,
			int size/* = 0*/) const
		{
			for( int i = 0; i < size; i++ )
				output[ i ] = GetShort( pos + i * sizeof( short ) );
		}

		// Returns int made based on bit stream
		int GaBinaryChromosome::GetInt(int pos/* = 0*/) const
		{			
			if( pos < 0 || pos > _codeSize )
				return 0;

			int res = 0;

			for( int i = 0; pos + i < _codeSize && i < sizeof( int ); i++ )
				res = ( res << 1 ) | (int)_bits[ pos + i ];

			return res;
		}

		// Return array of ints made based on bit stream
		void GaBinaryChromosome::GetInt(int* output,
			int pos/* = 0*/,
			int size/* = 0*/) const
		{
			for( int i = 0; i < size; i++ )
				output[ i ] = GetInt( pos + i * sizeof( int ) );
		}

		// Returns float made based on bit stream
		float GaBinaryChromosome::GetFloat(int pos/* = 0*/) const
		{
			if( pos < 0 || pos > _codeSize )
				return 0;

			int res = 0;

			for( int i = 0; pos + i < _codeSize && i < sizeof( int ); i++ )
				res = ( res << 1 ) | (int)_bits[ pos + i ];

			return *( (float*)&res );
		}

		// Return array of floats made based on bit stream
		void GaBinaryChromosome::GetFloat(float* output,
			int pos/* = 0*/,
			int size/* = 0*/) const
		{
			for( int i = 0; i < size; i++ )
				output[ i ] = GetFloat( pos + i * sizeof( float ) );
		}

		// Returns double made based on bit stream
		double GaBinaryChromosome::GetDouble(int pos/* = 0*/) const
		{
			if( pos < 0 || pos > _codeSize )
				return 0;

			long long res = 0;
 
			for( int i = 0; pos + i < _codeSize && i < sizeof( long long ); i++ )
				res = ( res << 1 ) | (int)_bits[ pos + i ];

			return *( (double*)&res );
		}

		// Return array of doubles made based on bit stream
		void GaBinaryChromosome::GetDouble(double* output,
			int pos/* = 0*/,
			int size/* = 0*/) const
		{
			for( int i = 0; i < size; i++ )
				output[ i ] = GetDouble( pos + i * sizeof( double ) );
		}

		// Creates array of bit from char.
		// Caller is responsible for array's memory.
		bool* GaBinaryChromosome::FromChar(char* input,
			int size)
		{
			if( !input || size <= 0 )
				return NULL;

			bool* bits = new bool[ size * sizeof( char ) ];

			for( int i = 0, k = 0; i < size; i++ )
			{
				for( int j = 0; j < sizeof( char ); j++, k++ )
					bits[ k ] = ( input[ i ] & ( 1 << j ) ) != 0;
			}

			return bits;
		}

		// Creates array of bit from short.
		// Caller is responsible for array's memory.
		bool* GaBinaryChromosome::FromShort(short* input,
			int size)
		{
			if( !input || size <= 0 )
				return NULL;

			bool* bits = new bool[ size * sizeof( short ) ];

			for( int i = 0, k = 0; i < size; i++ )
			{
				for( int j = 0; j < sizeof( short ); j++, k++ )
					bits[ k ] = ( input[ i ] & ( 1 << j ) ) != 0;
			}

			return bits;
		}

		// Creates array of bit from int.
		// Caller is responsible for array's memory.
		bool* GaBinaryChromosome::FromInt(int* input,
			int size)
		{
			if( !input || size <= 0 )
				return NULL;

			bool* bits = new bool[ size * sizeof( int ) ];

			for( int i = 0, k = 0; i < size; i++ )
			{
				for( int j = 0; j < sizeof( int ); j++, k++ )
					bits[ k ] = ( input[ i ] & ( 1 << j ) ) != 0;
			}

			return bits;
		}

		// Creates array of bit from float.
		// Caller is responsible for array's memory.
		bool* GaBinaryChromosome::FromFloat(float* input,
			int size)
		{
			if( !input || size <= 0 )
				return NULL;

			bool* bits = new bool[ size * sizeof( float ) ];

			for( int i = 0, k = 0; i < size; i++ )
			{
				for( int j = 0; j < sizeof( float ); j++, k++ )
					bits[ k ] = ( ( (int*)input )[ i ] & ( 1 << j ) ) != 0;
			}

			return bits;
		}

		// Creates array of bit from double.
		// Caller is responsible for array's memory.
		bool* GaBinaryChromosome::FromDouble(double* input,
			int size)
		{
			if( !input || size <= 0 )
				return NULL;

			bool* bits = new bool[ size * sizeof( double ) ];

			for( int i = 0, k = 0; i < size; i++ )
			{
				for( int j = 0; j < sizeof( double ); j++, k++ )
					bits[ k ] = ( ( (long long*)input )[ i ] & ( 1LL << j ) ) != 0;
			}

			return bits;
		}

		// Saves current chromosome's code before mutation
		void GaBinaryChromosome::PreapareForMutation()
		{
			_backupSize = _codeSize;

			_backupBits = new bool[ _codeSize ];
			memcpy( _backupBits, _bits, _codeSize );
		}

		// Accepts mutation and deletes backuped code
		void GaBinaryChromosome::AcceptMutation()
		{
			delete[] _backupBits;
			_backupBits = NULL;
		}

		// Rejects mutation and restores backuped chromosome's code
		void GaBinaryChromosome::RejectMutation()
		{
			delete[] _bits;
			_bits = _backupBits;
			_backupBits = NULL;

			_codeSize = _backupSize;
		}

	} // Representation
} // Chromosome
