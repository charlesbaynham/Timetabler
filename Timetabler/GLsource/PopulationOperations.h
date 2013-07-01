
/*! \file PopulationOperations.h
    \brief This file declares interfaces and classes needed to implement genetic operations which are performed over population.
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

#ifndef __GA_POPULATION_OPERATIONS_H__
#define __GA_POPULATION_OPERATIONS_H__

#include "Platform.h"
#include "Operation.h"
#include "Catalogue.h"
#include "Chromosome.h"
#include "SortedGroup.h"

using namespace Common;
using namespace Chromosome;
using namespace Population; 

namespace Population
{

	/// <summary>This class is base for parameters of selection operation.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaSelectionParams : public GaParameters
	{

	protected:

		/// <summary>Number of chromosomes which should be selected.</summary>
		int _selectionSize;

	public:

		/// <summary>This constructor initializes selection parameters with user-defined replacement size.</summary>
		/// <param name="selectionSize">number of chromosomes which should be selected.</param>
		GaSelectionParams(int selectionSize) { SetSelectionSize( selectionSize ); }

		/// <summary>This constructor initializes selection parameters with default values. Default selection size is 2.</summary>
		GaSelectionParams() : _selectionSize(0) {  }

		/// <summary>More details are given in specification of <see cref="GaParameters::Clone" /> method.</summary>
		virtual GaParameters* GACALL Clone() const { return new GaSelectionParams( *this ); }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns number of chromosomes which should be selected.</returns>
		inline int GACALL GetSelectionSize() const { return _selectionSize; }

		/// <summary><c>SetSelectionSize</c> method sets number of chromosomes which should be selected.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="size">new number of chromosomes for selection.</param>
		inline void GACALL SetSelectionSize(int size) { _selectionSize = size < 0 ? 0 : size; }

	};// END CLASS DEFINITION GaSelectionParams

	/// <summary>This class is used as storage for selection operation (result set). It uses sorted group to store indices of selected chromosomes. 
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaSelectionResultSet
	{

	protected:

		/// <summary>Pointer to sorted group which contains indices of selected chromosomes.</summary>
		GaSortedGroup* _selectedGroup;

	public:

		/// <summary>This constructor initializes sorted group of result set which stores selected chromosomes.</summary>
		/// <param name="selectionSize">number of chromosomes which can be stored by sorted group.</param>
		/// <param name="population">pointer to population on which selection is performed.</param>
		GAL_API
		GaSelectionResultSet(int selectionSize,
			const GaPopulation* population);

		/// <summary>Destructor frees memory used by sorted group which stores selected chromosomes.</summary>
		~GaSelectionResultSet() { delete _selectedGroup; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference to sorted group which stores selected chromosomes.</returns>
		inline GaSortedGroup& GACALL SelectedGroup() const { return *_selectedGroup; }

		/// <summary><c>GetAt</c> method returns smart pointer to chromosomes at given position in result set.
		///
		/// This method is not thread safe.</summary>
		/// <param name="pos">position of chromosomes in result set.</param>
		/// <returns>Method returns smart pointer to chromosome at given position in set.</returns>
		inline GaChromosomePtr GACALL GetAt(int pos) const { return _selectedGroup->GetChromosomeAt( pos ); }

		/// <summary><c>operator []</c> returns smart pointer to chromosomes at given position in result set.
		///
		/// This operator is not thread safe.</summary>
		/// <param name="pos">position of chromosomes in result set.</param>
		/// <returns>Operator returns smart pointer to chromosome at given position in set.</returns>
		inline GaChromosomePtr GACALL operator [](int pos) const { return _selectedGroup->GetChromosomeAt( pos ); }

	};// END CLASS DEFINITION GaSelectionResultSet

	/// <summary>This class is interface for selection operation which selects chromosomes from population which
	/// are going to be used as parents in production of new chromosomes.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaSelectionOperation : public GaOperation
	{

	public:

		/// <summary><c>operator ()</c> performs replacement operation.</summary>
		/// <param name="population">reference to population from which chromosomes are selected.</param>
		/// <param name="parameters">reference to parameters of selection operation.</param>
		/// <param name="result">pointer to result set which is used to store selected chromosomes.</param>
		virtual void GACALL operator ()(const GaPopulation& population,
			const GaSelectionParams& parameters,
			GaSelectionResultSet& result) const=0;

		/// <summary><c>MakeResultSet</c> method makes result set used by selection operation for storing chromosomes indices.
		/// Caller is responsible for memory allocated by this method for result set object.
		///
		/// This method is thread-safe.</summary>
		/// <param name="population">pointer to population to which result set is bound.</param>
		/// <param name="parameters">reference to parameters of selection operation.</param>
		/// <returns>Method returns pointer to newly created result set.</returns>
		virtual GaSelectionResultSet* GACALL MakeResultSet(GaPopulation* population,
			const GaSelectionParams& parameters) const { return new GaSelectionResultSet( parameters.GetSelectionSize(), population ); }

	};// END CLASS DEFINITION GaSelectionOperation

	/// <summary><c>GaSelectionPair</c> type is instance of <see cref="GaOperationParametersPair" /> template class and represents pair of
	/// selection operation and its parameters. Detailed description can be found in specification of
	/// <see cref="GaOperationParametersPair" /> template class.</summary>
	typedef GaOperationParametersPair<GaSelectionOperation, GaSelectionParams> GaSelectionPair;

	/// <summary><c>GaSelectionCatalogue</c> type is instance of <see cref="GaCatalogue" /> template class and represents catalogue of selection operations.
	/// Detailed description can be found in specification of <see cref="GaCatalogue" /> template class.</summary>
	typedef GaCatalogue<GaSelectionOperation> GaSelectionCatalogue;

	/// <summary>This class is base for parameters of coupling operation.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaCouplingParams : public GaParameters
	{

	private:

		/// <summary>Number of chromosomes which should be produces.</summary>
		int _numberOfOffsprings;

		/// <summary>Indicates that the coupling operation should check whether the produced offspring already exist in population before it is inserted in couling result set.</summary>
		bool _checkForDuplicates;

	public:

		/// <summary>This constructor initializes coupling parameters with user-defined number of produced offspring size.</summary>
		/// <param name="numberOfOffsprings">number of chromosomes which should be produced.</param>
		/// <param name="checkForDuplicates">this parameter should be set to <c>true</c> if operation should check whether the produced offspring already exist in population
		/// before it is inserted in couling result set.</param>
		GaCouplingParams(int numberOfOffsprings,
			bool checkForDuplicates) : _checkForDuplicates(checkForDuplicates) { SetNumberOfOffsprings( numberOfOffsprings ); }

		/// <summary>This constructor initializes coupling parameters with default values.
		/// Default number of produced offspring is 2 and operation does not check for duplicates.</summary>
		GaCouplingParams() : _numberOfOffsprings(2),
			_checkForDuplicates(false) { }

		/// <summary>More details are given in specification of <see cref="GaParameters::Clone" /> method.</summary>
		virtual GaParameters* GACALL Clone() const { return new GaCouplingParams( *this ); }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns number of chromosomes which should be produced.</returns>
		inline int GACALL GetNumberOfOffsprings() const { return _numberOfOffsprings; }

		/// <summary><c>SetNumberOfOffsprings</c> method sets number of chromosomes which should be produced.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="number">new number of chromosomes which should be produced.</param>
		inline void GACALL SetNumberOfOffsprings(int number) { _numberOfOffsprings = number < 0 ? 0 : number; }

		/// <summary><c>SetCheckForDuplicates</c> method sets whether the operation should check for duplicates when the offspring chromosomes are inserted in result set.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="check">this parameter should be set to <c>true</c> if operation should check whether the produced offspring already exist in population
		/// before it is inserted in couling result set.</param>
		inline void GACALL SetCheckForDuplicates(bool check) { _checkForDuplicates = check; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns <c>true</c> if operation should check whether the produced offspring already exist in population
		/// before it is inserted in couling result set.</returns>
		inline bool GACALL GetCheckForDuplicates() const { return _checkForDuplicates; }

	};// END CLASS DEFINITION GaCouplingParams

	/// <summary>This class is used as storage for coupling operation (result set). Result set contains pointer to selection result set which
	/// stores parent chromosomes. It also contains array of newly produced chromosomes and array of their parents' indices.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaCouplingResultSet
	{

	private:

		/// <summary>Pointer to array of smart pointers which are point to newly produced offspring.</summary>
		GaChromosomePtr* _offsprings;

		/// <summary>Array of parents' indices of newly created offspring.</summary>
		int* _parents;

		/// <summary>Number of chromosomes which can be stored in this result set.</summary>
		int _numberOfOffsprings;

		/// <summary>Indicates whehter the duplicated chromosomes should be removed from result set before they are inserted into population.</summary>
		bool _clearDuplicates;

		/// <summary>Pointer to result set of selection operation. It contains chromosomes which are used as parents to produce offspring.</summary>
		GaSelectionResultSet* _selection;

	public:

		/// <summary>This constructor initializes coupling result set with maximum number of offspring which can be stored and
		/// selection result set which stores parents.</summary>
		/// <param name="numberOfOffsprings">maximum number of offspring which this result set can store.</param>
		/// <param name="selectionResults">pointer to selection results which stores parents.</param>
		GAL_API
		GaCouplingResultSet(int numberOfOffsprings,
			GaSelectionResultSet* selectionResults);

		/// <summary>Destructor frees memory of array which stores parents' indices and array of smart pointers to offspring.</summary>
		GAL_API
		virtual ~GaCouplingResultSet();

		/// <summary><c>GetOffspringAt</c> method returns newly created chromosome and its parent's index at specified position in result set.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="pos">position of offspring chromosome.</param>
		/// <param name="offspring">reference to smart pointer to which is stored pointer to new chromosome.</param>
		/// <param name="parent">reference to variable to which is stored index of new chromosome's parent.</param>
		/// <returns>Method returns <c>true</c> if chromosome exists at the position.</returns>
		GAL_API
		bool GACALL GetOffspringAt(int pos,
			GaChromosomePtr& offspring,
			int& parent) const;

		/// <summary><c>SetOffspringAt</c> method sets smart pointer to new chromosome and index of its parent at specified position in result set.
		/// It can be used to insert new chromosomes in result set.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="pos">position in result set which is changed.</param>
		/// <param name="offspring">smart pointer to new chromosome which is inserted.</param>
		/// <param name="parent">index of parent's chromosome.</param>
		/// <returns>Method returns <c>true</c> if new chromosomes is inserted in result set.</returns>
		GAL_API
		bool GACALL SetOffspringAt(int pos,
			GaChromosomePtr offspring,
			int parent);

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns maximal number of offspring which this result can store.</returns>
		inline int GACALL GetNumberOfOffsprings() const { return _numberOfOffsprings; }

		/// <summary><c>SetNumberOOffspring</c> method changes size of arrays so they can store specified number of offspring.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="number">new number of chromosomes which can be stored in this result set.</param>
		GAL_API
		void GACALL SetNumberOfOffsprings(int number);

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns pointer to array of smart pointers to newly created chromosomes.</returns>
		inline GaChromosomePtr* GACALL GetOffspringsBuffer() const { return _offsprings; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns pointer to array of parent's indices.</returns>
		inline int* GACALL GetParentsBuffer() const { return _parents; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference to selection which stores parent chromosomes.</returns>
		inline GaSelectionResultSet& GACALL GetSelectionResultSet() const { return *_selection; }

		/// <summary><c>SetClearDuplicates</c> method setsh whether the duplicated chromosomes should be removed from result set before they are inserted into population.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="clear">this parameter should be set to <c>true</c> if the duplicated chromosomes should be removed from result set
		/// before they are inserted into population.</param>
		inline void GACALL SetClearDuplicates(bool clear) { _clearDuplicates = clear; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns <c>true</c> if the duplicated chromosomes should be removed from result set before they are inserted into population.</returns>
		inline bool GACALL GetClearDuplicates() const { return _clearDuplicates; }

	};// END CLASS DEFINITION GaCouplingResultSet

	/// <summary>This class is interface for coupling operations which produce offspring from selected chromosomes.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaCouplingOperation : public GaOperation
	{

	public:

		/// <summary><c>operator ()</c> performs coupling operation. Because this operation is parallelized, caller must specify ID its working thread,
		/// and total number of working threads which are used.</summary>
		/// <param name="population">reference to population which hosts selected chromosomes.</param>
		/// <param name="output">reference to coupling result set which will store produced offspring.</param>
		/// <param name="parameters">reference to parameters of replacement operation.</param>
		/// <param name="workerId">ID of the worker (working thread) which called this operator.</param>
		/// <param name="numberOfWorkers">number of workers (working threads).</param>
		virtual void GACALL operator ()(const GaPopulation& population,
			GaCouplingResultSet& output,
			const GaCouplingParams& parameters,
			int workerId,
			int numberOfWorkers) const=0;

	};// END CLASS DEFINITION GaCouplingOperation

	/// <summary><c>GaCouplingPair</c> type is instance of <see cref="GaOperationParametersPair" /> template class and represents pair of
	/// coupling operation and its parameters. Detailed description can be found in specification of
	/// <see cref="GaOperationParametersPair" /> template class.</summary>
	typedef GaOperationParametersPair<GaCouplingOperation, GaCouplingParams> GaCouplingPair;

	/// <summary><c>GaCouplingCatalogue</c> type is instance of <see cref="GaCatalogue" /> template class and represents catalogue of coupling operations.
	/// Detailed description can be found in specification of <see cref="GaCatalogue" /> template class.</summary>
	typedef GaCatalogue<GaCouplingOperation> GaCouplingCatalogue;

	/// <summary>This class is base for parameters of replacement operation. 
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaReplacementParams : public GaParameters
	{

	protected:

		/// <summary>Number of chromosomes which should be replaced.</summary>
		int _replacemetSize;

	public:

		/// <summary>This constructor initializes replacement parameters with user-defined replacement size.</summary>
		/// <param name="replacementSize">number of chromosomes which should be replaced.</param>
		GaReplacementParams(int replacementSize) { SetReplacementSize( replacementSize ); }

		/// <summary>This constructor initializes replacement parameters with default values. Default replacement size is 2.</summary>
		GaReplacementParams() : _replacemetSize(0) { }

		/// <summary>More details are given in specification of <see cref="GaParameters::Clone" /> method.</summary>
		virtual GaParameters* GACALL Clone() const { return new GaReplacementParams( *this ); }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns number of chromosomes which should be replaced.</returns>
		inline int GACALL GetReplacementSize() const { return _replacemetSize; }

		/// <summary><c>SetReplacementSize</c> method sets number of chromosomes which should be replaced.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="size">new number of chromosomes for replacement.</param>
		inline void GACALL SetReplacementSize(int size) { _replacemetSize = size < 0 ? 0 : size; }

	};// END CLASS DEFINITION GaReplacementParams

	/// <summary>This class is interface for replacement operations which replace chromosomes of population with new chromosomes.</summary>
	class GaReplacementOperation : public GaOperation
	{

	public:

		// Replaces existing chromosomes with new ones based on passed parameters and selection results.
		/// <summary><c>operator ()</c> performs replacement operation. </summary>
		/// <param name="population">reference to population which hosts chromosomes.</param>
		/// <param name="parameters">reference to parameters of replacement operation.</param>
		/// <param name="newChromosomes">reference to result set of coupling operation.</param>
		virtual void GACALL operator ()(GaPopulation& population,
			const GaReplacementParams& parameters,
			const GaCouplingResultSet& newChromosomes) const=0;

	};// END CLASS DEFINITION GaReplacementOperation

	/// <summary><c>GaReplacementPair</c> type is instance of <see cref="GaOperationParametersPair" /> template class and represents pair
	/// of replacement operation and its parameters. Detailed description can be found in specification of
	/// <see cref="GaOperationParametersPair" /> template class.</summary>
	typedef GaOperationParametersPair<GaReplacementOperation, GaReplacementParams> GaReplacementPair;

	/// <summary><c>GaReplacementCatalogue</c> type is instance of <see cref="GaCatalogue" /> template class and represents catalogue of replacement operations.
	/// Detailed description can be found in specification of <see cref="GaCatalogue" /> template class.</summary>
	typedef GaCatalogue<GaReplacementOperation> GaReplacementCatalogue;

	/// <summary>This class is base for parameters of scaling operation.</summary>
	class GaScalingParams : public GaParameters { };// END CLASS DEFINITION GaScalingParams

	// Interface for scaling operation
	/// <summary>This class is interface for scaling operations which transform chromosomes' fitness values.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaScalingOperation : public GaOperation
	{

	public:

		/// <summary><c>operator ()</c> performs scaling operation and returns scaled fitness value.</summary>
		/// <param name="chromosome">reference to chromosome which fitness is scaled.</param>
		/// <param name="population">reference to population which hosts specified chromosome.</param>
		/// <param name="parameters">reference to parameters of scaling operation.</param>
		/// <returns>Method returns scaled fitness.</returns>
		virtual float GACALL operator ()(const GaScaledChromosome& chromosome,
			const GaPopulation& population,
			const GaScalingParams& parameters) const=0;

		/// <summary><c>IsRankingBased</c> method should return <c>true</c> if scaling of fitness value is based on ranking of chromosome in population.</summary>
		/// <returns>Method returns <c>true</c> if scaling operation scales chromosome's fitness based on its ranking in population.
		/// If ranking is not used in scaling method should return <c>false</c>.</returns>
		virtual bool GACALL IsRankingBased() const=0;

		/// <summary>NeedRescaling method checks current population for consistence of chromosomes' scaled fitness values.</summary>
		/// <param name="population">reference to population which is checked.</param>
		/// <param name="parameters">reference to parameters of scaling operation.</param>
		/// <returns>Method returns <c>false</c> if all scaled fitness values of chromosomes are consistent.
		/// If rescaling of some or all chromosomes in population should be done, method returns <c>true</c>.</returns>
		virtual bool GACALL NeedRescaling(const GaPopulation& population,
			const GaScalingParams& parameters) const=0;

	};// END CLASS DEFINITION GaScalingOperation

	/// <summary><c>GaScalingPair</c> type is instance of <see cref="GaOperationParametersPair" /> template class and represents pair of
	/// scaling operation and its parameters. Detailed description can be found in specification of
	/// <see cref="GaOperationParametersPair" /> template class.</summary>
	typedef GaOperationParametersPair<GaScalingOperation, GaScalingParams> GaScalingPair;

	/// <summary><c>GaScalingCatalogue</c> type is instance of <see cref="GaCatalogue" /> template class and represents catalogue of scaling operations.
	/// Detailed description can be found in specification of <see cref="GaCatalogue" /> template class.</summary>
	typedef GaCatalogue<GaScalingOperation> GaScalingCatalogue;

} // Population

#endif // __GA_POPULATION_OPERATIONS_H__
