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
