//
//  inputGUI.cpp
//  Timetabler
//
//  Created by Charles Baynham on 08/07/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "inputGUI.h"


GUITutor::GUITutor(inputGUI* parent) :
    GUIelement(parent)
{
    _tutor = new Tutor(0, "", NULL, NULL);
    
    _IDLabel = new WText("ID:", _visOutput);
    _ID = new WLineEdit(_visOutput);
    _nameLabel = new WText("Name:", _visOutput);
    _name = new WLineEdit(_visOutput);
    _subjectsLabel = new WText("Subjects:",_visOutput);
    _subjects = new WSelectionBox(_visOutput);
    _notLabel = new WText("Unavailable times:",_visOutput);
    _notSlots = new WSelectionBox(_visOutput);
    
    addDeleteButton();
}
GUISubject::GUISubject(inputGUI* parent) :
    GUIelement(parent)
{
    _subject = new Subject(0, "");
    
    _IDLabel = new WText("ID:", _visOutput);
    _ID = new WLineEdit(_visOutput);
    _nameLabel = new WText("Name:", _visOutput);
    _name = new WLineEdit(_visOutput);
    
    addDeleteButton();
}
GUIStudent::GUIStudent(inputGUI* parent) :
    GUIelement(parent)
{
    _student = new Student("", NULL, 2, NULL);
    
    _nameLabel = new WText("Name:", _visOutput);
    _name = new WLineEdit(_visOutput);
    _interviewsLabel = new WText("Number of interviews:", _visOutput);
    _numInterviews = new WComboBox(_visOutput);
        _numInterviews->addItem("2");
        _numInterviews->addItem("4");
        _numInterviews->setCurrentIndex(0);
    _subjectLabel = new WText("Subject:", _visOutput);
    _subject = new WComboBox(_visOutput);
    _prevLabel = new WText("Previous tutors:", _visOutput);
    _prevTutors = new WSelectionBox(_visOutput);
    //loop over the index and populate the new selection box
    for (vector<Tutor*>::iterator it = _parent->getTutorIndex().begin(); it != _parent->getTutorIndex().end(); it++)
        _prevTutors->addItem( (*it)->getName() );
    
    // Add the delete button nherited from GUIelement
    addDeleteButton();
}

void inputGUI::addBlankTutor () {
    GUITutor* newTut = new GUITutor(this);
    _tutors.push_back(newTut); // Add new GUI element to list...
    _tutorTab->addWidget( (*newTut)() ); // and to the display
    
    // Add to the index list, then
    // loop over all GUI elements in the student tab, adding this new tutor to the options
    _tutorIndex.push_back(newTut->getTutor());
    for ( list<GUIStudent*>::iterator it = _students.begin(); it != _students.end(); it++) {
        (*it)->addTutorOption(newTut->getTutor());
    }
}
void inputGUI::addBlankSubject () {
    GUISubject* newSubj = new GUISubject(this);
    _subjects.push_back(newSubj); // Add new GUI element to list...
    _subjectTab->addWidget( (*newSubj)() ); // and to the display
    _subjectIndex.push_back( newSubj->getSubject() );  // And to the other relevant elements
}
void inputGUI::addBlankStudent () {
    GUIStudent* newStud = new GUIStudent(this);
    _students.push_back(newStud); // Add new GUI element to list...
    _studentTab->addWidget( (*newStud)() ); // and to the display
}

void inputGUI::removeGUITutor(GUITutor* t)
{
    // get the index of the tutor, then remove from the index
    vector<Tutor*>::iterator search = find(_tutorIndex.begin(), _tutorIndex.end(), t->getTutor());
    int currIndex = search - _tutorIndex.begin();
    _tutorIndex.erase(search);
    
    // loop over all GUI elements in the student tab, removing this new tutor from the options
    for ( list<GUIStudent*>::iterator it = _students.begin(); it != _students.end(); it++) {
        (*it)->removeTutorOption(currIndex);
    }
    
    _tutorTab->removeWidget( (*t)() );
    _tutors.remove(t);
}
void inputGUI::removeGUIStudent(GUIStudent* s)
{
    _studentTab->removeWidget( (*s)() );
    _students.remove(s);
}
void inputGUI::removeGUISubject(GUISubject* s)
{
    _subjectTab->removeWidget( (*s)() );
    _subjects.remove(s);
}

// Specialisations for adding delete buttons
void GUITutor::addDeleteButton()
{
    _deleteMe = new WPushButton("Delete", _visOutput);
    
    // Bind the button to the removeGUItutor function of _parent, passing this object as the argument to be deleted
    _deleteMe->clicked().connect( boost::bind( &inputGUI::removeGUITutor, _parent, this ) );
}
void GUISubject::addDeleteButton()
{
    _deleteMe = new WPushButton("Delete", _visOutput);
    
    // Bind the button to the removeGUItutor function of _parent, passing this object as the argument to be deleted
    _deleteMe->clicked().connect( boost::bind( &inputGUI::removeGUISubject, _parent, this ) );
}
void GUIStudent::addDeleteButton()
{
    _deleteMe = new WPushButton("Delete", _visOutput);
    
    // Bind the button to the removeGUItutor function of _parent, passing this object as the argument to be deleted
    _deleteMe->clicked().connect( boost::bind( &inputGUI::removeGUIStudent, _parent, this ) );
}

// Initialise the display elements and store pointers to them in the inputGUI class
inputGUI::inputGUI(WContainerWidget* parent) :
    _tutorTab(new WContainerWidget()),
    _subjectTab(new WContainerWidget()),
    _studentTab(new WContainerWidget()),
    _submitTab(new WContainerWidget())
{
    // create a menu
    _menu = new WTabWidget(parent);
    
//    _menu->setInternalPathEnabled();
//    _menu->setInternalBasePath("/input");
    
//    //Add tabs:
//    _menu->addTab( new WText("intro"), "Introduction");
//    _menu->addTab( new WText("Not yet available"), "Download", WTabWidget::LoadPolicy::PreLoading);
    
    // Add the tabs to the menu
    _menu->addTab(_subjectTab, "Subjects");
    _menu->addTab(_tutorTab, "Tutors");
    _menu->addTab(_studentTab, "Students");


    _menu->addTab(_submitTab, "Submit");
    WPushButton* submit = new WPushButton("Submit");
    _submitTab->addWidget(submit);
    submit->clicked().connect( this, &inputGUI::submit );
    
    //Add "new" buttons
    WPushButton* newTut = new WPushButton("Add new");
    WPushButton* newStud = new WPushButton("Add new");
    WPushButton* newSubj = new WPushButton("Add new");
    
    newTut->clicked().connect( this, &inputGUI::addBlankTutor );
    newStud->clicked().connect( this, &inputGUI::addBlankStudent );
    newSubj->clicked().connect( this, &inputGUI::addBlankSubject );
    
    _tutorTab->addWidget(newTut);
    _studentTab->addWidget(newStud);
    _subjectTab->addWidget(newSubj);
    
}

void inputGUI::submit() {
    
    ;
}


