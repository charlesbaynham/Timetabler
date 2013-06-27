//
//  TimetablerInst.cpp
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "TimetablerInst.h"

using namespace Population;
using namespace Population::ReplacementOperations;
using namespace Population::SelectionOperations;
using namespace Algorithm::SimpleAlgorithms;
using namespace Algorithm::StopCriterias;

void TTObserver::NewBestChromosome(const GaChromosome& newChromosome, const GaAlgorithm& algorithm) {
    printf("New best! Has fitness of %f\n", newChromosome.GetFitness());
}

void TTObserver::EvolutionStateChanged(GaAlgorithmState newState, const GaAlgorithm& algorithm) {
    cout << "New state is " << newState << endl;
    if (newState == GAS_CRITERIA_STOPPED) // we found a solution, so get the best chromosome:
    {
        GaChromosomePtr result;
        algorithm.GetPopulation(0).GetBestChromosomes(&result, 0, 1); // store best chromosone in result
        cout << "Fitness of final solution is " << result->GetFitness() << endl;
    }
}
// To be written. Edit

TimetablerInst TimetablerInst::_instance;

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
	_ccb = new GaChromosomeDomainBlock<list<Student*> >( NULL, &_crossoverOperation, &_mutationOperation, &_fitnessOperation, GaFitnessComparatorCatalogue::Instance().GetEntryData( "GaMaxFitnessComparator" ), _chromosomeParams );
    
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
    
    
	// make parameters for replacement operation
	// replace 8 chromosomes
	// but keep 2 best chromosomes in population
	GaReplaceElitismParams repParam( 8, 2 );
    
	// make parameters for coupling operation
	// coupling operation will produce 8 new chromosomes from selected parents
	GaCouplingParams coupParam( 8, false );
    
	// make population configuration
	// use defined population parameters
	// use same comparator for sorting as comparator used by chromosomes
	// use selection operation which randomly selects chromosomes
	// use replacement operation which randomly chooses chromosomes from population
	// which are going to be replaced, but keeps best chromosomes
	// use simple coupling
	// disable scaling
	_populationConfig = new GaPopulationConfiguration ( populationParams, &_ccb->GetFitnessComparator(),
                                                       GaSelectionCatalogue::Instance().GetEntryData( "GaSelectRandom" ), &selParam,
                                                       GaReplacementCatalogue::Instance().GetEntryData( "GaReplaceRandom" ), &repParam,
                                                       GaCouplingCatalogue::Instance().GetEntryData( "GaSimpleCoupling" ), &coupParam,
                                                       NULL, NULL );
    
	// make population
	// with previously defined prototype of chromosomes and population configuration
	_population = new GaPopulation( _prototype, _populationConfig );
    
	// make parameters for genetic algorithms
	// algorithm will use two workers
	GaMultithreadingAlgorithmParams algorithmParams( 2 );
	// make incremental algorithm with periously defined population and parameters
	_algorithm = new GaIncrementalAlgorithm( _population, algorithmParams );
    
	// make parameters for stop criteria based on fitness value
	// stop when best chromosome reaches fitness value of 1
	GaFitnessCriteriaParams criteriaParams( 0, GFC_MORE_THEN_EQUALS_TO, GSV_BEST_FITNESS );
    // This line needs changing
    
	// sets algorithm's stop criteria (base on fitness value) and its parameters
	_algorithm->SetStopCriteria( GaStopCriteriaCatalogue::Instance().GetEntryData( "GaFitnessCriteria" ), 
                                &criteriaParams );
    
	// subscribe observer
	_algorithm->SubscribeObserver( &_observer );
}


TimetablerInst::~TimetablerInst()
{
	delete _algorithm;
    
	delete _population;
	delete _populationConfig;
    
	delete _prototype;
	delete _ccb;
	delete _chromosomeParams;
    
	// Free resources used by GAL
	GaFinalize();
}