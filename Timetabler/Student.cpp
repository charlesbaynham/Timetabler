//
//  Student.cpp
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "Student.h"

using namespace std;

int _nextID = 1;

Student::Student(int ID, const string& name, Subject* subj, int noInterviews, list<Tutor*> prevTutors) :
    _ID(ID),
    _name(name),
    _subject(subj),
    _noInterviews(noInterviews),
    _prevTutors(prevTutors) { }
