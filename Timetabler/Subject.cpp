//
//  Subject.cpp
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "Subject.h"

int Subject::_nextID = 1;

Subject::Subject(int ID, const string& name) :
    _ID(ID), _name(name) {
        if (ID==0) _ID = _nextID++;
        else if (ID >= _nextID) _nextID = ID+1;
}