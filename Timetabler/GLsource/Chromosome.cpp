
/*! \file Chromosome.cpp
    \brief This file implements methods of abstract chromosome classes.
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
#include "Chromosome.h"
#include "ChromosomeOperations.h"
#include "Population.h"

namespace Chromosome
{

	// Initialization of scaled value
	GaScaledChromosome::GaScaledChromosome(GaChromosomePtr chromosome,
		GaPopulation* population,
		int index) : _chromosome(chromosome),
		_population(population),
		_index(index),
		_groups(GASGT_NONE)
	{
		Rescale();
	}

	// Default constructor
	GaScaledChromosome::GaScaledChromosome(GaPopulation* population) : _groups(GASGT_NONE),
		_index(-1),
		_chromosome(GaChromosomePtr::NullPtr),
		_population(population) { }

	// Returns fitness value (scaled or non scaled fitness) for comparison
	float GaScaledChromosome::GetFitnessForComparison() const
	{
		return _population->IsScaledFitnessUsed() ? _scaledFitness : _chromosome->GetFitness();
	}

	// Rescale scaled fitness value
	void GACALL GaScaledChromosome::Rescale()
	{ 
		const GaScalingPair& sc = _population->GetConfiguration().Scaling();
		// calculates scaled fitness value if scaling is enambled
		_scaledFitness =  sc.HasOperation() ? sc.GetOperation()( *this, *_population, sc.GetParameters() ) : _scaledFitness = _chromosome->GetFitness();
	}

	// Compares fitnesses of two chromosomes. Returns:
	//  a, -1 if the fitness value of this chromosome is lower then the given value
	//  b.  0 if the fitness values of both chromosomes are equal
	//  c.  1 if the fitness value of this chromosome is greater then the given value
	int GaScaledChromosome::CompareFitnesses(float c) const
	{
		return ( *_population->GetConfiguration().GetSortComparator() )( GetFitnessForComparison(), c );
	}

	// Sets index of chromosome in pupulation
	void GaScaledChromosome::SetIndex(int index)
	{
		_index = index;
		const GaScalingPair& sc = _population->GetConfiguration().Scaling();
		// scaling enabled?
		if( sc.HasOperation() && sc.GetOperation().IsRankingBased() )
			// scale fitness
			_scaledFitness = sc.GetOperation()( *this, *_population, sc.GetParameters() );
	}

	// Mutation operation
	void GaDefaultChromosome::Mutation()
	{
		// check mutating probability
		if( GaGlobalRandomBoolGenerator->Generate( _configBlock->_parameters->GetMutationProbability() ) )
		{
			// backup chromosome's code if needed
			if( _configBlock->_parameters->GetImprovingMutationsFlag() )
				PreapareForMutation();

			PerformMutation();

			float newFitness = CalculateFitness();

			// improvment?
			if( _configBlock->_parameters->GetImprovingMutationsFlag() )
			{
				if( CompareFitnesses( newFitness ) > 0 )
					// mutation did not make any improvments - reject it
					RejectMutation();
				else
				{
					_fitness = newFitness;

					// mutation did make improvments - accept it
					AcceptMutation();
				}
			}
			else
				_fitness = newFitness;
		}
	}

	// Performs crossover operation and make new chiled from this and another given chromosome,
	GaChromosomePtr GaDefaultChromosome::Crossover(GaChromosomePtr secondParent) const
	{
		GaChromosomePtr newChromosome;

		// check crossover probability
		if( GaGlobalRandomBoolGenerator->Generate( _configBlock->_parameters->GetCrossoverProbability() ) )
		{
			newChromosome = PerformCrossover( secondParent );
			newChromosome->RefreshFitness();
		}
		else
			newChromosome = MakeCopy( false );

		return newChromosome;
	}

	// Compares fitnesses of two chromosomes. Returns:
	//  a, -1 if the fitness value of this chromosome is lower then the given value
	//  b.  0 if the fitness values of both chromosomes are equal
	//  c.  1 if the fitness value of this chromosome is greater then the given value
	int GaDynamicOperationChromosome::CompareFitnesses(float c) const
	{ 
		return ( *( (GaChromosomeOperationsBlock*)_configBlock )->_fitnessComparator )( _fitness, c );
	}

	// Performs crossover operation and make new chiled from this and another given chromosome,
	GaChromosomePtr GaDynamicOperationChromosome::PerformCrossover(GaChromosomePtr secondParent) const
	{
		return ( *( (GaChromosomeOperationsBlock*)_configBlock )->_crossoverOperation )( this, &( *secondParent ) );
	}

	// Performs mutation operation
	void GaDynamicOperationChromosome::PerformMutation()
	{
		return ( *( (GaChromosomeOperationsBlock*)_configBlock )->_mutationOperation )( this );
	}

	// Calculate the fitness of chromosome
	float GaDynamicOperationChromosome::CalculateFitness() const
	{
		return ( *( (GaChromosomeOperationsBlock*)_configBlock )->_fitnessOperation )( this );
	}

} // Chromosome
