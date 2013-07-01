
/*! \file SelectionOperations.h
    \brief This file contains declaration of classes that implement selection operations and their parameters.
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

#ifndef __GA_SELECTION_OPERATIONS_H__
#define __GA_SELECTION_OPERATIONS_H__

#include "PopulationOperations.h"

using namespace Population;

namespace Population
{
	/// <summary>Contains implementation of some basic selection operations.</summary>
	namespace SelectionOperations
	{

		/// <summary><c>GaSelectBest</c> selection is selects defined number of best chromosomes. If population is not-sorted, this selection can only
		/// select chromosomes which are in best sorted group. This selection use <see cref="GaSelectionParams" /> class for parameters.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaSelectBest : public GaSelectionOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaSelectionOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(const GaPopulation& population,
				const GaSelectionParams& parameters,
				GaSelectionResultSet& result) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaSelectionParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaSelectionParams(); }

			/// <summary>Valid parameters must have selection size grater then 0.
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return ( (const GaSelectionParams&) parameters ).GetSelectionSize() > 0; }

		};// END CLASS DEFINITION GaSelectBest

		/// <summary>This class should be used (directly or by inheritance) by selection operations which can control duplicates of chromosome in result set.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		class GaSelectDuplicatesParams : public GaSelectionParams
		{

		private:

			/// <summary>Indicates if duplicates of same chromosome are allowed in selection result set.</summary>
			bool _duplicates;

		public:

			/// <summary>This constructor initializes parameters with user defined values.</summary>
			/// <param name="duplicates">if this parameter is set to <c>true</c>, duplicates of same chromosome in result set are allowed.
			/// If it is set to <c>false</c> duplicates are not allowed.</param>
			/// <param name="selectionSize">number of chromosomes which should be selected.</param>
			GaSelectDuplicatesParams(bool duplicates,
				int selectionSize) : GaSelectionParams(selectionSize),
				_duplicates(duplicates) { }

			/// <summary>This constructor initializes parameters with default values. Duplicates are not allowed, and selection size is 2.</summary>
			GaSelectDuplicatesParams() : _duplicates(false) { }

			/// <summary>More details are given in specification of <see cref="GaParameters::Clone" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual GaParameters* GACALL Clone() const { return new GaSelectDuplicatesParams( *this ); }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns <c>true</c> if duplicates of same chromosome in result set are allowed, otherwise it returns <c>false</c>.</returns>
			inline bool GACALL GetDuplicates() const { return _duplicates; }

			// Set to TRUE if dupilcates in result set are allowed
			/// <summary><c>SetDuplicates</c> method sets acceptance of duplicates of chromosomes in selection result set.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="duplicates">if this parameter is set to <c>true</c>, duplicates of same chromosome in result set are allowed.
			/// If it is set to <c>false</c> duplicates are not allowed.</param>
			inline void GACALL SetDuplicates(bool duplicates) { _duplicates = duplicates; }

		};// END CLASS DEFINITION GaSelectDuplicatesParams

		/// <summary><c>GaSelectRandom</c> selection is randomly selects defined number of chromosomes. This selection use
		/// <see cref="GaSelectDuplicatesParams" /> class for parameters. 
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaSelectRandom : public GaSelectionOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaSelectionOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(const GaPopulation& population,
				const GaSelectionParams& parameters,
				GaSelectionResultSet& result) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaSelectDuplicatesParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaSelectDuplicatesParams(); }

			/// <summary>Valid parameters must have selection size grater then 0.
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return ( (const GaSelectionParams&) parameters ).GetSelectionSize() > 0; }

		protected:

			/// <summary><c>Select</c> method randomly selects only one chromosome.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="results">reference to selection result set which is used to store selected chromosome.</param>
			/// <param name="population">reference to population on which selection is performed.</param>
			/// <param name="parameters">reference to parameters of population.</param>
			/// <returns>Method returns index of selected chromosome.</returns>
			GAL_API
			virtual void GACALL Select(GaSelectionResultSet& results,
				const GaPopulation& population,
				const GaSelectionParams& parameters) const;

		};// END CLASS DEFINITION GaSelectRandom

		/// <summary>This class is used by <see cref="GaSelectRandomBest" /> selection operations.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		class GaSelectRandomBestParams : public GaSelectDuplicatesParams
		{

		private:

			/// <summary>Number of chromosomes which should be selected using random selection.</summary>
			int _groupSize;

		public:

			/// <summary>This constructor initializes parameters with user defined values.</summary>
			/// <param name="selectionSize">number of chromosomes which should be selected.</param>
			/// <param name="duplicates">if this parameter is set to <c>true</c>, duplicates of same chromosome in result set are allowed.
			/// If it is set to <c>false</c> duplicates are not allowed.</param>
			/// <param name="groupSize">number of chromosomes which should be selected randomly.</param>
			GaSelectRandomBestParams(int selectionSize,
				bool duplicates,
				int groupSize) : GaSelectDuplicatesParams(duplicates, selectionSize),
				_groupSize( groupSize ) { }

			/// <summary>This constructor initializes parameters with default values.
			/// Duplicates are not allowed, selection size is 2, and group size is 4.</summary>
			GaSelectRandomBestParams() : _groupSize(4) { }

			/// <summary>More details are given in specification of <see cref="GaParameters::Clone" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual GaParameters* GACALL Clone() const { return new GaSelectRandomBestParams( *this ); }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns number of chromosomes which should be selected using random selection.</returns>
			inline int GACALL GetGroupSize() const { return _groupSize; }

			/// <summary><c>SetGroupSize</c> sets number of chromosomes which should be selected using random selection.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="size">number of chromosomes which should be selected using random selection.</param>
			inline void GACALL SetGroupSize(int size) { _groupSize = size; }

		};// END CLASS DEFINITION GaSelectRandomBestParams

		/// <summary><c>GaSelectRandomBest</c> selection is randomly selects number of chromosomes and then it cuts off chromosomes with worst fitness values to
		/// fit in selection size. This selection use <see cref="GaSelectRandomBestParams" /> class for parameters. 
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaSelectRandomBest : public GaSelectRandom
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaSelectionOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(const GaPopulation& population,
				const GaSelectionParams& parameters,
				GaSelectionResultSet& result) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaSelectRandomBestParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaSelectRandomBestParams(); }

			/// <summary>Valid parameters must have selection size grater then 0 and group size greater then or equal to selection size.
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const
			{
				const GaSelectRandomBestParams& p = (const GaSelectRandomBestParams&)parameters;
				return p.GetSelectionSize() > 0 && p.GetGroupSize() >= p.GetSelectionSize();
			}

		};// END CLASS DEFINITION GaSelectRandomBest

		/// <summary>When <c>GaSelectRouletteWheel</c> selection is used chromosomes with better fitness values have better chances to be selected.
		/// It virtually transforms fitness value of chromosome to probability of selection. Population on which selection performed must be sorted.
		/// This selection can work with minimization or maximization of fitness values, as well as negative fitness values. This selection use
		/// <see cref="GaSelectDuplicatesParams" /> class for parameters. 
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaSelectRouletteWheel : public GaSelectionOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaSelectionOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(const GaPopulation& population,
				const GaSelectionParams& parameters,
				GaSelectionResultSet& result) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaSelectDuplicatesParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaSelectDuplicatesParams(); }

			/// <summary>Valid parameters must have selection size grater then 0.
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return ( (const GaSelectionParams&) parameters ).GetSelectionSize() > 0; }

		protected:

			/// <summary><c>Select</c> method selects only one chromosome using roulette wheel selection algorithm.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="result">reference to selection result set which is used to store selected chromosome.</param>
			/// <param name="population">reference to population on which selection is performed.</param>
			/// <param name="parameters">reference to parameters of population.</param>
			/// <returns>Method returns index of selected chromosome.</returns>
			GAL_API
			virtual int GACALL Select(const GaSelectionResultSet& result,
				const GaPopulation& population,
				const GaSelectionParams& parameters) const;

		};// END CLASS DEFINITION GaSelectRouletteWheel

		/// <summary>This class is used by <c>GaSelectTournament</c> selection operations.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		class GaSelectTournamentParams : public GaSelectDuplicatesParams
		{

		private:

			/// <summary>Number of roulette wheel selections which are going to be executed before selection chooses one chromosome.</summary>
			int _numberOfSelections;

		public:

			/// <summary>This constructor initializes parameters with user defined values.</summary>
			/// <param name="selectionSize">number of chromosomes which should be selected.</param>
			/// <param name="duplicates">if this parameter is set to <c>true</c>, duplicates of same chromosome in result set are allowed.
			/// If it is set to <c>false</c> duplicates are not allowed.</param>
			/// <param name="numberOfSelections">number of roulette wheel selections which are going to be executed before
			/// selection chooses one chromosome.</param>
			GaSelectTournamentParams(int selectionSize,
				bool duplicates,
				int numberOfSelections) : GaSelectDuplicatesParams(duplicates, selectionSize),
				_numberOfSelections(numberOfSelections) { }

			/// <summary>This constructor initializes parameters with default values. Duplicates are not allowed, and selection size is 2 and
			/// number of roulette wheel selections is 1.</summary>
			GaSelectTournamentParams() : _numberOfSelections(1) { }

			/// <summary>More details are given in specification of <see cref="GaParameters::Clone" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual GaParameters* GACALL Clone() const { return new GaSelectTournamentParams( *this ); }

			// Returns how many chromosomes are selected using roulette wheel selection
			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns number of roulette wheel selections which are going to be executed before selection chooses one chromosome.</returns>
			inline int GACALL GetNumberOfSelections() const { return _numberOfSelections; }

			// Sets how many chromosomes are selected using roulette wheel selection
			/// <summary><c>SetNumberOfSelection</c> method sets number of roulette wheel selections which are going to be executed before
			/// selection chooses one chromosome.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="number">number of roulette wheel selections.</param>
			inline void GACALL SetNumberOfSelections(int number) { _numberOfSelections = number; }

		};// END CLASS DEFINITION GaSelectTournamentParams

		/// <summary><c>GaSelectTorunament</c> selection is similar to <see cref="GaSelectRouletteWheel" /> selection. For one place in result set
		/// it performs multiple roulette wheel selection before it chooses only one chromosome with best fitness value among selected. This increases
		/// probability of selection of chromosome with better fitness value. This selection use <see cref="GaSelectTorunamentParams" /> class for parameters.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaSelectTournament : public GaSelectRouletteWheel
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaSelectionOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(const GaPopulation& population,
				const GaSelectionParams& parameters,
				GaSelectionResultSet& result) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaSelectTournamentParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaSelectTournamentParams(); }

			/// <summary>Valid parameters must have selection size and number of selections grater then 0.
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const
			{
				const GaSelectTournamentParams& p = (const GaSelectTournamentParams&)parameters;
				return p.GetSelectionSize() > 0 && p.GetNumberOfSelections() > 0;
			}

		};// END CLASS DEFINITION GaSelectTournament

		/// <summary><c>GaSelectWorst</c> selection is selects defined number of worst chromosomes. If population is not-sorted, this selection can only
		/// select chromosomes which are in worst sorted group. This selection use <see cref="GaSelectionParams" /> class for parameters.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaSelectWorst : public GaSelectionOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaSelectionOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(const GaPopulation& population,
				const GaSelectionParams& parameters,
				GaSelectionResultSet& result) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaSelectionParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaSelectionParams(); }

			/// <summary>Valid parameters must have selection size grater then 0.
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return ( (const GaSelectionParams&) parameters ).GetSelectionSize() > 0; }

		};// END CLASS DEFINITION GaSelectBest

	} // SelectionOperations
} // Population

#endif // __GA_SELECTION_OPERATIONS_H__
