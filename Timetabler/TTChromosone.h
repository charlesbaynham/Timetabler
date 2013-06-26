//
//  TTChromosone.h
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#ifndef Timetabler_TTChromosone_h
#define Timetabler_TTChromosone_h

#include "Student.h"
#include <ext/hash_map>
using namespace __gnu_cxx;

#include "GLSource/Initialization.h"

#include "GLSource/MultiValueChromosome.h"
#include "GLSource/ChromosomeOperations.h"

#include "GLSource/Population.h"
#include "GLSource/SelectionOperations.h"
//#include "GLSource/ReplacementOperations.h"

//#include "GLSource/StopCriterias.h"
//#include "GLSource/IncrementalAlgorithm.h"



class Chromosone : public GaMultiValueChromosome<list<Student*>> {
    
    hash_map<Student*, int> _lookup;
    hash_map<Student*, int> _backupLookup;
    
public:
    
    Chromosone(GaChromosomeDomainBlock<list<Student*> >* configBlock);
    
	Chromosone(const Chromosone& c, bool setupOnly);
    
	/*virtual ~Chromosone() { } // These must be added back in later(probably)
    
    virtual GaChromosomePtr MakeCopy(bool setupOnly) const { return new Chromosone( *this, setupOnly ); }
    
	virtual GaChromosomePtr MakeNewFromPrototype() const;
    
	virtual void PreapareForMutation();
    
	virtual void AcceptMutation();
    
	virtual void RejectMutation(); */
    
	// Returns reference to table of classes
	inline const hash_map<Student*, int>& GetStudentLookup() const { return _lookup; }
    
	// Return reference to array of time-space slots
	inline const vector<list<Student*> >& GetSlots() const { return _values; }
};


class TTFitness : public GaFitnessOperation
{
public:
    
	virtual float GACALL operator ()(const GaChromosome* chromosome) const;
    
	virtual GaParameters* GACALL MakeParameters() const { return NULL; }
    
	virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return true; }
};


class TTMutation : public GaMutationOperation
{
public:
    
	virtual void GACALL operator ()(GaChromosome* parent) const;
    
	virtual GaParameters* GACALL MakeParameters() const { return NULL; }
    
	virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return true; }

};


class TTCrossover : public GaCrossoverOperation
{
    
public:
    
	virtual GaChromosomePtr GACALL operator ()(const GaChromosome* parent1,
                                               const GaChromosome* parent2) const;
    
	virtual GaParameters* GACALL MakeParameters() const { return NULL; }
    
	virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return true; }
    
    
};

#endif
