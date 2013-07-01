
/*! \file IncrementalAlgorithm.cpp
    \brief This file implements class of incremental genetic algorithm with overlapping population.
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
#include "IncrementalAlgorithm.h"

namespace Algorithm
{
	namespace SimpleAlgorithms
	{

		// Sets new parameters for algorithm
		void GaIncrementalAlgorithm::SetAlgorithmParameters(const GaAlgorithmParams& parameters)
		{
			// change parameters of multithreading
			GaMultithreadingAlgorithm::SetAlgorithmParameters( parameters );

			// save parameters
			_parameters = (const GaMultithreadingAlgorithmParams&) parameters;
		}

		// Step of control flow before workers start
		void GaIncrementalAlgorithm::BeforeWorkers()
		{
			// update statistics of population
			_population->NextGeneration();

			// get buffers
			GaSelectionResultSet* selection = &_buffer->GetSelectionResultSet();

			// change the buffers' sizes if needed
			selection->SelectedGroup().SetMaxSize( _population->GetConfiguration().Selection().GetParameters().GetSelectionSize() );
			_buffer->SetNumberOfOffsprings( _population->GetConfiguration().Coupling().GetParameters().GetNumberOfOffsprings() );

			// selection
			_population->GetConfiguration().Selection().GetOperation()( *_population, _population->GetConfiguration().Selection().GetParameters(), *selection );
		}

		// One step of work flow
		void GaIncrementalAlgorithm::WorkStep(int workerId)
		{
			// coupling
			_population->GetConfiguration().Coupling().GetOperation()( *_population, *_buffer, _population->GetConfiguration().Coupling().GetParameters(), 
				workerId, _parameters.GetNumberOfWorkers() );
		}

		// Step of control flow after workers finish
		void GaIncrementalAlgorithm::AfterWorkers()
		{
			// replacement
			_population->GetConfiguration().Replacement().GetOperation()( *_population, _population->GetConfiguration().Replacement().GetParameters(), *_buffer );

			// update population
			_population->EndOfGeneration();

			// rais "update statistics" event
			_observers.StatisticUpdate( _population->GetStatistics(), *this );

			// get best chromosome
			int i;
			_population->GetBestChromosomes( &i, 0, 1 );
			GaChromosomePtr f = _population->GetAt( i ).GetChromosome();

			// best chromosome changed?
			if( _population->GetStatistics().GetCurrentGeneration() == 1 || *f != *_bestChromosome )
			{
				// raise "new best chromosome found" event
				_bestChromosome = f;
				_observers.NewBestChromosome( *_bestChromosome, *this );
			}
		}

	} // SimpleAlgorithms
} // Algorithm
