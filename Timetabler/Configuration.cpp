//
//  Configuration.cpp
//  Timetabler
//
//  Created by Charles Baynham on 27/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "Configuration.h"

#include <iostream>
#include <stdlib.h>
#include <ext/hash_map>
#include <unordered_set>

Configuration Configuration::_instance;

int Configuration::parseFile(const char* fileName) {
    
    //clear all
    _tutors.clear();
    _subjects.clear();
    _students.clear();
    
    //open file
    ifstream input(fileName);
    
    if(input.fail()) return 1; // Error opening file
    
    string line;
    while (input.is_open() && !input.eof()) {
        // get lines until start of new object is not found
		getline( input, line );
		TrimString( line );
        
		// get type of object, parse obect and store it
        if( line.compare("#subject") == 0 )
		{
			Subject* s = ParseSubject( input );
			if( s )
				this->addSubject(s);
		}
		else if( line.compare("#tutor") == 0 )
		{
			Tutor* t = ParseTutor( input );
			if( t )
				this->addTutor(t);
		}
		else if( line.compare("#student") == 0 )
		{
			Student* s = ParseStudent( input );
			if( s )
            {
				// Push student to the list
                this->addStudent(s);
                
                // create duplicates for however many interviews each student will have (with different IDs, but same baseID)
                for (int i = 1; i < s->getNoInterviews(); i++) {
                    Student* next = new Student(s);
                    this->addStudent(next);
                }
            }
		}
    }
    
    input.close();
    
    _isEmpty = false;
    
    processNotSlots(); // Work out the _notSlots element for each tutor
    
    cout << "Input from file: "<<numStudents()<<" students, "<<numTutors()<<" tutors and "<<numSubjects()<<" subjects.\n";

    return 0;
}

// Get configuration directly from a function call (called by the GUI)
void Configuration::setup( hash_map<int, Tutor*> tutors, hash_map<int, Subject*> subjects, list<Student*> students )
{
    _tutors = tutors; _subjects=subjects; _students=students;
    
    processNotSlots(); // Work out the _notSlots element for each tutor and student
    
    _isEmpty = false;
    
    cout << "Input from GUI: "<<numStudents()<<" students, "<<numTutors()<<" tutors and "<<numSubjects()<<" subjects.\n";
}


// Reads subject's data from config file, makes object and returns pointer to it
// Returns NULL if method cannot parse configuration data
Subject* Configuration::ParseSubject(ifstream& file) {
    
    int id;
    string name;
    
    while (!file.eof()){
        string key, value;
        
        if ( !GetConfigBlockLine(file, key, value)) break;
        
        if (key.compare("id") == 0) id = atoi(value.c_str());
        else if (key.compare("name") == 0) name = value;
    }
    
    return id == 0 ? NULL : new Subject(id,name);
}

Tutor* Configuration::ParseTutor(ifstream& file) {
    
    int id;
    string name;
    list<Subject*> subjects;
    list<int> notTimes;
    
    while (!file.eof()){
        string key, value;
        
        if ( !GetConfigBlockLine(file, key, value)) break;
        
        if (key.compare("id") == 0) id = atoi(value.c_str());
        else if (key.compare("name") == 0) name = value;
        else if (key.compare("subj") == 0) subjects.push_back(getSubject(atoi(value.c_str())));
        else if (key.compare("notTime") == 0) notTimes.push_back(atoi(value.c_str()));
    }
    
    return (id==0) ? NULL : new Tutor(id, name, subjects, notTimes);
}

// Reads student's data from config file, makes object and returns pointer to it
// Returns NULL if method cannot parse configuration data
Student* Configuration::ParseStudent(ifstream& file) {
    
    int baseID=0;
    string name;
    Subject* subject; // Interview subject
    int noInterviews;  // Number of interviews (2 or 4)
    list<Tutor*> prevTutors; // Previous tutors to be avoided
    list<int> notTimes; // times to be avoided
    
    while (!file.eof()){
        string key, value;
        
        if ( !GetConfigBlockLine(file, key, value)) break;
        
        if (key.compare("baseID") == 0) baseID = atoi(value.c_str());
        else if (key.compare("name") == 0) name = value;
        else if (key.compare("noInterviews") == 0) noInterviews = atoi(value.c_str());
        else if (key.compare("subj") == 0) subject = getSubject(atoi(value.c_str()));
        else if (key.compare("prevTutor") == 0) prevTutors.push_back(getTutor(atoi(value.c_str())));
        else if (key.compare("notTime") == 0) notTimes.push_back(atoi(value.c_str()));
    }
    
    return !baseID ? NULL : new Student(baseID, name, subject, noInterviews, prevTutors, notTimes);
}

