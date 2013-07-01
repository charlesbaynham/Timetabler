
/*! \file Observing.cpp
    \brief This file contains implementation of classes needed to observe execution of an genetic algorithm.
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

#include "Observing.h"
#include "Algorithm.h"

namespace Observing
{
	// Notifies the observer that new statistical information is available
	void GaObserversList::StatisticUpdate(const GaStatistics& statistics,
		const GaAlgorithm& algorithm)
	{
		LOCK_THIS_OBJECT( lock );

		for( GaObserversListType::const_iterator it = _observers.begin(); it != _observers.end(); it++ )
			(*it)->StatisticUpdate( statistics, algorithm );
	}

	// Notifies observer that new best chromosome has found
	void GaObserversList::NewBestChromosome(const GaChromosome& newChromosome,
		const GaAlgorithm& algorithm)
	{
		LOCK_THIS_OBJECT( lock );

		for( GaObserversListType::const_iterator it = _observers.begin(); it != _observers.end(); it++ )
			(*it)->NewBestChromosome( newChromosome, algorithm );
	}

	// Notifies observer that state of evolution (problem sloving) has changed.
	void GaObserversList::EvolutionStateChanged(GaAlgorithmState newState,
		const GaAlgorithm& algorithm)
	{
		LOCK_THIS_OBJECT( lock );

		for( GaObserversListType::const_iterator it = _observers.begin(); it != _observers.end(); it++ )
			(*it)->EvolutionStateChanged( newState, algorithm );
	}

	// Subscribes observer
	GaObserversList& GaObserversList::operator +=(GaObserver& observer)
	{
		LOCK_THIS_OBJECT( lock );

		// observer can subscribe only once
		_observers.remove( &observer );
		_observers.push_back( &observer );

		return *this;
	}

	// Unsubscribes observer
	GaObserversList& GaObserversList::operator -=(GaObserver& observer)
	{
		LOCK_THIS_OBJECT( lock );

		_observers.remove( &observer );

		return *this;
	}

	// Returns the number of subscribed observers
	int GaObserversList::GetObserverCount() const
	{
		LOCK_THIS_OBJECT( lock );

		return (int)_observers.size();
	}
	
} // Observing
