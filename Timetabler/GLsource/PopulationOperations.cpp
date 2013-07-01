
/*! \file PopulationOperations.cpp
    \brief This file implements classes needed for genetic operations which are performed over population.
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

#include "PopulationOperations.h"
#include "Population.h"

namespace Population
{

	// Initialization
	GaSelectionResultSet::GaSelectionResultSet(int selectionSize,
		const GaPopulation* population)
	{
		_selectedGroup = new GaSortedGroup( population, selectionSize, GASGT_OTHER, population->GetConfiguration().GetSortComparator() );
	}

	// Initialization of the result set
	GaCouplingResultSet::GaCouplingResultSet(int numberOfOffsprings,
		GaSelectionResultSet* selectionResults)
	{
		if( numberOfOffsprings > 0 )
		{
			_numberOfOffsprings = numberOfOffsprings;

			// allocate memory for oggsprings infromation
			_offsprings = new GaChromosomePtr[ _numberOfOffsprings ];
			_parents = new int[ _numberOfOffsprings ];

			for( int i = 0; i < _numberOfOffsprings; i++ )
				_parents[ i ] = -1;

			_selection = selectionResults;
		}
		else
		{
			_numberOfOffsprings = 0;
			_parents = NULL;
			_offsprings = NULL;
		}

		_selection = selectionResults;
	}

	// Frees aquired resources
	GaCouplingResultSet::~GaCouplingResultSet()
	{
		if( _parents )
			delete[] _parents;

		if( _offsprings )
			delete[] _offsprings;
	}

	// Returns offspring at given position and its parent
	bool GaCouplingResultSet::GetOffspringAt(int pos,
		GaChromosomePtr& offspring,
		int& parent) const
	{
		if( pos < 0 || pos >= _numberOfOffsprings )
			return false;

		offspring = _offsprings[ pos ];
		parent = _parents[ pos ];

		return true;
	}

	// Sets offspring at given position
	bool GaCouplingResultSet::SetOffspringAt(int pos,
		GaChromosomePtr offspring,
		int parent)
	{
		if( pos < 0 || pos >= _numberOfOffsprings )
			return false;

		_offsprings[ pos ] = offspring;
		_parents[ pos ] = parent;

		return true;
	}

	// Sets number of offsprings
	void GaCouplingResultSet::SetNumberOfOffsprings(int number)
	{
		if( _numberOfOffsprings == number )
			return;

		// no offsprings
		if( number <= 0 )
		{
			// free memory 

			if( _offsprings )
				delete[] _offsprings;

			if( _parents )
				delete[] _parents;

			_numberOfOffsprings = 0;
			_offsprings = NULL;
			_parents = NULL;

			return;
		}

		// old arrays
		int oldSize = _numberOfOffsprings;
		GaChromosomePtr* oldOffsprings = _offsprings;
		int* oldParents = _parents;

		// alocate memory for new arrays
		_offsprings = new GaChromosomePtr[ number ];
		_parents = new int[ number ];

		int lim = 0;
		if( _offsprings )
		{
			lim = _numberOfOffsprings < number ? _numberOfOffsprings : number;

			// copy values form old arrays to new ones
			for( int i = 0; i < lim; i++ )
			{
				_offsprings[ i ] = oldOffsprings[ i ];
				_parents[ i ] = oldParents[ i ];
			}
		}

		// if new array is bigger then old, parent values in extended part of new array is invalid
		for( int i = _numberOfOffsprings; i < number; i++ )
			_parents[ i ] = -1;

		_numberOfOffsprings = number;
	}

} // Population
