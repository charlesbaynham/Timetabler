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
#include <Wt/WPushButton>

using namespace std;
using namespace Wt;

class GUITutor;

class inputGUI : public WObject {
    
    list<GUITutor*> _tutors;
    
    WTabWidget* _menu;
    
    WContainerWidget* _tutorTab;
    
    //    void addTutor();
    //    void removeTutor(int ID);
    //
    //    void addStudent(int baseID, string name, Subject* subject, int noInterviews);
    //    void removeStudent(int baseID);
    //
    //    void addSubject(int ID, string name);
    //    void removeSubject(int ID);
    
public:
    // Initialise this object: create a menu in parent
    inputGUI(WContainerWidget* parent);
    
    inline void redraw(WContainerWidget* parent) { ;/*parent->addWidget(_menu);*/ }
    
    // Add GUI elements for data entry
    void addBlankStudent();
    void addBlankSubject();
    void addBlankTutor();
    
    //remove t from the visible _tutorTab and from the list
    void removeGUITutor(GUITutor* t);
    
    //debug
    void removeFirstTutor() {
        removeGUITutor(_tutors.front());
    }
    //end debug
    
    
//    // Set the menu to the arg and add the rendered Tutor, Student and Subject tabs
//    void setMenu( WTabWidget& menu );
    
//    // Refreshes all the tabs. 
//    void refresh();
    
//    // refreshes the _tutorTab
//    void renderTutor();
    
};

class GUITutor {
    
    Tutor* _tutor;
    
    WLineEdit* _ID;
    WLineEdit* _name;
    
    WContainerWidget* _visOutput;
    
public:
    GUITutor();
    
    // The overloaded () function returns the Container Widget that is visible and contains the other widgets
    WContainerWidget* operator() () { return _visOutput; }

};

#endif /* defined(__Timetabler__inputGUI__) */