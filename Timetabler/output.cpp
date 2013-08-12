//
//  output.cpp
//  Timetabler
//
//  Created by Charles Baynham on 01/07/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "output.h"

outputCSV outputCSV::_instance;
outputSolution outputSolution::_instance;

using namespace Chromosome;

void outputSolution::operator() (string filename, const GaChromosome& chromo, bool append) {
    
    const Chromosone* c = dynamic_cast<const Chromosone*>(&chromo);
    
    ofstream out;
    if (append)
        out.open(filename, ios::out | ios::app);
    else
        out.open(filename, ios::out);
    
    out << "% The following is a solution previously found by the timetabler. \n"
    "% It can be used to attempt to alter previous timetables with minimum disruption\n\n";
    
    vector< list<Student*> > slots = c->GetSlots();
    
    int numSlots = (int)c->GetSlots().size();
    
    // This should match up with the calculated size by the config when it's read back in (numtutors * slots_in_day)
    out << "#size = " << numSlots << endl << endl;
    
    out << "% Slot = baseID\n\n";
    
    out << "#solution" << endl;
    // for each slot
    for (int i=0; i<numSlots; i++) {
        //for each student in that slot
        for (list<Student*>::iterator it = slots[i].begin(); it != slots[i].end(); it++) {
            // output the slot and baseID
            out << "\t" << i << " = " << (*it)->getBaseID() << endl;
        }
    }
    out << "#end" << endl;
    
    out.close();
}

// output the solution as a CSV (not the config)
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
    fprintf(f, "Slot,Student,Tutor\n");
    for (list<timetableSlot>::iterator it = slotList.begin(); it != slotList.end(); it++) {
        if (detailed)
            fprintf(f, "%i,\"%s (%s)\",\"%s (%s)\"\n", (*it).getTime(), (*it).getStudentName().c_str(), (*it).getStudentSubject().c_str(), (*it).getTutorName().c_str(), (*it).getTutorSubjects().c_str() );
        else
            fprintf(f, "%i,\"%s\",\"%s\"\n", (*it).getTime(), (*it).getStudentName().c_str(), (*it).getTutorName().c_str());
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
    map<Subject*, float> subjs = _tutor->getSubjects();
    for (map<Subject*, float>::iterator it=subjs.begin(); it != subjs.end(); it++) {
        out.append( (*it).first->getName() );
        out.append(", ");
    }
    //remove final ", "
    string::iterator strIt = out.end();
    if (out.length() >= 2) {
        strIt--;
        out.erase(strIt);
        strIt--;
        out.erase(strIt);
    }
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
_tutors(Configuration::getInstance().getTutors())
{
    // Store list of students by baseID: (n.b. don't use the ID of the students stored here as it it undefined)
    const list<Student*> students = Configuration::getInstance().getStudents();
    for (list<Student*>::const_iterator it = students.begin(); it != students.end(); it++) {
        _students[ (*it)->getBaseID() ] = (*it);
    }
    
    // Create timetables
    _chromo = dynamic_cast<const Chromosone*>(chromo);
    _studentTT = new studentTT(_chromo);
    _tutorTT = new tutorTT(_chromo);
}


tutorTT::tutorTT(const Chromosone* chromo)
{
    if ( !(chromo == NULL) ) {
        
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
            
            //            //debug
            //            for (hash_map<int, Student*>::iterator it=newTutor->_students.begin(); it!=newTutor->_students.end(); it++) {
            //
            //                printf("Student at time %i with %s is %s\n", (*it).first, newTutor->getTutorName().c_str(), (*it).second->getName().c_str());
            //
            //            }
            //            //debug end
            
            // Add the new tutor to the list
            _tutors.push_back(newTutor);
            
        }
    }
}

studentTT::studentTT(const Chromosone* chromo)
{
    if ( !(chromo == NULL) ) {
        
        hash_map<Student*, int> lookup = chromo->GetStudentLookup();
        
        // map of baseID->timetable for all the students' timetables
        _students.clear();
        
        //For every appointment (not for every student, since most students appear more than once)
        for (hash_map<Student*, int>::iterator it = lookup.begin(); it != lookup.end(); it++) {
//            if the baseID of this student is not already present in _students then create it:
            int baseID = (*it).first->getBaseID();
            if ( _students.find( baseID ) == _students.end() )
            {
                studentTT_student* newStudent = new studentTT_student( (*it).first );
                _students[ baseID ] = newStudent;
            }
            
            // Then, add the appropriate tutor for this slot to the (possibly newly created) student
            // First, evaluate the slot
            div_t division = div((*it).second, SLOTS_IN_DAY);
            int tutorID = division.quot + 1;
            int time = division.rem;
            Tutor* tutor = Configuration::getInstance().getTutor(tutorID);
            
            // now, add to the map
            _students[baseID]->addTutor(time, tutor);
        }
    }
}

Student* tutorTT::getTutorApt(int tutorID, int time)
{
    for (tutorTTList::iterator it=_tutors.begin(); it != _tutors.end(); it++)
    {
        if ( (*it)->getTutorID() == tutorID ) {
            Student* student = (*it)->getStudent(time);
            return student;
        }
    }
    return NULL;
}


Tutor* studentTT::getStudentApt(int studentBaseID, int time)
{
    studentTT_student* studentTimetable;
    if ( (studentTimetable = _students[studentBaseID]) )    return studentTimetable->getTutor(time);
    
    return NULL;
}

