//
//  Student.cpp
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "Student.h"

using namespace std;

int Student::_nextID = 1;

Student::Student(int ID, const string& name, Subject* subj, int noInterviews, list<Tutor*> prevTutors) :
    _ID(ID),
    _baseID(ID),
    _name(name),
    _subject(subj),
    _noInterviews(noInterviews),
    _prevTutors(prevTutors) { }

Student::Student(const string& name, Subject* subj, int noInterviews, list<Tutor*> prevTutors) :
    _name(name),
    _subject(subj),
    _noInterviews(noInterviews),
_prevTutors(prevTutors) { _ID = _nextID++; _baseID = _ID; }

//create new student as copy of previous but with next id
Student::Student(Student* copy) {
    _name = copy->_name;
    _subject = copy->_subject;
    _noInterviews = copy->_noInterviews;
    _prevTutors = copy->_prevTutors;
    
    _baseID = copy->_baseID;
    _ID = _nextID++;
}