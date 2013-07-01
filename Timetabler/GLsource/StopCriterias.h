
/*! \file StopCriterias.h
    \brief This file declares classes of classes that implement stop criterias and their parameters.
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

#ifndef __GA_STOP_CRITERIAS_H__
#define __GA_STOP_CRITERIAS_H__

#include <math.h>
#include "Platform.h"
#include "AlgorithmOperations.h"

using namespace Algorithm;

namespace Algorithm
{
	/// <summary>Contains implementation of stop criterias used by genetic algorithms.</summary>
	namespace StopCriterias
	{

		/// <summary><c>GaGenerationCriteriaParams</c> class is used by <see cref="GaGenerationCriteria" /> class as parameters for the criteria.
		/// It contains number of generation after which execution of algorithm should be stopped.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		class GaGenerationCriteriaParams : public GaStopCriteriaParams
		{

		private:

			/// <summary>Number of generations after which algorithm should stops its execution.</summary>
			int _numberOfGenerations;

		public:

			/// <summary>This constructor initializes parameters with user-defined number of generations.</summary>
			/// <param name="numberOfGeneration">number of generations after which algorithm should stops its execution.</param>
			GaGenerationCriteriaParams(int numberOfGeneration) : _numberOfGenerations(numberOfGeneration) { }

			/// <summary>This constructor initializes parameters with default values. Default number of generations is 1000.</summary>
			GaGenerationCriteriaParams() : _numberOfGenerations(1000) { }

			/// <summary>More details are given in specification of <see cref="GaParameters::Clone" /> method.</summary>
			virtual GaParameters* GACALL Clone() const { return new GaGenerationCriteriaParams( *this ); }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns number of generations after which algorithm should stops its execution.</returns>
			inline int GACALL GetNumberOfGeneration() const { return _numberOfGenerations; }

			/// <summary><c>SetNumberOfGeneration</c> method sets number of generations after which algorithm should stops its execution.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="number">number of generations.</param>
			inline void GACALL SetNumberOfGeneration(int number) { _numberOfGenerations = number; }

		};// END CLASS DEFINITION GaGenerationCriteriaParams

		/// <summary><c>GaGenerationCriteria</c> is used to stop genetic algorithm when it reaches specified number of generations.
		/// This stop criteria uses <see cref="GaGenerationCriteriaParams" /> class as parameters.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaGenerationCriteria : public GaStopCriteria
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaStopCriteria::Evaluate" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual bool GACALL Evaluate(const GaAlgorithm& algorithm,
				const GaStopCriteriaParams& parameters) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method makes new instance of <see cref="GaGenerationCriteriaParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaGenerationCriteriaParams(); }

			/// <summary>Valid parameters must have number of generations grater then 0.
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const 
			{
				return ( (const GaGenerationCriteriaParams&) parameters ).GetNumberOfGeneration() > 0;
			}

		};// END CLASS DEFINITION GaGenerationCriteria

		/// <summary>This enumeration is used by stop criterias which are based on fitness values to specify type of comparison
		/// of current and desired values.</summary>
		enum GaFitnessCriteriaComparison
		{

			/// <summary>comparison should return <c>true</c> if current value is less then desired.</summary>
			GFC_LESS_THEN = 0x1, 

			/// <summary>comparison should return <c>true</c> if current value is greater then desired value.</summary>
			GFC_MORE_THEN = 0x2, 

			/// <summary>comparison should return <c>true</c> if current value and desired value are equal.</summary>
			GFC_EQUALS_TO = 0x4, 

			/// <summary>comparison should return <c>true</c> if current value is less then or equals to desired value.</summary>
			GFC_LESS_THEN_EQUALS_TO = GFC_LESS_THEN | GFC_EQUALS_TO, 

			/// <summary>comparison should return <c>true</c> if current value is greater then or equals to desired value.</summary>
			GFC_MORE_THEN_EQUALS_TO = GFC_MORE_THEN | GFC_EQUALS_TO

		};

		/// <summary><c>GaFitnessCriteriaParams</c> class is used by <see cref="GaFitnessCriteria" /> class as parameters for the criteria.
		/// It contains desired fitness value of specified type and it also specifies type of comparison which is used to compare desired and
		/// current fitness values.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		class GaFitnessCriteriaParams : public GaStopCriteriaParams
		{

		private:

			/// <summary>Stores desired fitness value for stopping the algorithm.</summary>
			float _fitnessLimit;

			/// <summary>Type of comparison of desired value and current value. </summary>
			GaFitnessCriteriaComparison _comparison;

			/// <summary>Type of fitness values which are compared.</summary>
			GaStatValueType _valueType;

		public:

			/// <summary>This constructor initializes parameters with user-defined values.</summary>
			/// <param name="fitnessLimit">desired fitness value.</param>
			/// <param name="comparison">type of comparison of desired and current fitness values.</param>
			/// <param name="valueType">type of fitness values which are compared.</param>
			GaFitnessCriteriaParams(float fitnessLimit,
				GaFitnessCriteriaComparison comparison,
				GaStatValueType valueType) : _fitnessLimit(fitnessLimit),
				_comparison(comparison),
				_valueType(valueType) { }

			/// <summary>This constructor initializes parameters with default values. Default desired fitness value is 1, type of comparison is
			/// <c>GFC_EQUALS_THEN</c> and type of fitness value is <c>GSV_BEST_FITNESS_SCALED</c>.</summary>
			GaFitnessCriteriaParams() : _fitnessLimit(1),
				_comparison(GFC_EQUALS_TO),
				_valueType(GSV_BEST_FITNESS_SCALED) { }

			/// <summary>More details are given in specification of <see cref="GaParameters::Clone" /> method.</summary>
			virtual GaParameters* GACALL Clone() const { return new GaFitnessCriteriaParams( *this ); }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns desired fitness value.</returns>
			inline float GACALL GetFitnessLimit() const { return _fitnessLimit; }

			/// <summary><c>SetFitnessLimit</c> method sets desired fitness value.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="limit">desired fitness value.</param>
			inline void GACALL SetFitnessLimit(float limit) { _fitnessLimit = limit; }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns type of comparison of desired value and current value.</returns>
			inline GaFitnessCriteriaComparison GACALL GetComparison() const { return _comparison; }

			/// <summary><c>SetComparation</c> method sets type of comparison of desired value and current value.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="comparison">type of comparison.</param>
			inline void GACALL SetComparison(GaFitnessCriteriaComparison comparison) { _comparison = comparison; }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns type of fitness values which are compared.</returns>
			inline GaStatValueType GACALL GetValueType() const { return _valueType; }

			/// <summary><c>SetValueType</c> method sets type of fitness values which are compared.
			/// 
			/// This method is not thread-safe.</summary>
			/// <param name="type">– type of fitness values.</param>
			inline void GACALL SetValueType(GaStatValueType type) { _valueType = type; }

		};// END CLASS DEFINITION GaFitnessCriteriaParams

		/// <summary><c>GaFitnessCriteria</c> is used to stop genetic algorithm when specified type of fitness value reaches desired value.
		/// This stop criteria uses <see cref="GaFitnessCriteriaParams" /> class as parameters.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaFitnessCriteria : public Algorithm::GaStopCriteria
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaStopCriteria::Evaluate" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL Evaluate(const GaAlgorithm& algorithm,
				const GaStopCriteriaParams& parameters) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaFitnessCriteriaParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaFitnessCriteriaParams(); }

			/// <summary>More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method always returns <c>true</c>.</returns>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return true; }

		};// END CLASS DEFINITION GaFitnessCriteria

		class GaFitnessProgressCriteria;

		/// <summary><c>GaFitnessProgressCriteriaParams</c> class is used by <see cref="GaFitnessProgressCriteria" /> class as parameters for the criteria.
		/// It contains desired progress of fitness value of specified type; it also specifies type of comparison which is used to compare desired and
		/// current progresses and number of generation which should continuously fail to meet required progress before algorithm stops.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		class GaFitnessProgressCriteriaParams : public GaStopCriteriaParams
		{
			friend class GaFitnessProgressCriteria;

		private:

			/// <summary>Required progress of fitness value.</summary>
			float _requiredProgress;

			/// <summary>If this attribute is <c>true</c>, absolute progress of fitness value is measured, otherwise relative progress is measured.</summary>
			bool _absolute;

			/// <summary>Type of comparison of desired progress and current progress.</summary>
			GaFitnessCriteriaComparison _comparison;

			/// <summary>Type of fitness values which progress is measured.</summary>
			GaStatValueType _valueType;

			/// <summary>Number of continuously generations which should fail to meet required progress of fitness value
			/// before algorithm stops its execution.</summary>
			int _historyDepth;

			/// <summary>Current number of continuously generations which failed to meet required progress of fitness value.</summary>
			mutable int _current;

		public:

			// Initialization of the parameters
			/// <summary>This constructor initializes parameters with user-defined values.</summary>
			/// <param name="progress">required progress of fitness value.</param>
			/// <param name="absolute">if this parameters is <c>true</c>, absolute progress of fitness value is measured,
			/// otherwise relative progress is measured.</param>
			/// <param name="comparison">type of comparison of desired progress and current progress.</param>
			/// <param name="valueType">type of fitness values which progress is measured.</param>
			/// <param name="historyDepth">number of continuously generations which should fail to meet required progress of fitness value
			/// before algorithm stops its execution.</param>
			GaFitnessProgressCriteriaParams(float progress,
				bool absolute,
				GaFitnessCriteriaComparison comparison,
				GaStatValueType valueType,
				int historyDepth) : _requiredProgress(progress),
				_absolute(absolute),
				_comparison(comparison),
				_valueType(valueType),
				_historyDepth(historyDepth),
				_current(0) { }

			/// <summary>This constructor initializes parameters with default values. Required progress is 0 and absolute progress is measured,
			/// comparison type is <c>GFC_EQUALS_TO</c>, fitness type is <c>GSV_BEST_FITNESS_SCALED</c> and history depth is 10.</summary>
			GaFitnessProgressCriteriaParams() : _requiredProgress(0),
				_absolute(true),
				_comparison(GFC_EQUALS_TO),
				_valueType(GSV_BEST_FITNESS_SCALED),
				_historyDepth(10),
				_current(0) { }

			/// <summary>More details are given in specification of <see cref="GaParameters::Clone" /> method.</summary>
			virtual GaParameters* GACALL Clone() const { return new GaFitnessProgressCriteriaParams( *this ); }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns required progress of fitness value.</returns>
			inline float GACALL GetRequiredProgress() const { return _requiredProgress; }

			/// <summary><c>SetRequiredProgress</c> method sets required progress of fitness value. 
			///
			/// This method is not thread-safe.</summary>
			/// <param name="progress">required progress.</param>
			inline void GACALL SetRequiredProgress(float progress) { _requiredProgress = progress; }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns <c>true</c> if absolute progress of fitness value is measured. If relative progress is measured,
			/// method returns <c>false</c>.</returns>
			inline bool GACALL GetAbsolute() const { return _absolute; }

			/// <summary><c>SetAbsolute</c> method sets whether the absolute or relative progress of fitness value is going to be measured.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="absolute">if this parameters is <c>true</c>, absolute progress of fitness value is measured,
			/// otherwise relative progress is measured.</param>
			inline void GACALL SetAbsolute(bool absolute) { _absolute = absolute; }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns type of comparison of desired progress and current progress</returns>
			inline GaFitnessCriteriaComparison GACALL GetComparison() const { return _comparison; }

			/// <summary><c>SetComparation</c> method sets type of comparison of desired progress and current progress.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="comparison">type of comparison.</param>
			inline void GACALL SetComparison(GaFitnessCriteriaComparison comparison) { _comparison = comparison; }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns type of fitness values which progress is measured.</returns>
			inline GaStatValueType GACALL GetValueType() const { return _valueType; }

			/// <summary><c>SetValueType</c> method sets type of fitness values which progress is measured. 
			///
			/// This method is not thread-safe.</summary>
			/// <param name="type">type of fitness value.</param>
			inline void GACALL SetValueType(GaStatValueType type) { _valueType = type; }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns number of continuously generations which should fail to meet required progress of fitness value
			/// before algorithm stops its execution.</returns>
			inline int GACALL GetHistoryDepth() const { return _historyDepth; }

			/// <summary><c>SetHistoryDepth</c> method sets number of continuously generations which should fail to meet
			/// required progress of fitness value before algorithm stops its execution. 
			///
			/// This method is not thread-safe.</summary>
			/// <param name="depth">number of generations.</param>
			inline void GACALL SetHistoryDepth(int depth) { _historyDepth = depth; }

		private:

			// Returns number of previous generation successfully complied the stop criteria
			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns current number of continuously generations which failed to meet required progress of fitness value.</returns>
			inline int GACALL GetCurrent() const { return _current; }

			// Sets number of previous generation successfully complied the stop criteria
			/// <summary><c>SetCurrent</c> method sets current number of continuously generations which failed to meet required progress of fitness value. 
			///
			/// This method is not thread-safe.</summary>
			/// <param name="current">current number of generations.</param>
			inline void GACALL SetCurrent(int current) const { _current = current; }

		};// END CLASS DEFINITION GaFitnessProgressCriteriaParams

		/// <summary><c>GaFitnessProgressCriteria</c> is used to stop genetic algorithm when specified number of generations fail
		/// to meet required progress of defined type of fitness value. This stop criteria uses <see cref="GaFitnessProgressCriteriaParams" />
		/// class as parameters.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaFitnessProgressCriteria : public GaStopCriteria
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaStopCriteria::Evaluate" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual bool GACALL Evaluate(const GaAlgorithm& algorithm,
				const GaStopCriteriaParams& parameters) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaFitnessProgressCriteriaParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaFitnessProgressCriteriaParams(); }

			/// <summary>Valid parameters must have history depth larger then 0, and value of required progress mustn't be 0.
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const
			{
				const GaFitnessProgressCriteriaParams& p = (const GaFitnessProgressCriteriaParams&)parameters;
				return p.GetHistoryDepth() > 0 && fabs( p.GetRequiredProgress() ) > 0;
			}

		};// END CLASS DEFINITION GaFitnessCriteriaProgress

	} // StopCriterias
} // Algorithm

#endif // __GA_STOP_CRITERIAS_H__
