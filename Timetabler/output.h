//
//  output.h
//  Timetabler
//
//  Created by Charles Baynham on 01/07/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

//CSV output format for now

#ifndef __Timetabler__output__
#define __Timetabler__output__

#include <iostream>
#include <fstream>

#include "TTChromosone.h"

class outputCSV {
    static outputCSV _instance;
    
public:

    // output a chromosome
    void operator ()(char * filename, const GaChromosome& chromo);
    
    static inline outputCSV getInstance(){ return _instance; }
};

// A class for assembling the completed timetable, ready to be printed
class timetableSlot {

    int _time;
    int _slot;
    Tutor* _tutor;
    Student* _student;
    
public:
    inline int getTime() { return _time; }
    inline string getTutorName() { return _tutor->getName(); }
    inline string getStudentName() { return _student->getName(); }
    
    inline bool operator< (const timetableSlot comp) const { return (_time < comp._time); }
    inline bool operator> (const timetableSlot comp) const { return (_time > comp._time); }
    inline bool operator>= (const timetableSlot comp) const { return (_time >= comp._time); }
    inline bool operator<= (const timetableSlot comp) const { return (_time <= comp._time); }
    
    // Given slot, work out the tutor and the time
    timetableSlot(int slot, Student* student);
};

class outputRaw {
    static outputRaw _instance;
public:
    void operator ()(char * filename, const GaChromosome& chromo);
    static inline outputRaw getInstance() { return _instance; }
};

#endif /* defined(__Timetabler__output__) */
