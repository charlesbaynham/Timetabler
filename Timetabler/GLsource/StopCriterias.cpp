
/*! \file StopCriterias.cpp
    \brief This file implements stop criteria classes.
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

#include "FPEqual.h"
#include "Algorithm.h"
#include "StopCriterias.h"

namespace Algorithm
{
	namespace StopCriterias
	{

		// Compares the state of the algorithm against given stopping condition and given paremeters,
		// and returns TRUE if that condition is reached.
		bool GaGenerationCriteria::Evaluate(const GaAlgorithm& algorithm,
			const GaStopCriteriaParams& parameters) const
		{
			return algorithm.GetAlgorithmStatistics().GetCurrentGeneration() >= ( (const GaGenerationCriteriaParams&) parameters ).GetNumberOfGeneration();
		}

		// Compares the state of the algorithm against given stopping condition and given paremeters,
		// and returns TRUE if that condition is reached.
		bool GaFitnessCriteria::Evaluate(const GaAlgorithm& algorithm,
			const GaStopCriteriaParams& parameters) const
		{
			const GaFitnessCriteriaParams& p = (const GaFitnessCriteriaParams&)parameters;
			float current = algorithm.GetAlgorithmStatistics().GetValue( p.GetValueType() ).GetCurrent();

			bool res = false;

			// fitness limit reached
			if( p.GetComparison() & GFC_EQUALS_TO )
				res |= FloatsEqual( current, p.GetFitnessLimit() );
			if( p.GetComparison() & GFC_LESS_THEN )
				res |= current < p.GetFitnessLimit();
			if( p.GetComparison() & GFC_MORE_THEN )
				res |= current > p.GetFitnessLimit();

			return res;
		}

		// Compares the state of the algorithm against given stopping condition and given paremeters,
		// and returns TRUE if that condition is reached.
		bool GaFitnessProgressCriteria::Evaluate(const GaAlgorithm& algorithm,
			const GaStopCriteriaParams& parameters) const
		{
			const GaFitnessProgressCriteriaParams& p = (const GaFitnessProgressCriteriaParams&)parameters;

			float previous = algorithm.GetAlgorithmStatistics().GetValue( p.GetValueType() ).GetPrevious();
			float current = algorithm.GetAlgorithmStatistics().GetValue( p.GetValueType() ).GetCurrent();

			float progress;
			 
			progress = fabs( current - previous );
			if( p.GetAbsolute() )
				progress = progress / previous * 100;

			bool res = false;

			// limit of progress reached
			if( p.GetComparison() & GFC_EQUALS_TO )
				res |= FloatsEqual( progress, p.GetRequiredProgress() );
			if( p.GetComparison() & GFC_LESS_THEN )
				res |= progress < p.GetRequiredProgress();
			if( p.GetComparison() & GFC_MORE_THEN )
				res |= progress > p.GetRequiredProgress();

			if( res )
			{
				// increas
				int c = p.GetCurrent();
				p.SetCurrent( c + 1 );

				// history depth reached?
				if( c < p.GetHistoryDepth() )
					res = false;
				else
					p.SetCurrent( 0 );
			}
			else
				// restart history
				p.SetCurrent( 0 );

			return res;
		}

	}// StopCriterias
} // Algorithm
