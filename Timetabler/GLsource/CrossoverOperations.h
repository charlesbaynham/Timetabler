
/*! \file CrossoverOperations.h
    \brief This file contains declaration of classes that implement crossover operations.
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

#ifndef __GA_CROSSOVER_OPERATIONS_H__
#define __GA_CROSSOVER_OPERATIONS_H__

#include "Platform.h"
#include "ChromosomeOperations.h"

using namespace Chromosome;

namespace Chromosome
{
	/// <summary>Contains implementation of some basic crossover operations.</summary>
	namespace CrossoverOperations
	{

		/// <summary><c>GaMultiValueCrossover</c> crossover operation create offspring by choosing specified number of crossover points at random positions,
		/// then it alternately copies values from parents' codes, and at very crossover point it changes parent from which it copies values.
		/// The first parent from which copying starts is chosen randomly.
		///
		/// <img src="graphic/mv_cr.png"></img>
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaMultiValueCrossover : public GaCrossoverOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaCrossoverOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual GaChromosomePtr GACALL operator ()(const GaChromosome* parent1,
				const GaChromosome* parent2) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaChromosomeParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaChromosomeParams(); }

			/// <summary>More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>This method always returns <c>true</c>.</returns>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return true; }

		};// END CLASS DEFINITION GaMultiValueCrossover

		/// <summary><c>GaAddCrossover</c> crossover operation creates offspring which code has values which are calculated form corresponding values of
		/// codes of both parents by summing them.
		///
		/// <img src="graphic/a_cr.png"></img>
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaAddCrossover : public GaCrossoverOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaCrossoverOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual GaChromosomePtr GACALL operator ()(const GaChromosome* parent1,
				const GaChromosome* parent2) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaChromosomeParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaChromosomeParams(); }

			/// <summary>More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>This method always returns <c>true</c>.</returns>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const
			{
				return ( (const GaChromosomeParams&) parameters ).GetNumberOfCrossoverPoints() > 0;
			}

		};// END CLASS DEFINITION GaAddCrossover

		/// <summary><c>GaSubCrossover</c> crossover operation creates offspring which code has values which are calculated form corresponding values of
		/// codes of both parents by subtracting them.
		///
		/// <img src="graphic/s_cr.png"></img>
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaSubCrossover : public Chromosome::GaCrossoverOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaCrossoverOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual GaChromosomePtr GACALL operator ()(const GaChromosome* parent1,
				const GaChromosome* parent2) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaChromosomeParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaChromosomeParams(); }

			/// <summary>More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>This method always returns <c>true</c>.</returns>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return true; }

		};// END CLASS DEFINITION GaSubCrossover

		/// <summary><c>GaMidpointCrossover</c> crossover operation creates offspring which code has values which are calculated form corresponding values of
		/// codes of both parents by midpoint algorithm.
		///
		/// <img src="graphic/mp_cr.png"></img>
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaMidpointCrossover : public GaCrossoverOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaCrossoverOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual GaChromosomePtr GACALL operator ()(const GaChromosome* parent1,
				const GaChromosome* parent2) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaChromosomeParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaChromosomeParams(); }

			/// <summary>More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>This method always returns true.</returns>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return true; }

		};// END CLASS DEFINITION GaMidpointCrossover

	} // Crossover
} // Chromosome

#endif // __GA_CROSSOVER_OPERATIONS_H__
