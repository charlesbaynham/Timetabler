/*! \file AlgorithmState.h
    \brief This file contains declaration of GaAlgorithmState datatype.
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

#ifndef __GA_ALGORITHM_STATE_H__
#define __GA_ALGORITHM_STATE_H__

namespace Algorithm
{

	/// <summary>This enumeration defines states of an algorithm.</summary>
	enum GaAlgorithmState
	{
		/// <summary>algorithm its populations and other control and data structures is not initialized.</summary>
		GAS_UNINITIALIZED = 0x1, 

		/// <summary>algorithm execution is stopped by the user.</summary>
		GAS_USER_STOPED = 0x2, 

		/// <summary>algorithm reached given criteria and it is stopped.</summary>
		GAS_CRITERIA_STOPPED = 0x4, 

		/// <summary>temporary paused, but it can resume execution at any time.</summary>
		GAS_PAUSED = 0x8, 

		/// <summary>algorithm is running.</summary>
		GAS_RUNNING = 0x10, 

		/* --- combined states --- */

		/// <summary>used for testing of state of an algorithm and includes all states except running state.</summary>
		GAS_NOT_RUNNING = 0xF, 

		/// <summary>used to test an algorithm, for one of two possible stopped states.</summary>
		GAS_STOPPED = 0x6
	};

} // Algorithm

#endif // __GA_ALGORITHM_STATE_H__
