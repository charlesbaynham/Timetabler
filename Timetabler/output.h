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
#include <map>

#include "TTChromosone.h"

//hold the timetable for a single tutor
class tutorTT_tutor {
    
    friend class tutorTT;
    
    Tutor* _tutor;
    hash_map<int, Student*> _students;
    
public:
    inline int getTutorID() { return _tutor->getID(); }
    inline string getTutorName() { return _tutor->getName(); }
    inline Tutor* getTutor() { return _tutor; }
    
    inline int getStudentID(int time) { return _students[time]->getID(); }
    inline string getStudentName(int time) { return _students[time]->getName(); }
    inline Student* getStudent(int time) { return _students[time]; }
    
    inline tutorTT_tutor(Tutor* tutor) : _tutor(tutor){}
    inline void addStudent(int time, Student* student) { _students[time]=student; }
};

// Hold a complete timetable, formatted for the tutors' point of view
typedef list< tutorTT_tutor* > tutorTTList;

class tutorTT {
    // list of tutor's timetables
    tutorTTList _tutors;
public:
    inline tutorTTList getTT() { return _tutors; }
    
    //return which student the tutor has at time
    Student* getTutorApt(int tutorID, int time);
    inline Student* getTutorApt(Tutor* tutor, int time) { return getTutorApt(tutor->getID(), time); }
    
    tutorTT(const Chromosone*);
};

//hold the timetable for a single student
class studentTT_student {
    
    friend class studentTT;
    
    Student* _student;
    hash_map<int, Tutor*> _tutors;
    
public:
    inline int getStudentID() { return _student->getID(); }
    inline string getStudentName() { return _student->getName(); }
    inline Student* getStudent() { return _student; }
    
    inline int getTutorID(int time) { return _tutors[time]->getID(); }
    inline string getTutorName(int time) { return _tutors[time]->getName(); }
    inline Tutor* getTutor(int time) { return _tutors[time]; }
    
    inline studentTT_student(Student* student) : _student(student) {}
    inline void addTutor(int time, Tutor* tutor) { _tutors[time]=tutor; }
};

// Hold a complete timetable, formatted for the students' point of view
typedef map<int, studentTT_student* > studentTTList;

class studentTT {
    studentTTList _students;
public:
//    inline studentTTList getTT() { return _students; }
    
    //return which student the tutor has at time
    Tutor* getStudentApt(int studentBaseID, int time);
    inline Tutor* getStudentApt(Student* student, int time) { return getStudentApt(student->getBaseID(), time); }
    
    studentTT(const Chromosone*);
};


// Hold the finished timetable, from both points of view
class finishedTT {
    // Hold the timetables:
    tutorTT* _tutorTT;
    studentTT* _studentTT;
    const Chromosone* _chromo;
    
    // Hold all the students and tutors:
    hash_map<int, Student*> _students;
    hash_map<int, Tutor*> _tutors;
    
public:
    
    inline tutorTT* getTutorOrientated() { return _tutorTT; }
    inline studentTT* getStudentOrientated() { return _studentTT; }
    
    inline hash_map<int, Student*> getAllStudents() { return _students; }
    inline hash_map<int, Tutor*> getAllTutors() { return _tutors; }
    
    finishedTT(const GaChromosome* chromo);
};


////

// output the final chromosome in a format that can be read back in by Configuration
class outputSolution {
    
    static outputSolution _instance;
public:
    
    static inline outputSolution getInstance() { return _instance; }
    void operator() (char * filename, const GaChromosome& chromo, bool append=false);
    
};


// output the timetable as a CSV file
class outputCSV {
    static outputCSV _instance;
    
public:

    // output a chromosome
    void operator ()(char * filename, const GaChromosome& chromo, bool detailed);
    
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
    inline string getStudentSubject() { return _student->getSubject()->getName(); }
    string getTutorSubjects();
    
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
