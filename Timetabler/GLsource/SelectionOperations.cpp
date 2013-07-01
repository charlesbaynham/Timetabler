
/*! \file SelectionOperations.cpp
    \brief This file contains implementation of selection operation classes.
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
#include "SelectionOperations.h"

using namespace Population;
using namespace Population::SelectionOperations;

namespace Population
{
	namespace SelectionOperations
	{

		// Selects chromosomes from the given population based on passed parameters
		void GaSelectBest::operator ()(const GaPopulation& population,
			const GaSelectionParams& parameters,
			GaSelectionResultSet& result) const
		{
			result.SelectedGroup().Clear();

			for( int i = 0; i < parameters.GetSelectionSize(); i++ )
			{
				int c;
				if( !population.GetBestChromosomes( &c, i, 1 ) )
					break;

				result.SelectedGroup().Add( c );
			}
		}

		// Selects chromosomes from the given population based on passed parameters
		void GaSelectWorst::operator ()(const GaPopulation& population,
			const GaSelectionParams& parameters,
			GaSelectionResultSet& result) const
		{
			result.SelectedGroup().Clear();

			for( int i = 0; i < parameters.GetSelectionSize(); i++ )
			{
				int c;
				if( !population.GetWorsChromosomes( &c, i, 1 ) )
					break;

				result.SelectedGroup().Add( c );
			}
		}

		// Selects chromosomes from the given population based on passed parameters
		void GaSelectRandom::operator ()(const GaPopulation& population,
			const GaSelectionParams& parameters,
			GaSelectionResultSet& result) const
		{
			result.SelectedGroup().Clear();

			int number = parameters.GetSelectionSize();
			if( ( (const GaSelectDuplicatesParams&)parameters ).GetDuplicates() )
				number = min( number, population.GetCurrentSize() );

			for( int i = 0; i < number; i++ )
			{
				Select( result, population, (const GaSelectRandomBestParams&) parameters );

				// no more space in result set?
				if( result.SelectedGroup().GetCurrentSize() == result.SelectedGroup().GetMaxSize() )
					return;
			}
		}

		// Randomly select one chromosome from given population
		void GaSelectRandom::Select(GaSelectionResultSet& results,
			const GaPopulation& population,
			const GaSelectionParams& parameters) const
		{
			bool d = ( (const GaSelectDuplicatesParams&)parameters ).GetDuplicates();
			int c;
			do
			{
				c = GaGlobalRandomIntegerGenerator->Generate( population.GetCurrentSize() - 1 );
			} while( !d && results.SelectedGroup().GetRanking( c ) >= 0 );

			results.SelectedGroup().Add( c );
		}

		// Selects chromosomes from the given population based on passed parameters
		void GaSelectRandomBest::operator ()(const GaPopulation& population,
			const GaSelectionParams& parameters,
			GaSelectionResultSet& result) const
		{
			result.SelectedGroup().Clear();

			int number = ( (const GaSelectRandomBestParams&)parameters ).GetGroupSize();
			if( ( (const GaSelectDuplicatesParams&)parameters ).GetDuplicates() )
				number = min( number, population.GetCurrentSize() );

			for( int i = 0; i < number; i++ )
				Select( result, population, parameters );
		}

		// Selects chromosomes from the given population based on passed parameters
		void GaSelectRouletteWheel::operator ()(const GaPopulation& population,
			const GaSelectionParams& parameters,
			GaSelectionResultSet& result) const
		{
			result.SelectedGroup().Clear();

			int number = ( (const GaSelectionParams&)parameters ).GetSelectionSize();
			if( ( (const GaSelectDuplicatesParams&)parameters ).GetDuplicates() )
				number = min( number, population.GetCurrentSize() );

			for( int i = 0; i < number; i++ )
			{
				if( result.SelectedGroup().Add( Select( result, population, parameters ) ) < 0 )
					return;
			}
		}

		// Select one chromosome
		int GaSelectRouletteWheel::Select(const GaSelectionResultSet& result,
			const GaPopulation& population,
			const GaSelectionParams& parameters) const
		{
			float total = population.GetStatistics().GetValue( GSV_TOTAL_FITNESS_SCALED ).GetCurrent();
			int size = population.GetCurrentSize();

			int i = 0;

			float disp = 0, worst = population.GetStatistics().GetValue( GSV_WORST_FITNESS_SCALED ).GetCurrent();
			int rev = 1;

			if( population.GetAt( 0 ) > population.GetAt( size - 1 ) )
			{
				// maximizing fitness
				rev = 1;

				// if the worst fitness value is negative, move all values so they ar all positive
				if( worst < 0 )
				{
					disp = 2 * -worst;
					total += size * disp; 
				}
			}
			else
			{
				// minimizing fitness
				rev = -1;

				// inverse coefficient
				float c = population.GetAt( size - 1 ).GetScaledFitness() + population.GetAt( 0 ).GetScaledFitness();
				total = c * size - total;

				// if the worst fitness value is negative, move all values so they are all positive
				if( c - worst < 0 )
				{
					disp = 2 * ( worst - c );
					total += size * disp;
				}

				disp += c;
			}

			// run the wheel
			float r = GaGlobalRandomFloatGenerator->Generate( total - 1 );
			for( float s = 0; s < r && i < size - 1; i++ )
				s += disp + rev * population.GetAt( i ).GetScaledFitness();

			// duplicates are not allowed and this one is
			if ( !( (const GaSelectDuplicatesParams&)parameters ).GetDuplicates() && 
				result.SelectedGroup().GetRanking( i ) >= 0 )
			{
				int l = i - 1, h = i + 1;
				// search for nearest chromosome to selected one which is not duplicate
				while( true )
				{
					// search above selected chromosome
					if( l >= 0 )
					{
						// chromosome found?
						if( result.SelectedGroup().GetRanking( l ) < 0 )
							return l;

						l--;
					}
					
					// search belove selected chromosome
					if( h < size )
					{
						// chromosome found?
						if( result.SelectedGroup().GetRanking( h ) < 0 )
							return h;

						h++;
					}
				}
			}

			return i;
		}

		// Selects chromosomes from the given population based on passed parameters
		void GaSelectTournament::operator ()(const GaPopulation& population,
			const GaSelectionParams& parameters,
			GaSelectionResultSet& result) const
		{
			result.SelectedGroup().Clear();

			int number = ( (const GaSelectionParams&)parameters ).GetSelectionSize();
			if( ( (const GaSelectDuplicatesParams&)parameters ).GetDuplicates() )
				number = min( number, population.GetCurrentSize() );

			for( int i = 0; i < number; i++ )
			{
				int best = -1;

				// select chromosome few times and choose the best from those selections
				for( int j = 0; j < ( (const GaSelectTournamentParams&)parameters ).GetNumberOfSelections(); j++ )
				{
					int c = Select( result, population, parameters );

					// is this chromosome better
					if( best < 0 || population.GetAt( best ).CompareFitnesses( population.GetAt( c ) ) < 0 )
						best = c;
				}

				result.SelectedGroup().Add( best );
			}
		}

	} // SelectionOperations
} // Population
