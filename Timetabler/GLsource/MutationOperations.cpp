
/*! \file MutationOperations.cpp
    \brief This file contains implementation of mutation operation classes.
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
#include "MutationOperations.h"

using namespace Chromosome;
using namespace Chromosome::Representation;
using namespace Chromosome::MutationOperations;

namespace Chromosome
{
	namespace MutationOperations
	{

		// Performs mutation operation on given chromosome.
		void GaFlipMutation::operator ()(GaChromosome* chromosome) const
		{
			GaMutableCode* code = dynamic_cast<GaMutableCode*>( chromosome );

			int e = GaGlobalRandomIntegerGenerator->Generate( chromosome->GetParameters().GetMutationSize() );
			if( !e )
				e = 1;

			int s = chromosome->GetCodeSize();
			for( int i = e; i > 0; i-- )
				code->Flip( GaGlobalRandomIntegerGenerator->Generate( s - 1 ), 1 );
		}

		// Performs mutation operation on given chromosome.
		void GaInvertMutation::operator ()(GaChromosome* chromosome) const
		{
			GaMutableCode* code = dynamic_cast<GaMutableCode*>( chromosome );

			int e = GaGlobalRandomIntegerGenerator->Generate( chromosome->GetParameters().GetMutationSize() );
			if( !e )
				e = 1;

			int s = chromosome->GetCodeSize();
			for( int i = e; i > 0; i-- )
				code->Invert( GaGlobalRandomIntegerGenerator->Generate( s - 1 ), 1 );
		}

		// Performs mutation operation on given chromosome.
		void GaSwapMutation::operator ()(GaChromosome* chromosome) const
		{
			GaSwapableCode* code = dynamic_cast<GaSwapableCode*>( chromosome );

			int e = GaGlobalRandomIntegerGenerator->Generate( chromosome->GetParameters().GetMutationSize() );
			if( !e )
				e = 1;

			int s = chromosome->GetCodeSize();
			for( int i = e; i > 0; i-- )
			{
				code->Swap( GaGlobalRandomIntegerGenerator->Generate( s - 1 ), 1, 
					GaGlobalRandomIntegerGenerator->Generate( s - 1 ), 1 );
			}
		}

	} // Mutation
} // Chromosome
