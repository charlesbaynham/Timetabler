
/*! \file Chromosome.h
    \brief This file contains declaration of interfaces and classes needed to implement behavior and representations chromosomes.
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

#ifndef __GA_CHROMOSOME_H__
#define __GA_CHROMOSOME_H__

#include "Platform.h"
#include "SmartPtr.h"
#include "Operation.h"
#include "SortedGroupType.h"

namespace Population
{
	class GaPopulation;
} // Population

using namespace Common;
using namespace Population;

/// <summary>Contatins interfaces, classes and datatypes needed to implement behavior and representations chromosomes and genetic operations.</summary>
namespace Chromosome
{

	/// <summary>This class is base class for all chromosomes' parameters.
	/// Built-in genetic operations (crossover, mutation, fitness function and comparator) share their parameters with chromosomes parameters.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaChromosomeParams : public GaParameters
	{

	protected:

		/// <summary>Mutation probability in interval (0, 1) of mutation operation being performed during coupling of selected parents.</summary>
		float _mutationProbability;

		/// <summary>Maximal number of affected values in chromosome�s code by mutation operation.</summary>
		int _mutationSize;

		/// <summary>This flag if set, instructs that only mutations which lead to improvement of fitness value should be accepted. If it isn't
		/// set, then all mutation should be accepted.</summary>
		bool _improvingOnlyMutations;

		/// <summary>probability in interval (0, 1) of crossover operation being performed during coupling of selected parents.</summary>
		float _crossoverProbability;

		/// <summary>Number of crossover points between two parents when crossover operation is performed.</summary>
		int _numberOfCrossoverPoints;

	public:

		/// <summary>This constructor initializes parameters with user-defined values.</summary>
		/// <param name="mutationProbability">mutation probability in interval (0, 1).</param>
		/// <param name="mutationSize">maximal number of affected values in chromosome's code by mutation operation.</param>
		/// <param name="improvingOnlyMutations">state of improving only mutation flag.</param>
		/// <param name="crossoverProbability">crossover probability in interval (0, 1).</param>
		/// <param name="numberOfCrossoverPoints">number of crossover points.</param>
		GaChromosomeParams(float mutationProbability,
			int mutationSize,
			bool improvingOnlyMutations,
			float crossoverProbability,
			int numberOfCrossoverPoints) : _mutationProbability(mutationProbability),
			_mutationSize(mutationSize),
			_improvingOnlyMutations(improvingOnlyMutations),
			_crossoverProbability(crossoverProbability),
			_numberOfCrossoverPoints(numberOfCrossoverPoints) { }

		/// <summary>This constructor initializes parameters with default values.
		/// <br>1. mutation probability: 3%
		/// <br>2. mutation size: 1 (only one value is mutated)
		/// <br>3. only improving mutations are accepted: yes
		/// <br>4. crossover probability: 80%
		/// <br>5. number of crossover points: 1
		/// </summary>
		GaChromosomeParams() : _mutationProbability(0.03f),
			_mutationSize(1),
			_improvingOnlyMutations(true),
			_crossoverProbability(0.8f),
			_numberOfCrossoverPoints(1) { }

		/// <summary>More details are given in specification of <see cref="GaParameters::Clone" /> method.
		///
		/// This method is not thread-safe.</summary>
		virtual GaParameters* GACALL Clone() const { return new GaChromosomeParams( *this ); }

		/// <summary>SetMutationProbability method sets probability of mutation operation being performed during coupling of selected parents.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="probability">new probability of mutation operation. Value is in interval (0, 1).</param>
		inline void GACALL SetMutationProbability(float probability) { _mutationProbability = probability; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns probability of mutation operation being performed during coupling of selected parents.</returns>
		inline float GACALL GetMutationProbability() const { return _mutationProbability; }

		/// <summary><c>SetMutationSize</c> method sets maximal number of affected values in chromosome's code by mutation operation. 
		///
		/// This method is not thread-safe.</summary>
		/// <param name="size">maximal number of affected values in chromosome's code.</param>
		inline void GACALL SetMutationSize(int size) { _mutationSize = size; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns maximal number of affected values in chromosome's code by mutation operation. </returns>
		inline int GetMutationSize() const { return _mutationSize; }

		/// <summary><c>SetImprovingMutationsFlag</c> method sets improving-mutation flag.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="improvingOnly">if this parameter is set to <c>true</c>, then only the mutations which lead to improvement of fitness value
		/// should be accepted. If this parameter is set to <c>false</c>, all mutations are accepted.</param>
		inline void GACALL SetImprovingMutationsFlag(bool improvingOnly) { _improvingOnlyMutations	= improvingOnly; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns <c>true</c> if only the mutations which lead to improvement of fitness value should be accepted.
		/// If all mutation should be accepted, then this method return <c>false</c>.</returns>
		inline bool GACALL GetImprovingMutationsFlag() const { return _improvingOnlyMutations; }

		/// <summary> <c>SetCrossoverProbability</c> method sets probability of crossover operation being performed during coupling of selected parents.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="probability">new probability of crossover operation. Value is in interval (0, 1).</param>
		inline void GACALL SetCrossoverProbability(float probability) { _crossoverProbability = probability; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns probability of crossover operation being performed during coupling of selected parents.</returns>
		inline float GACALL GetCrossoverProbability() const { return _crossoverProbability; }

		/// <summary><c>SetNumberOfCrossoverPoints</c> method sets number of crossover points. 
		///
		/// This method is not thread-safe.</summary>
		/// <param name="numberOfPoints">new number of crossover points.</param>
		inline void GACALL SetNumberOfCrossoverPoints(int numberOfPoints) { _numberOfCrossoverPoints = numberOfPoints; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns number of crossover points.</returns>
		inline int GACALL GetNumberOfCrossoverPoints() const { return _numberOfCrossoverPoints; }

	};// END CLASS DEFINITION GaChromosomeParams

	class GaChromosome;

	/// <summary><c>GaChromosomePtr</c> type is instance of <see cref="GaSmartPtr" /> template class and represents smart pointer to a chromosome.
	/// Detailed description can be found in specification of <see cref="GaSmartPtr" /> template class.</summary>
	typedef GaSmartPtr<GaChromosome> GaChromosomePtr;

	/// <summary><c>GaChromosomeConstPtr</c> type is instance of <see cref="GaSmartPtr" /> template class and represents smart pointer to a constant chromosome.
	/// Detailed description can be found in specification of <see cref="GaSmartPtr" /> template class.</summary>
	typedef GaSmartPtr<const GaChromosome> GaChromosomeConstPtr;

	/// <summary><c>GaChromosome</c> is interface for all chromosomes types in the library. Algorithms, populations and genetic operations assume
	/// that all chromosomes obey this interface. Genetic operations over chromosomes (crossover, mutation, fitness function) may require
	/// additional interfaces to be implemented, but all other built-in parts of the library don't have additional assumptions.</summary>
	class GaChromosome
	{

	public:

		/// <summary>Because this is base class, virtual destructor must be defined in order to enable calling of right destructor
		/// (destructor of inheriting class).</summary>
		virtual ~GaChromosome() { };

		/// <summary>This method performs crossover operation with two parents (<c>this</c> and <c>secondParent</c>) and produce offspring.</summary>
		/// <param name="secondParent">smart pointer to second parent.</param>
		/// <returns>Method returns smart pointer to newly created offspring.</returns>
		virtual GaChromosomePtr GACALL Crossover(GaChromosomePtr secondParent) const=0;

		/// <summary>This method performs mutation over <c>this</c> chromosome.</summary>
		virtual void GACALL Mutation()=0;

		/// <summary>This method produces copy of chromosome. It can create exact copy or it can create chromosome with new code,
		/// but with exact setup (parameters, operations, etc).</summary>
		/// <param name="setupOnly">it this parameter is set to <c>true</c> only the setup of the chromosome is copied into new one,
		/// and new chromosome code is created, otherwise the whole chromosome is copied (code and setup).</param>
		/// <returns>Method returns smart pointer to newly created chromosome.</returns>
		virtual GaChromosomePtr GACALL MakeCopy(bool setupOnly) const=0;

		/// <summary>This method creates new chromosome with exact setup (parameters, operations, etc) of this chromosome.</summary>
		/// <returns>Method returns smart pointer to newly created chromosome.</returns>
		virtual GaChromosomePtr GACALL MakeNewFromPrototype() const=0;

		/// <summary>This method returns fitness value of chromosome.</summary>
		/// <returns>Method returns fitness value of chromosome.</returns>
		virtual float GACALL GetFitness() const=0;

		/// <summary>This method compares fitness values of two chromosomes (<c>this</c> and <c>c</c>).
		/// NOTE: Comparison doesn't have to be arithmetical comparison.</summary>
		/// <param name="c">the second chromosome for comparison with <c>this</c>.</param>
		/// <returns>a)	-1 if fitness value of <c>this</c> is lower then value of <c>c</c>.
		/// <br>b)	0 if fitness values of both chromosomes are equal.
		/// <br>c)	1 if fitness value of <c>this</c> is greater then value of <c>c</c>. </returns>
		virtual int GACALL CompareFitnesses(GaChromosomePtr c) const=0;

		/// <summary>This method compares fitness values of two chromosomes (<c>this</c> and <c>c</c>).
		/// NOTE: Comparison doesn't have to be arithmetical comparison.</summary>
		/// <param name="c">the second chromosome for comparison with <c>this</c>.</param>
		/// <returns>a)	-1 if fitness value of <c>this</c> is lower then value of <c>c</c>.
		/// <br>b)	0 if fitness values of both chromosomes are equal.
		/// <br>c)	1 if fitness value of <c>this</c> is greater then value of <c>c</c>. </returns>
		virtual int GACALL CompareFitnesses(float c) const=0;

		/// <summary>This method recalculates fitness value of the chromosome and stores it.</summary>
		virtual void GACALL RefreshFitness()=0;

		/// <summary>This method returns reference of chromosome's parameters.</summary>
		/// <returns>Method returns reference of chromosome's parameters.</returns>
		virtual const GaChromosomeParams& GACALL GetParameters() const=0;

		/// <summary>This method sets pointer to new parameters of chromosome.</summary>
		/// <param name="p">pointer to new parameters.</param>
		virtual void GACALL SetParameters(GaChromosomeParams* p)=0;

		/// <summary>This method returns size of chromosome.s code.</summary>
		/// <returns>Method returns size of chromosome.s code.</returns>
		virtual int GACALL GetCodeSize() const=0;

		/// <summary>This operator copies setup and chromosome's code from <c>rhs</c>.</summary>
		/// <param name="rhs">reference to chromosome which should be copied.</param>
		/// <returns>Method returns reference to this.</returns>
		virtual GaChromosome& GACALL operator =(const GaChromosome& rhs)=0;

		/// <summary>This operator compares chromosomes' codes.</summary>
		/// <param name="c">reference to chromosome which is compared to <c>this</c> chromosome.</param>
		/// <returns>Operator returns <c>true</c> if the chromosomes are equal.</returns>
		virtual bool GACALL operator ==(const GaChromosome& c) const=0;

		/// <summary>This operator compares chromosomes' codes.</summary>
		/// <param name="c">reference to chromosome which is compared to <c>this</c> chromosome.</param>
		/// <returns>Operator returns <c>true</c> if the chromosomes are not equal.</returns>
		virtual bool GACALL operator !=(const GaChromosome& c) const=0;

	protected:

		/// <summary>This method calculates chromosome's fitness value, but it doesn't store it.</summary>
		/// <returns>Method returns calculated fitness value.</returns>
		virtual float GACALL CalculateFitness() const=0;

	};// END CLASS DEFINITION GaChromosome

	class GaFitnessComparator;

	/// <summary>This class wraps chromosome class and represents it in population. To reduce memory usage and improve performance
	/// same chromosome can be placed in multiple populations, but some chromosome's values has different values for separate populations.
	/// All values which are specific to the population are extracted from chromosome class to this class. Same object of <c>GaScaledChromosome</c>
	/// cannot be located in multiple populations.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaScaledChromosome
	{
		DEFINE_SYNC_CLASS

	private:

		/// <summary>Scaled (transformed) fitness value of the chromosome.</summary>
		float _scaledFitness;

		/// <summary>Pointer to population in which the chromosome is located.</summary>
		GaPopulation* _population;

		/// <summary>Smart pointer to chromosome which is bound to population with this object.</summary>
		GaChromosomePtr _chromosome;

		/// <summary>Flags od memberships in sorted groups of population.</summary>
		GaSortedGroupType _groups;

		/// <summary>Index of chromosome in population.</summary>
		int _index;

	public:

		/// <summary>This constructor fully initializes binding object (chromosome, population and index).</summary>
		/// <param name="chromosome">smart pointer to bound chromosome.</param>
		/// <param name="population">pointer to population to which chromosome is bound.</param>
		/// <param name="index">index of chromosome in population.</param>
		GaScaledChromosome(GaChromosomePtr chromosome,
			GaPopulation* population,
			int index);

		/// <summary>This constructor only specifies binding population.</summary>
		/// <param name="population">pointer to population to which chromosome is bound.</param>
		GaScaledChromosome(GaPopulation* population);

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns smart pointer to bound chromosome. Or <c>NULL</c> pointer if object doesn't bind chromosome.</returns>
		inline GaChromosomePtr GACALL GetChromosome() const { return _chromosome; }

		/// <summary><c>SetChromosome</c> sets pointer to bound chromosome.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="chromosome">smart pointer to bound chromosome.</param>
		inline void GACALL SetChromosome(GaChromosomePtr chromosome)
		{
			_chromosome = chromosome;
			Rescale();
		}

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns scaled (transformed) fitness value of bound chromosome.</returns>
		inline float GACALL GetScaledFitness() const { return _scaledFitness; }

		/// <summary><c>SetScaledFitness</c> method sets scaled (transformed) value of bound chromosome.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="fitness">new fitness value.</param>
		inline void GACALL SetScaledFitness(float fitness) { _scaledFitness = fitness; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns fitness value (scaled or original value) which is used for sorting and comparison of chromosomes in population.</returns>
		GAL_API
		float GACALL GetFitnessForComparison() const;

		/// <summary><c>Rescale</c> method recalculates scaled (transformed) fitness value of chromosome.</summary>
		GAL_API
		void GACALL Rescale();

		/// <summary>This method compares fitness values of two chromosomes (<c>this</c> and <c>c</c>).
		/// Which fitness value (scaled or original) is going to be used depends on flag set in population to which chromosomes are bound.
		/// NOTE: Comparison doesn't have to be arithmetical comparison.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="c">second chromosome for comparison with <c>this</c>.</param>
		/// <returns>a) -1 if fitness value of <c>this</c> is lower then value of <c>c</c>.
		/// <br>b) 0 if fitness values of both chromosomes are equal.
		/// <br>c) 1 if fitness value of <c>this</c> is greater then value of <c>c</c>. </returns>
		inline int GACALL CompareFitnesses(const GaScaledChromosome& c) const { return CompareFitnesses( c.GetFitnessForComparison() ); }

		/// <summary>This method compares fitness values of two chromosomes (<c>this</c> and <c>c</c>).
		/// Which fitness value (scaled or original) is going to be used depends on flag set in population to which chromosomes are bound.
		/// NOTE: Comparison doesn't have to be arithmetical comparison.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="c">second chromosome for comparison with <c>this</c>.</param>
		/// <returns>a) -1 if fitness value of <c>this</c> is lower then value of <c>c</c>.
		/// <br>b) 0 if fitness values of both chromosomes are equal.
		/// <br>c) 1 if fitness value of <c>this</c> is greater then value of <c>c</c>. </returns>
		GAL_API
		int GACALL CompareFitnesses(float c) const;

		/// <summary><c>ClearGroupFlag</c> method clears chromosome's membership flag for specified sorted group of population.
		///
		/// <param name="groups">flags of sorted groups that will be set.</param>
		inline void GACALL ClearGroupFlags(GaSortedGroupType groups) { _groups = (GaSortedGroupType)( _groups & ~groups ); }

		/// <summary><c>SetGroupFlag</c> method sets chromosome's membership flag for specified sorted group of population.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="groups">flags of sorted groups that will be set.</param>
		inline void GACALL SetGroupFlags(GaSortedGroupType groups) { _groups = (GaSortedGroupType)( _groups | groups ); }

		/// <summary>Method returns state of desired membership flag(s).
		///
		/// This method is not thread-safe.</summary>
		/// <param name="groups">desired flags of sorted groups which state is queried. Flags are cmbined with operator |.</param>
		/// <param name="all">parameter tells method how the states of flag are combined into result. It this parameter is set to <c>true</c>,
		/// method returns <c>true</c> only if all flags is set, otherwise if this parameter is set to <c>false</c>,
		/// method returns <c>true</c> if any of flags is set.</param>
		/// <returns>It returns <c>true</c> if any of desired flags is set and all parameter is set to <c>false</c>.
		/// If all parameter is set to <c>true</c>, method returns <c>true</c> only if all desired flags are set.</returns>
		inline bool GACALL GetGroupFlag(GaSortedGroupType groups,
			bool all = false) { return all ? ( _groups & groups ) == groups : ( _groups & groups ) != 0; }

		/// <summary><c>SetGroups</c> sets all flags of sorted group membership.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="groups">new values of flags of sorted groups membership.</param>
		inline void GACALL SetGroups(GaSortedGroupType groups) { _groups = groups; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns all values of flags of sorted group membership.</returns>
		inline GaSortedGroupType GACALL GetGroups() const { return _groups; }

		/// <summary><c>SetIndex</c> method sets index of bound chromosome in population.</summary>
		/// <param name="index">new index of bound chromosome.</param>
		GAL_API
		void GACALL SetIndex(int index);

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns index of bound chromosome in population.</returns>
		inline int GACALL GetIndex() const { return _index; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference to a population to which chromosome is bound.</returns>
		inline GaPopulation& GACALL GetPopulation() const { return *_population; }

		/// <summary>This operator is not thread-safe.</summary>
		/// <returns>Operator returns smart pointer to bound chromosome. Or <c>NULL</c> pointer if object isn't bound to chromosome.</returns>
		inline GACALL operator GaChromosomePtr() const { return _chromosome; }

		/// <summary>This operator is not thread-safe.</summary>
		/// <returns>Operator returns index of bound chromosome in population.</returns>
		inline GACALL operator float() const { return _scaledFitness; }

	};// END CLASS DEFINITION GaScaledChromosome

	class GaDefaultChromosome;

	/// <summary>This class represent base for chromosome's configuration block (CCB). CCB stores entire setup of chromosome.
	/// <c>GaChromosomeParamsBlock</c> class stores only pointer to chromosomes' parameters.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaChromosomeParamsBlock
	{
		friend class GaDefaultChromosome;

	protected:

		/// <summary>Pointer to chromosomes' parameters.</summary>
		GaChromosomeParams* _parameters;

	public:

		/// <summary>This constructor initializes CCB with pointer to chromosomes' parameters.</summary>
		/// <param name="parameters">pointer to chromosomes' parameters.</param>
		GaChromosomeParamsBlock(GaChromosomeParams* parameters) : _parameters(parameters) { }

		/// <summary>This is copy constructor. The constructor doesn't create copy of parameters' object,
		/// it only copies pointer to chromosomes' parameters.</summary>
		/// <param name="rhs">reference to CCB which is copied.</param>
		GaChromosomeParamsBlock(const GaChromosomeParamsBlock& rhs) : _parameters(rhs._parameters) { }

		/// <summary>This constructor initializes empty CCB.</summary>
		GaChromosomeParamsBlock() : _parameters(NULL) { }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference to chromosomes� parameters.</returns>
		inline const GaChromosomeParams& GACALL GetParameters() const { return *_parameters; }

		/// <summary><c>SetParameters</c> method sets pointer to chromosomes' parameters.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="params">pointer to new parameters.</param>
		inline void GACALL SetParameters(GaChromosomeParams* params) { _parameters = params; }

	};// END CLASS DEFINITION GaChromosomeParamsBlock

	// Implements some behavior concerning fitness value, parameters
	// and preaparing chromosome for crossover and mutation operations.
	/// <summary>GaDefaultChromosome class implements some basic feature of chromosome such as management of fitness value, 
	/// CCB and probability of crossover and mutation operations.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaDefaultChromosome : public GaChromosome
	{

	protected:

		/// <summary>Fitness value of the chromosome.</summary>
		float _fitness;

		/// <summary>Pointer to CCB.</summary>
		GaChromosomeParamsBlock* _configBlock;

	public:

		/// <summary>This constructor initializes chromsome with CCB.</summary>
		/// <param name="configBlock">pointer to CCB.</param>
		GaDefaultChromosome(GaChromosomeParamsBlock* configBlock) : _fitness(0),
			_configBlock(configBlock) { }

		/// <summary>Copy constructor.</summary>
		/// <param name="c">reference to chromosome which is copied.</param>
		/// <param name="setupOnly">if this parameter is <c>true</c>, only pointer to CCB is copied. If this parameter is <c>false</c>,
		/// chromosome's data and CCB is copied. </param>
		GaDefaultChromosome(const GaDefaultChromosome& c,
			bool setupOnly) : _configBlock(c._configBlock)
		{
			if( !setupOnly )
				_fitness = c._fitness;
		}

		/// <summary>This method handles probability of mutation and provides framework for improving-only mutations.
		/// It decides if mutation is going to be preformed and if it is going to be accepted. If improving-only flag is set,
		/// the method calls <see cref="PreapareForMutation" /> method, then <see cref="PerformeMutation" /> is called,
		/// after the mutation is performed it checks (only if improving-only flag is set) if the mutation produced chromosome with better fitness,
		/// if that is the case <see cref="AcceptMutation" /> is called, otherwise <see cref="RejectMutation" /> is called.
		///
		/// This method is not thread-safe.</summary>
		GAL_API
		virtual void GACALL Mutation();

		/// <summary>This method handles probability of crossover. It decides if crossover is going to be preformed.
		/// If crossover is going to be performed this method calls <see cref="PerformeCrossover" />.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="secondParent">smart pointer to second parent.</param>
		/// <returns>Method returns smart pointer to newly created offspring.</returns>
		GAL_API
		virtual GaChromosomePtr GACALL Crossover(GaChromosomePtr secondParent) const;

		/// <summary>This method is not-thread safe.</summary>
		/// <returns>Method returns fitness value of chromosome.</returns>
		virtual float GACALL GetFitness() const { return _fitness; }

		/// <summary>This method only extracts fitness value from <c>c</c>, and delegate responsibility to <see cref="ComapreFitnesses(float)" /> method.
		/// More details are given in specification of <see cref="GaChromosome::CompareFitnesses" /> of.
		///
		/// This method is not thread-safe.</summary>
		virtual int GACALL CompareFitnesses(GaChromosomePtr c) const { return CompareFitnesses( c->GetFitness() ); }

		/// <summary>More details are given in specification of <see cref="GaChromosome::CompareFitnesses" />.
		///
		/// This method is not thread-safe.</summary>
		virtual int GACALL CompareFitnesses(float c) const=0;

		/// <summary><c>RefreshFitness</c> method recalculates fitness value of the chromosome and stores it.
		/// 
		/// This method is not thread safe.</summary>
		virtual void GACALL RefreshFitness() { _fitness = CalculateFitness(); }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference of chromosome's parameters.</returns>
		virtual const GaChromosomeParams& GACALL GetParameters() const { return *_configBlock->_parameters; }

		/// <summary><c>SetParameters</c> method sets pointer to new parameters of chromosome.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="p">pointer to new parameters.</param>
		virtual void GACALL SetParameters(GaChromosomeParams* p) { _configBlock->_parameters = p; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>This method returns reference to CCB.</returns>
		virtual const GaChromosomeParamsBlock& GACALL GetConfigBlock() const { return *_configBlock; }

		/// <summary><c>SetConfigBlock</c> method sets pointer to new CCB.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="block">pointer to new CCB.</param>
		virtual void GACALL SetConfigBlock(GaChromosomeParamsBlock* block) { _configBlock = block; }

		/// <summary><c>operator =</c> copies setup and chromosome's code from <c>rhs</c>.
		///
		/// This operator is not thread-safe.</summary>
		/// <param name="rhs">reference to chromosome which is copied.</param>
		/// <returns>Method returns reference to this.</returns>
		virtual GaChromosome& GACALL operator =(const GaChromosome& rhs)
		{
			// copy data
			_fitness = ( (const GaDefaultChromosome&)rhs )._fitness;
			_configBlock = ( (const GaDefaultChromosome&)rhs )._configBlock;

			return *this;
		}

		/// <summary>More details are given in specification of <see cref="GaChromosome::operator ==" />.
		///
		/// This method is not thread-safe.</summary>
		virtual bool GACALL operator !=(const GaChromosome& c) const { return !( *this == c ); }

	protected:

		/// <summary>It is called when mutation should be performed over chromosome.</summary>
		virtual void GACALL PerformMutation()=0;

		/// <summary>. It is called when crossover should be performed over two chromosomes to produce offspring.</summary>
		/// <param name="secondParent">smart pointer to second parent.</param>
		/// <returns>Method returns smart pointer to newly created offspring.</returns>
		virtual GaChromosomePtr GACALL PerformCrossover(GaChromosomePtr secondParent) const=0;

		/// <summary>This method called before performing mutation if improving-only mutation flag is set. It should save chromosome's code
		/// so it can be restored if mutation produce chromosome with lower fitness value.</summary>
		virtual void GACALL PreapareForMutation()=0;

		/// <summary>This method is called after mutation is performed if improving-only mutation flag is set and mutation improved fitness value.
		/// The method can delete backed up chromosome's code which was made by <see cref="PreapareForMutation" /> method.</summary>
		virtual void GACALL AcceptMutation()=0;

		/// <summary>This method is called after mutation is performed if improving-only mutation flag is set and mutation degraded fitness value.
		/// The method restores old chromosome's code which was backed up with <see cref="PreapareForMutation" /> method.</summary>
		virtual void GACALL RejectMutation()=0;

	};// END CLASS DEFINITION GaDefaultChromosome

	class GaCrossoverOperation;
	class GaMutationOperation;
	class GaFitnessOperation;
	class GaDynamicOperationChromosome;

	/// <summary>This class is CCB for chromosomes which use extern genetic operations. This CCB stores pointer to those operations.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaChromosomeOperationsBlock : public GaChromosomeParamsBlock
	{
		friend class GaDynamicOperationChromosome;

	protected:

		/// <summary>Pointer to extern fitness operation.</summary>
		GaFitnessOperation* _fitnessOperation;

		/// <summary>Pointer to extern fitness comparator.</summary>
		GaFitnessComparator* _fitnessComparator;

		/// <summary>Pointer to extern mutation operation.</summary>
		GaMutationOperation* _mutationOperation;

		/// <summary>Pointer to extern crossover operation.</summary>
		GaCrossoverOperation* _crossoverOperation;

	public:

		/// <summary>This constructor initializes CCB with pointer to chromosomes' parameters and extern genetic operations.</summary>
		/// <param name="crossoverOperation">pointer to extern crossover operation.</param>
		/// <param name="mutationOperation">pointer to extern mutation operation.</param>
		/// <param name="fitnessOperation">pointer to extern fitness operation.</param>
		/// <param name="fitnessComparator">pointer to extern fitness comparator.</param>
		/// <param name="parameters">pointer to chromosomes� parameters.</param>
		GaChromosomeOperationsBlock(GaCrossoverOperation* crossoverOperation,
			GaMutationOperation* mutationOperation,
			GaFitnessOperation* fitnessOperation,
			GaFitnessComparator* fitnessComparator,
			GaChromosomeParams* parameters) : GaChromosomeParamsBlock(parameters),
			_crossoverOperation(crossoverOperation),
			_mutationOperation(mutationOperation),
			_fitnessOperation(fitnessOperation),
			_fitnessComparator(fitnessComparator) { }

		/// <summary>This is copy constructor. The constructor doesn't create copy of parameters' and operations' objects, 
		/// it only copies pointer to chromosomes' parameters and operations.</summary>
		/// <param name="rhs">reference to CCB which is copied.</param>
		GaChromosomeOperationsBlock(const GaChromosomeOperationsBlock& rhs) : GaChromosomeParamsBlock(rhs),
			_crossoverOperation(rhs._crossoverOperation),
			_mutationOperation(rhs._mutationOperation),
			_fitnessOperation(rhs._fitnessOperation),
			_fitnessComparator(rhs._fitnessComparator) { }

		/// <summary>This constructor initializes empty CCB.</summary>
		GaChromosomeOperationsBlock() : _crossoverOperation(NULL),
			_mutationOperation(NULL),
			_fitnessOperation(NULL),
			_fitnessComparator(NULL) { }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference to extern crossover operation.</returns>
		inline const GaCrossoverOperation& GACALL GetCrossoverOperation() const { return *_crossoverOperation; }

		/// <summary><c>SetCrossoverOperation</c> sets pointer to extern crossover operation.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="operation"></param>
		inline void GACALL SetCrossoverOperation(GaCrossoverOperation* operation) { _crossoverOperation = operation; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference to extern mutation operation.</returns>
		inline const GaMutationOperation& GACALL GetMutationOperation() const { return *_mutationOperation; }

		/// <summary><c>SetMutationOperation</c> sets pointer to extern mutation operation.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="operation">pointer to extern mutation operation.</param>
		inline void GACALL SetMutationOperation(GaMutationOperation* operation) { _mutationOperation = operation; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference to extern fitness operation.</returns>
		inline const GaFitnessOperation& GACALL GetFitnessOperation() const { return *_fitnessOperation; }

		/// <summary><c>SetFitnessOperation</c> sets pointer to extern fitness operation.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="operation">pointer to extern fitness operation.</param>
		inline void GACALL SetFitnessOperation(GaFitnessOperation* operation) { _fitnessOperation = operation; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns reference to extern fitness comparator.</returns>
		inline const GaFitnessComparator& GACALL GetFitnessComparator() const { return *_fitnessComparator; }

		/// <summary>SetFitnessComparator sets pointer to extern fitness comparator.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="comparator">pointer to extern fitness comparator.</param>
		inline void GACALL SetFitnessComparator(GaFitnessComparator* comparator) { _fitnessComparator = comparator; }

	};// END CLASS DEFINITION GaChromosomeOperationsBlock

	/// <summary>This class should be base for chromosomes which use extern genetic operations.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	/// No public or private methods are thread-safe.</summary>
	class GaDynamicOperationChromosome : public GaDefaultChromosome
	{

	public:

		/// <summary>This constructor initializes chromosome with CCB.</summary>
		/// <param name="configBlock">pointer to CCB.</param>
		GaDynamicOperationChromosome(GaChromosomeOperationsBlock* configBlock) : GaDefaultChromosome( configBlock ) { }

		/// <summary>Copy constructor.</summary>
		/// <param name="c">reference to chromosome which is copied.</param>
		/// <param name="setupOnly">if this parameter is <c>true</c>, only pointer to CCB is copied. If this parameter is <c>false</c>,
		/// chromosome's data and CCB is copied.</param>
		GaDynamicOperationChromosome(const GaDynamicOperationChromosome& c,
			bool setupOnly) : GaDefaultChromosome(c, setupOnly) { }

		// Compares fitnesses of two chromosomes. Returns:
		//  a, -1 if the fitness value of this chromosome is lower then the given value
		//  b.  0 if the fitness values of both chromosomes are equal
		//  c.  1 if the fitness value of this chromosome is greater then the given value
		/// <summary>CompareFitnesses method delegates control to extern fitness comparator specified in CCB. 
		///
		/// For more information see specification of <see cref="GaChromosome::CompareFitnesses" /> method.
		///
		/// This method is not thread-safe.</summary>
		GAL_API
		virtual int GACALL CompareFitnesses(float c) const;

	protected:

		/// <summary><c>PerformCrossover</c> method delegates control to extern crossover operation specified in CCB.
		///
		/// For more information see specification of <see cref="GaDefaultChromosome::PerformCrossover" /> method.
		///
		/// This method is not thread-safe.</summary>
		GAL_API
		virtual GaChromosomePtr GACALL PerformCrossover(GaChromosomePtr secondParent) const;

		/// <summary><c>PerformMutation</c> method delegates control to extern mutation operation specified in CCB.
		///
		/// For more information see specification of <see cref="GaDefaultChromosome::PerformMutation" /> method.
		///
		/// This method is not thread-safe.</summary>
		GAL_API
		virtual void GACALL PerformMutation();
		
		/// <summary><c>CalculateFitness</c> method delegates control to extern fitness operation specified in CCB.
		///
		/// For more information see specification of <see cref="GaDefaultChromosome::CalculateFitness" /> method.
		///
		/// This method is not thread-safe.</summary>
		GAL_API
		virtual float GACALL CalculateFitness() const;

	};// END CLASS DEFINITION GaDynamicOperationChromosome

} // Chromosome

#endif // __GA_CHROMOSOME_H__
