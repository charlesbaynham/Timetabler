
/*! \file Random.cpp
    \brief This file contains implementation of random number generators.
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

#include "Random.h"

namespace Common
{

	// Generator of random bits
	unsigned int GaRandomGenerator::Generate()
	{
		LOCK_THIS_OBJECT( lock );

		// calculate new state of the generator
		_currentState._z = 0x9069 * ( _currentState._z & 0xffff ) + ( _currentState._z >> 16 );
		_currentState._w = 0x4650 * ( _currentState._w & 0xffff ) + ( _currentState._w >> 16 );

		// generate new random value
		return ( _currentState._z << 16 ) + _currentState._w;
	}

	// Generate random single precision floating point number in interval 0..1
	float GaRandomGenerator::GenerateFloat()
	{
		GaUnsignedIntToFloat converter;

		// generate random bits
		converter.bits = Generate();

		// covert to double
		converter.bits = ( converter.bits & 0x007FFFFF ) | 0x3F800000;

		return converter.number - 1;
	}

	// Generate random double precision floating point number in interval 0..1
	double GaRandomGenerator::GenerateDouble()
	{
		GaUnsignedIntToDouble converter;

		// generate random bits
		converter.bits[ 0 ] = Generate();
		converter.bits[ 1 ] = Generate();

		// covert to double
		converter.bits[ _littleEndian ] = ( converter.bits[ 0 ] & 0x000FFFFF ) | 0x3FF00000;

		return converter.number - 1;
	}

	// Initialization of random generator
	void GaRandomGenerator::Initalization(unsigned int seed1,
		unsigned int seed2)
	{
		// initialize seed
		_currentState._w = seed1 ? seed1 : 0x1f123bb5;
		_currentState._z = seed2 ? seed2 : 0x159a55e5;

		// detecting big or little endian
		GaUnsignedIntToDouble converter;
		converter.number = 1;
		_littleEndian = converter.bits[ 1 ] == 0x3FF00000;
	}

} // Common
