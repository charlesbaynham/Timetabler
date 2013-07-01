
/*! \file FitnessComparators.h
    \brief This file contains declaration and implementation of basic fitness comparators.
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

#ifndef __GA_FITNESS_COMPARATORS_H__
#define __GA_FITNESS_COMPARATORS_H__

#include "Platform.h"
#include "ChromosomeOperations.h"

using namespace Chromosome;

namespace Chromosome
{
	/// <summary>Contains implementation of some basic fitness comparators.</summary>
	namespace FitnessComparators
	{

		/// <summary><c>GaMinFitnessComparator</c> can be used when objective of evolution is to minimize fitness value. 
		///
		/// This class is abstract and has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used
		/// with instances of this class, but because all built-in crossover operations are stateless they are thread-safe.</summary>
		class GaMinFitnessComparator : public GaFitnessComparator
		{

		public:

			/// <summary>Detailed description can be found in specification of <see cref="GaFitnessComparator::operator ()" />.</summary>
			virtual int GACALL operator ()(float fitness1,
				float fitness2) const { return (int)( fitness1 < fitness2 ) - (int)( fitness1 > fitness2 ); }

		};// END CLASS DEFINITION GaMinFitnessComparator

		/// <summary><c>GaMaxFitnessComparator</c> can be used when objective of evolution is to maximize fitness value.
		///
		/// This class is abstract and has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used
		/// with instances of this class, but because all built-in crossover operations are stateless they are thread-safe.</summary>
		class GaMaxFitnessComparator : public GaFitnessComparator
		{

		public:

			/// <summary>Detailed description can be found in specification of <see cref="GaFitnessComparator::operator ()" />.</summary>
			virtual int GACALL operator ()(float fitness1,
				float fitness2) const { return (int)( fitness1 > fitness2 ) - (int)( fitness1 < fitness2 ); }

		};// END CLASS DEFINITION GaMaxFitnessComparator

	} // FitnessComparators
} // Chromosome

#endif // __GA_FITNESS_COMPARATORS_H__