// Reads one line (key - value pair) from configuration file
bool Configuration::GetConfigBlockLine(ifstream& file, string& key, string& value)
{
	string line;
    
	// end of file
	while( !file.eof() )
	{
		// read line from config file
		getline( file, line );
		TrimString( line );
        
		// end of object's data
		if( line.compare( "#end" ) == 0 )
			return false;
        if( line.front() == '%' )
        {
            key = "comment";
            return true;
        }
        
		size_t p = line.find( '=' );
		if( p != string::npos )
		{
			// key
			key = line.substr( 0, p );
			TrimString( key );
            
			// value
			value = line.substr( p + 1, line.length() );
			TrimString( value );
            
			// key - value pair read successfully
			return true;
		}
	}
    
	// error
	return false;
}

// Removes blank characters from beginning and end of string
string& Configuration::TrimString(string& str)
{
	string::iterator it;
	for( it = str.begin(); it != str.end() && isspace( *it ); it++ )
		;
	str.erase( str.begin(), it );
    
	string::reverse_iterator rit;
	for( rit = str.rbegin(); rit != str.rend() && isspace( *rit ) ; rit++ )
		;
	str.erase( str.begin() + ( str.rend() - rit ), str.end() );
    
	return str;
}

// Loop over all tutors running processSlots() for each
void Configuration::processNotSlots() {

    for (hash_map<int, Tutor*>::iterator it = _tutors.begin(); it != _tutors.end(); it++) {
        Tutor* tut = (*it).second;
        tut->processSlots();
    }
    
}

void Configuration::dumpTutors(){
    //Print info about all the tutors
    hash_map<int, Tutor*> theTutors = _tutors;
    
    cerr << "\nTutors: \n\n";
    
    for (hash_map<int, Tutor*>::iterator it = theTutors.begin(); it != theTutors.end(); it++) {
        Tutor* tut = (*it).second;
        
        cerr << "Name: "<<tut->getName()<<" (ID="<<tut->getID() << ")\nSubjects:\n";
        list<Subject*> subjlist = tut->getSubjects();
        for (list<Subject*>::iterator it2=subjlist.begin(); it2 != subjlist.end(); it2++) {
            cerr << "\t" << (*it2)->getName() << "("<<(*it2)->getID()<<")\n";
        }
        cerr << "NotSlots:\n";
        list<int> thelist = tut->getNotSlots();
        for (list<int>::iterator it2=thelist.begin(); it2 != thelist.end(); it2++) {
            int notslot = *it2;
            cerr << "\t"<< notslot << "\n";
        }
        cerr << "\n***\n";
    }
    
}


void Configuration::dumpStudents(){
    //Print info about all the students
    list<Student*> theStudents = _students;
    
    cerr << "\nStudents\n\n";
    
    for (list<Student*>::iterator it = theStudents.begin(); it != theStudents.end(); it++) {
        Student* s = (*it);
        
        cerr << "Name: "<<s->getName()<<" (ID="<<s->getID() << ")\nSubject: " << s->getSubject()->getName();
        cerr << "\nNotTimes:\n";
        list<int> thelist = s->getNotTimes();
        for (list<int>::iterator it2=thelist.begin(); it2 != thelist.end(); it2++) {
            int nottime = *it2;
            cerr << "\t"<< nottime << "\n";
        }
        cerr << "\n***\n";
    }
    
}

// remove given objects from their container. Return false on failure

