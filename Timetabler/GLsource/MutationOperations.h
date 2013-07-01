
/*! \file MutationOperations.h
    \brief This file contains declaration of classes that implement mutation operations.
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

#ifndef __GA_MUTATION_OPERATIONS_H__
#define __GA_MUTATION_OPERATIONS_H__

#include "Platform.h"
#include "ChromosomeOperations.h"

using namespace Chromosome;

namespace Chromosome
{
	/// <summary>Contains implementation of some basic mutation operations.</summary>
	namespace MutationOperations
	{

		/// <summary><c>GaFlipMutation</c> mutation chooses random number of values at random positions in chromosome's code and sets
		/// them with random value from chromosome's value set. Maximum number of changed values is specified in chromosomes parameters.
		/// This mutation changes at least one of the values. 
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaFlipMutation : public GaMutationOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaMutationOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(GaChromosome* chromosome) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaChromosomeParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaChromosomeParams(); }

			/// <summary>More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns <c>true</c> if mutation size specified in parameters is greater then 0, otherwise it returns <c>false</c>.</returns>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return ( (const GaChromosomeParams&)parameters ).GetMutationSize() > 0; }

		};// END CLASS DEFINITION GaFlipMutation

		/// <summary>GaSapwMutation mutation chooses random number of pairs of values at random positions in chromosome's code and swaps their positions.
		/// Maximum number of swapped pairs is specified in chromosomes parameters. This mutation swaps at least on pair of values. 
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		class GaInvertMutation : public GaMutationOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaMutationOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(GaChromosome* chromosome) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaChromosomeParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaChromosomeParams(); }

			/// <summary>More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns <c>true</c> if mutation size specified in parameters is greater then 0, otherwise it returns <c>false</c>.</returns>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const {return ( (const GaChromosomeParams&)parameters ).GetMutationSize() > 0; }

		};// END CLASS DEFINITION GaInvertMutation

		/// <summary><c>GaInvertMutation</c> mutation chooses random number of values at random positions in chromosome's code and inverts them.
		/// Maximum number of inverted values is specified in chromosomes parameters. This mutation changes at least on value.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaSwapMutation : public GaMutationOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaMutationOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(GaChromosome* chromosome) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaChromosomeParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaChromosomeParams(); }

			/// <summary>More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns <c>true</c> if mutation size specified in parameters is greater then 0, otherwise it returns <c>false</c>.</returns>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return ( (const GaChromosomeParams&)parameters ).GetMutationSize() > 0; }

		};// END CLASS DEFINITION GaSwapMutation

	} // Mutation
} // Chromosome

#endif // __GA_MUTATION_OPERATIONS_H__
