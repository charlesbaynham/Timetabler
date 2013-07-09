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
#include <Wt/WComboBox>
#include <Wt/WSelectionBox>

using namespace std;
using namespace Wt;

class GUITutor;
class GUIStudent;
class GUISubject;

class inputGUI : public WObject {
    
    list<GUITutor*> _tutors;
    list<GUIStudent*> _students;
    list<GUISubject*> _subjects;
    
    WTabWidget* _menu;
    
    WContainerWidget* _tutorTab;
    WContainerWidget* _studentTab;
    WContainerWidget* _subjectTab;
    
    WContainerWidget* _submitTab;
    
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
    void removeGUIStudent(GUIStudent* s);
    void removeGUISubject(GUISubject* s);
    
    // process all the input data and use it to fill out Configuration and start the solution
    void submit();
    
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

// parent class for GUI elements
class GUIelement {

protected:
    WContainerWidget* _visOutput;
    
public:
    inline GUIelement() { _visOutput = new WContainerWidget(); }
    
    // The overloaded () function returns the Container Widget that is displayed and contains the other widgets
    WContainerWidget* operator() () { return _visOutput; }
};

class GUITutor : public GUIelement {
    
    Tutor* _tutor;
    
    WLineEdit* _ID;
    WText* _IDLabel;
    WLineEdit* _name;
    WText* _nameLabel;
    WSelectionBox* _subjects;
    WText* _subjectsLabel;
    WSelectionBox* _notSlots;
    WText* _notLabel;
    
    
public:
    GUITutor();
    
};


class GUIStudent : public GUIelement {
    
    Student* _student;
    
    WLineEdit* _name;
    WText* _nameLabel;
    WComboBox* _numInterviews;
    WText* _interviewsLabel;
    WComboBox* _subject;
    WText* _subjectLabel;
    WSelectionBox* _prevTutors;
    WText* _prevLabel;
    
public:
    GUIStudent();
};


class GUISubject : public GUIelement {
    
    Subject* _subject;
    
    WLineEdit* _ID;
    WText* _IDLabel;
    WLineEdit* _name;
    WText* _nameLabel;
    
public:
    GUISubject();
};

#endif /* defined(__Timetabler__inputGUI__) */