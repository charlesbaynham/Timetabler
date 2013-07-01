
/*! \file ChromosomeOperations.h
    \brief This file contains declaration of interfaces of genetic operations which are performed over chromosomes.
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

#ifndef __GA_CHROMOSOME_OPERATIONS_H__
#define __GA_CHROMOSOME_OPERATIONS_H__

#include "Platform.h"
#include "Catalogue.h"
#include "Operation.h"
#include "Chromosome.h"

using namespace Common;
using namespace Chromosome;

namespace Chromosome
{
	/// <summary><c>GaCrossoverOperation</c> class is base for crossover operations.</summary>
	class GaCrossoverOperation : public GaOperation
	{

	public:

		/// <summary>This operator extract raw pointer to parent chromosomes, and delegate responsibility to
		/// <c>operator ()(const GaChromosome*, const GaChromosome*)</c>.</summary>
		/// <param name="parent1">smart pointer to the first parent.</param>
		/// <param name="parent2">smart pointer to the seconf parent.</param>
		/// <returns>Method returns smart pointer to newly created offspring.</returns>
		GaChromosomePtr GACALL operator ()(GaChromosomePtr parent1,
			GaChromosomePtr parent2) const { return operator()( &( *parent1 ), &( *parent2 ) ); }

		/// <summary>This operator performs crossover operation over two parents and create on child.</summary>
		/// <param name="parent1">raw pointer to the first parent.</param>
		/// <param name="parent2">raw pointer to the seconf parent.</param>
		/// <returns>Method returns smart pointer to newly created offspring.</returns>
		virtual GaChromosomePtr GACALL operator ()(const GaChromosome* parent1,
			const GaChromosome* parent2) const=0;

	};// END CLASS DEFINITION GaCrossoverOperation

	/// <summary><c>GaCrossoverCatalogue</c> type is instance of <see cref="GaCatalogue" /> template class and represents catalogue of crossover operations.
	/// Detailed description can be found in specification of <see cref="GaCatalogue" /> template class.</summary>
	typedef GaCatalogue<GaCrossoverOperation> GaCrossoverCatalogue;

	/// <summary><c>GaMutationOperation</c> class is base for mutation operations.</summary>
	class GaMutationOperation : public GaOperation
	{

	public:

		/// <summary>This operator extract raw pointer to parent chromosomes, and delegate responsibility to <c>operator ()(const GaChromosome*)</c>.</summary>
		/// <param name="chromosome">smart pointer mutating chromosome.</param>
		inline void GACALL operator ()(GaChromosomePtr chromosome) const { return operator()( &( *chromosome ) ); }

		/// <summary>This operator performs mutation operation.</summary>
		/// <param name="chromosome">raw pointer to mutating chromosome.</param>
		virtual void GACALL operator ()(GaChromosome* chromosome) const=0;

	};// END CLASS DEFINITION GaMutationOperation

	/// <summary><c>GaMutationCatalogue</c> type is instance of <see cref="GaCatalogue" /> template class and represents catalogue of mutation operations.
	/// Detailed description can be found in specification of <see cref="GaCatalogue" /> template class.</summary>
	typedef GaCatalogue<GaMutationOperation> GaMutationCatalogue;

	/// <summary><c>GaFitnessOperation</c> class is base for fitness operations.</summary>
	class GaFitnessOperation : public GaOperation
	{

	public:

		/// <summary>This operator extract raw pointer to parent chromosomes, and delegate responsibility to <c>operator ()(const GaChromosome*)</c>.</summary>
		/// <param name="chromosome">smart pointer chromosome.</param>
		/// <returns>Method returns calculated fitness value of the chromosome.</returns>
		inline float GACALL operator ()(GaChromosomePtr chromosome) const { return operator()( &( *chromosome ) ); }

		/// <summary>This operator calculates fitness value of the chromosome.</summary>
		/// <param name="chromosome">raw pointer to mutating chromosome.</param>
		/// <returns>Method returns calculated fitness value of the chromosome.</returns>
		virtual float GACALL operator ()(const GaChromosome* chromosome) const=0;

	};// END CLASS DEFINITION GaFitnessOperation

	/// <summary><c>GaFitnessComparator</c> class is base for fitness comparators.</summary>
	class GaFitnessComparator : public GaOperation
	{

	public:

		// Compares two fitness values and returns:
		
		/// <summary>This operator compares fitness values. NOTE: Comparison doesn't have to be arithmetical comparison.</summary>
		/// <param name="fitness1">fitness value of the first chromosome.</param>
		/// <param name="fitness2">fitness value of the second chromosome.</param>
		/// <returns>a. -1 if the first fitness value lower then the second.
		/// <br>b.  0 if the fitness values are equal.
		/// <br>c.  1 if the first fitness value is greater then the second.</returns>
		virtual int GACALL operator ()(float fitness1,
			float fitness2) const=0;

		/// <summary>This method just overrides <see cref="GaOperation::MakeParameters" /> method and has no functionality.</summary>
		/// <returns>Method always returns <c>NULL</c>.</returns>
		virtual GaParameters* GACALL MakeParameters() const { return NULL; }

		/// <summary>This method just overrides <see cref="GaOperation::CheckParameters" /> method and has no functionality.</summary>
		/// <param name="parameters">parameter is igonred.</param>
		/// <returns>Method always returns <c>true</c>.</returns>
		virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return false; }

	};// END CLASS DEFINITION GaFitnessComparator

	/// <summary><c>GaFitnessComparatorCatalogue</c> type is instance of <see cref="GaCatalogue" /> template class and represents catalogue of fitness comparators.
	/// Detailed description can be found in specification of <see cref="GaCatalogue" /> template class.</summary>
	typedef GaCatalogue<GaFitnessComparator> GaFitnessComparatorCatalogue;

} // Chromosome

#endif // __GA_CHROMOSOME_OPERATIONS_H__
