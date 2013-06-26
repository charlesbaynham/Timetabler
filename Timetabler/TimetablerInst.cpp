//
//  TimetablerInst.cpp
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "TimetablerInst.h"

using namespace Population;
//using namespace Population::ReplacementOperations;
using namespace Population::SelectionOperations;

TimetablerInst::TimetablerInst()
{

    // initialize GAL internal structures
	GaInitialize();
	
	// make chromosome parameters
	// crossover probability: 80%
	// crossover points: 2
	// no "improving only mutations"
	// mutation probability: 3%
	// number of moved classes per mutation: 2
	_chromosomeParams = new GaChromosomeParams( 0.03F, 2, false, 0.8F, 2 );
    
	// make CCB with fallowing setup:
	// there are no value set
	// with ScheduleCrossover, ScheduleMutation, ScheduleFitness genetic operations
	// set fittnes comparator for maximizing fitness value
	// use previously defined chromosome's parameters
	_ccb = new GaChromosomeDomainBlock<list<Student*> >( NULL, 0, &_crossoverOperation, &_mutationOperation, &_fitnessOperation,
                                                            GaFitnessComparatorCatalogue::Instance().GetEntryData( "GaMaxFitnessComparator" ), _chromosomeParams );
    
	// make prototype of chromosome
	_prototype = new Chromosone( _ccb );
    
	// make population parameters
	// number of chromosomes in population: 100
	// population always has fixed number of chromosomes
	// population is not sorted
	// non-transformed(non-scaled) fitness values are used for sorting and tracking chromosomes
	// population tracks 5 best and 5 worst chromosomes
	GaPopulationParameters populationParams( 100, false, false, false, 5, 5 );
    
	// make parameters for selection operation
	// selection will choose 16 chromosomes
	// but only 8 best of them will be stored in selection result set
	// there will be no duplicates of chromosomes in result set
	GaSelectRandomBestParams selParam( 8, false, 16 );

}