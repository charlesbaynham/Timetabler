//
//  Tutor.h
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#ifndef Timetabler_Tutor_h
#define Timetabler_Tutor_h

#include <string>
#include <list>
#include <map>

#include "Subject.h"

using namespace std;

class Tutor {
    int _ID;
    string _name;
//    list<Subject*> _subjects;
    // map of done subjects -> proficiency (0-1)
    map<Subject*, float> _subjects;
    
    list<int> _notTimes; //List of times to avoid, from 0 to SLOTS_IN_DAY-1
    list<int> _notSlots; //List of slots that the above times preclude
    static int _nextID; // Next ID to assign if one is not given
    bool _slotsProcessed;
    
public:
    Tutor(int id, const string& name, map<Subject*, float> subjects, list<int> notTimes);
    
    inline int getID(){ return _ID; }
    inline const string& getName() { return _name; }
    inline map<Subject*, float> getSubjects() { return _subjects; }
    inline list<int> getNotTimes() { return _notTimes; }
    inline list<int> getNotSlots() { return _notSlots; }

    
    inline void addNotTime(int time) { _notTimes.push_back(time); _slotsProcessed=false; }
    inline void removeNotTime(int time) { _notTimes.remove(time); _slotsProcessed=false; }
    inline void clearNotTimes() { _notTimes.clear(); _slotsProcessed=false; }
    
    inline void setID(int ID) { _ID = ID; }
    inline void setName(string name) { _name = name; }
    
    inline void removeSubject(Subject* s) {
        map<Subject*, float>::iterator it = _subjects.find(s);
        if ( it != _subjects.end() )
            _subjects.erase( it );
    }
    inline void addSubject(Subject* s, float proficiency) { removeSubject(s); _subjects[s] = proficiency; }
    inline void clearSubjects() { _subjects.clear(); }
    
    void processSlots(); // Calculate the _notSlots list based on _notTimes and number of tutors in global config. 
    inline bool isProcessed() { return _slotsProcessed; }
};

#endif