bool Configuration::removeSubject( Subject* s ) {
    
    // Look up the subject by its ID
    hash_map<int, Subject*>::iterator it = _subjects.find( s->getID() );
    if ( it != _subjects.end() ) {
        // We've found the subject! Erase it...
        _subjects.erase(it);

        //now loop over all the tutors and remove this subject from their list if present
        for (hash_map<int, Tutor*>::iterator itTut=_tutors.begin(); itTut!=_tutors.end(); itTut++) {
            (*itTut).second->removeSubject(s);
        }
        
        //now, remove all students if this subject was theirs. Maybe think of a better way to handle this. edit.
        _students.remove_if( [s](Student* thisS){ return thisS->getSubject() == s; } );
        
        return true;
    }
    
    return false;
}

//bool Configuration::removeTutor( int ID ) {
//    // remove the tutor. No need to remove from students prevlist as will now just never cause a fitness penalty
//    
//    // Look up the tutor by their ID
//    hash_map<int, Tutor*>::iterator it = _tutors.find( ID );
//    if ( it != _tutors.end() ) {
//        _tutors.erase(it);
//        return true;
//    }
//
//    return false;
//}
//void Configuration::removeStudent( Student* s ) {
//    // Remove ALL instances of this student (i.e. those with the same baseID)
//    _students.remove_if( [s](Student* thisS){ return thisS->getBaseID() == s->getBaseID(); } );
//}


void Configuration::saveConfig(string filename) {
    
    ofstream output( filename.c_str() );
    
    output << "% Config file. \n"
    "% This file contains the setup for a solution.\n"
    "% N.B. because of the nature of genetic algorithms, using the same setup will not necessarily result in the same solution!\n"
    "% You can change the entries but remember: IDs don't matter,\n"
    "% except for Tutors:\n"
    "%	these MUST start at 1 and ascend from there, else the\n"
    "%	slot identification will break\n\n";
    
    // output the subjects
    for (hash_map<int, Subject*>::iterator it = _subjects.begin(); it != _subjects.end(); it++) {
        
        output << "#subject\n";
        
        output << "\tid = " << (*it).second->getID() << endl;
        
        output << "\tname = " << (*it).second->getName() << endl;
        
        output << "#end\n\n";
        
    }
    
    //output the tutors
    for (hash_map<int, Tutor*>::iterator it = _tutors.begin(); it != _tutors.end(); it++) {
        
        output << "#tutor\n";
        
        output << "\tid = " << (*it).second->getID() << endl;
        
        output << "\tname = " << (*it).second->getName() << endl;
        
        // subjects
        list<Subject*> subjects = (*it).second->getSubjects();
        for (list<Subject*>::iterator itSub = subjects.begin(); itSub!=subjects.end(); itSub++) {
            output << "\tsubj = " << (*itSub)->getID() << endl;
        }
        
        // nottimes
        list<int> notTimes = (*it).second->getNotTimes();
        for (list<int>::iterator itNotT = notTimes.begin(); itNotT!=notTimes.end(); itNotT++) {
            output << "\tnotTime = " << *itNotT << endl;
        }
        
        output << "#end\n\n";
    }
    
    //output the students
    //      build of list of which baseIDs have been done so that we don't duplicate:
    unordered_set<int> doneIDs;
    
    for (list<Student*>::iterator it = _students.begin(); it != _students.end(); it++) {
// If the base ID of the current student has already been processed then break

        int baseID = (*it)->getBaseID();
        
        if ( doneIDs.find( baseID ) != doneIDs.end() ) continue;
        
        doneIDs.insert( (*it)->getBaseID() );
        
        output << "#student\n";
        
        output << "\tbaseID = " << (*it)->getBaseID() << endl;
        
        output << "\tname = " << (*it)->getName() << endl;
        
        output << "\tsubj = " << (*it)->getSubject()->getID() << endl;
        
        output << "\tnoInterviews = " << (*it)->getNoInterviews() << endl;
        
        // nottimes
        list<int> notTimes = (*it)->getNotTimes();
        for (list<int>::iterator itNotT = notTimes.begin(); itNotT!=notTimes.end(); itNotT++) {
            output << "\tnotTime = " << *itNotT << endl;
        }
        
        // prevtutors
        list<Tutor*> prevTuts = (*it)->getPrevTutors();
        for (list<Tutor*>::iterator itPrev = prevTuts.begin(); itPrev!=prevTuts.end(); itPrev++) {
            output << "\tprevTutor = " << (*itPrev)->getID() << endl;
        }
        
        output << "#end\n\n";
    }
}


