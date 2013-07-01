
/*! \file Statistics.cpp
    \brief This file contains implementation of classes needed for tracking statistical information about populations and genetic algorithms.
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

#include "Statistics.h"
#include <algorithm>

using namespace std;

namespace Common
{

	// Clears all values
	void GaStatistics::Clear()
	{
		_currentGeneration = 0;

		for( int i = 0; i < GaNumberOfStatValueTypes; i++ )
		{
			_values[ i ].SetCurrent( 0 );
			_values[ i ].SetPrevious( 0 );
		}
	}

	// The generation switch, updates statistical information for new and previous generation.
	void GaStatistics::NextGeneration()
	{
		// move curent values to previous
		for( int i = 0; i < GaNumberOfStatValueTypes; i++ )
			_values[ i ].SetPrevious( _values[ i ].GetCurrent() );

		_currentGeneration++;
	}

	// Copies statistic values
	void GaStatistics::CopyFrom(const GaStatistics& stats,
		bool previous,
		bool current,
		bool currentGeneration)
	{
		// copy stats from previous generation
		if( previous )
		{
			for( int i = 0; i < GaNumberOfStatValueTypes; i++ )
				_values[ i ].SetPrevious( stats._values[ i ].GetPrevious() );
		}

		// copy stats from current generation
		if( current )
		{
			for( int i = 0; i < GaNumberOfStatValueTypes; i++ )
				_values[ i ].SetCurrent( stats._values[ i ].GetCurrent() );
		}

		// copy current generation
		if( currentGeneration )
			_currentGeneration = stats._currentGeneration;
	}

	// Get progress of a value between this and previous generation
	float GaStatistics::GetValueProgress(GaStatValueType value,
									 bool percent) const
	{
		// calculate progress
		const GaFloatStatValue* v = _values + value;
		float ret = v->GetCurrent() - v->GetPrevious();

		// convert to percents if needed
		if( percent )
			ret = v->GetPrevious() ? ret * 100 / v->GetPrevious() : 100;

		return ret;
	}

	// Change value
	void GaStatistics::ChangeValue(GaStatValueType type,
							   float value,
							   bool relative)
	{
		// save current value
		float temp = _values[ type ].GetCurrent();

		// update value
		if( relative )
			temp += value;
		else
			temp = value;
		_values[ type ].SetCurrent( temp );

		// update avarge values if needed
		if( type == GSV_TOTAL_FITNESS )
			_values[ GSV_AVG_FITNESS ].SetCurrent( temp / _values[ GSV_POPULATION_SIZE ].GetCurrent() );
		else if( type == GSV_TOTAL_FITNESS_SCALED )
			_values[ GSV_AVG_FITNESS_SCALED ].SetCurrent( temp / _values[ GSV_POPULATION_SIZE ].GetCurrent() );
		else if( type == GSV_POPULATION_SIZE )
		{
			_values[ GSV_AVG_FITNESS ].SetCurrent( temp / _values[ GSV_POPULATION_SIZE ].GetCurrent() );
			_values[ GSV_AVG_FITNESS_SCALED ].SetCurrent( temp / _values[ GSV_POPULATION_SIZE ].GetCurrent() );
		}
	}

	// Combines information
	GaStatistics& GaStatistics::operator +=(const GaStatistics& rhs)
	{
		// fitness of the best chromosome
		float a = _values[ GSV_BEST_FITNESS ].GetCurrent();
		float b = rhs._values[ GSV_BEST_FITNESS ].GetCurrent();

		_values[ GSV_BEST_FITNESS ].SetCurrent( ( *_fitnessComparator )( a, b ) > 0 ? a : b );
		_values[ GSV_BEST_FITNESS_SCALED ].SetCurrent( 0 );

		// fitness of the worst chromosome
		a = _values[ GSV_WORST_FITNESS ].GetCurrent();
		b = rhs._values[ GSV_WORST_FITNESS ].GetCurrent();

		_values[ GSV_WORST_FITNESS ].SetCurrent( ( *_fitnessComparator )( a, b ) < 0 ? a : b );
		_values[ GSV_WORST_FITNESS_SCALED ].SetCurrent( 0 );

		// sum of all fitness values
		_values[ GSV_TOTAL_FITNESS ].SetCurrent( _values[ GSV_TOTAL_FITNESS ].GetCurrent() + rhs._values[ GSV_TOTAL_FITNESS ].GetCurrent() );
		_values[ GSV_TOTAL_FITNESS_SCALED ].SetCurrent( 0 );

		// sum of two population sizes
		_values[ GSV_POPULATION_SIZE ].SetCurrent( _values[ GSV_POPULATION_SIZE ].GetCurrent() + rhs._values[ GSV_TOTAL_FITNESS ].GetCurrent() );

		// avarge fitness value
		_values[ GSV_AVG_FITNESS ].SetCurrent( _values[ GSV_TOTAL_FITNESS ].GetCurrent() / _values[ GSV_POPULATION_SIZE ].GetCurrent() );
		_values[ GSV_AVG_FITNESS_SCALED ].SetCurrent( 0 );

		return *this;
	}

} // Common
