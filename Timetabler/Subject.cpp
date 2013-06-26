//
//  Subject.cpp
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "Subject.h"

int Subject::_totalSubjects = 0;

Subject::Subject(const string& name) :
    _name(name)
{
    _ID = _totalSubjects++;
}