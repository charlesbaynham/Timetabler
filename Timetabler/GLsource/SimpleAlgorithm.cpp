
/*! \file SimpleAlgorithm.cpp
    \brief This file implements class of simple genetic algorithm with non-overlapping populations.
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
#include "SimpleAlgorithm.h"

namespace Algorithm
{
	namespace SimpleAlgorithms
	{

		// Initialize algorithm
		GaSimpleAlgorithm::GaSimpleAlgorithm(GaPopulation* population,
			const GaSimpleAlgorithmParams& parameters) : GaMultithreadingAlgorithm(parameters)
		{
			_populations[ GAPT_POPULATION_A ] = population;
			_populations[ GAPT_POPULATION_B ] = NULL;

			int buffers = parameters.GetNumberOfWorkers() * 2;

			_buffers[ GAPT_POPULATION_A ] =
				new GaCouplingResultSet( 0, new GaSelectionResultSet( 0, population ) );
			_buffers[ GAPT_POPULATION_B ] = NULL;

			_elitismBuffer = parameters.GetElitism() > 0 ? new GaChromosomePtr[ parameters.GetElitism() ] : NULL;

			_parameters = parameters;
		}

		// Free resources
		GaSimpleAlgorithm::~GaSimpleAlgorithm()
		{
			for( int i = 0; i < 2; i++ )
			{
				if( _buffers[ i ] )
				{
					delete &_buffers[ i ]->GetSelectionResultSet();
					delete _buffers[ i ];
				}
			}

			if( _populations[ GAPT_POPULATION_B ] )
				delete _populations[ GAPT_POPULATION_B ];

			if( _elitismBuffer )
				delete[] _elitismBuffer;
		}

		// Sets new parameters for algorithm
		void GaSimpleAlgorithm::SetAlgorithmParameters(const GaAlgorithmParams& parameters)
		{
			const GaSimpleAlgorithmParams& p = (const GaSimpleAlgorithmParams&)parameters;

			// change size of elitism buffer
			if( _parameters.GetElitism() != p.GetElitism() )
			{
				// free memory of old buffer
				if( _elitismBuffer )
					delete[] _elitismBuffer;

				// make new buffer
				if( p.GetElitism() > 0 )
					_elitismBuffer = new GaChromosomePtr[ p.GetElitism() ];
				else
					_elitismBuffer = NULL;
			}

			// change parameters of multithreading
			GaMultithreadingAlgorithm::SetAlgorithmParameters( parameters );

			// save parameters
			_parameters = (const GaSimpleAlgorithmParams&) parameters;
		}

		// Initialize the algorithm
		void GaSimpleAlgorithm::Initialize()
		{
			_currentPopulation = GAPT_POPULATION_A;

			_populations[ GAPT_POPULATION_A ]->InitializePopulation();

			if( _populations[ GAPT_POPULATION_B ] )
				delete _populations[ GAPT_POPULATION_B ];

			_populations[ GAPT_POPULATION_B ] = _populations[ GAPT_POPULATION_A ]->Clone( false );

			_buffers[ GAPT_POPULATION_B ] =
				new GaCouplingResultSet( 0, new GaSelectionResultSet( 0, _populations[ GAPT_POPULATION_B ] ) ); 
		}

		// Step of control flow before workers start
		void GaSimpleAlgorithm::BeforeWorkers()
		{
			GaPopulation* cp = _populations[ _currentPopulation ];
			// preapare population for next generation
			cp->NextGeneration();

			// get buffers
			GaCouplingResultSet* offsprings = _buffers[ _currentPopulation ];
			GaSelectionResultSet* selection = &offsprings->GetSelectionResultSet();

			// change the buffers' sizes if needed
			selection->SelectedGroup().SetMaxSize( cp->GetConfiguration().Selection().GetParameters().GetSelectionSize() );
			int couplingSize = cp->GetConfiguration().GetParameters().GetPopulationSize() - _parameters.GetElitism();
			cp->GetConfiguration().Coupling().GetParameters().SetNumberOfOffsprings( couplingSize );
			offsprings->SetNumberOfOffsprings( couplingSize );

			// selection
			cp->GetConfiguration().Selection().GetOperation()( *cp, cp->GetConfiguration().Selection().GetParameters(), *selection );

			_savedChromosomes = 0;
		}

		// One step of work flow
		void GaSimpleAlgorithm::WorkStep(int workerId)
		{
			GaPopulation* src = _populations[ _currentPopulation ];
			GaPopulation* dst = _currentPopulation == GAPT_POPULATION_A ? 
				_populations[ GAPT_POPULATION_B ] : _populations[ GAPT_POPULATION_A ];

			// get buffers
			GaCouplingResultSet* offsprings = _buffers[ _currentPopulation ];
			GaSelectionResultSet* selection = &offsprings->GetSelectionResultSet();

			// elitism enabled?
			if( !workerId && _parameters.GetElitism() > 0 )
				// save the best chromosomes from old generation
				_savedChromosomes = src->GetBestChromosomes( _elitismBuffer, 0, _parameters.GetElitism() );

			// coupling
			src->GetConfiguration().Coupling().GetOperation()( *src, *offsprings, src->GetConfiguration().Coupling().GetParameters(),
				workerId, _parameters.GetNumberOfWorkers() );
		}

		// Step of control flow after workers finish
		void GaSimpleAlgorithm::AfterWorkers()
		{
			// get buffer
			GaCouplingResultSet* offsprings = _buffers[ _currentPopulation ];

			// swap populations
			GaPopulation* pp = _populations[ _currentPopulation ];
			_currentPopulation = _currentPopulation == GAPT_POPULATION_A ? GAPT_POPULATION_B : GAPT_POPULATION_A;
			GaPopulation* cp = _populations[ _currentPopulation ];

			// insert new chromosomes
			cp->InsertGroup( offsprings->GetOffspringsBuffer(), offsprings->GetNumberOfOffsprings() );

			// copy best chromosomes from previous population
			if( _savedChromosomes )
				cp->InsertGroup( _elitismBuffer, _savedChromosomes );

			// update statistics of population
			cp->EndOfGeneration( *pp );

			// clear old population
			pp->Clear( true );

			// rais "update statistics" event
			_observers.StatisticUpdate( cp->GetStatistics(), *this );

			// get best chromosome
			int i;
			cp->GetBestChromosomes( &i, 0, 1 );
			GaChromosomePtr f = cp->GetAt( i ).GetChromosome();

			// best chromosome changed?
			if( cp->GetStatistics().GetCurrentGeneration() == 1 || *f != *_bestChromosome )
			{
				// raise "new best chromosome found" event
				_bestChromosome = f;
				_observers.NewBestChromosome( *_bestChromosome, *this );
			}
		}

	} // SimpleAlgorithms
} // Algorithm
