//
//  output.cpp
//  Timetabler
//
//  Created by Charles Baynham on 01/07/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "output.h"

outputCSV outputCSV::_instance;

using namespace Chromosome;

void outputCSV::operator()(char *filename, const GaChromosome& chromo) {
    
    const Chromosone* c = dynamic_cast<const Chromosone*>(&chromo);
    
    FILE * f = fopen(filename, "w");
    
    // Loop over all slots
    vector<list<Student*>> slots = c->GetSlots();
    int numSlots = (int)slots.size();
    for (int i = 0; i<numSlots; i++) {
        if ( !slots[i].empty() ) {
            // Work out and get the tutor that this slot corresponds to
            div_t division = div(i, SLOTS_IN_DAY);
            int tutorID = division.quot + 1;
            int time = division.rem;
            
            Tutor* tutor = Configuration::getInstance().getTutor(tutorID);
            
            //Print only first. Edit to work out what to do if more than one
            fprintf( f, "Time: %i\t\tTutor: %s\t\tName: %s\n", time, tutor->getName().c_str(), slots[i].front()->getName().c_str() );
        }
    }
    
    fclose(f);
}