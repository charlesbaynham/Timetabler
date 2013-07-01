
/*! \file ScalingOperations.h
    \brief This file contains declaration of classes that implements scaling operations and their parameters.
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

#ifndef __GA_SCALING_OPERATIONS_H__
#define __GA_SCALING_OPERATIONS_H__

#include "Chromosome.h"
#include "PopulationOperations.h"

using namespace Chromosome;
using namespace Population;

namespace Population
{
	/// <summary>Contains implementation of some basic scaling operations.</summary>
	namespace ScalingOperations
	{

		/// <summary><c>GaWindowScaling</c> operation calculates scaled fitness value of chromosome by subtracting fitness value of worst chromosome from
		/// fitness value of chromosome which is scaled. This operation can work with minimization or maximization of fitness values, as well as negative fitness
		/// values. This scaling operation is not based on ranking of chromosome and doesn't require any parameters.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaWindowScaling : public GaScalingOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaScalingOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual float GACALL operator ()(const GaScaledChromosome& chromosome,
				const GaPopulation& population,
				const GaScalingParams& parameters) const;

			/// <summary>More details are given in specification of <see cref="GaScalingOperation::IsRankingBased" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>This method returns <c>false</c>.</returns>
			virtual bool GACALL IsRankingBased() const { return false; }

			/// <summary>More details are given in specification of <see cref="GaScalingOperation::NeedRescaling" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns <c>true</c> if the fitness value of the worst chromosomes in population has changed.</returns>
			GAL_API
			virtual bool GACALL NeedRescaling(const GaPopulation& population,
				const GaScalingParams& parameters) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method always returns <c>NULL</c>.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return NULL; }

			/// <summary>More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method always returns <c>true</c>.</returns>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return true; }

		};// END CLASS DEFINITION GaWindowScaling

		/// <summary>This class should be used (directly or by inheritance) by scaling operations which use user defined factor of scaling.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		class GaScaleFactorParams : public GaScalingParams
		{

		private:

			/// <summary>Scaling factor.</summary>
			float _factor;

		public:

			/// <summary>This constructor initializes parameters with user defined scale factor.</summary>
			/// <param name="factor">scaling factor.</param>
			GaScaleFactorParams(float factor) : _factor(factor) { }

			/// <summary>This constructor initializes parameters with default values. Default scaling factor is 1.</summary>
			GaScaleFactorParams() : _factor( 1 ) { }

			/// <summary>More details are given in specification of <see cref="GaParameters::Clone" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual GaParameters* GACALL Clone() const { return new GaScaleFactorParams( *this ); }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns scaling factor.</returns>
			inline float GACALL GetFactor() const { return _factor; }

			/// <summary><c>SetFactor</c> method sets scaling factor. 
			///
			/// This method is not thread-safe.</summary>
			/// <param name="factor">scaling factor.</param>
			inline void GACALL SetFactor(float factor) { _factor = factor; }

		};// END CLASS DEFINITION GaScaleFactorParams

		/// <summary><c>GaExponentialScaling</c> operation calculates scaled fitness value of chromosome by raising fitness value to specified power
		/// which is defined by scale factor in parameters. This scaling operation is not based on ranking of chromosome and use <see cref="GaScaleFactor" />
		/// class for parameters.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaExponentialScaling : public GaScalingOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaScalingOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual float GACALL operator ()(const GaScaledChromosome& chromosome,
				const GaPopulation& population,
				const GaScalingParams& parameters) const;

			/// <summary>More details are given in specification of <see cref="GaScalingOperation::IsRankingBased" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>This method returns <c>false</c>.</returns>
			virtual bool GACALL IsRankingBased() const { return false; }

			/// <summary>More details are given in specification of <see cref="GaScalingOperation::NeedRescaling" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns true if scale factor in operation parameters has changed.</returns>
			GAL_API
			virtual bool GACALL NeedRescaling(const GaPopulation& population,
				const GaScalingParams& parameters) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaScaleFactorParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaScaleFactorParams(); }

			/// <summary>More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method always returns <c>true</c>.</returns>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return true; }

		};// END CLASS DEFINITION GaExponentialScaling

		/// <summary><c>GaLinearScaling</c> operation calculates scaled fitness value of chromosome by applying linear function <c>a * f + b</c> fitness value,
		/// to where a and b are calculated from scale factor which is provided in operation parameters. This scaling operation is not based on ranking
		/// of chromosome and use <see cref="GaScaleFactor" /> class for parameters.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaLinearScaling : public GaScalingOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaScalingOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual float GACALL operator ()(const GaScaledChromosome& chromosome,
				const GaPopulation& population,
				const GaScalingParams& parameters) const;

			/// <summary>More details are given in specification of <see cref="GaScalingOperation::IsRankingBased" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>This method returns <c>false</c>.</returns>
			virtual bool GACALL IsRankingBased() const { return false; }

			/// <summary>More details are given in specification of <see cref="GaScalingOperation::NeedRescaling" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns <c>true</c> if scale factor in operation parameters has changed.</returns>
			GAL_API
			virtual bool GACALL NeedRescaling(const GaPopulation& population,
				const GaScalingParams& parameters) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaScaleFactorParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaScaleFactorParams(); }

			/// <summary>More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method always returns <c>true</c>.</returns>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const
			{
				return ( (const GaScaleFactorParams&)parameters ).GetFactor() > 0 && ( (const GaScaleFactorParams&)parameters ).GetFactor() <= 1;
			}

		};// END CLASS DEFINITION GaLinearScaling

		/// <summary><c>GaNormalizationScaling</c> operation calculates scaled fitness based on ranking of chromosome.
		/// This scaling operation doesn't require any parameters. 
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaNormalizationScaling : public GaScalingOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaScalingOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual float GACALL operator ()(const GaScaledChromosome& chromosome,
				const GaPopulation& population,
				const GaScalingParams& parameters) const;

			/// <summary>More details are given in specification of <see cref="GaScalingOperation::IsRankingBased" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>This method returns <c>true</c>.</returns>
			virtual bool GACALL IsRankingBased() const { return true; }

			/// <summary>More details are given in specification of <see cref="GaScalingOperation::NeedRescaling" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns <c>true</c> if size of population has changed.</returns>
			GAL_API
			virtual bool GACALL NeedRescaling(const GaPopulation& population,
				const GaScalingParams& parameters) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method always returns <c>NULL</c>.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return NULL; }

			/// <summary>More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method always returns <c>true</c>.</returns>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return true; }

		};// END CLASS DEFINITION GaNormalizationScaling

	} // ScalingOperations
} // Population

#endif // __GA_SCALING_OPERATIONS_H__
