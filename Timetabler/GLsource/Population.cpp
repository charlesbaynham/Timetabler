
/*! \file Population.cpp
    \brief This file implements classes and datatypes of chromosomes population.
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

#include "Population.h"

namespace Population
{

	// Copy chromosome indicies to given sorted group
	void GaSortedGroup::CopyTo(GaSortedGroup& destination,
		bool sameSorting) const
	{
		destination.Clear();

		if( sameSorting )
		{
			// copy to destination in source's order
			for( int i = 0; i < _currentSize && i < destination._maxSize; i++, destination._currentSize++ )
				destination._chromosomesIndecies[ i ] = _chromosomesIndecies[ i ];
		}
		else
		{
			// add new chromsomes to destination in destination's sorting order
			for( int i = 0; i < _currentSize; i++ )
				destination.Add( _chromosomesIndecies[ i ] );
		}
	}

	// Adds chromosome to the group.
	// Returns position at which this chromosome has been inserted
	// or -1 if chromosome hasn't been inserted.
	int GaSortedGroup::Add(int chromosomeIndex)
	{
		// is group disabled?
		if( !_chromosomesIndecies || !_population || !_comparator || 
			( _population->GetAt( chromosomeIndex ).GetGroupFlag( _type ) && _type != GASGT_OTHER ) )
			return -1;

		// first chromosome?
		if( !_currentSize )
		{
			_chromosomesIndecies[ _currentSize++ ] = chromosomeIndex;

			// mark this chromosome that it's belongs to this group
			if( _type != GASGT_OTHER )
				_population->GetAt( chromosomeIndex ).SetGroupFlags( _type );

			return 0;
		}

		int rev = _type == GASGT_WORST ? -1 : 1;

		float fitness = _population->GetAt( chromosomeIndex ).GetFitnessForComparison();

		// the group is full, qickly check if the chromosome should be in the group
		if( _currentSize == _maxSize && 
			rev * (*_comparator)( fitness, _population->GetAt( _chromosomesIndecies[ _currentSize - 1 ] ).GetFitnessForComparison() ) <= 0 )
			return -1;

		// if the last chromosome from this group will be removed,
		// clears mark that the last chromosome belongs to this group
		bool removeLast = _currentSize == _maxSize && ( _type != GASGT_OTHER || _type != GASGT_NONE );
		if( removeLast )
			_population->GetAt( _chromosomesIndecies[ _currentSize - 1 ] ).ClearGroupFlags( _type );

		// find position at which the chromosome should be inserted and make room for it
		int i = _currentSize < _maxSize ? _currentSize : _maxSize - 1;
		for( ; i > 0 && rev * (*_comparator)( fitness, _population->GetAt( _chromosomesIndecies[ i - 1 ] ).GetFitnessForComparison() ) > 0; i-- )
			_chromosomesIndecies[ i ] = _chromosomesIndecies[ i - 1 ];

		// insert new chromosome
		_chromosomesIndecies[ i ] = chromosomeIndex;

		if( !removeLast )
			_currentSize++;

		// mark this chromosome that it's belongs to this group
		if( _type != GASGT_OTHER )
			_population->GetAt( chromosomeIndex ).SetGroupFlags( _type );

		return i;
	}

	// Removes chromosome index from the group.
	// Returns TRUE if the chromosome is removed.
	bool GaSortedGroup::Remove(int chromosomeIndex)
	{
		// group is not disabled and does chromosome belongs to the group?
		if( !_chromosomesIndecies ||
			( !_population->GetAt( chromosomeIndex ).GetGroupFlag( _type ) && _type != GASGT_OTHER ) )
			return false;

		int rev = _type == GASGT_WORST ? -1 : 1;

		float fitness = _population->GetAt( chromosomeIndex ).GetFitnessForComparison();

		// qickly check if the chromosome should be in the group
		if( rev * (*_comparator)( fitness, _population->GetAt( _chromosomesIndecies[ _currentSize - 1 ] ).GetFitnessForComparison() ) < 0 )
			return false;

		// find position of chromosome's index in the group
		int pos = 0;
		for( ; pos < _currentSize; pos++ )
		{
			if( _chromosomesIndecies[ pos ] == chromosomeIndex )
				break;
		}

		// is the chromosome in the group?
		if( pos < _currentSize )
		{
			// clears mark that this chromosome belongs to this group
			if( _type != GASGT_OTHER  )
				_population->GetAt( chromosomeIndex ).ClearGroupFlags( _type );

			// remove from the group
			for( int i = pos; i < _currentSize - 1 ; i++ )
				_chromosomesIndecies[ i ] = _chromosomesIndecies[ i + 1 ];

			_currentSize--;

			return true;
		}

		return false;
	}

	// Removes all chromosomes from the group
	void GaSortedGroup::Clear()
	{
		if( _type != GASGT_OTHER )
		{
			// remove chromosome's marks
			while( _currentSize > 0 )
				_population->GetAt( _chromosomesIndecies[ --_currentSize ] ).ClearGroupFlags( _type );
		}
		else
			_currentSize = 0;
	}

	// Returns ranking of chromosome.
	// If chromosomi is not in group returns -1;
	int GaSortedGroup::GetRanking(int chromosomeIndex)
	{
		// check parameters
		if( chromosomeIndex < 0 || chromosomeIndex >= _population->GetCurrentSize() || !_currentSize )
			return -1;

		int rev = _type == GASGT_WORST ? -1 : 1;

		// qickly check if the chromosome should be in the group
		float fitness = _population->GetAt( chromosomeIndex ).GetFitnessForComparison();
		if( rev * (*_comparator)( fitness, _population->GetAt( _chromosomesIndecies[ _currentSize - 1 ] ).GetFitnessForComparison() ) < 0 )
			return -1;

		// find chromosome
		for( int i = 0; i < _currentSize; i++ )
		{
			if( _chromosomesIndecies[ i ] == chromosomeIndex )
				return i;
		}

		// chromosome wasn't found
		return -1;
	}

	// Returns refernece to scaled chromosome at given position
	GaScaledChromosome& GaSortedGroup::GetScaledChromosomeAt(int pos) const
	{
		return _population->GetAt( _chromosomesIndecies[ pos ] );
	}

	// Sets maximum size of the group
	void GaSortedGroup::SetMaxSize(int size)
	{
		// nothing changed?
		if( size == _maxSize )
			return;

		if( size < 0 )
			_maxSize = 0;
		else
			_maxSize = size;

		int* oldIndecies = _chromosomesIndecies;

		if( !_maxSize )
		{
			// clear if size is zero
			Clear();
			_chromosomesIndecies = NULL;
		}
		else
		{
			// new array of chromosome indecies
			_chromosomesIndecies = new int[ _maxSize ];

			int len = 0;
			if( oldIndecies )
			{
				// copy indecies from old array
				len = _maxSize < _currentSize ? _maxSize : _currentSize;
				for( int i = 0; i < len; i++ )
					_chromosomesIndecies[ i ] = oldIndecies[ i ];
			}

			// new size
			_currentSize = len;
		}

		// free memory used by old array
		if( oldIndecies )
			delete[] oldIndecies;
	}

	// Returns fitnes comparator used to sort the group
	void GaSortedGroup::SetComparator(const GaFitnessComparator* comparator)
	{
		if( _comparator != comparator )
		{
			_comparator = comparator;

			int rev = _type == GASGT_WORST ? -1 : 1;

			// resort group
			for( int i = 0; i < _currentSize; i++ )
			{
				int c = _chromosomesIndecies[ i ];
				int j = i - 1;
				for( ; j >= 0 && rev * ( *_comparator )( _population->GetAt( _chromosomesIndecies[ j ] ), _population->GetAt( c ) ) > 0; j-- )
					_chromosomesIndecies[ j + 1 ] = _chromosomesIndecies[ j ];

				_chromosomesIndecies[ j + 1 ] = c;
			}
		}
	}

	// Initialization of the parameters
	GaPopulationParameters::GaPopulationParameters(int populationSize,
		bool resizable,
		bool sorting,
		bool useScaldeFitness,
		int bestTrackCount,
		int worstTrackCount) : _resizable(resizable),
		_sorting(sorting),
		_usingScaledFitness(useScaldeFitness)
	{
		SetPopulationSize( populationSize );
		SetBestTrackCount( bestTrackCount );
		SetWorstTrackCount( worstTrackCount );
	}

	// Sets how many best chromosomes in the population are cached
	void GaPopulationParameters::SetBestTrackCount(int count)
	{
		if( count < 1 )
			_bestTrackCount = 1;
		else if( count > _populationSize )
			_bestTrackCount = _populationSize;
		else
			_bestTrackCount = count;
	}

	// Sets how many worst chromosomes in the population are cached
	void GaPopulationParameters::SetWorstTrackCount(int count)
	{
		if( count < 1 )
			_worstTrackCount = 1;
		else if( count > _populationSize )
			_worstTrackCount = _populationSize;
		else
			_worstTrackCount = count;
	}

	// Global instance of default configuration
	GaPopulationConfiguration* GaPopulationConfiguration::_default = NULL;

	// Makes global instance of default configuration
	void GaPopulationConfiguration::MakeDefault()
	{
		if( !_default )
		{
			// default population parameters
			GaPopulationParameters deafultParams( 10, false, true, false, 0, 0 );

			// deafult comaratore used for sorting
			GaFitnessComparator* deafultComparator = GaFitnessComparatorCatalogue::Instance().GetEntryData( "GaMaxFitnessComparator" );

			// get default selection and make temp parameters for it
			GaSelectionOperation* defaultSelection = GaSelectionCatalogue::Instance().GetEntryData( "GaSelectRouletteWheel" );
			GaSelectionParams* selectionParams = (GaSelectionParams*)defaultSelection->MakeParameters();
			selectionParams->SetSelectionSize( 2 );

			// get default replacement and make temp parameters for it
			GaReplacementOperation* defaultReplacement = GaReplacementCatalogue::Instance().GetEntryData( "GaReplaceWorst" );
			GaReplacementParams* replacementParams = (GaReplacementParams*)defaultReplacement->MakeParameters();
			replacementParams->SetReplacementSize( 2 );

			// get default coupling and make temp parameters for it
			GaCouplingOperation* defaultCoupling = GaCouplingCatalogue::Instance().GetEntryData( "GaInverseCoupling" );
			GaCouplingParams* couplingParams = (GaCouplingParams*)defaultCoupling->MakeParameters();
			couplingParams->SetNumberOfOffsprings( 2 );

			// make default configuration
			_default = new GaPopulationConfiguration( deafultParams, deafultComparator, defaultSelection, selectionParams, defaultReplacement, replacementParams,
				defaultCoupling, couplingParams, NULL, NULL );

			// delete temp parameters
			if( selectionParams )
				delete selectionParams;
			if( replacementParams )
				delete replacementParams;
			if( couplingParams )
				delete couplingParams;
		}
	}

	// Binds population to the configuration
	void GaPopulationConfiguration::BindPopulation(GaPopulation* population,
		bool refresh)
	{
		if( population )
		{
			_populations.push_back( population );

			if( refresh )
			{
				// refresh population object
				population->SetParameters( _parameters );
				population->SetSortComparator( _sortingComparator );
				population->ResortPopulation( false, true, true );
			}
		}
	}

	// Sets parameter of the population(s)
	void GaPopulationConfiguration::SetParameters(const GaPopulationParameters& parameters)
	{
		// refresh populations
		for( list<GaPopulation*>::iterator it = _populations.begin(); it != _populations.end(); ++it )
			(*it)->SetParameters( _parameters );

		_parameters = parameters;
	}

	// Sets scaling operation
	void GaPopulationConfiguration::SetOperation(GaScalingOperation* operation,
											 GaScalingParams* parameters)
	{
		_scaling.SetOperation( operation, *parameters );

		// refresh populations
		for( list<GaPopulation*>::iterator it = _populations.begin(); it != _populations.end(); ++it )
			(*it)->ResortPopulation( false, true, false );
	}

	// Sets parameters for scaling operation
	void GaPopulationConfiguration::SetOperationParams(GaScalingParams* parameters)
	{
		_scaling.SetParameters( *parameters );

		// refresh populations
		for( list<GaPopulation*>::iterator it = _populations.begin(); it != _populations.end(); ++it )
			(*it)->ResortPopulation( false, true, false );
	}

	// Sets comparator used for sorting the chromosomes
	void GaPopulationConfiguration::SetSortComparator(const GaFitnessComparator* comparator)
	{
		if( _sortingComparator != comparator )
		{
			_sortingComparator = comparator;

			for( list<GaPopulation*>::iterator it = _populations.begin(); it != _populations.end(); ++it )
			{
				(*it)->SetSortComparator( _sortingComparator );
				(*it)->ResortPopulation( false, false, true );
			}
		}
	}

	// Prepares population for initialization
	GaPopulation::GaPopulation(GaChromosome* prototype,
		GaPopulationConfiguration* configuration) : _prototype(prototype),
		_configuration(configuration),
		_parameters(configuration->GetParameters()),
		_best( this, GASGT_BEST ),
		_worst( this, GASGT_WORST ),
		_statistics(configuration->GetSortComparator()),
		_currentSize(0)
	{
		_configuration->BindPopulation( this, false );

		_usingScaledFitness = _parameters.GetUsingScaledFitness();
		int populationSize = _parameters.GetPopulationSize();

		// initialize table of population's chromorsome table
		_chromosomes = new GaScaledChromosome*[ populationSize ];
		for( int i = 0; i < populationSize; i++ )
			_chromosomes[ i ] = NULL;

		// empty population
		_currentSize = 0;

		// set sizes for sorted groups of best and worst chromosomes
		if( !_parameters.GetSorting() )
		{
			_best.SetMaxSize( _parameters.GetBestTrackCount() );
			_worst.SetMaxSize( _parameters.GetWorstTrackCount() );
		}

		// set comparators for sorted groups of best and worst chromosomes
		_best.SetComparator( _configuration->GetSortComparator() );
		_worst.SetComparator( _configuration->GetSortComparator() );
	}

	// Free resources used by the population
	GaPopulation::~GaPopulation()
	{
		if( _chromosomes )
		{
			// free memory used by chromosomes
			for( int i = 0; i < _currentSize; i++ )
			{
				if( _chromosomes[ i ] )
					delete _chromosomes[ i ];
			}

			// free memory used by population's chromorsome table
			delete[] _chromosomes;
		}
	}

	// Virtual copy constructor
	GaPopulation* GaPopulation::Clone(bool copyChromosomes) const
	{
		// create new population with the same parameters
		GaPopulation* newPopulation = new GaPopulation( _prototype, _configuration );

		if( copyChromosomes )
		{
			// copy chromosomes
			for( int i = 0; i < _currentSize; i++ )
				newPopulation->_chromosomes[ i ] = new GaScaledChromosome( *_chromosomes[ i ] );

			// set size of new population
			newPopulation->_currentSize = _currentSize;

			// copy best and worst chromosome groups to new population if needed
			if( _parameters.GetSorting() )
			{
				_best.CopyTo( newPopulation->_best, true );
				_worst.CopyTo( newPopulation->_worst, true );
			}

			// copy statistics
			newPopulation->_statistics = _statistics;
		}

		return newPopulation;
	}

	// Initialization of the population, it creates random chromosomes based on provided chromosome prototype.
	void GaPopulation::InitializePopulation(bool fill /* = true */)
	{
		if( !_parameters.GetResizable() || fill )
		{
			// verify population parameters
			if( !_chromosomes || !_prototype )
				return;

			// clear best and worst chromosome tracking
			_best.Clear();
			_worst.Clear();

			// fill the population
			for( int i = 0; i < _parameters.GetPopulationSize(); i++ )
			{
				// make new chromosome from prototype
				GaChromosomePtr ptr = _prototype->MakeNewFromPrototype();
				ptr->RefreshFitness();
				float scaled = ptr->GetFitness();

				// remove old chromosome if any
				if( _chromosomes[ i ] )
					delete _chromosomes[ i ];

				// add new chromosome to the population
				_chromosomes[ i ] = new GaScaledChromosome( ptr, this, i );
			}

			// initialize statistics
			_statistics.Clear();

			_currentSize = _parameters.GetPopulationSize();

			// sort population if needed
			ResortPopulation( true, true, true );
		}
		else
		{
			// clear best and worst chromosome tracking
			_best.Clear();
			_worst.Clear();

			// fill the population
			for( int i = 0; i < _currentSize; i++ )
			{
				// remove old chromosome if any
				if( _chromosomes[ i ] )
					delete _chromosomes[ i ];
			}

			_currentSize = 0;

			// initialize statistics
			_statistics.Clear();
		}
	}

	// Fills the given array with indices of the best chromosomes in the population
	// Returns number of saved chromosomes
	int GaPopulation::GetBestChromosomes(int* results,
		int start,
		int numberOfChromosomes) const
	{
		// population initialized?
		if( !results || numberOfChromosomes <= 0 || !_chromosomes )
			return 0;

		if( _parameters.GetSorting() )
		{
			if( start >= _currentSize )
				return 0;

			// population is sorted - return first N indecies

			int end = start + numberOfChromosomes;
			int lim = _currentSize < end ? _currentSize : end;
			int i = 0;
			for( int j = start; j < lim; i++, j++ )
				results[ i ] = j;

			return i;
		}
		else
		{
			if( start >= _best.GetCurrentSize() )
				return 0;

			// get best chromosomes indecies form sorted group

			int end = start + numberOfChromosomes;
			int lim = _best.GetCurrentSize() < numberOfChromosomes ? _best.GetCurrentSize() : numberOfChromosomes;
			int i = 0;
			for( int j = start; j < lim; i++, j++ )
				results[ i ] = _best.GetAt( j );

			return i;
		}
	}

	// Fills the given array with the best chromosomes in the population
	// Returns number of saved chromosomes
	int GaPopulation::GetBestChromosomes(GaChromosomePtr* results,
		int start,
		int numberOfChromosomes) const
	{
		// population initialized?
		if( !results || numberOfChromosomes <= 0 || !_chromosomes )
			return 0;

		if( _parameters.GetSorting() )
		{
			if( start >= _currentSize )
				return 0;

			// population is sorted - return first N chromosomes

			int end = start + numberOfChromosomes;
			int lim = _currentSize < end ? _currentSize : end;
			int i = 0;
			for( int j = start; j < lim; i++, j++ )
				results[ i ] = _chromosomes[ j ]->GetChromosome();

			return i;
		}
		else
		{
			if( start >= _best.GetCurrentSize() )
				return 0;

			// get best chromosomes form sorted group

			int end = start + numberOfChromosomes;
			int lim = _best.GetCurrentSize() < numberOfChromosomes ? _best.GetCurrentSize() : numberOfChromosomes;
			int i = 0;
			for( int j = start; j < lim; i++, j++ )
				results[ i ] = _chromosomes[ _best.GetAt( j ) ]->GetChromosome();

			return i;
		}
	}

	// Fills the given array with indices of the worst chromosomes in the population
	// Returns number of saved chromosomes
	int GaPopulation::GetWorsChromosomes(int* results,
		int start,
		int numberOfChromosomes) const
	{
		// population initialized?
		if( !results || numberOfChromosomes <= 0 || !_chromosomes )
			return 0;

		if( _parameters.GetSorting() )
		{
			if( start >= _currentSize )
				return 0;

			// population is sorted - return last N indecies
			int end = start + numberOfChromosomes;
			int n = _currentSize < end ? _currentSize : end;
			int lim = _currentSize - n;
			int i = 0;
			for( int j = _currentSize - start - 1; j >= lim; i++, j-- )
				results[ i ] = j;

			return i;
		}
		else
		{
			if( start >= _worst.GetCurrentSize() )
				return 0;

			// get worst chromosomes indecies form sorted group
			int end = start + numberOfChromosomes;
			int lim = _worst.GetCurrentSize() < end ? _worst.GetCurrentSize() : end;
			int i = 0;
			for( int j = 0; j < lim; i++, j++ )
				results[ i ] = _worst.GetAt( j );

			return lim;
		}
	}

	// Fills the given array with the worst chromosomes in the population
	// Returns number of saved chromosomes
	int GaPopulation::GetWorsChromosomes(GaChromosomePtr* results,
		int start,
		int numberOfChromosomes) const
	{
		// population initialized?
		if( !results || numberOfChromosomes <= 0 || !_chromosomes )
			return 0;

		if( _parameters.GetSorting() )
		{
			if( start >= _currentSize )
				return 0;

			// population is sorted - return last N chromosomes
			int end = start + numberOfChromosomes;
			int n = _currentSize < end ? _currentSize : end;
			int lim = _currentSize - n;
			int i = 0;
			for( int j = _currentSize - start - 1; j >= lim; i++, j-- )
				results[ i ] = _chromosomes[ j ]->GetChromosome();

			return i;
		}
		else
		{
			if( start >= _worst.GetCurrentSize() )
				return 0;

			// get worst chromosomes form sorted group
			int end = start + numberOfChromosomes;
			int lim = _worst.GetCurrentSize() < end ? _worst.GetCurrentSize() : end;
			int i = 0;
			for( int j = 0; j < lim; i++, j++ )
				results[ i ] = _chromosomes[ _worst.GetAt( j ) ]->GetChromosome();

			return lim;
		}
	}

	// Replaces chromosome at given position with new one
	// Returns 1 if chromosome is replaced, otherwise returns 0
	int GaPopulation::Replace(int index,
		GaChromosomePtr newChromosome)
	{
		// verify parameters
		if( !_chromosomes || index < 0 || index >= _currentSize || newChromosome.IsNULL() )
			return 0;

		// difference of old and new chromosome's fitness value
		float fitnessDiff = newChromosome->GetFitness() - _chromosomes[ index ]->GetChromosome()->GetFitness();
		float scaledFitnessDiff = 0;

		if( _parameters.GetSorting() )
		{
			// make scaled chromosome
			GaScaledChromosome* newScaled = new GaScaledChromosome( newChromosome, this, -1 );

			GaScaledChromosome* oldScalde = _chromosomes[ index ];

			// difference of old and new chromosome's scaled fitness value
			scaledFitnessDiff = newScaled->GetScaledFitness() - oldScalde->GetScaledFitness();

			int res = oldScalde->CompareFitnesses( *newScaled );

			// new chromsome should be inserted at same position as old one had
			if( res == 0 )
			{
				// replace chromosomes
				delete _chromosomes[ index ];
				_chromosomes[ index ] = newScaled;
				newScaled->SetIndex( index );
			}
			else 
			{
				int oldIndex = index;

				// new chromosome should be placed below old one (new has worse fitness)
				if( res > 0 )
				{
					for( ; _chromosomes[ index ]->CompareFitnesses( *newScaled ) > 0 && index < _currentSize - 1; index++ )
					{
						_chromosomes[ index ] = _chromosomes[ index + 1 ];
						_chromosomes[ index ]->SetIndex( index );
					}
				}
				// new chromosome should be placed above old one (new has better fitness)
				else
				{
					for( ; _chromosomes[ index ]->CompareFitnesses( *newScaled ) < 0 && index > 0; index-- )
					{
						_chromosomes[ index ] = _chromosomes[ index - 1 ];
						_chromosomes[ index ]->SetIndex( index );
					}
				}

				// replace chromosomes
				delete oldScalde;
				_chromosomes[ index ] = newScaled;
				_chromosomes[ index ]->SetIndex( index );
			}
		}
		else
		{
			GaSortedGroupType groups = _chromosomes[ index ]->GetGroups();
			float oldScaledFitness = _chromosomes[ index ]->GetScaledFitness();

			// remove chromsome from the groups if it was in any
			_best.Remove( index );
			_worst.Remove( index );

			// replace chromosomes
			_chromosomes[ index ]->SetChromosome( newChromosome );

			// old chromosome was in best chromosome group
			if( groups & GASGT_BEST )
			{
				// try adding new chrosmsome into worst chromosome group
				_worst.Add( index );

				// find chromosome which should be placed in the group insted old one
				RefreshBestGroup();
			}

			// old chromosome was in worst chromosome group
			if( groups & GASGT_WORST )
			{
				// try adding new chrosmsome into best chromosome group
				_best.Add( index );

				// find chromosome which should be placed in the group insted old one
				RefreshWorstGroup();
			}

			if( !( groups & ( GASGT_BEST | GASGT_WORST ) ) )
			{
				// insert new chromosome in best or worst chromsome group if needed
				_best.Add( index );
				_worst.Add( index );
			}

			// difference of old and new chromosome's scaled fitness value
			scaledFitnessDiff += _chromosomes[ index ]->GetScaledFitness() - oldScaledFitness;
		}

		// update statistics with new values of fitnesses
		UpdateStatistics( fitnessDiff, scaledFitnessDiff );

		return 1;
	}

	// Replaces group chromosomes with new ones
	// Returns number of replaced chromosomes
	int GaPopulation::ReplaceGroup(int* indices,
		GaChromosomePtr* newChromosomes,
		int numberOfChromosomes)
	{
		// verify parameters
		if( !indices || !newChromosomes || numberOfChromosomes <= 0 || !_chromosomes )
			return 0;

		// number of replaced chromosomes
		int replaced = 0;

		float totalFitnessDiff = 0;
		float totalScaledFitnessDiff = 0;

		if( _parameters.GetSorting() )
		{
			struct GaReplacementTabelEntry
			{
				int index;

				GaScaledChromosome* chromosome;

				// Inserts entry into replacement table
				static void Insert(GaReplacementTabelEntry* table, int size, GaScaledChromosome* chromosome, int index)
				{
					if( index == -1 )
					{
						// find place in replacement table for new chromosome (use fitness value)
						for( ; size > 0 && chromosome->CompareFitnesses( *table[ size - 1 ].chromosome ) > 0; size-- )
							table[ size ] = table[ size - 1 ];
					}
					else
					{
						// find place in replacement table for new chromosome (chromosome index)
						for( ; size > 0 && chromosome->GetIndex() < table[ size - 1 ].index; size-- )
							table[ size ] = table[ size - 1 ];
					}

					// insert into replacement table
					table[ size ].index = index;
					table[ size ].chromosome = chromosome;
				}

			}* replacementTable = new GaReplacementTabelEntry[ 2 * numberOfChromosomes ];

			// size of replacement table
			int replacementTableSize = 0;

			// fill the replacement table
			for( int j = 0; j < 2; j++ )
			{
				for( int i = 0; i < numberOfChromosomes; i++ )
				{
					int index = indices[ i ];

					// verify passed index and chromosome pointer
					if( index < 0 || index >= _currentSize || newChromosomes[ i ].IsNULL() )
						continue;

					// choromosome which is going to be inserted into table
					GaScaledChromosome* chromosome = !j ? _chromosomes[ index ] 
						: new GaScaledChromosome( newChromosomes[ i ], this, -1 );

					// insert chromsome infos into replacement table
					GaReplacementTabelEntry::Insert( replacementTable, replacementTableSize, chromosome, !j ? index : -1 );

					replacementTableSize++;
				}
			}

			if( replacementTableSize > 0 )
			{
				int previousPosition = 0;
				do
				{
					int currentPosition = previousPosition;

					// find minimal group in replacement tabel which can be replaced at once
					for( int sum = 0; currentPosition < replacementTableSize; currentPosition++ )
					{
						sum += replacementTable[ currentPosition ].index == -1 ? +1 : -1;

						// minimal group found
						if( !sum )
							break;
					}

					// direction of passing through population and replacement table
					int dir = replacementTable[ previousPosition ].index >= 0 ? +1 : -1;

					// the first and the last indices (replacment table) of the minimal replacement group
					int replacementIndex = dir == +1 ? previousPosition : currentPosition;
					int limit = dir == +1 ? currentPosition : previousPosition;

					// current index at which new chromosome should be inserted
					int insertionIndex = replacementTable[ replacementIndex ].index;

					// replace all chromosomes in the minimal replacement group
					for( int currentIndex = insertionIndex; dir * replacementIndex <= dir * limit; )
					{
						GaReplacementTabelEntry* entry = replacementTable + replacementIndex;
						GaScaledChromosome* chromosome = 
							currentIndex < _currentSize && currentIndex >= 0 ? _chromosomes[ currentIndex ] : NULL;

						bool move = true;

						// chromosome should be inserted
						if( entry->index < 0 )
						{
							// insert as much as possible chromosome at this point
							//while( dir * replacementIndex <= dir * limit && 
							if(	( !chromosome || dir * chromosome->CompareFitnesses( *entry->chromosome ) <= 0 ) )
							{
								// add to population
								_chromosomes[ insertionIndex ] = entry->chromosome;
								_chromosomes[ insertionIndex ]->SetIndex( insertionIndex );

								// update fitness difference
								totalFitnessDiff += entry->chromosome->GetChromosome()->GetFitness();
								totalScaledFitnessDiff += entry->chromosome->GetScaledFitness();

								// next replacement entry
								replacementIndex += dir;
								//entry += dir;

								// move inseratation point
								insertionIndex += dir;

								move = false;
							}
						}
						// chromosome should be remove?
						else if( currentIndex == entry->index )
						{
							// update fitness difference
							totalFitnessDiff -= chromosome->GetChromosome()->GetFitness();
							totalScaledFitnessDiff -= chromosome->GetScaledFitness();

							// free memory used by scaled chromosome
							delete chromosome;

							// next replacement entry
							replacementIndex += dir;

							// next existing chromosome
							currentIndex += dir;

							move = false;

							replaced++;
						}

						if( move )
						{
							// new chromosome should not be inserted now just move existing chromosome
							_chromosomes[ insertionIndex ] = chromosome;
							_chromosomes[ insertionIndex ]->SetIndex( insertionIndex );
							insertionIndex += dir;

							// next existing chromosome
							currentIndex += dir;
						}
					}

					// move to the begining of new minimal replacement group
					previousPosition = currentPosition + 1;
				} while( previousPosition < replacementTableSize );
			}
			delete[] replacementTable;
		}
		else
		{
			bool bestRemoved = false, worstRemoved = false;

			// replace the chromosomes
			for( int i = 0; i <  numberOfChromosomes; i++ )
			{
				int index = indices[ i ];

				// verify passed arguments for replacement
				if( index >= 0 && index < _currentSize && !newChromosomes[ i ].IsNULL() )
				{
					GaSortedGroupType groups = _chromosomes[ index ]->GetGroups();

					// set flags if the groups should be refreshed
					bestRemoved |= ( groups & GASGT_BEST ) != 0;
					worstRemoved |= ( groups & GASGT_WORST ) != 0;

					// remove from sorted group if it was in any
					_best.Remove( index );
					_worst.Remove( index );

					// calculate the difference of fitness values between two chromosomes
					totalFitnessDiff += newChromosomes[ i ]->GetFitness() - _chromosomes[ index ]->GetChromosome()->GetFitness();
					float oldScaledFitnessDiff = _chromosomes[ index ]->GetScaledFitness();

					// replace chromsome
					_chromosomes[ index ]->SetChromosome( newChromosomes[ i ] );

					// try adding new chromosome in opposite group from old one
					if( ( groups & ( GASGT_BEST | GASGT_WORST ) ) == GASGT_BEST )
						_worst.Add( index );
					if( ( groups & ( GASGT_WORST | GASGT_BEST ) ) == GASGT_WORST )
						_best.Add( index );

					// calculate the difference of scaled fitness values between two chromosomes
					totalScaledFitnessDiff += _chromosomes[ index ]->GetScaledFitness() - oldScaledFitnessDiff;

					replaced++;
				}
			}

			// refresh groups if needed
			if( bestRemoved )
				RefreshBestGroup();
			if( worstRemoved )
				RefreshWorstGroup();
		}

		// update statistics with new values of fitnesses
		UpdateStatistics( totalFitnessDiff, totalScaledFitnessDiff );

		return replaced;
	}

	// Inserts chromosome into population
	// Returns 1 if chromosome is inserted, otherwise returns 0
	int GaPopulation::Insert(GaChromosomePtr chromosome)
	{
		if( chromosome.IsNULL() )
			return 0;

		float fitnessDiff = 0;
		float scaledFitnessDiff = 0;

		if( _parameters.GetSorting() )
		{
			GaScaledChromosome* scaled = new GaScaledChromosome( chromosome, this, -1 );

			// find the postition for chromosome and insert it
			int i = _currentSize - 1;
			for( ; i >= 0 && scaled->CompareFitnesses( *_chromosomes[ i ] ) >= 0; i-- )
			{
				// when population is full worst chromosome is removed
				if( i + 1 == _parameters.GetPopulationSize() )
				{
					// calculate fitness difference
					fitnessDiff -= _chromosomes[ i ]->GetChromosome()->GetFitness();
					scaledFitnessDiff -= _chromosomes[ i ]->GetScaledFitness();

					// remove chromosome
					delete _chromosomes[ i ];
				}
				else
				{
					// move chromosome down
					_chromosomes[ i + 1 ] = _chromosomes[ i ];
					_chromosomes[ i + 1 ]->SetIndex( i + 1 );
				}
			}

			// popunalion is full and new chromosome cannot fit in it
			if( i + 1 == _parameters.GetPopulationSize() )
				// remove new chromsome
				delete scaled;
			else
			{
				// insert chromosome
				_chromosomes[ i + 1 ] = scaled;
				scaled->SetIndex( i + 1 );

				// calculate fitness difference
				fitnessDiff += chromosome->GetFitness();
				scaledFitnessDiff += scaled->GetScaledFitness();
			}

			// increase population size
			if( _currentSize < _parameters.GetPopulationSize() )
				_currentSize++;
		}
		else
		{
			// population still not full?
			if( _currentSize < _parameters.GetPopulationSize() )
			{
				GaScaledChromosome* scaled = new GaScaledChromosome( chromosome, this, -1 );

				// insert chromosome
				_chromosomes[ _currentSize ] = scaled;
				scaled->SetIndex( _currentSize );

				// calculate fitness difference
				fitnessDiff += chromosome->GetFitness();
				scaledFitnessDiff += scaled->GetScaledFitness();

				// increase population size
				_currentSize++;

				// refresh best and worst chromosome groups
				_best.Add( _currentSize - 1 );
				_worst.Add( _currentSize - 1 );
			}
		}

		// updates statistics
		UpdateStatistics( fitnessDiff, scaledFitnessDiff );

		return 1;
	}

	// Inserts a group of choromosomes in population
	// Returns number of inserted chromosomes
	int GaPopulation::InsertGroup(GaChromosomePtr* chromosomes,
		int numberOfChromosomes)
	{
		if( !numberOfChromosomes  || !chromosomes )
			return 0;

		int inserted = 0;

		float fitnessDiff = 0;
		float scaledFitnessDiff = 0;

		if( _parameters.GetSorting() )
		{
			if( _currentSize > 0 )
			{
				// sorted table of new chromosomes
				GaScaledChromosome** insertionTable = new GaScaledChromosome*[ numberOfChromosomes ];
				int insertionSize = 0;

				// sort new chromosomes and put them in insertion table
				for( int i = 0; i < numberOfChromosomes; i++ )
				{
					if( !chromosomes[ i ].IsNULL() )
					{
						// make scaled chromosome
						GaScaledChromosome* sc = new GaScaledChromosome( chromosomes[ i ], this, -1 );

						// find the position in table at which the chromosome should be inserted
						int j = insertionSize - 1;
						for( ; j >= 0 && sc->CompareFitnesses( *insertionTable[ j ] ) >= 0; j-- )
							insertionTable[ j + 1 ] = insertionTable[ j ];

						// add chromosome to tabel
						insertionTable[ j + 1 ] = sc;
						insertionSize++;
					}
				}

				if( insertionSize > 0 )
				{
					int populationPos = _currentSize - 1;
					// insert chromosomes from insertion table to population
					for( int i = populationPos + insertionSize; insertionSize > 0; i-- )
					{
						// selects the worst chromosome
						bool n = populationPos < 0 || _chromosomes[ populationPos ]->CompareFitnesses( *insertionTable[ insertionSize - 1 ] ) >= 0;

						GaScaledChromosome* sc = n ? insertionTable[ --insertionSize ] : _chromosomes[ populationPos-- ];

						// selected chromosome fits in population?
						if( i < _parameters.GetPopulationSize() )
						{
							// insert or move chromosome
							_chromosomes[ i ] = sc;
							sc->SetIndex( i );

							// new chromosomes has been added
							if( n )
							{
								// increase population size
								_currentSize++;

								// update fitness values
								fitnessDiff += sc->GetChromosome()->GetFitness();
								scaledFitnessDiff += sc->GetScaledFitness();

								inserted++;
							}
						}
						// selected chromosome doesn't fit in population?
						else
						{
							// removing chromosome was in popultaion?
							if( !n )
							{
								_currentSize--;

								// update fitness values
								fitnessDiff -= sc->GetChromosome()->GetFitness();
								scaledFitnessDiff -= sc->GetScaledFitness();
							}

							// remove chromsome
							delete sc;
						}
					}
				}

				// delete temp insertion table
				delete[] insertionTable;
			}
			else
			{
				// insert new chromosomes into population in sorted order
				for( int i = 0; i < numberOfChromosomes; i++ )
				{
					if( !chromosomes[ i ].IsNULL() )
					{
						// make scaled chromosome
						GaScaledChromosome* sc = new GaScaledChromosome( chromosomes[ i ], this, -1 );

						// find the position at which the chromosome should be inserted
						int j = _currentSize - 1;
						for( ; j >= 0 && sc->CompareFitnesses( *_chromosomes[ j ] ) >= 0; j-- )
						{
							// remove last chromosome if the population is full
							if( j + 1 == _parameters.GetPopulationSize() )
							{
								GaScaledChromosome* m = _chromosomes[ j + 1 ];

								// update fitness values
								fitnessDiff -= m->GetChromosome()->GetFitness();
								scaledFitnessDiff -= m->GetScaledFitness();

								delete m;
							}

							// insert new chromosome
							_chromosomes[ j + 1 ] = _chromosomes[ j ];

							inserted++;
						}

						// new chromosome fit into population
						if( j + 1 == _parameters.GetPopulationSize() )
							// remove new chromosome which cannot fit
							delete sc;
						else
						{
							// add chromosome to table
							_chromosomes[ j + 1 ] = sc;
							_currentSize++;

							// update fitness values
							fitnessDiff += sc->GetChromosome()->GetFitness();
							scaledFitnessDiff += sc->GetScaledFitness();
						}
					}
				}
			}
		}
		else
		{
			// insert new chromosomes
			for( int i = 0; i < numberOfChromosomes; i++ )
			{
				// stop if population is full
				if( _currentSize >= _parameters.GetPopulationSize() )
					break;

				if( !chromosomes[ i ].IsNULL() )
				{
					// add new chromosome
					_chromosomes[ _currentSize ] = new GaScaledChromosome( chromosomes[ i ], this, _currentSize );
					_currentSize++;

					// refresh best and worst chromosome groups
					_best.Add( _currentSize - 1 );
					_worst.Add( _currentSize - 1 );

					inserted++;
				}
			}
		}

		// updates statistics
		UpdateStatistics( fitnessDiff, scaledFitnessDiff );

		return inserted;
	}

	// Removes chromosome at given position
	// Returns 1 if chromosome is removed, otherwise returns 0
	int GaPopulation::Remove(int chromosome)
	{
		// check parameters
		if( chromosome < 0 || !_parameters.GetResizable() || chromosome >= _currentSize )
			return 0;

		// chromosome which is going to be removed
		GaScaledChromosome* old = _chromosomes[ chromosome ];

		// fintes values differences
		float fitnessDiff = -old->GetChromosome()->GetFitness();
		float scaledFitnessDiff = -old->GetScaledFitness();

		// move chromosomes
		for( int i = chromosome; i < _currentSize; i++ )
		{
			_chromosomes[ i ] = _chromosomes[ i + 1 ];
			_chromosomes[ i ]->SetIndex( i );
		}

		if( !_parameters.GetSorting() )
		{
			// update best and worst chromosome groups

			_best.Clear();
			_worst.Clear();

			_currentSize--;

			RefreshBestGroup();
			RefreshWorstGroup();
		}
		else
			_currentSize--;

		// remove chromosome
		delete old;

		// update statistics
		UpdateStatistics( fitnessDiff, scaledFitnessDiff );

		return 1;
	}

	// Removes chromosomes at given positions
	// Returns number of removed chromosomes
	int GaPopulation::RemoveGroup(int* chromosomes,
		int numberOfChromosomes)
	{
		if( !chromosomes || numberOfChromosomes < 0 || !_parameters.GetResizable() )
			return 0;

		int removed = 0;

		float fitnessDiff = 0;
		float scaledFitnessDiff = 0;

		// sort remove table in descending by chromosome's index
		int* removeTable = new int[ numberOfChromosomes ];
		int removeTableSize = 0;
		for( int i = 0; i < numberOfChromosomes; i++ )
		{
			int c = chromosomes[ i ];

			// valid index?
			if( c >= 0 && c < _currentSize )
			{
				// find position to insert
				int j = removeTableSize - 1;
				for( ; j >= 0 && c < removeTable[ j ]; j-- )
					removeTable[ j + 1 ] = removeTable[ j ];

				// insert
				removeTable[ j + 1 ] = c;
				removeTableSize++;
			}
		}

		// clears best and worst chromosome groups
		if( !_parameters.GetSorting() )
		{
			_best.Clear();
			_worst.Clear();
		}

		// remove chromosomes
		for( int i = 0, pos = 0; removeTableSize > 0; i++ )
		{
			// current chromosome should be removed
			if( i == removeTable[ removeTableSize - 1 ] )
			{
				// fintes values differences
				fitnessDiff -= _chromosomes[ i ]->GetChromosome()->GetFitness();
				scaledFitnessDiff -= _chromosomes[ i ]->GetScaledFitness();

				// remove chromosome
				delete _chromosomes[ i ];

				// next chromosome index for removing
				removeTableSize--;
				_currentSize--;

				removed++;
			}
			else 
			{
				// needs moving of current chromosome
				if( pos != i )
				{
					_chromosomes[ pos ] = _chromosomes[ i ];
					_chromosomes[ pos ]->SetIndex( pos );
				}

				// try inserting current chromosome into sorted groups
				if( !_parameters.GetSorting() )
				{
					_best.Add( pos );
					_worst.Add( pos );
				}

				// next position
				pos++;
			}
		}

		// update statistics
		UpdateStatistics( fitnessDiff, scaledFitnessDiff );

		delete[] removeTable;

		return removed;
	}

	// Removes all chromosomes from population and clears statistics
	void GaPopulation::Clear(bool clearStatistics)
	{
		if( clearStatistics )
			_statistics.Clear();

		// remove all chromosomes
		if( _chromosomes )
		{
			for( int i = 0; i < _currentSize; i++ )
			{
				if( _chromosomes[ i ] )
					delete _chromosomes[ i ];

				_chromosomes[ i ] = 0;
			}
		}

		if( _parameters.GetResizable() )
			_currentSize = 0;
	}

	// Returns ranking of chromosome.
	// If chromosome has no ranking returns -1.
	int GaPopulation::GetChromosomeRanking(int chromosomeIndex,
									   bool reverse/* = false*/)
	{
		// check passed parameters
		if( chromosomeIndex < 0 || chromosomeIndex > _currentSize )
			return -1;

		if( _parameters.GetSorting() )
			// population is sorted, chromosome index is its rank
			return reverse ? _currentSize - chromosomeIndex - 1 : chromosomeIndex;

		// ranking of chromosome in best group
		int rank = _best.GetRanking( chromosomeIndex );
		if( rank >= 0 )
			return reverse ? _currentSize - rank : rank;

		// ranking of chromosome in worst group
		rank = _worst.GetRanking( chromosomeIndex );
		if( rank >= 0 )
			return reverse ? rank : _currentSize - rank;

		return -1;
	}

	// Sets parameter of the population
	void GaPopulation::SetParameters(const GaPopulationParameters& parameters)
	{
		bool refreshBest = false, refreshWorst = false, resort = true;

		int oldSize = _parameters.GetPopulationSize();
		int newSize = parameters.GetPopulationSize();
		
		int lim = _currentSize < newSize ? _currentSize : newSize;

		// size changed?
		if( oldSize != newSize )
		{
			// allocate memory for new array of chromosomes
			GaScaledChromosome** n = new GaScaledChromosome*[ newSize ];

			// copy existing chromosomes to new array
			for( int i = 0; i < lim; i++ )
				n[ i ] = _chromosomes[ i ];

			// remove some chromosomes if new population size is smaller then old
			for( int i = lim; i < _currentSize; i++ )
			{
				// remove chromosome from sorted groups if it was in any
				if( !parameters.GetSorting() )
				{
					refreshBest |= _best.Remove( i );
					refreshWorst |= _worst.Remove( i );
				}

				delete _chromosomes[ i ];
			}

			// should the population be filled
			if( !parameters.GetResizable() )
			{
				// add new chromosome if new population size is bigger then old
				for( int i = lim; i < newSize; i++ )
				{
					// because the new chromosomes is added the groups should be refreshed or the population resorted 
					refreshBest = refreshWorst = resort = true;

					// make new chromosome from prototype
					n[ i ] = new GaScaledChromosome( _prototype->MakeNewFromPrototype(), this, i );
				}
			}

			// swap arrays of chromosomes
			delete[] _chromosomes;
			_chromosomes = n;
		}

		_usingScaledFitness = parameters.GetUsingScaledFitness();

		if( !parameters.GetSorting() )
		{
			// the groups should be refreshed if new size is bigger
			refreshBest |= parameters.GetBestTrackCount() > _parameters.GetBestTrackCount();
			refreshWorst |= parameters.GetWorstTrackCount() > _parameters.GetWorstTrackCount();

			// set new sizes for sorted groups
			_best.SetMaxSize( parameters.GetBestTrackCount() );
			_worst.SetMaxSize( parameters.GetWorstTrackCount() );

			// save new parameters
			_parameters = parameters;

			// refresh sorted group if needed
			if( refreshBest )
				RefreshBestGroup();
			if( refreshWorst )
				RefreshWorstGroup();
		}
		else
		{
			// sorted groups are disabled
			_best.SetMaxSize( 0 );
			_worst.SetMaxSize( 0 );

			resort |= parameters.GetSorting() != _parameters.GetSorting();
			resort |= parameters.GetUsingScaledFitness() != _parameters.GetUsingScaledFitness();

			// save new parameters
			_parameters = parameters;

			// should the population be resorted?
			if( resort )
				ResortPopulation( false, false, true );
		}
	}

	// Sets comparator used for sorting the chromosomes
	void GaPopulation::SetSortComparator(const GaFitnessComparator* comparator)
	{
		_best.Clear();
		_worst.Clear();

		_best.SetComparator( _configuration->GetSortComparator() );
		_worst.SetComparator( _configuration->GetSortComparator() );
		_statistics.SetFitnessComparator( comparator );
	}

	// Updates statistics for population after replacing chromosome(s)
	void GaPopulation::UpdateStatistics(float fitnessChange,
									float scaledFitnessChange)
	{
		// updates size of population
		_statistics.ChangeValue( GSV_POPULATION_SIZE, (float)_currentSize, false );

		// updates total and avarge fitnesses
		_statistics.ChangeValue( GSV_TOTAL_FITNESS, fitnessChange, true );

		// updates scaled fitnes only if needed
		if( _configuration->Scaling().HasOperation() && _configuration->Scaling().GetOperation().IsRankingBased() )
			_statistics.ChangeValue( GSV_TOTAL_FITNESS_SCALED, scaledFitnessChange, true );

		// updates value of the best chromsome's fitness
		int best = 0;
		if( GetBestChromosomes( &best, 0, 1 ) )
		{
			_statistics.ChangeValue( GSV_BEST_FITNESS, _chromosomes[ best ]->GetChromosome()->GetFitness(), false );
			_statistics.ChangeValue( GSV_BEST_FITNESS_SCALED, _chromosomes[ best ]->GetScaledFitness(), false );
		}

		// updates value of the worst chromsome's fitness
		int worst = 0;
		if( GetWorsChromosomes( &worst, 0, 1 ) )
		{
			_statistics.ChangeValue( GSV_WORST_FITNESS, _chromosomes[ worst ]->GetChromosome()->GetFitness(), false );
			_statistics.ChangeValue( GSV_WORST_FITNESS_SCALED, _chromosomes[ worst ]->GetScaledFitness(), false );
		}
	}

	// Should be called when scaling operation is changed
	void GaPopulation::RescaleAll()
	{
		float totalScaled = 0;
		for( int i = 0; i < _currentSize; i++ )
		{
			_chromosomes[ i ]->Rescale();
			totalScaled += _chromosomes[ i ]->GetScaledFitness();
		}

		_statistics.ChangeValue( GSV_TOTAL_FITNESS_SCALED, totalScaled, false );
	}

	// Sorting of the population
	void GaPopulation::ResortPopulation(bool refreshFitnesses,
		bool scalingChanged,
		bool comparatorChanged)
	{
		float totalFittness = 0;

		if( refreshFitnesses )
		{
			// fitness evaluation is changed, calculate total fitness difference
			for( int i = 0; i < _currentSize; i++ )
			{
				_chromosomes[ i ]->GetChromosome()->RefreshFitness();
				totalFittness += _chromosomes[ i ]->GetChromosome()->GetFitness();
			}
			
			// updates statistics
			_statistics.ChangeValue( GSV_TOTAL_FITNESS, totalFittness, false );
			UpdateStatistics( 0, 0 );
		}

		if( refreshFitnesses || comparatorChanged )
		{
			// first step sorting is done using the unscaled fitnes values
			bool oldFlag = _usingScaledFitness;
			_usingScaledFitness = false;

			if( _parameters.GetSorting() )
				// resort population
				qsort( _chromosomes, _currentSize, sizeof( _chromosomes[ 0 ] ), GaPopulation::SortComparison );
			else
			{
				// no sorting, refresh best and worst chromosome groups

				_best.Clear();
				_worst.Clear();

				RefreshBestGroup();
				RefreshWorstGroup();
			}

			_usingScaledFitness = oldFlag;
		}

		// resacling of all chromosomes
		bool rescale = scalingChanged || ( ( refreshFitnesses || comparatorChanged ) && _configuration->Scaling().HasOperation() );

		// second step sorting (if needed) is done using scaled fitness values
		if( _usingScaledFitness )
		{
			// rescale of all chromosomes if needed
			if( rescale )
				RescaleAll();

			if( _parameters.GetSorting() )
				// resort population
				qsort( _chromosomes, _currentSize, sizeof( _chromosomes[ 0 ] ), GaPopulation::SortComparison );
			else
			{
				// no sorting, refresh best and worst chromosome groups

				_best.Clear();
				_worst.Clear();

				RefreshBestGroup();
				RefreshWorstGroup();
			}
		}

		if( _parameters.GetSorting() )
		{
			// update ranking of scaled chromosomes
			for( int i = 0; i < _currentSize; i++ )
				_chromosomes[ i ]->SetIndex( i );
		}

		// update sacaled fitnes values if the scaling is ranking based
		if( rescale && !_usingScaledFitness )
			RescaleAll();

		// updates new statistics
		UpdateStatistics( 0, 0 );
		_statistics.ChangeValue( GSV_TOTAL_FITNESS, totalFittness, false );
	}

	// Comparison of chromosomes for sorting population
	int GaPopulation::SortComparison(const void* first,
										  const void* second)
	{
		return -( *(const GaScaledChromosome**)first )->CompareFitnesses( **(const GaScaledChromosome**)second );
	}

} // Population
