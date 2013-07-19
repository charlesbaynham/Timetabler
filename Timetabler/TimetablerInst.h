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
#include <Wt/WText>

using namespace Wt;

#include "TTChromosone.h"
#include "output.h"


class TTObserver : public GaObserverAdapter {
    
    SysEventObject _event; // event for callback
    
    // function to call on completion
    function<void(GaChromosomePtr result) > _funcComplete = NULL;
    
public:
    TTObserver() { MakeEvent( _event, 0 ); }
    TTObserver(function<void(GaChromosomePtr result) > funcComplete) : _funcComplete(funcComplete) { TTObserver(); }
    
    inline void bindFunction( function<void(GaChromosomePtr result)> funcComplete ) { _funcComplete = funcComplete; }
    
	virtual ~TTObserver() { DeleteEvent( _event ); }
    
	void WaitEvent() { WaitForEvent( _event ); }
    
    virtual void NewBestChromosome(const GaChromosome& newChromosome, const GaAlgorithm& algorithm);
    
	virtual void EvolutionStateChanged(GaAlgorithmState newState, const GaAlgorithm& algorithm);

};

class TimetablerInst {
    
    static TimetablerInst* _instance; // Global instance
    
    // if we're using a previous solution, has it been added to the population yet?
    bool _bestAdded=false;
    
public:
    inline static TimetablerInst* getInstance() { return _instance; } // return global instance
    inline static void reset() {
        if(_instance)  {
            
            _instance->~TimetablerInst();
            _instance = new TimetablerInst();
            
        }
    }
    
    // Check if we've added the previous solution in yet
    // if not, return false and then set _bestAdded = true so that this method returns true henceforth
    inline bool bestAdded() {
        if (_bestAdded) return true;
        // else
        _bestAdded = true;
        return false;
    }
    
private:

    TTChromosomeParams* _chromosomeParams;
    
	TTCrossover _crossoverOperation;
    
	TTMutation _mutationOperation;

	TTFitness _fitnessOperation;

	GaChromosomeDomainBlock<list<Student*> >* _ccb;
    
	Chromosone* _prototype;
    
	GaPopulationConfiguration* _populationConfig;
    
	GaPopulation* _population;
    
	GaAlgorithm* _algorithm;
    
	TTObserver _observer;
    
public:

    
	TimetablerInst();
    
	~TimetablerInst();
    
#ifdef DEBUG
    inline Chromosone* getPrototype() { return _prototype; }
#endif
    
	inline GaAlgorithm* getAlgorithm() { return _algorithm; }
    
	inline const GaAlgorithm* getAlgorithm() const { return _algorithm; }
    
	inline TTObserver& getObserver() { return _observer; }
    
	inline const TTObserver& getObserver() const { return _observer; }
    
    // convenience, to register callback function for completion
    inline void registerObserverFunc(function<void(GaChromosomePtr result)> funcComplete) { _observer.bindFunction(funcComplete); }
    
    //debug edit
//    inline Chromosone* getPrototype() const { return _prototype; }
    
};


#endif /* defined(__Timetabler__TimetablerInst__) */
