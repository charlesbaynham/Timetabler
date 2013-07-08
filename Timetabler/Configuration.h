//
//  Configuration.h
//  Timetabler
//
//  Created by Charles Baynham on 27/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#ifndef __Timetabler__Configuration__
#define __Timetabler__Configuration__

#include <iostream>
#include <string>
#include <fstream>
#include <ext/hash_map>
using namespace __gnu_cxx;

#include "Student.h"
#include "Tutor.h"
#include "Subject.h"

class Configuration {
private:
    static Configuration _instance; // Global instance
    
public:
    inline static Configuration& getInstance() { return _instance; } // return global instance
    
private:
    
    //Lookup tables, mapping ID -> object
    hash_map<int, Tutor*> _tutors;
    hash_map<int, Subject*> _subjects;
    
    // List of students (list since will be dynamically moved)
    list<Student*> _students;
    
    bool _isEmpty; // initialised to true: indicates that the config is unparsed.
    
public:
    // Init
    Configuration() : _isEmpty(true) {}
    
    void dumpTutors();
    
    //free up
    //~Configuration();
    
    //Get the components:
    inline Tutor* getTutor(int ID) {
        hash_map<int, Tutor*>::iterator it = _tutors.find(ID);
        return it != _tutors.end() ? (*it).second : NULL; // If the ID exists in the map, iterate to it and return it
    }
    inline Subject* getSubject(int ID) {
        hash_map<int, Subject*>::iterator it = _subjects.find(ID);
        return it != _subjects.end() ? (*it).second : NULL; // If the ID exists in the map, iterate to it and return it
    }
    
    //Return the whole list of student pointers:
    inline list<Student*> getStudents() { return _students; }
    
    //Return whole hash_map of tutors
    inline hash_map<int, Tutor*> getTutors() { return _tutors; }
    
    //count the components
    inline int numTutors() { return (int)_tutors.size(); }
    inline int numSubjects() { return (int)_subjects.size(); }
    inline int numStudents() { return (int)_students.size(); }
    
    // Check to see if has been setup or not
    bool parsed() { return !_isEmpty; }
    
    //Parse a file for inputs
    int parseFile(char* fileName);
    
    // Given an object, add it to the relevant list
    inline void addSubject( Subject* s ) { _subjects.insert( pair<int, Subject*>( s->getID(), s ) ); }
    inline void addTutor( Tutor* t ) { _tutors.insert( pair<int, Tutor*>( t->getID(), t ) ); }
    inline void addStudent( Student* s ) { _students.push_back( s ); }
    
    //remove given object from its list
    bool removeSubject( Subject* s );
    inline bool removeSubject( int ID ) { return removeSubject( this->getSubject(ID) ); }
    bool removeTutor( Tutor* t );
    bool removeStudent( Student* s );

private:
    
	// Reads tutor's data from config file, makes object and returns pointer to it
	// Returns NULL if method cannot parse configuration data
	Tutor* ParseTutor(ifstream& file);
    
	// Reads subject's data from config file, makes object and returns pointer to it
	// Returns NULL if method cannot parse configuration data
	Subject* ParseSubject(ifstream& file);
    
	// Reads student's data from config file, makes object and returns pointer to it
	// Returns NULL if method cannot parse configuration data
	Student* ParseStudent(ifstream& file);
    
	// Reads one line (key - value pair) from configuration file
	bool GetConfigBlockLine(ifstream& file, string& key, string& value);
    
	// Removes blank characters from beginning and end of string
	string& TrimString(string& str);
    
    //Processes the slots that tutors can't do for every tutor
    void processTutorsSlots();
};

#endif /* defined(__Timetabler__Configuration__) */
