
/*! \file ScalingOperations.cpp
    \brief This file contains implementation of scaling operations classes.
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

#include "Population.h"
#include "ScalingOperations.h"

#include <math.h>

namespace Population
{
	namespace ScalingOperations
	{

		// Scales chromosome fitness value based on passed parameters
		float GaWindowScaling::operator ()(const GaScaledChromosome& chromosome,
			const GaPopulation& population,
			const GaScalingParams& parameters) const
		{
			float best = population.GetStatistics().GetValue( GSV_BEST_FITNESS_SCALED ).GetPrevious();
			float worst = population.GetStatistics().GetValue( GSV_WORST_FITNESS_SCALED ).GetPrevious();
			return chromosome.GetChromosome()->GetFitness() - ( best >= worst ? worst : best ) ;
		}

		// Returns TRUE if rescaling is needed for all chromosomes in population
		bool GaWindowScaling::NeedRescaling(const GaPopulation& population,
			const GaScalingParams& parameters) const
		{
			return population.GetStatistics().GetValue( GSV_WORST_FITNESS ).Changed();
		}

		// Scales chromosome fitness value based on passed parameters
		float GaExponentialScaling::operator ()(const GaScaledChromosome& chromosome,
			const GaPopulation& population,
			const GaScalingParams& parameters) const
		{
			float factor = ( (const GaScaleFactorParams&) parameters ).GetFactor();

			if( chromosome.GetChromosome()->GetFitness() == 0 && factor < 0 )
				return 0;

			return pow( chromosome.GetChromosome()->GetFitness(), factor );
		}

		// Returns TRUE if rescaling is needed for all chromosomes in population
		bool GaExponentialScaling::NeedRescaling(const GaPopulation& population,
			const GaScalingParams& parameters) const
		{
			GaScaledChromosome& c1 = population.GetAt( 0 );
			if( c1.GetScaledFitness() != (*this)( c1, population, parameters ) )
				return true;

			GaScaledChromosome& c2 = population.GetAt( 0 );
			if( c2.GetScaledFitness() != (*this)( c2, population, parameters ) )
				return true;

			return false;
		}

		// Scales chromosome fitness value based on passed parameters
		float GaLinearScaling::operator ()(const GaScaledChromosome& chromosome,
			const GaPopulation& population,
			const GaScalingParams& parameters) const
		{
			float a, b, d;

			float max = population.GetStatistics().GetValue( GSV_BEST_FITNESS ).GetPrevious();
			float avg = population.GetStatistics().GetValue( GSV_AVG_FITNESS ).GetPrevious();
			float min = population.GetStatistics().GetValue( GSV_WORST_FITNESS ).GetPrevious();

			float f = ( (const GaScaleFactorParams&)parameters ).GetFactor();

			if( min > ( f * avg - max ) / ( f - 1 ) )
			{
				d = max - avg;

				if( fabs( d ) < 0.00001 )
					a = 1, b = 0;
				else
				{
					a = avg / d;
					b = a * ( max - f * avg );
					a *= ( f - 1 );
				}
			}
			else
			{
				d = avg - min;

				if( fabs( d ) < 0.00001 )
					a = 1, b = 0;
				else
				{
					a = avg / d;
					b = -min * a;
				}
			}

			return a * chromosome.GetChromosome()->GetFitness() + b;
		}

		// Returns TRUE if rescaling is needed for all chromosomes in population
		bool GaLinearScaling::NeedRescaling(const GaPopulation& population,
			const GaScalingParams& parameters) const
		{
			if( population.GetStatistics().GetValue( GSV_BEST_FITNESS ).Changed() )
				return true;

			if( population.GetStatistics().GetValue( GSV_AVG_FITNESS ).Changed() )
				return true;

			if( population.GetStatistics().GetValue( GSV_WORST_FITNESS ).Changed() )
				return true;

			GaScaledChromosome& c = population.GetAt( 0 );
			return c.GetScaledFitness() != (*this)( c, population, parameters );
		}

		// Scales chromosome fitness value based on passed parameters
		float GaNormalizationScaling::operator ()(const GaScaledChromosome& chromosome,
			const GaPopulation& population,
			const GaScalingParams& parameters) const
		{
			return (float)( population.GetCurrentSize() - chromosome.GetIndex() );
		}

		// Returns TRUE if rescaling is needed for all chromosomes in population
		bool GaNormalizationScaling::NeedRescaling(const GaPopulation& population,
			const GaScalingParams& parameters) const
		{
			return population.GetStatistics().GetValue( GSV_POPULATION_SIZE ).Changed();
		}

	} // ScalingOperations
} // Population
