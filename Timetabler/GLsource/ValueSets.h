
/*! \file ValueSets.h
    \brief This file declares and implements value sets classes.
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

#ifndef __GA_VALUE_SETS_H__
#define __GA_VALUE_SETS_H__

#include <vector>
#include "Platform.h"
#include "GlobalRandomGenerator.h"
#include "Threading.h"

using namespace std;

namespace Chromosome
{
	namespace Representation
	{

		/// <summary>This template class is base class for all value set. Value sets store values which can be found in chromosomes' codes. 
		/// 
		/// This class has built-in synchronizator so it is allowed to use <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros
		/// with instances of this class, but no public or private methods are thread-safe.</summary>
		/// <param name="T">type of data in value set.</param>
		template <typename T>
		class GaValueSet
		{
			DEFINE_SYNC_CLASS

		protected:

			/// <summary>This attribute indicates that membership of original sets of values and inverted set of values are treated equally.
			/// If this attribute is set to <c>true</c>, inverted values are treated as members of the set, if it is set to <c>false</c>,
			/// inverted values are not treated as members of the set.</summary>
			bool _viceVersa;

		public:

			/// <summary>This constructor initializes value set with user-defined treatment of inverted values.</summary>
			/// <param name="viceVersa">if this parameter is set to <c>true</c>, inverted values are treated as members of the set,
			/// if it is set to <c>false</c>, inverted values are not treated as members of the set.</param>
			GaValueSet(bool viceVersa) : _viceVersa(viceVersa) { }

			/// <summary>This constructor initializes value set with inverted values as members of the set.</summary>
			GaValueSet() : _viceVersa(true) { }

			/// <summary>Because this is base class, virtual destructor must be defined in order to enable calling of right destructor
			/// (destructor of inheriting class).</summary>
			virtual ~GaValueSet() { }

			/// <summary>This method returns randomly chosen value from the set.</summary>
			/// <returns>Method returns randomly chosen value from the set.</returns>
			virtual T GACALL GenerateRandom() const=0;

			/// <summary><c>Inverse</c> method finds inverted value and stores it into inverted.</summary>
			/// <param name="value">value which should be inverted.</param>
			/// <param name="inverted">reference to variable to which inverted value is going to be stored.</param>
			/// <returns>Method returns <c>true</c> if inversion is successful.
			/// If specified value is not in set or it cannot be inverted it returns <c>false</c>.</returns>
			virtual bool GACALL Inverse(const T& value,
				T& inverted) const=0;

			/// <summary><c>Belongs</c> method checks membership of the value in this value set.</summary>
			/// <param name="value">value which is checked.</param>
			/// <returns>Method returns <c>true</c> if the value is member of the set.</returns>
			virtual bool GACALL Belongs(const T& value) const=0;

			/// <summary><c>ClosestValue</c> method returns nearest value which can be found in value set to specified value.</summary>
			/// <param name="value">value which nearest neighbor should be found.</param>
			/// <returns>Method returns reference to nearest value in value set to specified value.</returns>
			virtual const T& GACALL ClosestValue(const T& value) const=0;

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns <c>true</c> if both, original values and inverted values, treated as members of the set, or returns <c>false</c>,
			/// if inverted values are not treated as members.</returns>
			inline bool GACALL GetViceVersa() const { return _viceVersa; }

			/// <summary><c>SetViceVersa</c> method sets treatment of inverted values.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="viceVersa">if this parameter is set to <c>true</c>, inverted values are treated as members of the set, if it is set to <c>false</c>,
			/// inverted values are not treated as members of the set.</param>
			inline void GACALL SetViceVersa(bool viceVersa) { _viceVersa = viceVersa; }

		};// END CLASS DEFINITION GaValueSet

		/// <summary>This template represents value set with only one value and its counterpart (inverted value).
		/// 
		/// This class has built-in synchronizator so it is allowed to use <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros
		/// with instances of this class, but no public or private methods are thread-safe.</summary>
		/// <param name="T">type of data in value set.</param>
		template <typename T>
		class GaSingleValueSet : public GaValueSet<T>
		{

		private:

			/// <summary>Original value of the set.</summary>
			T _value;

			/// <summary><see cref="_value" />'s counterpart (inverted value) in the set.</summary>
			T _invertedValue;

		public:

			/// <summary>This constructor initializes value set with value and its counterpart.</summary>
			/// <param name="value">original value.</param>
			/// <param name="invertedValue">inverted value.</param>
			/// <param name="viceVersa">if this parameter is set to <c>true</c>, inverted values are treated as members of the set,
			/// if it is set to <c>false</c>, inverted values are not treated as members of the set.</param>
			GaSingleValueSet(const T& value,
				const T& invertedValue,
				bool viceVersa) : GaValueSet<T>(viceVersa),
				_value(value),
				_invertedValue(invertedValue) { }

			/// <summary>More details are given in specification of <see cref="GaValueSet::GenerateRandom" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual T GACALL GenerateRandom() const { return !this->_viceVersa || GaGlobalRandomBoolGenerator->Generate() ? this->_value : _invertedValue; }

			/// <summary>More details are given in specification of <see cref="GaValueSet::Inverse" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual bool GACALL Inverse(const T& value,
				T& inverted) const
			{
				if( value == _value )
				{
					inverted = _invertedValue;
					return true;
				}

				if( this->_viceVersa && value == _invertedValue )
				{
					inverted = _value;
					return true;
				}

				return false;
			}

			/// <summary>More details are given in specification of <see cref="GaValueSet::Belongs" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual bool GACALL Belongs(const T& value) const { return value == _value || ( this->_viceVersa && value == _invertedValue ); }

			/// <summary>More details are given in specification of <see cref="GaValueSet::ClosestValue" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual const T& GACALL ClosestValue(const T& value) const { return _value; }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns original value.</returns>
			inline const T& GACALL GetValue() const { return _value; }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns inverted value of the original.</returns>
			inline const T& GACALL GetInvertedValue() const { return _invertedValue; }

			/// <summary><c>GetValue</c> method returns original value and its counterpart.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="original">reference to variable to which original value will be stored.</param>
			/// <param name="inverted">reference to variable to which inverted value will be stored.</param>
			inline void GACALL GetValue(T& original,
				T& inverted)
			{
				original = _value;
				inverted = _invertedValue;
			}

			/// <summary><c>SetValue</c> method sets original value.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="value">new original value.</param>
			inline void GACALL SetValue(const T& value) { _value = value; }

			/// <summary><c>SetValue</c> method sets original value and its counterpart.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="original">new original value.</param>
			/// <param name="inverted">new inverted value.</param>
			inline void GACALL SetValue(const T& original,
				const T& inverted)
			{
				_value = original;
				_invertedValue = inverted;
			}

			/// <summary><c>SetValue</c> method sets inverted value.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="value">new inverted value.</param>
			inline void GACALL SetInvertedValue(const T& value) { _invertedValue = value; }

		};// END CLASS DEFINITION GaSingleValueSet

		/// <summary>This template represents value set with multiple values and their counterpart (inverted values).
		/// 
		/// This class has built-in synchronizator so it is allowed to use <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros
		/// with instances of this class, but no public or private methods are thread-safe.</summary>
		/// <param name="T">type of data in value set.</param>
		template <typename T>
		class GaMultiValueSet : public GaValueSet<T>
		{

		private:

			/// <summary>Array of original values.</summary>
			vector<T> _values;

			/// <summary>array of counterparts of values in <c>_values</c> (inverted values).</summary>
			vector<T> _invertedValues;

		public:

			/// <summary>This constructor initializes empty value set with user-defined treatment of inverted values.</summary>
			/// <param name="viceVersa">if this parameter is set to <c>true</c>, inverted values are treated as members of the set,
			/// if it is set to <c>false</c>, inverted values are not treated as members of the set.</param>
			GaMultiValueSet(bool viceVersa) : GaValueSet<T>( viceVersa ) { }

			/// <summary>This constructor initializes empty value set with inverted values as members of the set.</summary>
			GaMultiValueSet() { }

			/// <summary>More details are given in specification of <see cref="GaValueSet::GenerateRandom" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual T GACALL GenerateRandom() const
			{
				return !this->_viceVersa || GaGlobalRandomBoolGenerator->Generate()
					? _values[ GaGlobalRandomIntegerGenerator->Generate( (int)_values.size() - 1 ) ]
					: _invertedValues[ GaGlobalRandomIntegerGenerator->Generate( (int)_invertedValues.size() - 1 ) ];
			}

			/// <summary>More details are given in specification of <see cref="GaValueSet::Inverse" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual bool GACALL Inverse(const T& value,
				T& inverted) const
			{
				int pos = 0;
				for( typename vector<T>::const_iterator it = _values.begin(); it != _values.end(); it++, pos++ )
				{
					if( value == *it )
					{
						inverted = _invertedValues[ pos ];
						return true;
					}
				}

				if( this->_viceVersa )
				{
					pos = 0;
					for( typename vector<T>::const_iterator it = _invertedValues.begin(); it != _invertedValues.end(); it++, pos++ )
					{
						if( value == *it )
						{
							inverted = _values[ pos ];
							return true;
						}
					}
				}

				return false;
			}

			/// <summary>More details are given in specification of <see cref="GaValueSet::Belongs" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual bool GACALL Belongs(const T& value) const
			{
				for( typename vector<T>::const_iterator it = _values.begin(); it != _values.end(); it++ )
				{
					if( value == *it )
						return true;
				}

				if( this->_viceVersa )
				{
					for( typename vector<T>::const_iterator it = _invertedValues.begin(); it != _invertedValues.end(); it++ )
					{
						if( value == *it )
							return true;
					}
				}

				return false;
			}

			/// <summary>More details are given in specification of <see cref="GaValueSet::ClosestValue" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual const T& GACALL ClosestValue(const T& value) const
			{
				const T* closest = &_values[ 0 ];
				T diff = value < _values[ 0 ] ? _values[ 0 ] - value : value - _values[ 0 ];

				// find closest value
				for( typename vector<T>::const_iterator it = _values.begin() + 1; it != _values.end(); it++ )
				{
					if( value == *it )
						return value;

					// calculate the distance
					T d = value > *it ? value - *it : *it - value;

					// closer
					if( d < diff )
					{
						closest = &*it;
						diff = d;
					}
				}

				if( this->_viceVersa )
				{
					// find closest value
					for( typename vector<T>::const_iterator it = _invertedValues.begin(); it != _invertedValues.end(); it++ )
					{
						if( value == *it )
							return value;
						else if( value > *it )
						{
							// closer
							if( value - *it < *closest )
								closest = &*it;
						}
						else
						{
							// closer
							if( *it - value < *closest )
								closest = &*it;
						}
					}
				}

				return *closest;
			}

			/// <summary><c>Add</c> method inserts new value and its counterpart into value set.
			/// If original value is already in set call fails and method returns <c>false</c>.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="original">original value which is going to be added.</param>
			/// <param name="inverted">inverted value, counterpart of original value.</param>
			/// <returns>Method returns <c>true</c> if new value is added successfully.
			/// If original value already exists in set, method returns <c>false</c>.</returns>
			bool GACALL Add(const T& original,
				const T& inverted)
			{
				if( Belongs( original ) || ( this->_viceVersa && Belongs( inverted ) ) )
					return false;

				_values.push_back( original );
				_invertedValues.push_back( inverted );

				return true;
			}

			/// <summary><c>Add</c> method inserts new values and their counterpart into value set. New values which has duplicates in the set are ignored.
			/// Both arrays of values must be the same size.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="values">array of original values which are going to be added.</param>
			/// <param name="invertedValues">array of inverted values, counterparts of original values.</param>
			/// <param name="size">number of values which are going to be added.</param>
			/// <returns>Method returns number of successfully inserted values into set.</returns>
			int Add(const T* values,
				const T* invertedValues,
				int size)
			{
				int added = 0;

				for( int i = size - 1; i >= 0; i-- )
				{
					if( !( Belongs( values[ i ] ) || ( this->_viceVersa && Belongs( invertedValues[ i ] ) ) ) )
					{
						_values.push_back( values[ i ] );
						_invertedValues.push_back( invertedValues[ i ] );

						added++;
					}
				}

				return added;
			}

			/// <summary><c>Remove</c> method removes value and its counterpart from the set.
			/// It removes value from the set if it's equal to specified value or if specified value is equal to inverted value.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="value">value which is should be removed.</param>
			/// <returns>Method returns <c>true</c> if it removed value successfully. It value doesn't belong to the set, method returns <c>false</c>.</returns>
			bool GACALL Remove(const T& value)
			{
				typename vector<T>::iterator it1 = _values.begin();
				typename vector<T>::iterator it2 = _invertedValues.begin();

				for( ; it1 != _values.end(); it1++, it2++ )
				{
					if( value == *it1 || ( this->_viceVersa && value == *it2 ) )
					{
						_values.erase( it1 );
						_invertedValues.erase( it2 );
						return false;
					}
				}
			}

			/// <summary>Remove method removes value and its counterpart from the set. It removes value by its position in the set.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="pos">position of value which is should be removed.</param>
			/// <returns>Method returns <c>true</c> if it removed value successfully. It position is out of range it returns <c>false</c>.</returns>
			bool GACALL Remove(int pos)
			{
				typename vector<T>::iterator it1 = _values.begin();
				typename vector<T>::iterator it2 = _invertedValues.begin();

				for( int i = 0; it1 != _values.end(); it1++, it2++, i++ )
				{
					if( i == pos )
					{
						_values.erase( it1 );
						_invertedValues.erase( it2 );

						return false;
					}
				}
			}

			/// <summary><c>GetValue</c> method returns original value at given position.
			/// 
			/// This method is not thread-safe.</summary>
			/// <param name="pos">position of the value.</param>
			/// <returns>Method returns original value at given position.</returns>
			inline const T& GACALL GetValue(int pos) const { return _values[ pos ]; }

			/// <summary>GetValue method returns value and its counterpart at given position.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="pos">position of the value.</param>
			/// <param name="original">reference to variable to which original value is saved.</param>
			/// <param name="inverted">reference to variable to which inverted value is saved.</param>
			inline void GACALL GetValue(int pos,
				T& original,
				T& inverted)
			{
				original = _values[ pos ];
				inverted = _invertedValues[ pos ];
			}

			/// <summary><c>GetInvertedValue</c> method returns inverted value at given position.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="pos">position of the value.</param>
			/// <returns>Method returns inverted value at given position.</returns>
			inline const T& GACALL GetInvertedValue(int pos) const { return _invertedValues[ pos ]; }

			/// <summary><c>SetValue</c> method sets original value at given position.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="pos">position of the value.</param>
			/// <param name="value">new original value.</param>
			inline void GACALL SetValue(int pos,
				const T& value) { _values[ pos ] = value; }

			/// <summary><c>SetValue</c> method sets value and its counterpart at given position.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="pos">position of the value.</param>
			/// <param name="original">new original value.</param>
			/// <param name="inverted">new inverted value.</param>
			inline void GACALL SetValue(int pos,
				const T& original,
				const T& inverted)
			{
				_values[ pos ] = original;
				_invertedValues[ pos ] = inverted;
			}

			/// <summary><c>SetInvertedValue</c> method sets inverted value at given position.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="pos">position of the value.</param>
			/// <param name="value">new inverted value.</param>
			inline void GACALL SetInvertedValue(int pos,
				const T& value) { _invertedValues[ pos ] = value; }

		};// END CLASS DEFINITION GaMultiValueSet

		/// <summary>This template class represents bounds of interval value set.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		/// <param name="T">type of bounds. This type must support operators &gt;, &gt;=, &lt; and &lt;=.</param>
		template <typename T>
		class GaValueIntervalBounds
		{

		private:

			/// <summary>Lower bound.</summary>
			T _lower;

			/// <summary>Higher bound.</summary>
			T _higher;

		public:

			/// <summary>This constructor initializes bounds with user-defined values. Bounds are automatically sorted.</summary>
			/// <param name="lower">value of lower bound.</param>
			/// <param name="higher">value of higher bound.</param>
			GaValueIntervalBounds(const T& lower,
				const T& higher)
			{
				if( lower <= higher )
				{
					_lower = lower;
					_higher = higher;
				}
				else
				{
					_lower = higher;
					_higher = lower;
				}
			}

			/// <summary>This constructor creates undefined bounds.</summary>
			GaValueIntervalBounds() { }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns lower bound.</returns>
			inline const T& GACALL GetLowerBound() const { return _lower; }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns higher bound.</returns>
			inline const T& GACALL GetHigherBound() const { return _higher; }

			/// <summary><c>GetBounds</c> method returns both bounds.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="lower">reference to variable to which lower bound is going to be stored.</param>
			/// <param name="higher">reference to variable to which higher bound is going to be stored.</param>
			inline void GACALL GetBounds(T& lower,
				T& higher)
			{
				lower = _lower;
				higher = _higher;
			}

			/// <summary><c>SetLowerBound</c> method sets new lower bound. If new value for lower bound is higher then higher bound, call is ignored.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="lower">new value for lower bound.</param>
			inline void GACALL SetLowerBound(const T& lower)
			{
				if( lower <= _higher )
					_lower = lower;
			}

			/// <summary><c>SetHigherBound</c> method sets new higher bound. If new value for higher bound is lower then lower bound, call is ignored.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="higher">new value for higher bound.</param>
			inline void GACALL SetHigherBound(const T& higher)
			{
				if( _lower <= higher )
					_higher = higher;
			}

			/// <summary><c>SetBound</c> method sets both bounds. Bounds are automatically sorted.
			/// 
			/// This method is not thread-safe.</summary>
			/// <param name="lower">new value for lower bound.</param>
			/// <param name="higher">new value for higher bound.</param>
			void GACALL SetBounds(const T& lower,
				const T& higher)
			{
				if( lower <= higher )
				{
					_lower = lower;
					_higher = higher;
				}
				else
				{
					_lower = higher;
					_higher = lower;
				}
			}

			/// <summary><c>InBounds</c> method checks value to see if it is in bounds of the interval.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="value">value which is checked.</param>
			/// <returns>Method returns <c>true</c> if value is in bound, otherwise it returns <c>false</c>.</returns>
			inline bool GACALL InBounds(const T& value) const  { return value >= _lower && value <= _higher; }

		};// END CLASS DEFINITION GaValueIntervalBounds

		/// <summary>This template represents value set which has interval of values. Intervals as specified by theirs bounds.
		/// This value set uses user-specified random generator to generate values within the interval. 
		/// 
		/// This class has built-in synchronizator so it is allowed to use <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros
		/// with instances of this class, but no public or private methods are thread-safe.</summary>
		/// <param name="T">type of bounds. This type must support operators &gt;, &gt;=, &lt; and &lt;=.</param>
		template <typename T>
		class GaIntervalValueSet : public GaValueSet<T>
		{

		private:

			/// <summary>Bounds of interval of original values. </summary>
			GaValueIntervalBounds<T> _values;

			/// <summary>Bounds of interval of inverted values.</summary>
			GaValueIntervalBounds<T> _invertedValues;

			/// <summary>This attribute is pointer to random generator which is used for generating of random values within interval.</summary>
			GaRandom<T>* _randomGenerator;

		public:

			/// <summary>This constructor initializes value set with bounds and random generator.</summary>
			/// <param name="values">bounds of interval of original values.</param>
			/// <param name="invertedValues">bounds of interval of inverted values.</param>
			/// <param name="randomGenerator">pointer to random generator.</param>
			/// <param name="viceVersa">if this parameter is set to <c>true</c>, inverted values are treated as members of the set,
			/// if it is set to <c>false</c>, inverted values are not treated as members of the set.</param>
			GaIntervalValueSet(const GaValueIntervalBounds<T>& values,
				const GaValueIntervalBounds<T>& invertedValues,
				GaRandom<T>* randomGenerator,
				bool viceVersa) : GaValueSet<T>(viceVersa),
				_values(values),
				_invertedValues(invertedValues),
				_randomGenerator(randomGenerator) { }

			/// <summary>This constructor initializes value set with random generator, but doesn't define bounds of intervals.</summary>
			/// <param name="randomGenerator">pointer to random generator.</param>
			GaIntervalValueSet(GaRandom<T>* randomGenerator) : _randomGenerator(randomGenerator) { }

			/// <summary>More details are given in specification of <see cref="GaValueSet::GenerateRandom" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual T GACALL GenerateRandom() const
			{
				return !this->_viceVersa || GaGlobalRandomBoolGenerator->Generate()
					? _randomGenerator->Generate( _values.GetLowerBound(), _values.GetHigherBound() )
					: _randomGenerator->Generate( _invertedValues.GetLowerBound(), _invertedValues.GetHigherBound() );
			}

			/// <summary>More details are given in specification of <see cref="GaValueSet::Inverse" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual bool GACALL Inverse(const T& value,
				T& inverted) const
			{				
				if( _values.InBounds( value ) || ( this->_viceVersa && _invertedValues.InBounds( value ) ) )
				{
					inverted = _invertedValues.GetLowerBound() + _values.GetHigherBound() - value;
					return true;
				}

				return false;
			}

			/// <summary>More details are given in specification of <see cref="GaValueSet::ClosestValue" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual bool GACALL Belongs(const T& value) const { return _values.InBounds( value ) || ( this->_viceVersa && _invertedValues.InBounds( value ) ); }

			/// <summary>More details are given in specification of <see cref="GaValueSet::ClosestValue" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual const T& GACALL ClosestValue(const T& value) const
			{
				// value belongs to interval?
				if( _values.InBounds( value ) || ( this->_viceVersa && _invertedValues.InBounds( value ) ) )
					return value;

				// array of bounds for finding the closest
				const T* bounds[ 4 ];
				bounds[ 0 ] = &_values.GetLowerBound();
				bounds[ 1 ] = &_values.GetHigherBound();
				bounds[ 2 ] = &_invertedValues.GetLowerBound();
				bounds[ 3 ] = &_invertedValues.GetHigherBound();

				// start of search
				int i = this->_viceVersa ? 2 : 0;
				int ret = i + 1;
				T diff = value < *bounds[ ret ] ? *bounds[ ret ] - value : value - *bounds[ ret ];

				// find closes bounds
				for( ; i >= 0; i-- )
				{
					// calculate distance
					T d = value > *bounds[ i ] ? value - *bounds[ i ] : *bounds[ i ] - value;

					// closer
					if( d < diff )
					{
						ret = i;
						diff = d;
					}
				}

				return *bounds[ ret ];
			}

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns bounds of interval of original values.</returns>
			inline const GaValueIntervalBounds<T>& GACALL GetValueBounds() const { return _values; }

			/// <summary><c>SetValueBounds</c> method sets bounds of interval of original values.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="bounds">new bound of interval.</param>
			inline void GACALL SetValueBounds(const GaValueIntervalBounds<T>& bounds) { _values = bounds; }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns bounds of interval of inverted values.</returns>
			inline const GaValueIntervalBounds<T>& GACALL GetInvertedValueBounds() const { return _invertedValues; }

			/// <summary><c>SetInvertedValueBounds</c> method sets bounds of interval of inverted values.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="bounds">new bound of interval.</param>
			inline void GACALL SetInvertedValueBounds(const GaValueIntervalBounds<T>& bounds) { _invertedValues = bounds; }

			/// <summary><c>GetBounds</c> method returns bounds of both intervals.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="originals">reference to variable to which bounds of original values are stored.</param>
			/// <param name="inverted">reference to variable to which bounds of inverted values are stored. </param>
			inline void GACALL GetBounds(GaValueIntervalBounds<T>& originals,
				GaValueIntervalBounds<T>& inverted) const
			{
				originals = _values;
				inverted = _invertedValues;
			}

			/// <summary><c>SetBounds</c> method sets bounds of both intervals.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="originals">new bounds of interval of original values.</param>
			/// <param name="inverted">new bounds of interval of inverted values.</param>
			inline void GACALL SetBounds(const GaValueIntervalBounds<T>& originals,
				const GaValueIntervalBounds<T>& inverted)
			{
				_values = originals;
				_invertedValues = inverted;
			}

		};// END CLASS DEFINITION GaIntervalValueSet

		/// <summary> This template represents value set which is consisted of multiple value sets.  Value sets can be of any kind,
		/// but must contains same type of data.
		/// 
		/// This class has built-in synchronizator so it is allowed to use <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros
		/// with instances of this class, but no public or private methods are thread-safe.</summary>
		/// <param name="T">type of data in value set.</param>
		template <typename T>
		class GaCombinedValueSet : public GaValueSet<T>
		{

		private:

			/// <summary>Array of value sets.</summary>
			vector<GaValueSet<T>*> _sets;

		public:

			/// <summary>This constructor initializes empty value set.</summary>
			GaCombinedValueSet() { }

			/// <summary>More details are given in specification of <see cref="GaValueSet::GenerateRandom" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual T GACALL GenerateRandom() const
			{
				return _sets[ GaGlobalRandomIntegerGenerator->Generate( (int)_sets.size() - 1 ) ]->GenerateRandom();
			}

			/// <summary>More details are given in specification of <see cref="GaValueSet::Inverse" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual bool GACALL Inverse(const T& value,
				T& inverted) const
			{
				for( typename vector<GaValueSet<T>*>::const_iterator it = _sets.begin(); it != _sets.end(); it++ )
				{
					if( (*it)->Inverse( value, inverted ) )
						return true;
				}

				return false;
			}

			/// <summary>More details are given in specification of <see cref="GaValueSet::ClosestValue" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual bool GACALL Belongs(const T& value) const
			{
				for( typename vector<GaValueSet<T>*>::const_iterator it = _sets.begin(); it != _sets.end(); it++ )
				{
					if( (*it)->Belongs( value ) )
						return true;
				}

				return false;
			}

			/// <summary>More details are given in specification of <see cref="GaValueSet::ClosestValue" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual const T& GACALL ClosestValue(const T& value) const
			{
				// random choose
				T closest = _sets[ 0 ]->ClosestValue();

				// find closest value
				for( typename vector<GaValueSet<T>*>::const_iterator it = _sets.begin() + 1; it != _sets.end(); it++ )
				{
					// get closest value of current set
					T t = ( *it )->ClosestValue();

					// closer
					if( value - t < closest )
						closest = t;
				}

				return closest;
			}

			/// <summary><c>Add</c> method inserts new value set.
			///
			/// This method is not thread safe.</summary>
			/// <param name="set">pointer to value set which is going to be added.</param>
			void GACALL Add(GaValueSet<T>* set)
			{
				_sets.push_back(set);
			}

			/// <summary><c>Remove</c> method removes specified value set.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="set">pointer to value set which should be removed.</param>
			void GACALL Remove(GaValueSet<T>* set)
			{
				for( typename vector<GaValueSet<T>*>::iterator it = _sets.begin(); it != _sets.end(); it )
				{
					if( *it == set )
					{
						_sets.erase( it );
						return;
					}
				}
			}

			/// <summary><c>Remove</c> method removes value set at specified position.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="pos">position of the value set which is going to be removed.</param>
			void GACALL Remove(int pos)
			{
				typename vector<GaValueSet<T>*>::iterator it = _sets.begin();

				for( int i = 0; it != _sets.end(); it, i++ )
				{
					if( i == pos )
					{
						_sets.erase( it );
						return;
					}
				}
			}

			/// <summary>This method is not thread safe.</summary>
			/// <returns>Method returns number of value sets in this combined value set.</returns>
			inline int GACALL GetNumberOfSets() const { return _sets.size(); }

			/// <summary><c>operator []</c> returns reference to value set at specified position.
			///
			/// This operator is not thread-safe.</summary>
			/// <param name="pos">position of value set.</param>
			/// <returns>Operator returns reference to value set at specified position.</returns>
			inline GaValueSet<T>& GACALL operator [](int pos) const { return *_sets[ pos ]; }

		};// END CLASS DEFINITION GaCombinedValueSet

		/// <summary>This template represents value set with no additional limitations, except limitation introduced by the type and
		/// random generator used to generate values. 
		/// 
		/// This class has built-in synchronizator so it is allowed to use <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros
		/// with instances of this class, but no public or private methods are thread-safe.</summary>
		/// <param name="T">type of data in value set. This type must support unary <c>operators -</c>.</param>
		template <typename T>
		class GaUnboundValueSet : public GaValueSet<T>
		{

		private:

			/// <summary>Pointer to random generator which is used for generating of random values.</summary>
			GaRandom<T>* _randomGenerator;

		public:

			/// <summary>This constructor initializes value set with random generator.</summary>
			/// <param name="randomGenerator">pointer to random generator.</param>
			GaUnboundValueSet(GaRandom<T>* randomGenerator) : _randomGenerator(randomGenerator) { }

			/// <summary>More details are given in specification of <see cref="GaValueSet::GenerateRandom" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual T GenerateRandom() const { return _randomGenerator->Generate(); }

			/// <summary>More details are given in specification of <see cref="GaValueSet::Inverse" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual bool Inverse(const T& value,
				T& inverted) const
			{ 
				inverted = -value;
				return true;
			}

			/// <summary>More details are given in specification of <see cref="GaValueSet::ClosestValue" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual bool Belongs(const T& value) const { return true; }

			/// <summary>More details are given in specification of <see cref="GaValueSet::ClosestValue" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual const T& ClosestValue(const T& value) const { return value; }

		};// END CLASS DEFINITION GaUnboundValueSet

	} // Representation
} // Chromosome

#endif // __GA_VALUE_SETS_H__
