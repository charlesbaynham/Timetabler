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
#include <vector>
using namespace __gnu_cxx;

#include "TTChromosone.h"
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
    
    // model Chromosome that holds a previously found solution if one was loaded
    vector<list<int> > _prevSolution;
    // was one loaded?
    bool _prevSolutionLoaded=false;
    
public:
    // Init
    Configuration() : _isEmpty(true) {}
    
    //wipe the config
    inline void clear() { _isEmpty=true; _tutors.clear(); _subjects.clear(); _students.clear(); }
    
    // Output a config file containing the current configuration
    void saveConfig(string filename);
    
    void dumpTutors();
    void dumpStudents();
    void dumpSolution();

    
    inline vector<list<int> > getPrevSolution() { return _prevSolution; }
    inline bool prevSolutionLoaded() { return _prevSolutionLoaded; }

    // removes tutor t from the previous solution and adjusts all the slot values to compensate
    void removeTutorFromPrev(Tutor* t);
    
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
    
    list<Student*> getStudentsByBaseID(int baseID);
    
    //Return whole hash_map of tutors/subjects
    inline hash_map<int, Tutor*> getTutors() { return _tutors; }
    inline hash_map<int, Subject*> getSubjects() { return _subjects; }
    
    //count the components
    inline int numTutors() { return (int)_tutors.size(); }
    inline int numSubjects() { return (int)_subjects.size(); }
    inline int numStudents() { return (int)_students.size(); }
    
    // Check to see if has been setup or not
    inline bool isEmpty() { return _isEmpty; }
    
    //Parse a file for inputs
    int parseFile(const char* fileName);
    int parseFile(char* fileName) { return parseFile(fileName); }
    
    //set up Configuration using arguments
    void setup( hash_map<int, Tutor*> tutors, hash_map<int, Subject*> subjects, list<Student*> students );

    // Given an object, add it to the relevant list
    inline void addSubject( Subject* s ) { _subjects.insert( pair<int, Subject*>( s->getID(), s ) ); }
    inline void addTutor( Tutor* t ) { _tutors.insert( pair<int, Tutor*>( t->getID(), t ) ); }
    inline void addStudent( Student* s ) { _students.push_back( s ); }
    
    //remove given object from its list
    bool removeSubject( Subject* s );
    inline bool removeSubject( int ID ) { return removeSubject( this->getSubject(ID) ); }
    
    // Commented out because breaks the sequentiality (?) of the tutors
//    bool removeTutor( int ID );
//    inline bool removeTutor( Tutor* t ) { return removeTutor(t->getID()); }
    void removeStudent( Student* s );

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
    
    // Read's the previous solution from a file, makes a table of baseIDs in correct slots and returns it
    // Returns NULL if method cannot parse configuration data
    vector<list<int> > ParseSolution(ifstream& file);
    
	// Reads one line (key - value pair) from configuration file
	bool GetConfigBlockLine(ifstream& file, string& key, string& value);
    
	// Removes blank characters from beginning and end of string
	string& TrimString(string& str);
    
    //Processes the slots that tutors and students can't do for each
    void processNotSlots();
};

#endif /* defined(__Timetabler__Configuration__) */
