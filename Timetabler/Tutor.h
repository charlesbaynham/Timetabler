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

#include "Subject.h"

using namespace std;

class Tutor {
    int _ID;
    string _name;
    list<Subject*> _subjects;
    list<int> _notTimes; //List of times to avoid, from 0 to SLOTS_IN_DAY-1
    list<int> _notSlots; //List of slots that the above times preclude
    //static int _nextID; // Next ID to assign
    bool _slotsProcessed;
    
public:
    Tutor(int id, const string& name, list<Subject*> subjects, list<int> notTimes);
    
    inline int getID(){ return _ID; }
    inline const string& getName() { return _name; }
    inline list<Subject*> getSujects() { return _subjects; }
    inline list<int> getNotSlots() { return _notSlots; }
    
    void processSlots(); // Calculate the _notSlots list based on _notTimes and number of tutors in global config. 
    inline bool isProcessed() { return _slotsProcessed; }
};

#endif
