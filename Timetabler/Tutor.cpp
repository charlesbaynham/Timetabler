//
//  Tutor.cpp
//  Timetabler
//
//  Created by Charles Baynham on 26/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "Tutor.h"
#include "TTChromosone.h"

//int Tutor::_nextID = 0;

Tutor::Tutor(int id, const string& name, list<Subject*> subjects, list<int> notTimes) :
_ID(id),
_name(name),
_subjects(subjects),
_notTimes(notTimes) {
    _slotsProcessed = false;
}


void Tutor::processSlots() {
    
    _notSlots.empty(); // Empty notslots list in case it's already been done
    
    for (list<int>::iterator it = _notTimes.begin(); it != _notTimes.end(); it++){ // Loop through all notTimes...
        int notslot;
        notslot = *it + (_ID-1) * SLOTS_IN_DAY; //  translating them to notSlots

        _notSlots.push_back(notslot); // Store in list
    }
    
    _slotsProcessed = true;
}

