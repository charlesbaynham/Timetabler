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
#include "GUI.h"

#include <Wt/WApplication>
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
    WText* _submitLabel;
    
//    Update the GUI to match the contents of Configuration
    void updateFromConfig();
    
    // create the "Add new" buttons
    void createAddButtons();
    
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
    
    // Use the previous configuration saved in filename
    void usePrevious(string filename);
    
    // Reset the ID counters
    static void resetIDs();
    
//    inline void redraw(WContainerWidget* parent) {
//        WTabWidget* debug = _menu;
//        parent->addWidget(_menu); }
    
    // Add GUI elements for data entry
    void addBlankStudent();
    void addBlankSubject();
    void addBlankTutor();
    
    // Add populated GUI elements for data modification
    void addFilledStudent(Student* student);
    void addFilledSubject(Subject* subject);
    void addFilledTutor(Tutor* tutor);
//    const string& name, list<Subject*> subjects, list<int> notTimes);
    
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
    // debug
    WContainerWidget* _subjectsCont;
    
    typedef pair< WContainerWidget* , pair<WText*, WSelectionBox> > subjectChoice;
    
    list< subjectChoice > _subjectsL;
    //end debug
    
    WSelectionBox* _notSlots;
    WText* _notLabel;
    
    
public:
    GUITutor(inputGUI* parent, Tutor* tutor);

    GUITutor(inputGUI* parent) : GUITutor(parent, NULL) {}

    
    void addDeleteButton();
    inline Tutor* getTutor() { return _tutor; }

    // call the method in inputGUI to update for a changed tutor entry
    void callUpdate();
    
    inline void decrementID() { _ID--; callUpdate(); }
    inline static void decrementGlobalID() { _nextID--; }
    inline static void resetGlobalID(int ID=1) { _nextID = ID; }
    
    void addSubjectOption(Subject* n) { _subjects->addItem(n->getName()); }
    void addSubjectOption(Subject* n, int position) { _subjects->insertItem(position, n->getName()); }
    
    void removeSubjectOption(int index) { _subjects->removeItem(index); }
    
    // changes the subject at index to the new one
    void changeSubjectOption(Subject* n, int index) {
        bool selected = ( _subjects->currentIndex() == index );
        removeSubjectOption(index);
        addSubjectOption(n, index);
        if (selected) _subjects->setCurrentIndex(index);
    }

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
    WSelectionBox* _notSlots;
    WText* _notLabel;
    
    int _baseID=0; // Left at 0 by the interface. Can be set to a value if reading from a file, otherwise ignored
    
public:
    GUIStudent(inputGUI* parent, Student* student);
    GUIStudent(inputGUI* parent) : GUIStudent(parent, NULL) {}
    
    void addDeleteButton();
    inline Student* getStudent() {return _student;}
    
    void addTutorOption(Tutor* n) { _prevTutors->addItem(n->getName()); }
    void addTutorOption(Tutor* n, int position) { _prevTutors->insertItem(position, n->getName()); }
    
    void removeTutorOption(int index) { _prevTutors->removeItem(index); }
    
    // changes the tutor at index to the new one
    void changeTutorOption(Tutor* n, int index) {
        bool selected = ( _prevTutors->currentIndex() == index );
        removeTutorOption(index);
        addTutorOption(n, index);
        if (selected) _prevTutors->setCurrentIndex(index);
    }
    
    ////
    
    inline void addSubjectOption(Subject* n) { _subject->addItem(n->getName()); }
    inline void addSubjectOption(Subject* n, int position) { _subject->insertItem(position, n->getName()); }
    
    // Removes the subject at index from the selection box
    inline void removeSubjectOption(int index) { _subject->removeItem(index); }

    // changes the subject at index to the new one
    void changeSubjectOption(Subject* n, int index) {
        bool selected = ( _subject->currentIndex() == index );
        removeSubjectOption(index);
        addSubjectOption(n, index);
        if (selected) _subject->setCurrentIndex(index);
    }

    // call the methods in inputGUI to update for a changed subject entry
    void callUpdate();

};


class GUISubject : public GUIelement {
    
    Subject* _subject;
    
    int _ID;
    static int _nextID;
    WLineEdit* _name;
    WText* _nameLabel;
    
public:
    GUISubject(inputGUI* parent, Subject* subject);
    GUISubject(inputGUI* parent) : GUISubject(parent, NULL) {}


    void addDeleteButton();
    inline Subject* getSubject() { return _subject; }
    
    inline static void resetGlobalID(int ID=1) { _nextID = ID; }
    
    // call the methods in inputGUI to update for a changed subject entry
    void callUpdate();

};

#endif /* defined(__Timetabler__inputGUI__) */