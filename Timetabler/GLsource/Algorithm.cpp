
/*! \file Algorithm.cpp
    \brief This file implements methods of abstract genetic algorithm classes.
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

#include "Algorithm.h"

namespace Algorithm
{

	// Sends signal to algoritham to start evolution and problem solving.
	// It can be specified if the solving is continued from previosly saved state
	// or evolution can be started from the beginning discharging saved state,
	void GaBaseAlgorithm::StartSolving(bool continueSolving)
	{
		BlockStateChange();

		GaAlgorithmState _oldState = _state;

		switch( _state )
		{

		case GAS_UNINITIALIZED:
		case GAS_USER_STOPED:
			if( !continueSolving )
			{
				// restart and initialize algorithm
				Initialize();

				// update state
				_state = GAS_RUNNING;

				// start execution
				if( !OnStart() )
					// something went wrong - restart to old state
					_state = _oldState;
			}
			break;

		case GAS_CRITERIA_STOPPED:
			if( continueSolving )
			{
				// update state
				_state = GAS_RUNNING;

				// start execution
				if( !OnResume() )
					// something went wrong - restart to old state
					_state = _oldState;
			}
			else
			{
				// restart and initialize algorithm
				Initialize();

				// update state
				_state = GAS_RUNNING;

				// start execution
				if( !OnStart() )
					// something went wrong - restart to old state
					_state = _oldState;
			}

			break;

		case GAS_PAUSED:
			if( continueSolving )
			{
				// update state
				_state = GAS_RUNNING;

				// start execution
				if( !OnResume() )
					// something went wrong - restart to old state
					_state = _oldState;
			}
			break;

		}

		// raise state change event
		if( _state == GAS_RUNNING )
			_observers.EvolutionStateChanged( _state, *this );

		ReleaseStateChange();
	}

	// Sends signal to algorithm to stop evolution and problem solving.
	// and to discharge current state.
	void GaBaseAlgorithm::StopSolving()
	{
		BlockStateChange();

		switch( _state )
		{

		case GAS_RUNNING:
		case GAS_PAUSED:
			// stop execution
			if( OnStop() )
			{
				// execution stopped successafully - update state
				_state = GAS_USER_STOPED;

				// raise state change event
				_observers.EvolutionStateChanged( _state, *this );

			}
			break;

		}

		ReleaseStateChange();
	}

	// Sends signal to algorithm to temporary stop evolution and problem solving.
	// but to save the current state.
	void GaBaseAlgorithm::PauseSolving()
	{
		BlockStateChange();

		if( _state == GAS_RUNNING )
		{
			// pause execution
			if( OnPause() )
			{
				// execution paused successafully - update state
				_state = GAS_PAUSED;

				// raise state change event
				_observers.EvolutionStateChanged( _state, *this );
			}
		}

		ReleaseStateChange();
	}

	// Check to see if stop criteria is reached.
	// if it is reached then sets algorithm state to GAS_CRITERIA_STOPPED and return TRUE.
	bool GaBaseAlgorithm::CheckStopCriteria()
	{
		if( !&_stopCriteria.GetOperation() || _stopCriteria.GetOperation().Evaluate( *this, _stopCriteria.GetParameters() ) )
		{
			if( _state == GAS_RUNNING )
			{
				_state = GAS_CRITERIA_STOPPED;

				// raise state change event
				_observers.EvolutionStateChanged( _state, *this );
			}

			return true;
		}

		return false;
	}

} // Algorithm
