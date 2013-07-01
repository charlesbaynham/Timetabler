
/*! \file SortedGroup.h
    \brief This file declares GaSortedGroup class.
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

#ifndef __GA_SORTED_GROUP_H__
#define __GA_SORTED_GROUP_H__

#include "Platform.h"
#include "Threading.h"
#include "ChromosomeOperations.h"

namespace Population
{

	class GaPopulation;

	/// <summary>Sorted group stores indices of chromosomes in sorted order, chromosomes must be from same population. For sorting,
	/// group use provided fitness comparator. Direction depends on type of group. It group type is <c>GASGT_BEST</c> or <c>GASGT_OTHER</c> chromosomes
	/// are sorted in descending order by their fitness value, if group type is <c>GASGT_WORST</c> chromosomes are sorted in ascending order
	/// by their fitness value. Sorted group can only accept fixed number of chromosomes.
	///
	/// This class has built-in synchronizator so it is allowed to use <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros with instances of this class,
	/// but no public or private methods are thread-safe.</summary>
	class GaSortedGroup
	{

		DEFINE_SYNC_CLASS

	private:

		/// <summary>Pointer to population which hosts chromosomes in sorted group.</summary>
		const GaPopulation* _population;

		/// <summary>Pointer to array of chromosomes' indices.</summary>
		int* _chromosomesIndecies;

		/// <summary>Pointer to comparator which is used for sorting chromosomes.</summary>
		const GaFitnessComparator* _comparator;

		/// <summary>Maximal number of chromosomes which can be stored in this sorted group.</summary>
		int _maxSize;

		/// <summary>Current number of chromosomes in this sorted group.</summary>
		int _currentSize;

		/// <summary>Type of sorted group.</summary>
		GaSortedGroupType _type;

	public:

		/// <summary>This constructor allocates memory for array of chromosomes' indices, and initializes comparator, group type,
		/// and binds group to specified population.</summary>
		/// <param name="population">pointer to population to which this group is bound.</param>
		/// <param name="maxSize">maximal number of chromosomes which can be stored in the group.</param>
		/// <param name="type">type of group.</param>
		/// <param name="comparator">pointer to fitness comparator which is used for sorting.</param>
		GaSortedGroup(const GaPopulation* population,
			int maxSize,
			GaSortedGroupType type,
			const GaFitnessComparator* comparator) : _currentSize(0),
			_population(population),
			_maxSize(maxSize),
			_type(type),
			_comparator(comparator)
		{
			_chromosomesIndecies = _maxSize > 0 ? new int[ _maxSize ] : NULL;

			if( _type == GASGT_NONE )
				_type = GASGT_OTHER;
		}

		/// <summary>This constructor just bounds group to population and initializes its type, but doesn't allocate memory for array of chromosomes' indices.
		/// Because maximal group size is not specified, chromosomes cannot be inserted in this group before calling <see cref="SetMaxSize" />.</summary>
		/// <param name="population">pointer to population to which this group is bound.</param>
		/// <param name="type">type of group.</param>
		GaSortedGroup(const GaPopulation* population,
			GaSortedGroupType type) : _chromosomesIndecies(NULL),
			_currentSize(0),
			_maxSize(0),
			_comparator(NULL),
			_population(population),
			_type(type)
		{
			if( _type == GASGT_NONE )
				_type = GASGT_OTHER;
		}

		/// <summary>Destructor frees memory used by the array of chromosomes' indices.</summary>
		~GaSortedGroup()
		{
			if( _chromosomesIndecies )
				delete[] _chromosomesIndecies;
		}

		/// <summary>This method copies chromosomes' indices to destination group. It copies indices until destination group is not full,
		/// or until all chromosomes from source group aren't copied.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="destination">reference to group which is destination of copying.</param>
		/// <param name="sameSorting">instructs to copy method not to resort chromosomes in destination population. If two groups use same sorting policy
		/// this parameter should be <c>true</c>, to improve performance because resorting is skipped. If they use different sorting policies this parameter
		/// should be <c>false</c>, because that instructs method to resort indices in destination group according to its sorting policy.</param>
		GAL_API
		void GACALL CopyTo(GaSortedGroup& destination,
			  bool sameSorting) const;

		/// <summary><c>Add</c> method inserts chromosome in sorted group. This method preserves sorting. Before this method inserts chromosome it checks
		/// to see if chromosome is already in the group. Check is done based on group membership flags of scaled chromosome object. If group is not full
		/// and chromosome is not already in the group, it is inserted. If group is full, chromosome is inserted only if it has better fitness
		/// (by sorting policy: sorting comparator + group type) then the chromosome which has worst fitness (by sorting policy) in the group, it this is the
		/// case, worst chromosome is removed from the group (and membership flag of the group in scaled chromosome object is cleared) and new chromosome is
		/// inserted. If chromosome is inserted successfully this method sets membership flag of the group in scaled chromosome object.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="chromosomeIndex">index of chromosome in population.</param>
		/// <returns>Method returns position in group at which it is inserted, if it is inserted successfully.
		/// If chromosome is not inserted, method returns -1.</returns>
		GAL_API
		int GACALL Add(int chromosomeIndex);

		/// <summary><c>Remove</c> method removes chromosome from sorted group. This method preserves sorting of the group. Before this method removes
		/// chromosome it checks to see if chromosome is in the group. Check is done based on group membership flags of scaled chromosome object.
		/// If chromosome is removed successfully this method clears membership flag of the group in scaled chromosome object.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="chromosomeIndex">index of chromosome in population.</param>
		/// <returns>Method returns <c>true</c> if chromosome is found and removed successfully.
		/// If chromosome doesn't belong to the group, this method returns <c>false</c>.</returns>
		GAL_API
		bool GACALL Remove(int chromosomeIndex);

		/// <summary><c>Clear</c> method removes all chromosomes from the group and clears theirs membership flags.
		///
		/// This method is not thread-safe.</summary>
		GAL_API
		void GACALL Clear();

		/// <summary><c>GetRanking</c> method returns ranking of chromosome in this sorted group.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="chromosomeIndex">index of chromosome which rank in the group is queried.</param>
		/// <returns>Method returns ranking of chromosome in this group. If chromosome doesn't belong to this group, method returns -1.</returns>
		GAL_API
		int GACALL GetRanking(int chromosomeIndex);

		/// <summary><c>GetAt</c> method returns index of chromosome at specified position in the group.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="pos">position of chromosome in group.</param>
		/// <returns>Method returns index of chromosome at specified position in the group. If position is out of range, method returns -1.</returns>
		inline int GACALL GetAt(int pos) const
		{
			if( pos >= _currentSize || pos < 0 || !_chromosomesIndecies )
				return -1;

			return _chromosomesIndecies[ pos ];
		}

		/// <summary><c>GetScaledChromosomeAt</c> method returns reference to scaled chromosome object at specified position in the group.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="pos">position of chromosome in group.</param>
		/// <returns>Method returns reference to scaled chromosome object at specified position in the group.</returns>
		GAL_API
		GaScaledChromosome& GACALL GetScaledChromosomeAt(int pos) const;

		/// <summary><c>GetChromosomeAt</c> method returns smart pointer to chromosome object at specified position in the group.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="pos">position of chromosome in group.</param>
		/// <returns>Method returns smart pointer to chromosome object at specified position in the group.</returns>
		inline GaChromosomePtr GACALL GetChromosomeAt(int pos) const { return GetScaledChromosomeAt( pos ).GetChromosome(); }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns maximal number of chromosomes which can be stored in this group.</returns>
		inline int GACALL GetMaxSize() const { return _maxSize; }

		/// <summary><c>SetMaxSize</c> method sets maximal number of chromosomes which can be stored in this group.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="size">new maximal number of chromosome.</param>
		GAL_API
		void GACALL SetMaxSize(int size);

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns number of chromosomes which are currently in group.</returns>
		inline int GACALL GetCurrentSize() const { return _currentSize; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns type of the group.</returns>
		inline GaSortedGroupType GACALL GetType() const { return _type; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns pointer to fitness comparator which is used for sorting chromosomes.</returns>
		inline const GaFitnessComparator* GACALL GetComparator() const { return _comparator; }

		/// <summary><c>SetComparator</c> method sets comparator which is used for sorting chromosome in this group.
		/// If comparator is changed, this method resort chromosomes in group, according to new sorting policy.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="comparator">pointer to new fitness comparator.</param>
		GAL_API
		void GACALL SetComparator(const GaFitnessComparator* comparator);

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Method returns pointer to population to which this group is bound.</returns>
		inline const GaPopulation* GACALL GetPopulation() const{ return _population; }

		/// <summary><c>operator []</c> returns index of chromosome at specified position in the group.
		///
		/// This operator is not thread-safe.</summary>
		/// <param name="pos">position of chromosome in group.</param>
		/// <returns>Operator returns index of chromosome at specified position in the group. If position is out of range, operator returns -1.</returns>
		inline int GACALL operator [](int pos) const { return GetAt( pos ); }

	};// END CLASS DEFINITION GaSortedGroup

} // Population

#endif // __GA_SORTED_GROUP_H__
