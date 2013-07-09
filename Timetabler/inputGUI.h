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
    
//    Lists to hold all the GUI elements for the three tabs
    list<GUITutor*> _tutors;
    list<GUIStudent*> _students;
    list<GUISubject*> _subjects;
    
//    2 indices to hold the poisition of tutors / subjects in the WSelectionBox/WComboBox elements.
//      I.e. _tutorIndex[2] == tutorX  implies that the item at index 2 in a selection box refers to tutorX
    vector<Tutor*> _tutorIndex;
    vector<Subject*> _subjectIndex;
    
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
    
//    inline void redraw(WContainerWidget* parent) { ;/*parent->addWidget(_menu);*/ }
    
    // Add GUI elements for data entry
    void addBlankStudent();
    void addBlankSubject();
    void addBlankTutor();
    
    //remove t from the visible _tutorTab and from the list
    void removeGUITutor(GUITutor* t);
    void removeGUIStudent(GUIStudent* s);
    void removeGUISubject(GUISubject* s);
    
    //modify the options when a tutor / subject is changed
    void changeTutorOptions( Tutor* newtut, Tutor* oldtut);
    void changeSubjectOptions( Subject* newsub, Subject* oldsub);

    // return the indexes
    vector<Tutor*> getTutorIndex() { return _tutorIndex; }
    vector<Subject*> getSubjectIndex() { return _subjectIndex; }
    
    // process all the input data and use it to fill out Configuration and start the solution
    void submit();
    
    
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
    inputGUI* _parent;
    
    WContainerWidget* _visOutput;
    
    WPushButton* _deleteMe;
    
    virtual void addDeleteButton()=0;
public:
    inline GUIelement(inputGUI* parent) : _parent(parent) { _visOutput = new WContainerWidget(); }
    
    // The overloaded () function returns the Container Widget that is displayed and contains the other widgets
    WContainerWidget* operator() () { return _visOutput; }
};

class GUITutor : public GUIelement {
    
    Tutor* _tutor;
    
    int _ID;
    static int _nextID;
    WLineEdit* _name;
    WText* _nameLabel;
    WSelectionBox* _subjects;
    WText* _subjectsLabel;
    WSelectionBox* _notSlots;
    WText* _notLabel;
    
    
public:
    GUITutor(inputGUI* parent);
    void addDeleteButton();
    inline Tutor* getTutor() { return _tutor; }

    // call the method in inputGUI to update for a changed tuto entry
    void callUpdate();
    
    inline void decrementID() { _ID--; callUpdate(); }
    inline static void decrementGlobalID() { _nextID--; }
    
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
    GUIStudent(inputGUI* parent);
    void addDeleteButton();
    inline Student* getStudent() {return _student;}
    
    void addTutorOption(Tutor* n) { _prevTutors->addItem(n->getName()); }
    void addTutorOption(Tutor* n, int position) { _prevTutors->insertItem(position, n->getName()); }
    
    void removeTutorOption(int index) {
        _prevTutors->removeItem(index);
    }
    
};


class GUISubject : public GUIelement {
    
    Subject* _subject;
    
    WLineEdit* _ID;
    WText* _IDLabel;
    WLineEdit* _name;
    WText* _nameLabel;
    
public:
    GUISubject(inputGUI* parent);
    void addDeleteButton();
    inline Subject* getSubject() { return _subject; }

    // call the methods in inputGUI to update for a changed subject entry
    void callSubjectUpdate();
};

#endif /* defined(__Timetabler__inputGUI__) */