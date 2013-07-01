
/*! \file SingleValueChromosome.h
    \brief This file contains declaration and implementation of single-value chromosome classes (with and without support for arithmetic crossover operations).
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

#ifndef __GA_SINGLE_VALUE_CHROMOSOME__
#define __GA_SINGLE_VALUE_CHROMOSOME__

#include "Chromosome.h"
#include "Platform.h"
#include "RepresentationInterfaces.h"
#include "DomainChromosome.h"

using namespace Chromosome;
using namespace Chromosome::Representation;

namespace Chromosome
{
	namespace Representation
	{

		/// <summary>This class can be used for chromosomes which code has only one value. 
		/// Value can be of any type as long as that type obeys requirements introduced by value set. 
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		/// <param name="TYPE">type of value in chromosome's code.</param>
		template <typename TYPE>
		class GaSingleValueChromosome : public GaDomainChromosome<TYPE>,
			public GaMutableCode
		{

		protected:

			/// <summary>Chromosome's code.</summary>
			TYPE _value;

			/// <summary>Backup copy chromosome's code before mutation if improving-only mutation flag is set in chromosome's parameters.</summary>
			TYPE _backup;

		public:

			/// <summary>More details are given in specification of <see cref="GaMutableCode::Flip" /> method.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="start">parameter is ignored.</param>
			/// <param name="size">parameter is ignored.</param>
			virtual void GACALL Flip(int start,
				int size) { _value = ( (GaChromosomeDomainBlock<TYPE>*)_configBlock )->GetValueSet( 0 )->GenerateRandom(); }

			/// <summary>More details are given in specification of <see cref="GaMutableCode::Invert" /> method.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="start">parameter is ignored.</param>
			/// <param name="size">parameter is ignored.</param>
			virtual void GACALL Invert(int start,
				int size) { ( (GaChromosomeDomainBlock<TYPE>*)_configBlock )->GetValueSet( 0 )->Inverse( _value, _value ); }

			/// <summary>This constructor initializes chromosome with CCB and its code.</summary>
			/// <param name="value">chromosome's code.</param>
			/// <param name="configBlock">pointer to CCB.</param>
			GaSingleValueChromosome(TYPE value,
				GaChromosomeDomainBlock<TYPE>* configBlock) : GaDomainChromosome(configBlock),
				_value(GetClosestValue( value, 0 )) { }

			/// <summary>This constructor initializes chromosome with CCB and randomly generated code.</summary>
			/// <param name="configBlock">pointer to CCB.</param>
			GaSingleValueChromosome(GaChromosomeDomainBlock<TYPE>* configBlock) : GaDomainChromosome(configBlock)
			{
				_value = ( (GaChromosomeDomainBlock<TYPE>*)_configBlock )->GetValueSet( 0 )->GenerateRandom();
			}

			/// <summary>This is copy constructor.</summary>
			/// <param name="c">reference to chromosome which is copied.</param>
			/// <param name="setupOnly">if this parameter is <c>true</c>, only pointer to CCB is copied. If this parameter is <c>false</c>,
			/// chromosome's data and CCB is copied.</param>
			GaSingleValueChromosome(const GaSingleValueChromosome<TYPE>& c,
				bool setupOnly) : GaDomainChromosome(c, setupOnly)
			{
				if( !setupOnly )
					_value = c._value;
			}

			/// <summary>More details are given in specification of <see cref="GaChromosome::MakeCopy" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual GaChromosomePtr GACALL MakeCopy(bool setupOnly) const { return new GaSingleValueChromosome( *this, setupOnly ); }

			/// <summary>This method uses <see cref="MakeCopy" /> method to create chromosome's object, so if additional steps are not needed
			/// for creating chromosome from prototype, classes which inherits this class don't have to override this method.
			/// 
			/// More details are given in specification of <see cref="GaChromosome::MakeNewFromPrototype" /> method.</summary>
			virtual GaChromosomePtr GACALL MakeNewFromPrototype() const
			{
				// make chromosome with exact setup
				GaChromosomePtr newPtr = MakeCopy( true );
				GaSingleValueChromosome* newChromosome = dynamic_cast<GaSingleValueChromosome*>( &( *newPtr ) );

				newChromosome->_value = 
					( (GaChromosomeDomainBlock<TYPE>*)_configBlock )->GetValueSet( 0 )->GenerateRandom();

				return newPtr;
			}

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method always returns 1.</returns>
			virtual int GACALL GetCodeSize() const { return 1; }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns value which represents chromosome's code.</returns>
			inline TYPE GACALL GetValue() const { return _value; }

			/// <summary><c>SetValue</c> method sets value which represents chromosome's code.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="value">new value of chromosome's code.</param>
			inline void GACALL SetValue(TYPE value) { _value = GetClosestValue( value, 0 ); }

			/// <summary>More details are given in specification of <see cref="GaChromosome::operator =" />.
			///
			/// This method is not thread-safe.</summary>
			virtual GaChromosome& GACALL operator =(const GaChromosome& rhs)
			{
				_value = dynamic_cast<const GaSingleValueChromosome&>( rhs )._value;

				return GaDefaultChromosome::operator =( rhs );
			}

			/// <summary>More details are given in specification of <see cref="GaChromosome::operator ==" />.
			///
			/// This method is not thread-safe.</summary>
			virtual bool GACALL operator ==(const GaChromosome& c) const
			{
				return _value == dynamic_cast<const GaSingleValueChromosome&>( c )._value;
			}

		protected:

			/// <summary><c>PreapareForMutation</c> method saves backup of chromosome's code. It overwrite previous backup.</summary>
			virtual void GACALL PreapareForMutation() { _backup = _value; }

			/// <summary>This method overrides abstract method of <see cref="GaChromosome" /> class, and has no functionality.</summary>
			virtual void GACALL AcceptMutation() { }

			/// <summary><c>RejectMutation</c> method restores chromosome's code from backup.</summary>
			virtual void GACALL RejectMutation() { _value = _backup; }

		};// END CLASS DEFINITION GaSingleValueChromosome

		/// <summary>This class can be used for chromosomes which code has only one value that supports arithmetic operations.
		/// Value can be of any type as long as that type obeys requirements introduced by value set and has defined arithmetic operators.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		/// <param name="TYPE">type of value in chromosome's code. This type must support binary operators
		/// <c>+</c>, <c>-</c>, <c>*</c>, <c>/</c> and <c>/</c> with right hand operand of <c>int</c> type.</param>
		template <typename TYPE>
		class GaSVArithmeticChromosome : public GaSingleValueChromosome<TYPE>,
			public GaArithmeticalCode
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaArithmeticalCode::operator +" />.
			///
			/// This method is not thread-safe.</summary>
			GaChromosomePtr GACALL operator +(const GaArithmeticalCode& rhs) const
			{
				GaChromosomePtr newPtr = MakeCopy( true );
				GaSVArithmeticChromosome* newChromosome = 
					dynamic_cast<GaSVArithmeticChromosome*>( &( *newPtr ) );

				newChromosome->_value = 
					GetClosestValue( _value + dynamic_cast<const GaSVArithmeticChromosome&>( rhs )._value, 0 );

				return newPtr;
			}

			/// <summary>More details are given in specification of <see cref="GaArithmeticalCode::operator -" />.
			///
			/// This method is not thread-safe.</summary>
			GaChromosomePtr GACALL operator -(const GaArithmeticalCode& rhs) const
			{
				GaChromosomePtr newPtr = MakeCopy( true );
				GaSVArithmeticChromosome* newChromosome = 
					dynamic_cast<GaSVArithmeticChromosome*>( &( *newPtr ) );

				newChromosome->_value = 
					GetClosestValue( _value - dynamic_cast<const GaSVArithmeticChromosome&>( rhs )._value, 0 );

				return newPtr;
			}

			/// <summary>More details are given in specification of <see cref="GaArithmeticalCode::operator *" />.
			///
			/// This method is not thread-safe.</summary>
			GaChromosomePtr GACALL operator *(const GaArithmeticalCode& rhs) const
			{
				GaChromosomePtr newPtr = MakeCopy( true );
				GaSVArithmeticChromosome* newChromosome = 
					dynamic_cast<GaSVArithmeticChromosome*>( &( *newPtr ) );

				newChromosome->_value = 
					GetClosestValue( _value * dynamic_cast<const GaSVArithmeticChromosome&>( rhs )._value, 0 );

				return newPtr;
			}

			/// <summary>More details are given in specification of <see cref="GaArithmeticalCode::operator /" />.
			///
			/// This method is not thread-safe.</summary>
			GaChromosomePtr GACALL operator /(const GaArithmeticalCode& rhs) const
			{
				GaChromosomePtr newPtr = MakeCopy( true );
				GaSVArithmeticChromosome* newChromosome = 
					dynamic_cast<GaSVArithmeticChromosome*>( &( *newPtr ) );

				newChromosome->_value = 
					GetClosestValue( _value / dynamic_cast<const GaSVArithmeticChromosome&>( rhs )._value, 0 );

				return newPtr;
			}

			/// <summary>More details are given in specification of <see cref="GaArithmeticalCode::Midpoint" /> method.
			///
			/// This method is not thread-safe.</summary>
			GaChromosomePtr GACALL Midpoint(const GaArithmeticalCode& c) const
			{
				GaChromosomePtr newPtr = MakeCopy( true );
				GaSVArithmeticChromosome* newChromosome = 
					dynamic_cast<GaSVArithmeticChromosome*>( &( *newPtr ) );

				newChromosome->_value = 
					GetClosestValue( ( _value + dynamic_cast<const GaSVArithmeticChromosome&>( c )._value ) / 2, 0 );

				return newPtr;
			}

			/// <summary>This constructor initializes chromosome with CCB and its code.</summary>
			/// <param name="value">chromosome's code.</param>
			/// <param name="configBlock">pointer to CCB.</param>
			GaSVArithmeticChromosome(TYPE value,
				GaChromosomeDomainBlock<TYPE>* configBlock) : GaSingleValueChromosome(value, configBlock) { }

			/// <summary>This constructor initializes chromosome with CCB and randomly generated code.</summary>
			/// <param name="configBlock">pointer to CCB.</param>
			GaSVArithmeticChromosome(GaChromosomeDomainBlock<TYPE>* configBlock) :
				GaSingleValueChromosome(configBlock) { }

			/// <summary>This is copy constructor.</summary>
			/// <param name="c">reference to chromosome which is copied.</param>
			/// <param name="setupOnly">if this parameter is <c>true</c>, only pointer to CCB is copied. If this parameter is <c>false</c>,
			/// chromosome's data and CCB is copied.</param>
			GaSVArithmeticChromosome(const GaSVArithmeticChromosome<TYPE>& c,
				bool setupOnly) : GaSingleValueChromosome(c, setupOnly) { }

			/// <summary>More details are given in specification of <see cref="GaChromosome::MakeCopy" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual GaChromosomePtr GACALL MakeCopy(bool setupOnly) const { return new GaSVArithmeticChromosome( *this, setupOnly ); }

		};// END CLASS DEFINITION GaSVArithmeticChromosome

	} // Representation
} // Chromosome

#endif // __GA_SINGLE_VALUE_CHROMOSOME__
