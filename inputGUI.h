//
//  inputGUI.h
//  Timetabler
//
//  Created by Charles Baynham on 08/07/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#ifndef __Timetabler__inputGUI__
#define __Timetabler__inputGUI__

#include <iostream>

#include "TimetablerInst.h"

#include <Wt/WTabWidget>
#include <Wt/WLineEdit>
#include <Wt/WContainerWidget>

using namespace std;
using namespace Wt;

class GUITutor;

class inputGUI {
    
    list<GUITutor*> _tutors;
    
    //    void addTutor();
    //    void removeTutor(int ID);
    //
    //    void addStudent(int baseID, string name, Subject* subject, int noInterviews);
    //    void removeStudent(int baseID);
    //
    //    void addSubject(int ID, string name);
    //    void removeSubject(int ID);
    
public:
    
    // Add GUI elements for data entry
    void addBlankStudent();
    void addBlankSubject();
    void addBlankTutor();
};

class GUITutor {
    
    Tutor* _tutor;
    
    WLineEdit* _ID;
    WLineEdit* _name;
    
    WContainerWidget* _visOutput;
    
public:
    GUITutor();
    
    // The overloaded () function returns the Container Widget that is visible and contains the others
    WContainerWidget* operator() () { return _visOutput; }

};

#endif /* defined(__Timetabler__inputGUI__) */
