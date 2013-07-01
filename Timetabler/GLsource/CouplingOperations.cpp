
/*! \file CouplingOperations.cpp
    \brief This file contains implementation of coupling operation classes.
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
#include "Population.h"
#include "CouplingOperations.h"

using namespace Population;
using namespace Population::CouplingOperations;

namespace Population
{
	namespace CouplingOperations
	{

		/// <summary><c>CheckForDuplicates</c> checks whether the chromosome already exists in the population.</summary>
		/// <param name="population">population against which the new chromosome should be tested.</param>
		/// <param name="newChromosome">chromosome that should be checked.</param>
		/// <returns>Method returns <c>true</c> if at least one equal chromosome is found in the population.</returns>
		bool GACALL CheckForDuplicates(const GaPopulation& population,
			const GaChromosome& newChromosome)
		{
			for( int i = population.GetCurrentSize() - 1; i >= 0; i-- )
			{
				if( *population.GetAt( i ).GetChromosome() == newChromosome )
					return true;
			}

			return false;
		}

		/// <summary><c>ProduceOffspring</c> function produces offspring and stores it to coupling output.</summary>
		/// <param name="population">population in which the coupling occures.</param>
		/// <param name="parent1">the first parent.</param>
		/// <param name="parent2">the second parent.</param>
		/// <param name="output">coupling result set that will store produced offspring.</param>
		/// <param name="offspringIndex">index at which the new offspring chromosome should be stored in soupling result set.</param>
		/// <param name="parentIndex">index of one parent.</param>
		/// <param name="offspringCount">number of offsprings that should be produced before the best one is selected.</param>
		/// <param name="checkDuplicates">if this parameter is set to <c>true</c> it instructs function not to insert chromosomes in result set
		/// if equal ones already exist in the population.</param>
		void GACALL ProduceOffspring(const GaPopulation& population,
			GaChromosomePtr parent1,
			GaChromosomePtr parent2,
			GaCouplingResultSet& output,
			int offspringIndex,
			int parentIndex,
			int offspringCount,
			bool checkDuplicates)
		{
			GaChromosomePtr best;
			float bestFitness;

			// make few offspring but choose only the best one
			for( int i = offspringCount; i > 0; i-- )
			{
				// make new offspring
				GaChromosomePtr offspring = parent1->Crossover( parent2 );
				offspring->Mutation();

				// is this chromosome the best
				if( best.IsNULL() || offspring->GetFitness() > bestFitness )
				{
					// save the best chromosome
					bestFitness = offspring->GetFitness();
					best = offspring;
				}
			}

			if( !checkDuplicates || !CheckForDuplicates( population, *best ) )
				output.SetOffspringAt( offspringIndex, best, parentIndex );
			else
				output.SetOffspringAt( offspringIndex, GaChromosomePtr::NullPtr, -1 );
		}

		// Couples selected chromosomes and produce offsprings based on passed parameters
		void GaSimpleCoupling::operator ()(const GaPopulation& population,
			GaCouplingResultSet& output,
			const GaCouplingParams& parameters,
			int workerId,
			int numberOfWorkers) const
		{
			GaSortedGroup& parents = output.GetSelectionResultSet().SelectedGroup();
			int size = parents.GetCurrentSize();

			// how much offsprings should be produced
			int lim = parameters.GetNumberOfOffsprings() <= output.GetNumberOfOffsprings()
				? parameters.GetNumberOfOffsprings()
				: output.GetNumberOfOffsprings();

			// how much offsprings should be produced
			int outSize = lim / numberOfWorkers;
			if( outSize % 2 )
				outSize++;
			// first offspring position
			int outStart = outSize * workerId;

			// last worker do more if the work cann't be divided equaly
			if( workerId == numberOfWorkers - 1 )
				outSize = lim - outStart;

			// last offspring position
			lim = outStart + outSize;

			for( int i = outStart; i < lim; i += 2 )
			{
				// get parent's indices
				int pi1 = parents[ i % size ];
				int pi2 = parents[ ( i + 1 ) % size ];

				// get parents
				GaChromosomePtr p1 = population.GetAt( pi1 ).GetChromosome();
				GaChromosomePtr p2 = population.GetAt( pi2 ).GetChromosome();

				// make new chromosomes and save them to result set
				ProduceOffspring( population, p1, p2, output, i, pi1, 1, parameters.GetCheckForDuplicates() );
				if( i + 1 < lim )
					ProduceOffspring( population, p2, p1, output, i + 1, pi2, 1, parameters.GetCheckForDuplicates() );
			}

			// sets whether replacement operation should purge duplicates before it inserts offspring chromosomes into population
			output.SetClearDuplicates( parameters.GetCheckForDuplicates() );
		}

		// Couples selected chromosomes and produce offsprings based on passed parameters
		void GaCrossCoupling::operator ()(const GaPopulation& population,
			GaCouplingResultSet& output,
			const GaCouplingParams& parameters,
			int workerId,
			int numberOfWorkers) const
		{
			GaSortedGroup& parents = output.GetSelectionResultSet().SelectedGroup();
			int size = parents.GetCurrentSize();

			// how much offsprings should be produced
			int lim = parameters.GetNumberOfOffsprings() <= output.GetNumberOfOffsprings()
				? parameters.GetNumberOfOffsprings()
				: output.GetNumberOfOffsprings();

			// how much offsprings should be produced
			int outSize = lim / numberOfWorkers;
			// first offspring position
			int outStart = outSize * workerId;

			// last worker do more if the work cann't be divided equaly
			if( workerId == numberOfWorkers - 1 )
				outSize += lim % numberOfWorkers;

			// last offspring position
			lim = outStart + outSize;

			for( int i = outStart; i < lim; i++ )
			{
				// get parent's indices
				int pi1 = parents[ i % size ];
				int pi2 = parents[ ( i + 1 ) % size ];

				// get parents
				GaChromosomePtr p1 = population.GetAt( pi1 ).GetChromosome();
				GaChromosomePtr p2 = population.GetAt( pi2 ).GetChromosome();

				// make few offspring but choose only the best one
				ProduceOffspring( population, p1, p2, output, i, pi1,
					( (const GaMulitpleCrossoverCouplingParams&) parameters ).GetOffspringsPerParentPair(), parameters.GetCheckForDuplicates() );
			}

			// sets whether replacement operation should purge duplicates before it inserts offspring chromosomes into population
			output.SetClearDuplicates( parameters.GetCheckForDuplicates() );
		}

		// Couples selected chromosomes and produce offsprings based on passed parameters
		void GaInverseCoupling::operator ()(const GaPopulation& population,
			GaCouplingResultSet& output,
			const GaCouplingParams& parameters,
			int workerId,
			int numberOfWorkers) const
		{
			GaSortedGroup& parents = output.GetSelectionResultSet().SelectedGroup();
			int size = parents.GetCurrentSize();

			// how much offsprings should be produced
			int lim = parameters.GetNumberOfOffsprings() <= output.GetNumberOfOffsprings()
				? parameters.GetNumberOfOffsprings()
				: output.GetNumberOfOffsprings();

			// how much offsprings should be produced
			int outSize = lim / numberOfWorkers;
			// first offspring position
			int outStart = outSize * workerId;

			// last worker do more if the work cann't be divided equaly
			if( workerId == numberOfWorkers - 1 )
				outSize += lim % numberOfWorkers;

			// last offspring position
			lim = outStart + outSize;

			for( int i = outStart; i < lim; i++ )
			{
				// get parent's indices
				int j = i % size;
				int pi1 = parents[ j ];
				int pi2 = parents[ size - 1 - j ];

				// get parents
				GaChromosomePtr p1 = population.GetAt( pi1 ).GetChromosome();
				GaChromosomePtr p2 = population.GetAt( pi2 ).GetChromosome();

				// make few offspring but choose only the best one
				ProduceOffspring( population, p1, p2, output, i, pi1,
					( (const GaMulitpleCrossoverCouplingParams&) parameters ).GetOffspringsPerParentPair(), parameters.GetCheckForDuplicates() );
			}

			// sets whether replacement operation should purge duplicates before it inserts offspring chromosomes into population
			output.SetClearDuplicates( parameters.GetCheckForDuplicates() );
		}

		// Couples selected chromosomes and produce offsprings based on passed parameters
		void GaBestAlwaysCoupling::operator ()(const GaPopulation& population,
			GaCouplingResultSet& output,
			const GaCouplingParams& parameters,
			int workerId,
			int numberOfWorkers) const
		{
			GaSortedGroup& parents = output.GetSelectionResultSet().SelectedGroup();
			int size = parents.GetCurrentSize();

			// how much offsprings should be produced
			int lim = parameters.GetNumberOfOffsprings() <= output.GetNumberOfOffsprings()
				? parameters.GetNumberOfOffsprings()
				: output.GetNumberOfOffsprings();

			// how much offsprings should be produced
			int outSize = lim / numberOfWorkers;
			// first offspring position
			int outStart = outSize * workerId;

			// last worker do more if the work cann't be divided equaly
			if( workerId == numberOfWorkers - 1 )
				outSize += lim % numberOfWorkers;

			// last offspring position
			lim = outStart + outSize;

			// the best selected chromosome is always one of the parents
			GaChromosomePtr p1 = output.GetSelectionResultSet()[ 0 ];

			for( int i = outStart; i < lim; i++ )
			{
				// get other parent
				int pi2 = parents[ i % size ];
				GaChromosomePtr p2 = population.GetAt( pi2 );

				// make few offspring but choose only the best one
				ProduceOffspring( population, p1, p2, output, i, pi2,
					( (const GaMulitpleCrossoverCouplingParams&) parameters ).GetOffspringsPerParentPair(), parameters.GetCheckForDuplicates() );
			}

			// sets whether replacement operation should purge duplicates before it inserts offspring chromosomes into population
			output.SetClearDuplicates( parameters.GetCheckForDuplicates() );
		}

		// Couples selected chromosomes and produce offsprings based on passed parameters
		void GaRandomCoupling::operator ()(const GaPopulation& population,
			GaCouplingResultSet& output,
			const GaCouplingParams& parameters,
			int workerId,
			int numberOfWorkers) const
		{
			GaSortedGroup& parents = output.GetSelectionResultSet().SelectedGroup();
			int size = parents.GetCurrentSize();

			// how much offsprings should be produced
			int lim = parameters.GetNumberOfOffsprings() <= output.GetNumberOfOffsprings()
				? parameters.GetNumberOfOffsprings()
				: output.GetNumberOfOffsprings();

			// parents which will be used by this worker
			int inSize = size / numberOfWorkers;
			int inStart = inSize * workerId;

			// how much offsprings should be produced and position for saving offsprings
			int outSize = lim / numberOfWorkers;
			int outStart = outSize * workerId;

			// last worker do more if the work cann't be divided equaly
			if( workerId == numberOfWorkers - 1 )
			{
				inSize += size % numberOfWorkers;
				outSize += lim % numberOfWorkers;
			}

			// last offspring position
			lim = outStart + outSize;

			// copules
			for( int i = outStart, j = 0; i < lim; i++, j++ )
			{
				// get parent's indices
				int pi1 = parents[ inStart + j % inSize ];
				int pi2 = parents[ GaGlobalRandomIntegerGenerator->Generate( size - 1 ) ];

				// get parents
				GaChromosomePtr p1 = population.GetAt( pi1 ).GetChromosome();
				GaChromosomePtr p2 = population.GetAt( pi2 ).GetChromosome();
				
				// make few offspring but choose only the best one
				ProduceOffspring( population, p1, p2, output, i, pi1,
					( (const GaMulitpleCrossoverCouplingParams&) parameters ).GetOffspringsPerParentPair(), parameters.GetCheckForDuplicates() );
			}

			// sets whether replacement operation should purge duplicates before it inserts offspring chromosomes into population
			output.SetClearDuplicates( parameters.GetCheckForDuplicates() );
		}

	} // CouplingOperations
} // Population
