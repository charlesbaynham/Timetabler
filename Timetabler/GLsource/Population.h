
/*! \file Population.h
    \brief This file declares classes and datatypes of chromosomes population.
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

#ifndef __GA_POPULATION_H__
#define __GA_POPULATION_H__

#include <list>

#include "Platform.h"
#include "Threading.h"
#include "Statistics.h"
#include "Chromosome.h"
#include "SortedGroup.h"
#include "ChromosomeOperations.h"
#include "PopulationOperations.h"

using namespace std;

using namespace Common;
using namespace Population;
using namespace Chromosome;

/// <summary>Contatins interfaces, classes and datatypes used to implement population of chromosomes and genetic operations.</summary>
namespace Population
{

	/// <summary><c>GaPopulationParameters</c> class represents parameters of population.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary></summary>
	class GaPopulationParameters : public GaParameters
	{

	private:

		/// <summary>Maximal population size (number of chromosomes in population).</summary>
		int _populationSize;

		/// <summary>This attribute indicate if number of chromosomes in population can change. If it is set to <c>true</c> number of chromosomes can vary.
		/// If this attribute is set to <c>false</c>, number of chromosomes in population is equal to maximal number.</summary>
		bool _resizable;

		/// <summary>If this attribute is set to <c>true</c>, chromosomes in population is sorted by their fitness value (original or scaled).</summary>
		bool _sorting;

		/// <summary>If this attribute is set to <c>true</c>, scaled fitness values are used for sorting chromosomes in population.
		/// If it is set to <c>false</c>, original (non-scaled) fitness values are used.</summary>
		bool _usingScaledFitness;

		/// <summary>This attribute stores number of best chromosomes of which population keeps track.
		/// This attribute is ignored if population is sorted.</summary>
		int _bestTrackCount;

		/// <summary>This attribute stores number of worst chromosomes of which population keeps track.
		/// This attribute is ignored if population is sorted.</summary>
		int _worstTrackCount;

	public:

		/// <summary>This constructor initializes population parameters with user-defined values.</summary>
		/// <param name="populationSize">size of population (maximal number of chromosomes in population).</param>
		/// <param name="resizable">instructs population that number of chromosomes can change.</param>
		/// <param name="sorting">instructs population to sort chromosomes according to their fitness values.</param>
		/// <param name="useScaldeFitness">instructs population to use scaled (transformed) fitness value for sorting chromosomes.</param>
		/// <param name="bestTrackCount">number of best chromosomes of which track is being kept.</param>
		/// <param name="worstTrackCount">number of worst chromosomes of which track is being kept.</param>
		GAL_API
		GaPopulationParameters(int populationSize,
			bool resizable,
			bool sorting,
			bool useScaldeFitness,
			int bestTrackCount,
			int worstTrackCount);

		/// <summary>More details are given in specification of <see cref="GaParameters::Clone" /> method.
		///
		/// This method is not thread-safe.</summary>
		virtual GaParameters* GACALL Clone() const { return new GaPopulationParameters( *this ); }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns maximal population size (number of chromosomes in population).</returns>
		inline int GACALL GetPopulationSize() const { return _populationSize; }

		/// <summary><c>SetPopulationSize</c> method sets maximal population size.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="size">size of population.</param>
		inline void GACALL SetPopulationSize(int size) { _populationSize = size >= 2 ? size : 2; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns <c>true</c> if number of chromosomes can change, otherwise it returns <c>false</c>.</returns>
		inline bool GACALL GetResizable() const { return _resizable; }

		/// <summary><c>SetResizable</c> method sets capability of population to accept changes of number of chromosomes in it. 
		///
		/// This method is not thread-safe.</summary>
		/// <param name="resizable">instructs population that number of chromosomes can change.</param>
		inline void GACALL SetResizable(bool resizable) { _resizable = resizable; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns <c>true</c> if chromosomes in population is sorted by their fitness value (original or scaled),
		/// otherwise it returns <c>false</c>.</returns>
		inline bool GACALL GetSorting() const { return _sorting; }

		/// <summary><c>SetSorting</c> method sets capability of population to sort chromosomes by their fitness value.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="sorting">instructs population to sort chromosomes according to their fitness values.</param>
		inline void GACALL SetSorting(bool sorting) { _sorting = sorting; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns <c>true</c> if population use scaled fitness value to sort chromosomes,
		/// otherwise if non-scaled fitness values are used it returns <c>false</c>.</returns>
		inline bool GACALL GetUsingScaledFitness() const { return _usingScaledFitness; }

		/// <summary><c>SetUseScaledFitness</c> method sets capability of population to use scaled fitness values of chromosomes for sorting them. 
		///
		/// This method is not thread-safe.</summary>
		/// <param name="useScaledFitness">instructs population to use scaled (transformed) fitness value for sorting chromosomes.</param>
		inline void GACALL SetUsingScaledFitness(bool useScaledFitness) { _usingScaledFitness = useScaledFitness; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns number of best chromosomes of which population keeps track.</returns>
		inline int GACALL GetBestTrackCount() const { return _bestTrackCount; }

		/// <summary><c>SetBestTrackCount</c> method sets number of best chromosomes of which population keeps track
		///
		/// This method is not thread-safe.</summary>
		/// <param name="count">number of best chromosomes of which track is being kept.</param>
		GAL_API
		void GACALL SetBestTrackCount(int count);

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns number of best chromosomes of which population keeps track.</returns>
		inline int GACALL GetWorstTrackCount() const { return _worstTrackCount; }

		/// <summary><c>SetWorstTrackCount</c> method sets number of worst chromosomes of which population keeps track. 
		///
		/// This method is not thread-safe.</summary>
		/// <param name="count">number of worst chromosomes of which track is being kept.</param>
		GAL_API
		void  GACALL SetWorstTrackCount(int count);

	};// END CLASS DEFINITION GaPopulationParameters

	class GaPopulation;

	/// <summary>This class stores parameters and genetic operations of a population or a group of populations. All populations bound to the configuration,
	/// are updated and notified every time when configuration is changed. Note that each population copies parameters' of the population and use local copy.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaPopulationConfiguration
	{

	private:

		/// <summary>Global instance of default configuration.</summary>
		GAL_API
		static GaPopulationConfiguration* _default;

		/// <summary>Selection operation and its parameters.</summary>
		GaSelectionPair _selection;

		/// <summary>Replacement operation and its parameters.</summary>
		GaReplacementPair _replacement;

		/// <summary>Scaling operation and its parameters.</summary>
		GaScalingPair _scaling;

		/// <summary>Coupling operation and its parameters.</summary>
		GaCouplingPair _coupling;

		/// <summary>Population's parameters</summary>
		GaPopulationParameters _parameters;

		/// <summary>List of populations which use this configuration.</summary>
		list<GaPopulation*> _populations;

		/// <summary>Pointer to comparator used for sorting the chromosomes in population.</summary>
		const GaFitnessComparator* _sortingComparator;

	public:

		/// <summary><c>MakeDefault</c> method makes global instance of default configuration.
		///
		/// This method is not thread-safe.</summary>
		GAL_API
		static void GACALL MakeDefault();

		/// <summary><c>FreeDefault</c> method frees global instance of default configuration.
		///
		/// This method is not thread-safe.</summary>
		static void GACALL FreeDefault()
		{
			if( _default )
				delete _default;

			_default = NULL;
		}

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference to default configuration.</returns>
		static GaPopulationConfiguration& GetDefault() { return *_default; }

		/// <summary>This constructor initializes configuration by cloning default configuration.</summary>
		GaPopulationConfiguration() : _parameters(_default->_parameters),
			_sortingComparator(_default->_sortingComparator),
			_selection(_default->_selection),
			_replacement(_default->_replacement),
			_coupling(_default->_coupling),
			_scaling(_default->_scaling) { }

		/// <summary>This constructor initializes configuration with user defined operations and parameters.</summary>
		/// <param name="populationParams">reference to populations' parameters.
		/// It makes copy of parameters object by using <c>GaParameters::Clone</c> method.</param>
		/// <param name="sortComparator"></param>
		/// <param name="selectionOperation">pointer to selection operation.</param>
		/// <param name="selectionParams">pointer to parameters of selection operation.</param>
		/// <param name="replacementOperation">pointer to replacement operation.</param>
		/// <param name="replacementParams">pointer to parameters of replacement operation.</param>
		/// <param name="couplingOperation">pointer to coupling operation.</param>
		/// <param name="couplingParams">pointer to parameters of coupling operation.</param>
		/// <param name="scalingOperation">pointer to scaling operation.</param>
		/// <param name="scalingParams">pointer to parameters of scaling operation.</param>
		GaPopulationConfiguration(const GaPopulationParameters& populationParams,
			const GaFitnessComparator* sortComparator,
			GaSelectionOperation* selectionOperation,
			GaSelectionParams* selectionParams,
			GaReplacementOperation* replacementOperation,
			GaReplacementParams* replacementParams,
			GaCouplingOperation* couplingOperation,
			GaCouplingParams* couplingParams,
			GaScalingOperation* scalingOperation,
			GaScalingParams* scalingParams): _parameters(populationParams),
			_sortingComparator(sortComparator),
			_selection(selectionOperation, *selectionParams),
			_replacement(replacementOperation, *replacementParams),
			_coupling(couplingOperation, *couplingParams),
			_scaling(scalingOperation, *scalingParams) { }

		/// <summary><c>BindPopulation</c> method binds population to this configuration.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="population">pointer to population which should be bound.</param>
		/// <param name="refresh">this parameter should be set to <c>true</c> if population should be notified about change 
		/// and refreshed according to new parameters.</param>
		GAL_API
		void GACALL BindPopulation(GaPopulation* population,
			bool refresh);

		// Unbinds population form the configuration
		/// <summary><c>UnbindPopulation</c> method unbinds population from this configuration.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="population">pointer to population which should be unbound.</param>
		inline void GACALL UnbindPopulation(GaPopulation* population)
		{
			if( population )
				_populations.remove( population );
		}

		// Returns parameter of the population(s)
		/// <summary>This method is not thread-safe.</summary>
		/// <returns></returns>
		inline const GaPopulationParameters& GACALL GetParameters() const { return _parameters; }

		// Sets parameter of the population(s)
		/// <summary></summary>
		/// <param name="parameters"></param>
		GAL_API
		void GACALL SetParameters(const GaPopulationParameters& parameters);

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference to object that holds pointer to selection operation which should be executed by the algorithm
		/// and its parameters.</returns>
		inline const GaSelectionPair& GACALL Selection() const { return _selection; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference to object that holds pointer to replacement operation which should be executed by the algorithm
		/// and its parameters.</returns>
		inline const GaReplacementPair& GACALL Replacement() const { return _replacement; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference to object that holds pointer to scaling operation which should be executed by the algorithm
		/// and its parameters.</returns>
		inline const GaScalingPair& GACALL Scaling() const { return _scaling; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference to object that holds pointer to coupling operation which should be executed by the algorithm
		/// and its parameters.</returns>
		inline const GaCouplingPair& GACALL Coupling() const { return _coupling; }

		/// <summary>This <c>SetOperation</c> method sets selection operation and its parameters which should be used by genetic algorithm.
		/// It makes copy of parameters object by using <c>GaParameters::Clone</c> method. The method also notifies all bound populations about the change.
		///
		/// This method is not thread safe.</summary>
		/// <param name="operation">pointer to new selection operation.</param>
		/// <param name="parameters">pointer to parameters of selection operation.</param>
		inline void GACALL SetOperation(GaSelectionOperation* operation,
			GaSelectionParams* parameters) { _selection.SetOperation( operation, *parameters ); }

		/// <summary>This <c>SetOperation</c> method sets replacement operation and its parameters which should be used by genetic algorithm.
		/// It makes copy of parameters object by using <c>GaParameters::Clone</c> method. The method also notifies all bound populations about the change.
		///
		/// This method is not thread safe.</summary>
		/// <param name="operation">pointer to new replacement operation.</param>
		/// <param name="parameters">pointer to parameters of replacement operation.</param>
		inline void GACALL SetOperation(GaReplacementOperation* operation,
			GaReplacementParams* parameters) { _replacement.SetOperation( operation, *parameters ); }

		/// <summary>This <c>SetOperation</c> method sets scaling operation and its parameters which should be used by genetic algorithm.
		/// It makes copy of parameters object by using <c>GaParameters::Clone</c> method. The method also notifies all bound populations about the change.
		///
		/// This method is not thread safe.</summary>
		/// <param name="operation">pointer to new scaling operation.</param>
		/// <param name="parameters">pointer to parameters of scaling operation.</param>
		GAL_API
		void GACALL SetOperation(GaScalingOperation* operation,
			GaScalingParams* parameters);

		/// <summary>This <c>SetOperation</c> method sets coupling operation and its parameters which should be used by genetic algorithm.
		/// It makes copy of parameters object by using <c>GaParameters::Clone</c> method. The method also notifies all bound populations about the change.
		///
		/// This method is not thread safe.</summary>
		/// <param name="operation">pointer to new coupling operation.</param>
		/// <param name="parameters">pointer to parameters of coupling operation.</param>
		inline void GACALL SetOperation(GaCouplingOperation* operation,
			GaCouplingParams* parameters) { _coupling.SetOperation( operation, *parameters ); }

		/// <summary>This <c>SetOperationParams</c> method sets parameters for selection operation.
		/// It makes copy of parameters object by using <c>GaParameters::Clone</c> method. The method also notifies all bound populations about the change.
		///
		/// This method is not thread safe.</summary>
		/// <param name="parameters">pointer to parameters of selection operation.</param>
		inline void GACALL SetOperationParams(GaSelectionParams* parameters) { _selection.SetParameters( *parameters ); }

		/// <summary>This <c>SetOperationParams</c> method sets parameters for replacement operation.
		/// It makes copy of parameters object by using <c>GaParameters::Clone</c> method. The method also notifies all bound populations about the change.
		///
		/// This method is not thread safe.</summary>
		/// <param name="parameters">pointer to parameters of replacement operation.</param>
		inline void GACALL SetOperationParams(GaReplacementParams* parameters) { _replacement.SetParameters( *parameters ); }

		/// <summary>This <c>SetOperationParams</c> method sets parameters for scaling operation.
		/// It makes copy of parameters object by using <c>GaParameters::Clone</c> method. The method also notifies all bound populations about the change.
		///
		/// This method is not thread safe.</summary>
		/// <param name="parameters">pointer to parameters of scaling operation.</param>
		GAL_API
		void GACALL SetOperationParams(GaScalingParams* parameters);

		/// <summary>This <c>SetOperationParams</c> method sets parameters for coupling operation.
		/// It makes copy of parameters object by using <c>GaParameters::Clone</c> method. The method also notifies all bound populations about the change.
		///
		/// This method is not thread safe.</summary>
		/// <param name="parameters">pointer to parameters of coupling operation.</param>
		inline void GACALL SetOperationParams(GaCouplingParams* parameters) { _coupling.SetParameters( *parameters ); }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns pointer to fitness comparator which is used for sorting chromosomes in population.</returns>
		inline const GaFitnessComparator* GACALL GetSortComparator() const { return _sortingComparator; }

		// Sets comparator used for sorting the chromosomes
		/// <summary><c>SetSortComaprator</c> method sets fitness comparator which is used for sorting chromosomes.
		/// The method also notifies all bound populations about the change.</summary>
		/// <param name="comparator">pointer to new fitness comparator.</param>
		GAL_API
		void GACALL SetSortComparator(const GaFitnessComparator* comparator);

	};// END CLASS DEFINITION GaPopulationConfiguration
		
	/// <summary><c>GaPopulation</c> class is used to host chromosomes. Population also provides statistical information about chromosomes in population
	/// by using <see cref="GaStatistics" /> class. Same chromosome can be hosted by multiple populations because they are stored in population using
	/// scaled chromosome object which contains smart pointer to chromosome and value of scaled fitness (because this value depends on population,
	/// but not on chromosome itself and same chromosome can be in multiple populations). Chromosomes in population can be sorted which provides easy way
	/// of tracking best and worst chromosomes, but if chromosomes are not sorted, population provides to sorted groups which stores indices of best and
	/// worst chromosomes. Sorting of chromosomes in population or in sorted groups is done my using provided sorting comparator. Both scaled and non-scaled
	/// fitness values of chromosomes can be used for sorting, which value is going to be used depends on specified parameters, but if scaling operation is not
	/// defined for the population, non-scaled fitness values are used. Maximal number of chromosomes which can be hosted is specified in population's parameters.
	/// Also whether the population is going to be fixed-size (number of chromosomes in population is always equals to maximal number of chromosomes it can store)
	/// or resizable (population can host from 0 to maximal number of chromosomes) is specified in population's parameters. During population initialization,
	/// provided prototype is used to create chromosomes which fill the population.  Fixed-size populations are always filled during initialization,
	/// which is not the case with resizable populations. Besides hosting of chromosomes, population also binds extern genetic operations (selection, coupling,
	/// scaling, and replacement) which are going to be used by genetic algorithm during evolution.
	///
	/// This class has built-in synchronizator so it is allowed to use <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaPopulation
	{

		friend class GaPopulationConfiguration;

		DEFINE_SYNC_CLASS;

	private:

		/// <summary>If this attribute is set to <c>true</c>, scaled fitness values are used for sorting chromosomes in population.
		/// If it is set to <c>false</c>, original (non-scaled) fitness values are used. Value of this attribute is copied from population's parameters.</summary>
		bool _usingScaledFitness;

		/// <summary>Number of chromosomes currently in population.</summary>
		int _currentSize;

		/// <summary>Statistical information about population.</summary>
		GaStatistics _statistics;

		/// <summary>Pointer to array of scaled chromosome objects. Chromosomes which belong to population are stored in this array.</summary>
		GaScaledChromosome** _chromosomes;

		/// <summary>Pointer to chromosome which represents prototype for initialization of population.</summary>
		GaChromosome* _prototype;

		/// <summary>This sorted group keeps track of best chromosomes in population. If population is sorted, this group is ignored.
		/// Group uses same fitness comparator for sorting as population.</summary>
		GaSortedGroup _best;

		/// <summary>This sorted group keeps track of worst chromosomes in population. If population is sorted, this group is ignored.
		/// Group uses same fitness comparator for sorting as population, but it inverse results of comparisons in order to provide inverse sorting of chromosomes.</summary>
		GaSortedGroup _worst;

		/// <summary></summary>
		GaPopulationConfiguration* _configuration;

		/// <summary>Population's parameters.</summary>
		GaPopulationParameters _parameters;

	public:

		/// <summary>This constructor initializes population with its configuration and prototype chromosomes. It also allocates memory for array
		/// which is used to store chromosomes, and initializes sorted groups which keep track of best and worst chromosomes.</summary>
		/// <param name="prototype">pointer to chromosomes which is prototype for making chromosomes during population initialization.</param>
		/// <param name="configuration">pointer to population's configuration.</param>
		GAL_API
		GaPopulation(GaChromosome* prototype,
			GaPopulationConfiguration* configuration);

		/// <summary>Destructor frees memory used by scaled chromosome objects which are hosted in population and array which stores them.</summary>
		GAL_API
		~GaPopulation();

		/// <summary><c>Clone</c> method makes copy of population's object. It can make copy of whole object (with setup and chromosomes) or it can make empty
		/// copy of population's object (only setup, without coping chromosomes). Copying of chromosomes is done by making new scaled chromosomes object which
		/// holds smart pointers to same chromosomes. Caller is responsible for memory allocated by this method for new population's object.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="copyChromosomes">if this parameter is set to <c>true</c>, both setup and chromosomes are copied to new object,
		/// otherwise new object is empty, but with same setup.</param>
		GAL_API
		GaPopulation* GACALL Clone(bool copyChromosomes) const;

		/// <summary>This method removes all chromosomes from population and clears sorted group and statistical information.
		/// If population is not resizable or user specified so, population is filled with new chromosomes which are built based on provided prototype,
		/// also it sorts new chromosomes if needed and update sorted groups and statistical information.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="fill">if this parameter is set to <c>true</c>, population is filled wit new chromosomes built on specified prototype.
		/// If this parameter is set to <c>false</c>, population is only cleared. This parameter is ignored if population is not resizable, and population
		/// is always filled.</param>
		/// <returns>Method returns pointer to newly created chromosome's object.</returns>
		GAL_API
		void GACALL InitializePopulation(bool fill = true);

		/// <summary>This method fills provided array with indices of best chromosomes in population. If population is not sorted this method can only
		/// provide number of best chromosomes which is equal to number of tracked best chromosomes.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="results">pointer to array which is going to be filled.</param>
		/// <param name="start">index of the first best chromosome which is going to be copied.</param>
		/// <param name="numberOfChromosomes">number of queried chromosomes' indices.</param>
		/// <returns>Method returns number of saved indices in output array.</returns>
		GAL_API
		int GACALL GetBestChromosomes(int* results,
			int start,
			int numberOfChromosomes) const;

		/// <summary>This method fills provided array with smart pointer to best chromosomes in population. If population is not sorted this method can only
		/// provide number of best chromosomes which is equal to number of tracked best chromosomes.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="results">pointer to array which is going to be filled.</param>
		/// <param name="start">index of the first best chromosome which is going to be copied.</param>
		/// <param name="numberOfChromosomes">number of queried chromosome' indices.</param>
		/// <returns>Method returns number of saved smart pointer to chromosomes in output array.</returns>
		GAL_API
		int GACALL GetBestChromosomes(GaChromosomePtr* results,
			int start,
			int numberOfChromosomes) const;

		/// <summary>This method fills provided array with smart pointer to worst chromosomes in population. If population is not sorted this method can only
		/// provide number of worst chromosomes which is equal to number of tracked worst chromosomes.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="results">pointer to array which is going to be filled.</param>
		/// <param name="start">index of the last worst chromosome which is going to be copied.</param>
		/// <param name="numberOfChromosomes">number of queried chromosome' indices.</param>
		/// <returns>Method returns number of saved indices in output array.</returns>
		GAL_API
		int GACALL GetWorsChromosomes(int* results,
			int start,
			int numberOfChromosomes) const;

		/// <summary>This method fills provided array with indices of worst chromosomes in population. If population is not sorted this method can only
		/// provide number of worst chromosomes which is equal to number of tracked worst chromosomes.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="results">pointer to array which is going to be filled.</param>
		/// <param name="start">index of the last worst chromosome which is going to be copied.</param>
		/// <param name="numberOfChromosomes">number of queried chromosome' indices.</param>
		/// <returns>Method returns number of saved smart pointer to chromosomes in output array.</returns>
		GAL_API
		int GACALL GetWorsChromosomes(GaChromosomePtr* results,
			int start,
			int numberOfChromosomes) const;

		/// <summary><c>Replace</c> method removes chromosome with specified index from population and insert new chromosome. Scaled fitness value of
		/// new chromosome is automatically calculated. This method preserve sorting of chromosomes and update sorted group and statistical information.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="index">index of chromosome which is going to be removed.</param>
		/// <param name="newChromosome">smart pointer to chromosome which is going to be inserted into population.</param>
		/// <returns>Method returns 1 if chromosome is replaced, otherwise returns 0.</returns>
		GAL_API
		int GACALL Replace(int index,
			GaChromosomePtr newChromosome);

		/// <summary><c>Replace</c> method removes group of chromosomes with specified indices from population and insert new chromosomes. Scaled fitness values
		/// of new chromosomes are automatically calculated. If index of one chromosome in specified more then once in array of chromosomes which
		/// should be removed, results are unexpected and may lead to failures. This method preserve sorting of chromosomes and update sorted group and
		/// statistical information.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="indices">pointer to array of indices of chromosomes which are going to be removed.</param>
		/// <param name="newChromosomes">pointer to array of smart pointer to chromosomes which are going to be inserted into population.</param>
		/// <param name="numberOfChromosomes">number of chromosomes which are going to be replaced.</param>
		/// <returns>Method returns number of replaced chromosomes.</returns>
		GAL_API
		int GACALL ReplaceGroup(int* indices,
			GaChromosomePtr* newChromosomes,
			int numberOfChromosomes);

		/// <summary><c>Insert</c> method inserts new chromosome into population. If population is not sorted new chromosome can be inserted if population
		/// is resizable and is not full. If population is sorted new chromosome is inserted at appropriate place (according to sorting policy),
		/// but if population is fixed-size or resizable but full, this method virtually extends population size by one before it inserts chromosome,
		/// after inserting it removes chromosome with worst fitness value (new chromosome is also included). This method preserve sorting of chromosomes
		/// and update sorted group and statistical information.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="chromosome">smart pointer to chromosome which is going to be inserted into population.</param>
		/// <returns>Method returns 1 if chromosome is inserted, otherwise returns 0.</returns>
		GAL_API
		int GACALL Insert(GaChromosomePtr chromosome);

		/// <summary><c>Insert</c> method inserts new chromosomes into population. If population is not sorted new chromosomes can be inserted if population
		/// is resizable and is not full, if there is more new chromosomes then space in population, only first chromosomes are inserted. If population is
		/// sorted new chromosomes are inserted at appropriate places (according to sorting policy), but if population is fixed-size or resizable but full,
		/// this method virtually extends population size so it is able to host all chromosome before it inserts chromosomes, after inserting it
		/// removes chromosomes with worst fitness values(new chromosomes are also included). This method preserve sorting of chromosomes and update sorted group
		/// and statistical information.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="chromosomes">pointer to array of smart pointer to chromosomes which are going to be inserted into population.</param>
		/// <param name="numberOfChromosomes">number of chromosomes which should be inserted.</param>
		/// <returns>Method returns number of inserted chromosomes.</returns>
		GAL_API
		int GACALL InsertGroup(GaChromosomePtr* chromosomes,
			int numberOfChromosomes);

		/// <summary><c>Remove</c> method removes chromosome with specified index from population. To remove chromosome from population, it must be resizable.
		/// This method preserve sorting of chromosomes and update sorted group and statistical information.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="chromosome">index of chromosome which is going to be removed.</param>
		/// <returns>Method returns 1 if chromosome is removed, otherwise returns 0.</returns>
		GAL_API
		int GACALL Remove(int chromosome);

		/// <summary><c>Remove</c> method removes chromosomes with specified indices from population. To remove chromosomes from population, it must be resizable.
		/// If index of one chromosome in specified more then once in array of chromosomes which should be removed, results are unexpected and
		/// may lead to failures. This method preserve sorting of chromosomes and update sorted group and statistical information.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="chromosomes">pointer to array of indices of chromosomes which are going to be removed.</param>
		/// <param name="numberOfChromosomes">number of chromosomes which are going to be removed.</param>
		/// <returns>Method returns number of removed chromosomes.</returns>
		GAL_API
		int GACALL RemoveGroup(int* chromosomes,
			int numberOfChromosomes);

		/// <summary><c>NextGeneration</c> method prepares population for new generation. It should be called before new cycle of execution of
		/// genetic operations by genetic algorithm.
		///
		/// This method is not thread-safe.</summary>
		inline void GACALL NextGeneration() { _statistics.NextGeneration(); }

		/// <summary><c>EndOfGeneration</c> method is used for evolutions with non-overlapping population. It is used to finalize cycle of genetic operations'
		/// execution and should be called by genetic algorithm at the end of a generation.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="previous">reference to population which is used in previous generation by the algorithm.</param>
		inline void GACALL EndOfGeneration(const GaPopulation& previous)
		{
			// rescale population if needed
			if( _configuration->Scaling().HasOperation() &&
				_configuration->Scaling().GetOperation().NeedRescaling( *this, _configuration->Scaling().GetParameters() ) )
				RescaleAll();

			// copy statistics from previous population
			_statistics.CopyFrom( previous._statistics, true, false, true );
		}

		/// <summary><c>EndOfGeneration</c> method is used for evolutions with overlapping population. It is used to finalize cycle of genetic operations'
		/// execution and should be called by genetic algorithm at the end of a generation.
		///
		/// This method is not thread-safe.</summary>
		inline void GACALL EndOfGeneration()
		{
			// rescale population if needed
			if( _configuration->Scaling().HasOperation() &&
				_configuration->Scaling().GetOperation().NeedRescaling( *this, _configuration->Scaling().GetParameters() ) )
				RescaleAll();
		}

		/// <summary><c>Clear</c> method removes all chromosomes form population and clears statistical information if specified.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="clearStatistics">if this parameter is set to <c>true</c>, method clears statistics and removes chromosomes,
		/// otherwise it keeps statistics.</param>
		GAL_API
		void GACALL Clear(bool clearStatistics);

		/// <summary><c>GetChromosomeRanking</c> method returns ranking of chromosomes with specified index based on its fitness value (scaled or non-scaled).
		/// If population is sorted, all chromosomes has ranking. If it is unsorted only chromosomes in best and worst sorted groups has ranking.
		/// Also inverse chromosome ranking can be queried.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="chromosomeIndex">index of chromosome which rank is queried.</param>
		/// <param name="reverse">if this is set to <c>true</c>, inverse ranking is queried, otherwise normal ranking is queried.</param>
		/// <returns>Method returns rnking of the chromosome in the population. If chromosome has no ranking, method returns -1.</returns>
		GAL_API
		int GACALL GetChromosomeRanking(int chromosomeIndex,
			bool reverse = false);

		/// <summary>This method is not thread-safe.</summary>
		/// <param name="position">index of chromosome.</param>
		/// <returns>Method returns reference to scaled chromosome with specified index.</returns>
		inline GaScaledChromosome& GACALL GetAt(int position) const { return *_chromosomes[ position ]; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference to object which contains statistical information about population.</returns>
		inline const GaStatistics& GACALL GetStatistics() const { return _statistics; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference to population configuration.</returns>
		inline const GaPopulationConfiguration& GetConfiguration() const { return *_configuration; }

		/// <summary><c>SetConfiguration</c> method sets population's configuration.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="configuration">pointer to new configuration object.</param>
		void SetConfiguration(GaPopulationConfiguration* configuration)
		{ 
			if( _configuration != configuration )
			{
				_configuration->UnbindPopulation( this );

				_configuration = configuration;
			
				if( _configuration )
					_configuration->BindPopulation( this, true );
			}
		}

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns <c>true</c> if population use scale fitness values for sorting.
		/// If population use non-scaled fitness values, it returns <c>false</c>. </returns>
		inline bool GACALL IsScaledFitnessUsed() { return _usingScaledFitness; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns pointer to chromosome which is used as prototype during initialization of population.</returns>
		inline const GaChromosome& GACALL GetPrototype() const { return *_prototype; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns number of chromosomes in population. If population is fixed size this method returns maximal number of chromosomes
		/// if population is initialized, or 0 if it is not.</returns>
		inline int GACALL GetCurrentSize() const { return _currentSize; }

	private:

		/// <summary><c>SetParameters</c> method sets parameters of population. It copies values of parameters into built-in parameters' object of population
		/// and updates population according to new parameters.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="parameters">reference to object of new population's parameters.</param>
		GAL_API
		void SetParameters(const GaPopulationParameters& parameters);

		/// <summary><c>SetSortComaprator</c> method sets fitness comparator which is used for sorting chromosomes. It update sorting of chromosomes,
		/// sorted groups and statistical information according to new sorting policy.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="comparator">pointer to new fitness comparator.</param>
		GAL_API
		void SetSortComparator(const GaFitnessComparator* comparator);

		/// <summary><c>RefreshBestGroup</c> method refreshes content of best sorted group.
		///
		/// This method is not thread-safe.</summary>
		inline void GACALL RefreshBestGroup()
		{
			for( int i = 0; i < _currentSize; i++ )
				_best.Add( i );
		}

		/// <summary><c>RefreshWorstGroup</c> method refreshes content of worst sorted group.
		/// This method is not thread-safe.</summary>
		inline void GACALL RefreshWorstGroup()
		{
			for( int i = 0; i < _currentSize; i++ )
				_worst.Add( i );
		}

		/// <summary><c>UpdateStatistics</c> method updates all population statistics.
		/// This method is not thread-safe.</summary>
		/// <param name="fitnessChange">value by which sum of non-scaled fitness values of all chromosomes in population has change after last update.</param>
		/// <param name="scaledFitnessChange">value by which sum of scaled fitness values of all chromosomes in population has change after last update.</param>
		GAL_API
		void GACALL UpdateStatistics(float fitnessChange,
			float scaledFitnessChange);

		/// <summary><c>RescaleAll</c> method recalculates scaled fitness values of all chromosomes in population and stores them.
		/// It also updates population statistics.</summary>
		GAL_API
		void GACALL RescaleAll();

		/// <summary>ResortPopulation method resorts population when some of part of sorting policy is changed. After resorting, method updates statistics.
		/// If population is not sorted, this method updates sorted group.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="refreshFitnesses">if this parameter is set to true, before resorting all fitness values should be recalculated
		/// (i.e. after change of fitness operation).</param>
		/// <param name="scalingChanged">if this parameter is set to true, before resorting all scaled fitness values should be recalculated
		/// (i.e. after change of scaling operation).</param>
		/// <param name="comparatorChanged">this parameter indicates that fitness comparator which is used for sorting has changed.</param>
		GAL_API
		void GACALL ResortPopulation(bool refreshFitnesses,
			bool scalingChanged,
			bool comparatorChanged);

		/// <summary>This method is used by CRT's <c>qsort</c> function for sorting array which stores chromosomes.</summary>
		GAL_API
		static int APICALL SortComparison(const void* first,
			const void* second);

	};// END CLASS DEFINITION GaPopulation

} // Population

#endif // __GA_POPULATION_H__
