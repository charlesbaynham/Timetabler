
/*! \file MultiValueChromosome.h
    \brief This file contains declaration and implementation of multi-value chromosome classes (with and without support for arithmetic crossover operations).
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

#ifndef __GA_MULTI_VALUE_CHROMOSOME_H__
#define __GA_MULTI_VALUE_CHROMOSOME_H__

#include <vector>
#include "Chromosome.h"
#include "Platform.h"
#include "RepresentationInterfaces.h"
#include "DomainChromosome.h"

using namespace std;
using namespace Chromosome;
using namespace Chromosome::Representation;

namespace Chromosome
{
	namespace Representation
	{

		/// <summary>This class is used by <see cref="GaMultiValueChromosome" /> template class for extracting of single values from chromosome's code
		/// and other manipulation on chromosome's code.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		/// <param name="TYPE">type of value.</param>
		template <typename TYPE>
		class GaChromosomeValue : public GaCodeValue
		{

		private:

			/// <summary>Single value of chromosome's code.</summary>
			TYPE _value;

		public:

			/// <summary>Detailed description can be found in specification of <see cref="GaCodeValue::Initialize" />.</summary>
			virtual void GACALL Initialize() { }

			/// <summary>Detailed description can be found in specification of <see cref="GaCodeValue::FromBuffer" />.</summary>
			virtual void GACALL FromBuffer(const GaCodeValuesBuffer& buffer,
				int pos)
			{
				if( pos >= 0 && pos < ( buffer.GetPosition() / (int)sizeof( TYPE ) ) )
					_value = ( (TYPE*)buffer.GetBuffer() )[ pos ];
			}

			/// <summary>This constructor initializes value.</summary>
			/// <param name="value">user-defined value.</param>
			GaChromosomeValue(TYPE value) : _value(value) { }

			/// <summary>This constructor calls <see cref="Initialize" /> method.</summary>
			GaChromosomeValue() { Initialize(); }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns stored value.</returns>
			inline TYPE GACALL GetValue() const { return _value; }

			/// <summary><c>SetValue</c> method sets stored value.
			///
			/// This method is not thread safe.</summary>
			/// <param name="value">new value.</param>
			inline void GACALL SetValue(TYPE value) { _value = value; }

		};// END CLASS DEFINITION GaChromosomeValue

		/// <summary>This class can be used for chromosomes which code has multiple values. Values can be of any type, but all values must have same type,
		/// they must use same value set and must obey requirements introduced by the value set. 
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		/// <param name="TYPE">type of value in chromosome's code.</param>
		template <typename TYPE>
		class GaMultiValueChromosome : public GaDomainChromosome<TYPE>,
			public GaSwapableCode,
			public GaSizableCode,
			public GaMutableCode,
			public GaMultiValueCode
		{

		protected:

			/// <summary>Chromosome's code.</summary>
			vector<TYPE> _values;

			/// <summary>Backup copy chromosome's code before mutation if improving-only mutation flag is set in chromosome's parameters.</summary>
			vector<TYPE> _backup;

		public:

			/// <summary>More details are given in specification of <see cref="GaSizableCode::Remove" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual void GACALL Remove(int start,
				int size)
			{
				if( size > 0 && start >=0 && start < (int)_values.size() )
					_values.erase( _values.begin() + start, _values.begin() + ( start + size ) );
			}

			/// <summary>More details are given in specification of <see cref="GaSizableCode::Insert" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual void GACALL Insert(int start,
				GaCodeValue* data,
				int size)
			{
				if( !data || size <= 0 || start < 0 || start > (int)_values.size() )
					return;

				// insert at the end
				if( start == _values.size() )
				{
					for( int i = 0; i < size; i++ )
						_values.push_back( this->GetClosestValue( ( (GaChromosomeValue<TYPE>*)data )[ i ].GetValue(), i ) );
				}
				else
				{
					vector<TYPE> tmp;
					tmp.reserve( size );
					for( int i = 0; i < size; i++ )
						tmp.push_back( this->GetClosestValue( ( (GaChromosomeValue<TYPE>*)data )[ i ].GetValue(), i ) );

					_values.insert( _values.begin() + start, tmp.begin(), tmp.end() );
				}
			}

			/// <summary>If swapping two parts overlap, position of second part is moved after the end of first part.
			///
			/// More details are given in specification of <see cref="GaSwapableCode::Swap" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual void GACALL Swap(int start1,
				int size1,
				int start2,
				int size2)
			{
				// check params
				if( start1 < 0 || start1 >= (int)_values.size() || start2 < 0 || start2 >= (int)_values.size()
					|| size1 <= 0 || size2 <= 0 || !_values.size() || start1 == start2 )
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

				// temp buffer for placing new code
				vector<TYPE> newCode;
				newCode.reserve( _values.size() );

				// copy values before first swapping sequence
				newCode.insert( newCode.begin(), _values.begin(), _values.begin() + start1 );
				// place second swapping sequence to new location
				newCode.insert( newCode.end(), _values.begin() + start2, _values.begin() + ( start2 + size2 ) );
				// copy values between two swapping sequence
				newCode.insert( newCode.end(), _values.begin() + ( start1 + size1 ), _values.begin() + start2 );
				// place first swapping sequence to new location
				newCode.insert( newCode.end(), _values.begin() + start1, _values.begin() + ( start1 + size1 ) );
				// copy values after second swapping sequence
				newCode.insert( newCode.end(), _values.begin() + ( start2 + size2 ),  _values.end() );

				// save new code
				_values = newCode;
			}

			/// <summary>More details are given in specification of <see cref="GaMutableCode::Flip" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual void GACALL Flip(int start,
				int size)
			{
				if( size <= 0 || start < 0 || start >= (int)_values.size() )
					return;

				typename vector<TYPE>::iterator it = _values.begin() + start;
				for( int i = 0; i < size; i++, it++ )
					*it = ( (GaChromosomeDomainBlock<TYPE>*)this->_configBlock )->GetValueSet( i )->GenerateRandom();
			}

			/// <summary>More details are given in specification of <see cref="GaMutableCode::Invert" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual void GACALL Invert(int start,
				int size)
			{
				if( size <= 0 || start < 0 || start >= (int)_values.size() )
					return;

				typename vector<TYPE>::iterator it = _values.begin() + start;
				for( int i = 0; i < size; i++, it++ )
					( (GaChromosomeDomainBlock<TYPE>*)this->_configBlock )->GetValueSet( i )->Inverse( *it, *it );
			}

			/// <summary>More details are given in specification of <see cref="GaMultiValueCode::MakeBuffer" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual GaCodeValuesBuffer* GACALL MakeBuffer(int size) const { return new GaCodeValuesBuffer( size * sizeof( TYPE ) ); }

			/// <summary>More details are given in specification of <see cref="GaMultiValueCode::FillBuffer" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual void GACALL FillBuffer(int pos,
				int size,
				GaCodeValuesBuffer& buffer) const
			{
				// check params
				if( size <= 0 || pos < 0 || pos >= (int)_values.size() )
					return;

				// get buffer params
				int s = pos + size < (int)_values.size() ? size : (int)_values.size() - pos;
				TYPE* b = (TYPE*)buffer.GetBuffer();
				int p = buffer.GetPosition() / sizeof( TYPE );

				// save to buffer
				for( int i = 0; i < s; i++ )
					b[ p + i ] = _values[ pos + i ];

				// move buffer pointer
				buffer.Move( s * sizeof( TYPE ) );
			}

			/// <summary>More details are given in specification of <see cref="GaMultiValueCode::FromBuffer" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual void GACALL FromBuffer(const GaCodeValuesBuffer& buffer)
			{
				_values.clear();

				// size of code
				int size = buffer.GetPosition() / sizeof( TYPE );
				_values.reserve( size );

				// copy from buffer to code
				for( int i = 0; i < size; i++ )
					_values.push_back( ( (TYPE*)buffer.GetBuffer() )[ i ] );
			}

			/// <summary>This constructor initializes chromosome with CCB and user-defined code.</summary>
			/// <param name="values">values of chromosome's code.</param>
			/// <param name="size">size of chromosome's code.</param>
			/// <param name="configBlock">pointer to CCB.</param>
			GaMultiValueChromosome(TYPE* values,
				int size,
				GaChromosomeDomainBlock<TYPE>* configBlock) : GaDomainChromosome<TYPE>(configBlock)
			{
				if( size > 0 && values )
				{
					_values.reserve( size );

					for( int i = 0; i < size; i++ )
						_values.push_back( GetClosestValue( values[ i ] ), i );
				}
			}

			/// <summary>This constructor initializes chromosome with CCB and random code with defined size.</summary>
			/// <param name="size">size of chromosome's code.</param>
			/// <param name="configBlock">pointer to CCB.</param>
			GaMultiValueChromosome(int size,
				GaChromosomeDomainBlock<TYPE>* configBlock) : GaDomainChromosome<TYPE>(configBlock)
			{
				_values.reserve( size );

				for( int i = 0; i < size; i++ )
					_values.push_back( ( (GaChromosomeDomainBlock<TYPE>*)this->_configBlock )->GetValueSet( i )->GenerateRandom() );
			}

			/// <summary>This constructor initializes chromosome with CCB.</summary>
			/// <param name="configBlock">pointer to CCB.</param>
			GaMultiValueChromosome(GaChromosomeDomainBlock<TYPE>* configBlock) : GaDomainChromosome<TYPE>(configBlock) { }

			/// <summary>This is copy constructor.</summary>
			/// <param name="c">reference to chromosome which is copied.</param>
			/// <param name="setupOnly">if this parameter is <c>true</c>, only pointer to CCB is copied. If this parameter is <c>false</c>,
			/// chromosome's data and CCB is copied.</param>
			GaMultiValueChromosome(const GaMultiValueChromosome<TYPE>& c,
				bool setupOnly) : GaDomainChromosome<TYPE>(c, setupOnly)
			{
				// copy code to new chromosome
				if( !setupOnly )
					_values = c._values;
			}

			/// <summary>More details are given in specification of <see cref="GaChromosome::MakeCopy" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual GaChromosomePtr GACALL MakeCopy(bool setupOnly) const { return new GaMultiValueChromosome( *this, setupOnly ); }

			/// <summary>This method uses <see cref="MakeCopy" /> method to create chromosome's object, so if additional steps are not needed
			/// for creating chromosome from prototype, classes which inherits this class don't have to override this method.
			/// 
			/// More details are given in specification of <see cref="GaChromosome::MakeNewFromPrototype" /> method.</summary>
			virtual GaChromosomePtr GACALL MakeNewFromPrototype() const
			{
				// make chromosome with exact setup
				GaChromosomePtr newPtr = MakeCopy( true );
				GaMultiValueChromosome* newChromosome = dynamic_cast<GaMultiValueChromosome*>( &( *newPtr ) );

				// generate random chromosome code
				if( _values.size() )
				{
					vector<TYPE>& collection = newChromosome->_values;
					collection.reserve( _values.size() );

					for( int i = 0; i < (int)_values.size(); i++ )
						collection.push_back( ( (GaChromosomeDomainBlock<TYPE>*)this->_configBlock )->GetValueSet( i )->GenerateRandom() );
				}

				return newPtr;
			}

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method always returns number of values in chromosome's code.</returns>
			virtual int GACALL GetCodeSize() const { return (int)_values.size(); }

			/// <summary><c>GetAt</c> method returns value of chromosome's at specified position.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="pos">position of the value.</param>
			/// <returns>Method returns value of chromosome's code at specified position.</returns>
			inline TYPE GACALL GetAt(int pos) const { return _values[ pos ]; }

			/// <summary><c>SetAt</c> method sets value of chromosome's code at given position.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="value">new value.</param>
			/// <param name="pos">position of value.</param>
			inline void GACALL SetAt(TYPE value,
				int pos) { _values[ pos ] = GetClosestValue( value, pos ); }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns reference to vector of values which represent chromosome's code.</returns>
			inline const vector<TYPE>& GACALL GetCode() const { return _values; }

			/// <summary>More details are given in specification of <see cref="GaChromosome::operator =" />.
			///
			/// This method is not thread-safe.</summary>
			virtual GaChromosome& GACALL operator =(const GaChromosome& rhs)
			{
				const vector<TYPE>& c = dynamic_cast<const GaMultiValueChromosome&>( rhs )._values;
				_values = c;

				return GaDefaultChromosome::operator =( rhs );
			}

			/// <summary>More details are given in specification of <see cref="GaChromosome::operator ==" />.
			///
			/// This method is not thread-safe.</summary>
			virtual bool GACALL operator ==(const GaChromosome& c) const
			{
				const vector<TYPE>& b = dynamic_cast<const GaMultiValueChromosome&>( c )._values;

				// compare length of chromosomes
				if( b.size() != _values.size() )
					return false;

				// compare values
				for( int i = (int)_values.size() - 1; i >= 0; i-- )
				{
					if( b[ i ] != _values[ i ] )
						return false;
				}

				return true;
			}

		protected:

			/// <summary><c>PreapareForMutation</c> method copies values form chromosome's code to backup vector.</summary>
			virtual void GACALL PreapareForMutation() { _backup.insert( _backup.begin(), _values.begin(), _values.end() ); }

			/// <summary>This method only clears backup vector.</summary>
			virtual void GACALL AcceptMutation() { _backup.clear(); }

			/// <summary><c>RejectMutation</c> method restores chromosome's code from backup vector. It also clears backup vector.</summary>
			virtual void GACALL RejectMutation()
			{
				_values.clear();
				_values = _backup;
				_backup.clear();
			}

		};// END CLASS DEFINITION GaMultiValueChromosome

		/// <summary>This class can be used for chromosomes which code has multiple values that support arithmetic operations. Values can be of any type,
		/// but all values must have same type, they must use same value set and must obey requirements introduced by the value set.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		/// </summary>
		/// <param name="TYPE">type of value in chromosome's code. This type must support binary operators
		/// <c>+</c>, <c>-</c>, <c>*</c>, <c>/</c> and <c>/</c> with right hand operand of <c>int</c> type.</param>
		template <typename TYPE>
		class GaMVArithmeticChromosome : public GaMultiValueChromosome<TYPE>,
			public GaArithmeticalCode
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaArithmeticalCode::operator +" />.
			///
			/// This method is not thread-safe.</summary>
			GaChromosomePtr GACALL operator +(const GaArithmeticalCode& rhs) const
			{
				GaChromosomePtr newPtr = MakeCopy( true );
				GaMVArithmeticChromosome* newChromosome = 
					dynamic_cast<GaMVArithmeticChromosome*>( &( *newPtr ) );
				vector<TYPE> b = dynamic_cast<const GaMVArithmeticChromosome&>( rhs )._values;

				// size of new chromosome
				int s = (int)( b.size() <= this->_values.size() ? b.size() : this->_values.size() );
				vector<TYPE>& res = newChromosome->_values;
				res.reserve( s );

				// make new code
				for( int i = 0; i < s; i++ )
					res.push_back( GetClosestValue( this->_values[ i ] + b[ i ], i ) );

				return newPtr;
			}

			/// <summary>More details are given in specification of <see cref="GaArithmeticalCode::operator -" />.
			///
			/// This method is not thread-safe.</summary>
			GaChromosomePtr GACALL operator -(const GaArithmeticalCode& rhs) const
			{
				GaChromosomePtr newPtr = MakeCopy( true );
				GaMVArithmeticChromosome* newChromosome =  
					dynamic_cast<GaMVArithmeticChromosome*>( &( *newPtr ) );
				vector<TYPE> b = dynamic_cast<const GaMVArithmeticChromosome&>( rhs )._values;

				// size of new chromosome
				int s = (int)( b.size() <= this->_values.size() ? b.size() : this->_values.size() );
				vector<TYPE>& res = newChromosome->_values;
				res.reserve( s );

				// make new code
				for( int i = 0; i < s; i++ )
					res.push_back( GetClosestValue( this->_values[ i ] - b[ i ], i ) );

				return newPtr;
			}

			/// <summary>More details are given in specification of <see cref="GaArithmeticalCode::operator *" />.
			///
			/// This method is not thread-safe.</summary>
			GaChromosomePtr GACALL operator *(const GaArithmeticalCode& rhs) const
			{
				GaChromosomePtr newPtr = MakeCopy( true );
				GaMVArithmeticChromosome* newChromosome = 
					dynamic_cast<GaMVArithmeticChromosome*>( &( *newPtr ) );
				vector<TYPE> b = dynamic_cast<const GaMVArithmeticChromosome&>( rhs )._values;

				// size of new chromosome
				int s = (int)( b.size() <= this->_values.size() ? b.size() : this->_values.size() );
				vector<TYPE>& res = newChromosome->_values;
				res.reserve( s );

				// make new code
				for( int i = 0; i < s; i++ )
					res.push_back( GetClosestValue( this->_values[ i ] * b[ i ], i ) );

				return newPtr;
			}

			/// <summary>More details are given in specification of <see cref="GaArithmeticalCode::operator /" />.
			///
			/// This method is not thread-safe.</summary>
			GaChromosomePtr GACALL operator /(const GaArithmeticalCode& rhs) const
			{
				GaChromosomePtr newPtr = MakeCopy( true );
				GaMVArithmeticChromosome* newChromosome = 
					dynamic_cast<GaMVArithmeticChromosome*>( &( *newPtr ) );
				vector<TYPE> b = dynamic_cast<const GaMVArithmeticChromosome&>( rhs )._values;

				// size of new chromosome
				int s = (int)( b.size() <= this->_values.size() ? b.size() : this->_values.size() );
				vector<TYPE>& res = newChromosome->_values;
				res.reserve( s );

				// make new code
				for( int i = 0; i < s; i++ )
					res.push_back( GetClosestValue( this->_values[ i ] / b[ i ], i ) );

				return newPtr;
			}

			/// <summary>More details are given in specification of <see cref="GaArithmeticalCode::Midpoint" /> method.
			///
			/// This method is not thread-safe.</summary>
			GaChromosomePtr GACALL Midpoint(const GaArithmeticalCode& c) const
			{
				GaChromosomePtr newPtr = MakeCopy( true );
				GaMVArithmeticChromosome* newChromosome = 
					dynamic_cast<GaMVArithmeticChromosome*>( &( *newPtr ) );
				vector<TYPE> b = dynamic_cast<const GaMVArithmeticChromosome&>( c )._values;

				// size of new chromosome
				int s = (int)( b.size() <= this->_values.size() ? b.size() : this->_values.size() );
				vector<TYPE>& res = newChromosome->_values;
				res.reserve( s );

				// make new code
				for( int i = 0; i < s; i++ )
					res.push_back( GetClosestValue( ( this->_values[ i ] + b[ i ] ) / 2, i ) );

				return newPtr;
			}

			/// <summary>This constructor initializes chromosome with CCB and user-defined code.</summary>
			/// <param name="values">values of chromosome's code.</param>
			/// <param name="size">size of chromosome's code.</param>
			/// <param name="configBlock">pointer to CCB.</param>
			GaMVArithmeticChromosome(TYPE* values,
				int size,
				GaChromosomeDomainBlock<TYPE>* configBlock) :
				GaMultiValueChromosome<TYPE>(values, size, configBlock) { }

			/// <summary>This constructor initializes chromosome with CCB and random code with defined size.</summary>
			/// <param name="size">size of chromosome's code.</param>
			/// <param name="configBlock">pointer to CCB.</param>
			GaMVArithmeticChromosome(int size,
				GaChromosomeDomainBlock<TYPE>* configBlock) : GaMultiValueChromosome<TYPE>(size, configBlock) { }

			/// <summary>This constructor initializes chromosome with CCB.</summary>
			/// <param name="configBlock">pointer to CCB.</param>
			GaMVArithmeticChromosome(GaChromosomeDomainBlock<TYPE>* configBlock) : 
				GaMultiValueChromosome<TYPE>(configBlock) { }

			/// <summary>This is copy constructor.</summary>
			/// <param name="c">reference to chromosome which is copied.</param>
			/// <param name="setupOnly">if this parameter is <c>true</c>, only pointer to CCB is copied. If this parameter is <c>false</c>,
			/// chromosome's data and CCB is copied.</param>
			GaMVArithmeticChromosome(const GaMVArithmeticChromosome<TYPE>& c,
				bool setupOnly) : GaMultiValueChromosome<TYPE>(c, setupOnly) { }

			/// <summary>More details are given in specification of <see cref="GaChromosome::MakeCopy" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual GaChromosomePtr GACALL MakeCopy(bool setupOnly) const { return new GaMVArithmeticChromosome( *this, setupOnly  ); }

		};// END CLASS DEFINITION GaMVArithmeticChromosome

	} // Representation
} // Chromosome

#endif // __GA_MULTI_VALUE_CHROMOSOME_H__
