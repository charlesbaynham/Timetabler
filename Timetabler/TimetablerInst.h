//
//  TimetablerInst.h
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#ifndef __Timetabler__TimetablerInst__
#define __Timetabler__TimetablerInst__

#include <iostream>

#include "TTChromosone.h"


class TimetablerInst {
    static TimetablerInst _instance;
    
    GaChromosomeParams* _chromosomeParams;
    
	TTCrossover _crossoverOperation;
    
	TTMutation _mutationOperation;

	TTFitness _fitnessOperation;
//
	GaChromosomeDomainBlock<list<Student*> >* _ccb;
    
	Chromosone* _prototype;
    
//	GaPopulationConfiguration* _populationConfig;
    
	GaPopulation* _population;
    
//	GaAlgorithm* _algorithm;
//    
//	ScheduleObserver _observer;
    
public:
    inline static TimetablerInst& getInstance() { return _instance; }
    
    inline static TimetablerInst& GetInstance() { return _instance; }
    
	TimetablerInst();
    
	~TimetablerInst();
    
//	inline GaAlgorithm* GetAlgorithm() { return _algorithm; }
//    
//	inline const GaAlgorithm* GetAlgorithm() const { return _algorithm; }
//    
//	inline ScheduleObserver& GetObserver() { return _observer; }
//    
//	inline const ScheduleObserver& GetObserver() const { return _observer; }

    
};

#endif /* defined(__Timetabler__TimetablerInst__) */
