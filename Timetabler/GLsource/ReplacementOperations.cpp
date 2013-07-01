
/*! \file ReplacementOperations.cpp
    \brief This file contains implementation of replacement operation classes.
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
#include "ReplacementOperations.h"

using namespace Population;
using namespace Population::ReplacementOperations;

namespace Population
{
	namespace ReplacementOperations
	{

		/// <summary><c>RemoveDuplicates</c> method removes duplicates of chromosomes in input from coupling operation if it is required.</summary>
		/// <param name="input">intput buffer that should be cleared.</param>
		void GACALL RemoveDuplicates(const GaCouplingResultSet& input)
		{
			// clearing required?
			if( input.GetClearDuplicates() )
			{
				int count = input.GetNumberOfOffsprings(), p;
				for( int i = 0; i < count; i++ )
				{
					GaChromosomePtr offspring1;
					input.GetOffspringAt( i, offspring1, p );

					// removed chromosome?
					if( !offspring1.IsNULL() )
					{
						// find duplicates
						for( int j = i + 1; j < count; j++ )
						{
							GaChromosomePtr offspring2;
							input.GetOffspringAt( j, offspring2, p );

							// is it duplicate?
							if( !offspring2.IsNULL() && *offspring1 == *offspring2 )
								// remove duplicate
								input.GetOffspringsBuffer()[ j ] = GaChromosomePtr::NullPtr;
						}
					}
				}
			}
		}

		// Replaces existing chromosomes with new ones based on passed parameters and selection results.
		void GaReplaceWorst::operator ()(GaPopulation& population,
			const GaReplacementParams& parameters,
			const GaCouplingResultSet& newChromosomes) const
		{
			int maxSize = min( parameters.GetReplacementSize(), newChromosomes.GetNumberOfOffsprings() );

			// get worst chromosomes
			int* old = new int[ maxSize ];
			int size = population.GetWorsChromosomes( old, 0, maxSize );

			// replace them
			RemoveDuplicates( newChromosomes );
			population.ReplaceGroup( old, newChromosomes.GetOffspringsBuffer(), size );

			delete[] old;
		}

		// Replaces existing chromosomes with new ones based on passed parameters and selection results.
		void GaReplaceRandom::operator ()(GaPopulation& population,
			const GaReplacementParams& parameters,
			const GaCouplingResultSet& newChromosomes) const
		{
			int size = min( parameters.GetReplacementSize(), newChromosomes.GetNumberOfOffsprings() );
			int elitism = ( (const GaReplaceElitismParams&) parameters ).GetElitism();

			int populationSize = population.GetCurrentSize();
			bool sorted = population.GetConfiguration().GetParameters().GetSorting();

			// trying to save all chromosomes? 
			if( elitism >= populationSize )
				return;

			// adjust replacement size to fit elitisam constraint
			if( size > populationSize - elitism )
				size = populationSize - elitism;

			int* old = new int[ size ];

			for( int i = 0; i < size; i++ )
			{
				int index;
				volatile bool duplicate = false;

				do
				{
					if( !sorted )
					{
						int ranking;

						// select chromosome to be replace that fits elitism constraint
						do
						{
							index = GaGlobalRandomIntegerGenerator->Generate( populationSize - 1 );
							ranking = population.GetChromosomeRanking( index );
						} while( ranking >= 0 && ranking < elitism );
					}
					else
						// select chromosome to be replace that fits elitism constraint
						index = GaGlobalRandomIntegerGenerator->Generate( elitism, populationSize - 1 );

					// is it already in replacement group?
					for( int j = 0; j < i; j++ )
					{
						duplicate = old[ j ] == index;
						if( duplicate )
							break;
					}
				} while( duplicate );

				// insert to replacement group
				old[ i ] = index;
			}

			// replace
			RemoveDuplicates( newChromosomes );
			population.ReplaceGroup( old, newChromosomes.GetOffspringsBuffer(), size );

			delete[] old;
		}

		// Replaces existing chromosomes with new ones based on passed parameters and selection results.
		void GaReplaceParents::operator ()(GaPopulation& population,
			const GaReplacementParams& parameters,
			const GaCouplingResultSet& newChromosomes) const
		{
			int size = min( parameters.GetReplacementSize(), newChromosomes.GetNumberOfOffsprings() );
			RemoveDuplicates( newChromosomes );
			population.ReplaceGroup( newChromosomes.GetParentsBuffer(), newChromosomes.GetOffspringsBuffer(), size );
		}

		// Replaces existing chromosomes with new ones based on passed parameters and selection results.
		void GaReplaceBest::operator ()(GaPopulation& population,
			const GaReplacementParams& parameters,
			const GaCouplingResultSet& newChromosomes) const
		{
			int maxSize = min( parameters.GetReplacementSize(), newChromosomes.GetNumberOfOffsprings() );

			// get best chromosomes
			int* old = new int[ maxSize ];
			int size = population.GetBestChromosomes( old, 0, maxSize );

			// replace them
			RemoveDuplicates( newChromosomes );
			population.ReplaceGroup( old, newChromosomes.GetOffspringsBuffer(), size );

			delete[] old;
		}

	} // ReplacementOperations
} // Population
