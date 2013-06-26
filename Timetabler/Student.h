//
//  Student.h
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#ifndef Timetabler_Student_h
#define Timetabler_Student_h

#include <string>
#include <list>

using namespace std;

#include "Subject.h"
#include "Tutor.h"

class Student {
    
private:
    int _ID;
    string _name;
    Subject* _subject; // Interview subject
    int _noInterviews;  // Number of interviews (2 or 4)
    list<Tutor*> _prevTutors; // Previous tutors to be avoided
//    list<Tutor*> _tutors;
    
public:
    Student(int ID, const string& name, Subject* subj, int noInterviews, list<Tutor*> prevTutors); // Initiate
    
    inline int getID(){ return _ID; }
    inline const string& getName() { return _name; }
};

#endif
