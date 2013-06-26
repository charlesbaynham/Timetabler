//
//  Tutor.cpp
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "Tutor.h"

int Tutor::_totalTutors=0;

Tutor::Tutor(const string& name, list<Subject*> subjects, list<int> notTimes) :
_name(name),
_subjects(subjects),
_notTimes(notTimes)
{
    _ID = _totalTutors++;
}

Tutor::~Tutor() { _totalTutors--; }