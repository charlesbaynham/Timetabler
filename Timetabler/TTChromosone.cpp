//
//  TTChromosone.cpp
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "TTChromosone.h"
#define SLOTS_IN_DAY 4
#define NUM_TUTORS 3 //Get RID of this

Chromosone::Chromosone(GaChromosomeDomainBlock<list<Student*> >* configBlock) :
    GaMultiValueChromosome<list<Student*> >(configBlock)
{
    _values.resize( SLOTS_IN_DAY * NUM_TUTORS ); //Get RID of num_tutors
}

Chromosone::Chromosone(const Chromosone& c, bool setupOnly) :
    GaMultiValueChromosome<list<Student*> >(c, setupOnly)
{
    if (!setupOnly) { // Then copy the data
        _values = c._values;
        _lookup = c._lookup;
        _backupLookup = c._backupLookup;
    }
    else { // Reserve space
        _values.resize( SLOTS_IN_DAY * NUM_TUTORS ); //Get RID of num_tutors
    }
}

void TTMutation::operator ()(GaChromosome* parent) const
{
    printf("lol, I did something.\n"); //Again, not finished
    
}

float TTFitness::operator()(const GaChromosome* chromosome) const{
    return 10; // not finished edit.
}


GaChromosomePtr TTCrossover::operator ()(const GaChromosome* parent1, const GaChromosome* parent2) const
{
    const Chromosone* c1 = dynamic_cast<const Chromosone*>( parent1 );
    const Chromosone* c2 = dynamic_cast<const Chromosone*>( parent2 );
    
    Chromosone* n = new Chromosone(*c1, false);
    
    return n; //Obviously not finished.  EDIT
}



