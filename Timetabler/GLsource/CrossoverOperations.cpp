
/*! \file CrossoverOperations.cpp
    \brief This file contains implementation of crossover operation classes.
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
#include "RepresentationInterfaces.h"
#include "CrossoverOperations.h"

using namespace Chromosome;
using namespace Chromosome::Representation;
using namespace Chromosome::CrossoverOperations;

namespace Chromosome
{
	namespace CrossoverOperations
	{

		// Performs crossover operation and make new child from two given parents.
		GaChromosomePtr GaMultiValueCrossover::operator ()(const GaChromosome* parent1,
			const GaChromosome* parent2) const
		{
			GaChromosomePtr newChromosome = parent1->MakeCopy( true );
			GaMultiValueCode* code = dynamic_cast<GaMultiValueCode*>( &( *newChromosome ) );

			const GaMultiValueCode* pc1 = dynamic_cast<const GaMultiValueCode*>( parent1 );
			const GaMultiValueCode* pc2 = dynamic_cast<const GaMultiValueCode*>( parent2 );

			int maxPos = parent1->GetCodeSize() <= parent2->GetCodeSize()
				 ? parent1->GetCodeSize() 
				 : parent2->GetCodeSize();

			// create temp buffer for making new code
			GaCodeValuesBuffer* buffer = code->MakeBuffer( parent1->GetCodeSize() >= parent2->GetCodeSize()
				 ? parent1->GetCodeSize() 
				 : parent2->GetCodeSize() );

			// array of crossover points
			int* pos = new int[ parent1->GetParameters().GetNumberOfCrossoverPoints() + 1 ];
			pos[ 0 ] = 0;
			int c = 1;

			// make random crossover points
			for( int i = parent1->GetParameters().GetNumberOfCrossoverPoints() - 1; i >= 0; i-- )
			{
				bool found = false;
				int p;
				do
				{
					// generate radnom point
					p = GaGlobalRandomIntegerGenerator->Generate( maxPos - 1 );

					// check if point already exists
					for( int j = c - 1; j >= 0; j-- )
					{
						// found?
						if( p == pos[ j ] )
						{
							found = true;
							break;
						}
					}

					found = false;
				}
				while( found );

				int j = c;
				// fin position of crossover point in array
				for( ; j > 0 && p < pos[ j - 1 ]; j-- )
					pos[ j ] = pos[ j - 1 ];

				// inser crossover point
				pos[ j ] = p;
				c++;
			}

			bool k = GaGlobalRandomBoolGenerator->Generate();
			for( int i = 0; i < c; i++ )
			{
				int p = pos[ i ];

				// copy code form parent1
				if( k )
				{
					// size of part which will be copied into new chromosome
					int s = i != ( c - 1 ) ? pos[ i + 1 ] - pos[ i ] : parent1->GetCodeSize() - pos[ i ];

					// copy part of parent into buffer for new chromosome
					pc1->FillBuffer( p, s, *buffer );
				}
				// copy code form parent2
				else
				{
					// size of part which will be copied into new chromosome
					int s = i != ( c - 1 ) ? pos[ i + 1 ] - pos[ i ] : parent2->GetCodeSize() - pos[ i ];

					// copy part of parent into buffer for new chromosome
					pc2->FillBuffer( p, s, *buffer );
				}

				k = !k;
			}

			// set new code for chromosome
			code->FromBuffer( *buffer );

			// delete temp buffers
			delete[] pos;
			delete buffer;

			return newChromosome;
		}

		// Performs crossover operation and make new child from two given parents.
		GaChromosomePtr GaAddCrossover::operator ()(const GaChromosome* parent1,
			const GaChromosome* parent2) const
		{
			return *dynamic_cast<const GaArithmeticalCode*>( parent1 ) + *dynamic_cast<const GaArithmeticalCode*>( parent2 );
		}

		// Performs crossover operation and make new child from two given parents.
		GaChromosomePtr GaSubCrossover::operator ()(const GaChromosome* parent1,
			const GaChromosome* parent2) const
		{
			return *dynamic_cast<const GaArithmeticalCode*>( parent1 ) - *dynamic_cast<const GaArithmeticalCode*>( parent2 );
		}

		// Performs crossover operation and make new child from two given parents.
		GaChromosomePtr GaMidpointCrossover::operator ()(const GaChromosome* parent1,
			const GaChromosome* parent2) const
		{
			return dynamic_cast<const GaArithmeticalCode*>( parent1 )->Midpoint( *dynamic_cast<const GaArithmeticalCode*>( parent2 ) );
		}

	} // Crossover
} // Chromosome
