
/*! \file AlgorithmOperations.h
    \brief This file declares interfaces for genetic algorithm's stop criterias.
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

#ifndef __GA_STOP_CRITERIA_H__
#define __GA_STOP_CRITERIA_H__

#include "Platform.h"
#include "Operation.h"

using namespace Common;

namespace Algorithm
{
	class GaAlgorithm;

	/// <summary>Interface for parameters of stop criteria operation.</summary>
	class GaStopCriteriaParams : public GaParameters { }; // END CLASS DEFINITION GaStopCriteriaParams

	/// <summary>This class is interface for stop criteria of genetic algorithm.
	///
	/// This class is abstract and has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances
	/// of this class, but because all built-in stop criterias operations are stateless they are thread-safe.</summary>
	class GaStopCriteria : public GaOperation
	{

	public:

		/// <summary>This method compares current state of the algorithm against desired state specified in parameters.</summary>
		/// <param name="algorithm">reference to algorithm which state is checked.</param>
		/// <param name="parameters">reference to parameters of stop criteria which describe desired state.</param>
		/// <returns>Method returns <c>true</c> if current state of the algorithm satisfies desired state.
		/// If the algorithm hasn't reached desired state, method returns <c>false</c>.</returns>
		virtual bool GACALL Evaluate(const GaAlgorithm& algorithm,
						const GaStopCriteriaParams& parameters) const=0;

	};// END CLASS DEFINITION GaStopCriteria

	/// <summary><c>GaStopCriteriaCatalogue</c> type is instance of <see cref="GaCatalogue" /> template class and represents catalogue of stop criterias.
	/// Detailed description can be found in specification of <see cref="GaCatalogue" /> template class.</summary>
	typedef GaOperationParametersPair<GaStopCriteria, GaStopCriteriaParams> GaStopCriteriaPair;

	/// <summary><c>GaStopCriteriaPair</c> type is instance of <see cref="GaOperationParametersPair" /> template class and represents pair of stop criteria
	/// and its parameters. Detailed description can be found in specification of <see cref="GaOperationParametersPair" /> template class.</summary>
	typedef GaCatalogue<GaStopCriteria> GaStopCriteriaCatalogue;

} // Algorithm

#endif // __GA_STOP_CRITERIA_H__
