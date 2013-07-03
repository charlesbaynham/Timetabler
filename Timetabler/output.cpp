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

void outputCSV::operator()(char *filename, const GaChromosome& chromo, bool detailed=false) {
    
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
        if (detailed)
            fprintf(f, "%i\t\t%s (%s)\t\t%s (%s)\n", (*it).getTime(), (*it).getStudentName().c_str(), (*it).getStudentSubject().c_str(), (*it).getTutorName().c_str(), (*it).getTutorSubjects().c_str() );
        else
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

string timetableSlot::getTutorSubjects()
{
    string out;
    
    //construct string with list of subjects
    list<Subject*> subjs = _tutor->getSubjects();
    for (list<Subject*>::iterator it=subjs.begin(); it != subjs.end(); it++) {
        out.append( (*it)->getName() );
        out.append(", ");
    }
    //remove final ", "
    string::iterator strIt = out.end();
    strIt--;
    out.erase(strIt);
    strIt--;
    out.erase(strIt);
    
    return out;
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

finishedTT::finishedTT(const GaChromosome* chromo) :
    _studentTT(NULL),_tutorTT(NULL)
{
    _chromo = dynamic_cast<const Chromosone*>(chromo);
    _studentTT = new studentTT(_chromo);
    _tutorTT = new tutorTT(_chromo);
}


tutorTT::tutorTT(const Chromosone* chromo)
{
    if ( !(chromo == NULL) ) {
        
        printf("Hello world\n");
        
        vector<list<Student*> > values = chromo->GetSlots();
        
        int numSlots = (int)values.size();

        //for each tutor:
        for (int tutorBaseSlot=0; tutorBaseSlot < numSlots; tutorBaseSlot+=SLOTS_IN_DAY)
        {
            int tutorID = div(tutorBaseSlot, SLOTS_IN_DAY).quot + 1;
            tutorTT_tutor* newTutor = new tutorTT_tutor( Configuration::getInstance().getTutor(tutorID) );
            
            //for each time of this tutor
            for (int slot=tutorBaseSlot; slot < (tutorBaseSlot+SLOTS_IN_DAY); slot++) {
                int time = div(slot, SLOTS_IN_DAY).rem;
                // We assume that by this point there's only one student per slot. If not, we ignore the second. Edit.
                Student* student = values[slot].front();
                
                // Add the student to this tutor's timetable:
                if ( !values[slot].empty() ) newTutor->addStudent( time, student );
            }
            
            //debug
            for (hash_map<int, Student*>::iterator it=newTutor->_students.begin(); it!=newTutor->_students.end(); it++) {
                
                printf("Student at time %i with %s is %s\n", (*it).first, newTutor->getTutorName().c_str(), (*it).second->getName().c_str());
                
            }
            //debug end
            
            // Add the new tutor to the list
            _tutors.push_back(newTutor);

        }
        printf("Tutors done\n");
    }
}

studentTT::studentTT(const Chromosone* chromo){
    
//    vector<list<Student*> > _values = chromo->GetSlots();
    
}


