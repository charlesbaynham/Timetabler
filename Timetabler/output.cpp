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
    
    list<timetableSlot> slotList;
    
    // Loop over all slots
    vector<list<Student*>> slots = c->GetSlots();
    int numSlots = (int)slots.size();
    for (int i = 0; i<numSlots; i++) {
        if ( !slots[i].empty() ) {
            //for each student in this slot (we hope only 1)
            for (list<Student*>::iterator it=slots[i].begin(); it != slots[i].end(); it++)
            {
            slotList.push_back( timetableSlot(i, *it) );
            }

        }
    }
    
    // Sort into time order
    slotList.sort();
    
    //output into CSV
    fprintf(f, "Time,\tStudent,\tTutor\n");
    for (list<timetableSlot>::iterator it = slotList.begin(); it != slotList.end(); it++) {
        fprintf(f, "%i,\t%s,\t%s\n", (*it).getTime(), (*it).getStudentName().c_str(), (*it).getTutorName().c_str());
    }
    
    fclose(f);
}


timetableSlot::timetableSlot(int slot, Student* student) :
    _slot(slot),
    _student(student)
{
    // Work out and get the tutor that this slot corresponds to
    div_t division = div(slot, SLOTS_IN_DAY);
    int tutorID = division.quot + 1;
    
    _time = division.rem;
    _tutor = Configuration::getInstance().getTutor(tutorID);
}


outputRaw outputRaw::_instance;
void outputRaw::operator ()(char * filename, const GaChromosome& chromo) {

    const Chromosone* c = dynamic_cast<const Chromosone*>(&chromo);
    
    hash_map<Student*, int> lookup = c->GetStudentLookup();

    vector< pair<Student*, int> > vec;
    
    for ( hash_map<Student*, int>::const_iterator it = lookup.begin(); it != lookup.end(); it++ ) {
        
        Student * stud;
        int id;
        
        stud = (*it).first;
        id = (*it).second;
        
        pair<Student*, int> Npair = *new pair<Student*, int>(stud, id);
        
        vec.push_back(Npair);
    }
    
    
    //sort the vector into ascending by ID order
    sort(vec.begin(), vec.end());
    
    FILE * f = fopen(filename, "w");
    
    fprintf(f, "Student:\tSlot\n");
    for (int i=0; i<vec.size(); i++){
        fprintf(f, "%s\t\t%i\n", vec[i].first->getName().c_str(), vec[i].second);
    }
    
    fclose(f);

}





