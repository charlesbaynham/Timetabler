
/*! \file Statistics.h
    \brief This file contains declaration of classes and datatypes needed for tracking statistical information about populations and genetic algorithms.
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

#ifndef __GA_STATISTICS_H__
#define __GA_STATISTICS_H__

#include "Platform.h"
#include "ChromosomeOperations.h"

namespace Common
{
	/// <summary><c>GaStatValueType</c> enumeration defines all types of statistical values which are managed my <see cref="GaStatistics" /> class.</summary>
	enum GaStatValueType
	{
		/// <summary>fitness value of the best chromosome in population(s).</summary>
		GSV_BEST_FITNESS, 
		/// <summary>scaled (transformed) fitness value of the best chromosome in population.</summary>
		GSV_BEST_FITNESS_SCALED, 
		/// <summary>average fitness value of all chromosomes in population(s).</summary>
		GSV_AVG_FITNESS, 
		/// <summary>average scaled (transformed) fitness value of all chromosomes in population.</summary>
		GSV_AVG_FITNESS_SCALED, 
		/// <summary>fitness value of the worst chromosome in population(s).</summary>
		GSV_WORST_FITNESS, 
		/// <summary>scaled (transformed) fitness value of the worst chromosome in population.</summary>
		GSV_WORST_FITNESS_SCALED, 
		/// <summary>sum of fitness values of all chromosomes in population(s).</summary>
		GSV_TOTAL_FITNESS, 
		/// <summary>sum of scaled (transformed) of all chromosomes in population.</summary>
		GSV_TOTAL_FITNESS_SCALED,
		/// <summary>the number of chromosomes in population.</summary>
		GSV_POPULATION_SIZE
	};

	/// <summary>This global constant contains number of statistical values in <see cref="GaStatistic" /> class 
	/// and in <see cref="GaStatValueType" /> enumeration.</summary>
	const int GaNumberOfStatValueTypes = GSV_POPULATION_SIZE - GSV_BEST_FITNESS + 1;

	// Statistical value
	/// <summary><c>GaStatValue</c> template class stores single statistical value and keeps track of changes of that value between generations.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	/// <param name="T">type of statistical value.</param>
	template <typename T>
	class GaStatValue
	{

	private:

		// Current value
		/// <summary>Stores statistical value of current generation.</summary>
		T _current;

		// Previous value
		/// <summary>Stores statistical value from previous generation.</summary>
		T _previous;

	public:

		// Initialization of the values
		/// <summary>Initialize statistical values with user defined values for current and previous generations.</summary>
		/// <param name="current">value for the current generation.</param>
		/// <param name="previous">value for the previous generation.</param>
		GaStatValue(T current,
			T previous) : _current(current),
			_previous(previous) { }

		/// <summary>Initializes statistical values with zero values.</summary>
		GaStatValue() : _current(0),
			_previous(0) { }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Returns statistical value for the current generation.</returns>
		inline T GACALL GetCurrent() const { return _current; }

		/// <summary><c>SetCurrent</c> method sets value for the current generation.
		///
		/// This method is not thread safe.</summary>
		/// <param name="value">new value for the current generation.</param>
		inline void GACALL SetCurrent(T value) { _current = value; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Returns value for the previous generation.</returns>
		inline T GACALL GetPrevious() const { return _previous; }

		/// <summary><c>SetPrevious</c> method sets value for the previous generation.
		///
		/// This method is not thread safe.</summary>
		/// <param name="value">new value for the previous generation.</param>
		inline void GACALL SetPrevious(T value) { _previous = value; }

		/// <summary><c>Changed</c> method checks for changes of value between generations.
		///
		/// This method is not thread-safe.</summary>
		/// <returns>Method returns <c>true</c> if value from previous generation has changed, otherwise it returns <c>false</c>.</returns>
		inline bool GACALL Changed() const { return _previous != _current; }

		/// <summary><c>operator +</c> makes new object by summing corresponding values of <c>this</c> object and <c>rhs</c> object.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="rhs">reference to the second object which is used for summing, the first object is <c>this</c> object.</param>
		/// <returns>Operator returns new object with summed values.</returns>
		inline GaStatValue<T> GACALL operator +(const GaStatValue<T>& rhs)
		{
			GaStatValue<T> r = *this;
			r._current += rhs._current;
			r._previous += rhs._previous;

			return r;
		}

		/// <summary><c>operator +=</c> adds corresponding values of <c>rhs</c> object and <c>this</c> object.
		///
		/// This operator is not thread-safe.</summary>
		/// <param name="rhs">reference to the second object which is used for summing, the first object is <c>this</c> object.</param>
		/// <returns>Operator returns reference to <c>this</c> object.</returns>
		inline GaStatValue<T>& GACALL operator +=(const GaStatValue<T>& rhs)
		{
			_current += rhs._current;
			_previous += rhs._previous;

			return *this;
		}

	};// END CLASS DEFINITION GaStatValue

	/// <summary><c>GaFloatStatValue</c> type is instance of <see cref="GaStatValue" /> template class
	/// for single precision floating-point statistical values.</summary>
	typedef GaStatValue<float> GaFloatStatValue;

	/// <summary><c>GaStatistics</c> class keeps track of statistical information about populations, groups of populations or algorithms.
	/// It contains information about previous and current statistics, and provides information about progress of statistical values.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaStatistics
	{

	private:

		/// <summary>Generation counter keeps track of current generation of a population or an algorithm.</summary>
		int _currentGeneration;

		/// <summary>Array of statistical information. All statistical values are single precision floating-point numbers.</summary>
		GaFloatStatValue _values[ GaNumberOfStatValueTypes ];

		/// <summary>Fitness comparator used to compare statistical values.</summary>
		const GaFitnessComparator* _fitnessComparator;

	public:

		// Initialize statistical information
		/// <summary>Default constructor initializes all statistical values to zeros and restart generation counter.</summary>
		/// <param name="comarator">fitness comparator used to compare statistical values.</param>
		GaStatistics(const GaFitnessComparator* comarator) : _currentGeneration(0),
			_fitnessComparator(comarator) { }

		// Clears all values
		/// <summary><c>Clear</c> method clears all statistical information and restarts generation counter.
		///
		///This method is not thread-safe.</summary>
		GAL_API
		void GACALL Clear();

		/// <summary><c>NextGeneration</c> method prepares statistical values for next generation (shifts current values to previous)
		/// and increments generation counter.
		///
		/// This method is not thread-safe</summary>
		GAL_API
		void GACALL NextGeneration();

		/// <summary><c>CopyFrom</c> method copies statistical information and/or generation counter from another object.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="stats">object form which information is going to be copied.</param>
		/// <param name="previous">if it is set to <c>true</c>, then statistical information form previous generation is copied.</param>
		/// <param name="current">if it is set to <c>true</c>, then statistical information from current generation is copied.</param>
		/// <param name="currentGeneration">if it is set to <c>true</c>, then generation counter is copied.</param>
		GAL_API
		void GACALL CopyFrom(const GaStatistics& stats,
			bool previous,
			bool current,
			bool currentGeneration);

		// Returns current generation
		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Returns value of generation counter.</returns>
		inline int GACALL GetCurrentGeneration() const { return _currentGeneration; }

		// Get progress of a value between this and previous generation
		/// <summary><c>GetValueProgress</c> method calculates progress of statistical value between previous and current generation.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="value">statistical value which progress is queried.</param>
		/// <param name="percent">if it is set to <c>true</c>, the method returns progress of the value in percents,
		/// otherwise it returns absolute progress.</param>
		/// <returns>Returns progress of statistical value between previous and current generation. 
		/// If percent parameter is <c>true</c>, the method return progress in percents, otherwise it returns absolute progress.</returns>
		GAL_API
		float GACALL GetValueProgress(GaStatValueType value,
			bool percent) const;

		/// <summary>The method changes statistical value of current generation, change can be absolute or a relative offset to the current value.
		/// If <c>GSV_TOTAL_FITNESS</c>, <c>GSV_TOTAL_FITNESS_SCALED</c> or <c>GSV_POPULATION_SIZE</c> values are changed, <c>GSV_AVG_FITNESS</c>
		/// and <c>GSV_AVG_FITNESS_SCALED</c> values are updated automatically. 
		///
		/// This method is not thread-safe.</summary>
		/// <param name="type">statistical value which is going to be changed.</param>
		/// <param name="value">new value or relative offset to the current value.</param>
		/// <param name="relative">if is set to <c>true</c>, value parameter is interpreted as relative offset to the current value, 
		/// if it is set to <c>false</c>, value parameter is interpreted as new value.</param>
		GAL_API
		void GACALL ChangeValue(GaStatValueType type,
			float value,
			bool relative);

		/// <summary>This method is not thread-safe.</summary>
		/// <param name="value">statistical value which is queried.</param>
		/// <returns>Returns reference to queried statistical value.</returns>
		inline const GaFloatStatValue& GACALL GetValue(GaStatValueType value) const { return _values[ value ]; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Returns pointer to fitness comparator used to compare statistical values.</returns>
		inline const GaFitnessComparator* GACALL GetFitnessComarator() const { return _fitnessComparator; }

		// Sets fitness comparator used to compare statistical values
		/// <summary><c>SetFitnessComparator</c> method sets fitness comparator which is going to be used to compare statistical values.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="comparator">fitness comparator.</param>
		inline void GACALL SetFitnessComparator(const GaFitnessComparator* comparator) { _fitnessComparator = comparator; }

		// Combines information
		/// <summary>This operator produce new object by combining two instances of <c>GaStatistic</c> class.
		/// For detailed description of combining statistical values see <see cref="operator +=" /> operator.
		///
		/// This operator is not thread-safe.</summary>
		/// <param name="rhs">reference to the second instance of <c>GaStatistic</c>, the first instance is <c>this</c> object.</param>
		/// <returns>Operator returns new instance of GaStatistic class by value.</returns>
		inline GaStatistics GACALL operator +(const GaStatistics& rhs)
		{
			GaStatistics r = *this;
			r += rhs;
			return r;
		}

		/// <summary>This operator combines statistical values of two instances of <c>GaStatistics</c> class. Result of combining is stored in this object.
		/// Combining is done in fallowing way:
		///	<br> 1. Better value between two <c>GSV_BEST_FITNESS</c> values is chosen for new <c>GSV_BEST_FITNESS</c> value.
		///	<br> 2. Worse value between two <c>GSV_WORST_FITNESS</c> values is chosen for new <c>GSV_WORST_FITNESS</c>.
		///	<br> 3. Two <c>GSV_TOTAL_FITNESS</c> values are summed and the sum becomes new <c>GSV_TOTAL_FITNESS</c> value.
		///	<br> 4. Two <c>GSV_POPULATION_SIZE</c> values are summed and the sum becomes new <c>GSV_POPULATION_SIZE</c>.
		///	<br> 5. <c>GSV_AVG_FITNES</c> is calculated as: <c>GSV_TOTAL_FITNESS/GSV_POPULATION_SIZE</c>,
		///			where <c>GSV_TOTAL_FITNESS</c> and <c>GSV_POPULATION_SIZE</c> have new values.
		///	<br> 6. All scaled fitness values are restarted to zeros. These values cannot be combined because they are calculated differently
		///		   for different populations. 
		///
		/// This operation is not thread-safe.</summary>
		/// <param name="rhs">reference to the instance of class <c>GaStatistics</c> which is used for combining result statistical values of this object.</param>
		/// <returns>The operator returns reference to <c>this</c> object.</returns>
		GAL_API
		GaStatistics& GACALL operator +=(const GaStatistics& rhs);

	};// END CLASS DEFINITION GaStatistics

} // Common

#endif //__GA_STATISTICS_H__
