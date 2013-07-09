//
//  inputGUI.cpp
//  Timetabler
//
//  Created by Charles Baynham on 08/07/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "inputGUI.h"

GUITutor::GUITutor()
{
    _IDLabel = new WText("ID:", _visOutput);
    _ID = new WLineEdit(_visOutput);
    _nameLabel = new WText("Name:", _visOutput);
    _name = new WLineEdit(_visOutput);
    _subjectsLabel = new WText("Subjects:",_visOutput);
    _subjects = new WSelectionBox(_visOutput);
    _notLabel = new WText("Unavailable times:",_visOutput);
    _notSlots = new WSelectionBox(_visOutput);
}
GUISubject::GUISubject()
{
    _IDLabel = new WText("ID:", _visOutput);
    _ID = new WLineEdit(_visOutput);
    _nameLabel = new WText("Name:", _visOutput);
    _name = new WLineEdit(_visOutput);
}
GUIStudent::GUIStudent()
{
    _nameLabel = new WText("Name:", _visOutput);
    _name = new WLineEdit(_visOutput);
    _interviewsLabel = new WText("Number of interviews:", _visOutput);
    _numInterviews = new WComboBox(_visOutput);
    _subjectLabel = new WText("Subject:", _visOutput);
    _subject = new WComboBox(_visOutput);
    _prevLabel = new WText("Previous tutors:", _visOutput);
    _prevTutors = new WSelectionBox(_visOutput);
}

void inputGUI::addBlankTutor () {
    GUITutor* newTut = new GUITutor;
    _tutors.push_back(newTut); // Add new GUI element to list...
    _tutorTab->addWidget( (*newTut)() ); // and to the display
}
void inputGUI::addBlankSubject () {
    GUISubject* newSubj = new GUISubject;
    _subjects.push_back(newSubj); // Add new GUI element to list...
    _subjectTab->addWidget( (*newSubj)() ); // and to the display
}
void inputGUI::addBlankStudent () {
    GUIStudent* newStud = new GUIStudent;
    _students.push_back(newStud); // Add new GUI element to list...
    _studentTab->addWidget( (*newStud)() ); // and to the display
}

void inputGUI::removeGUITutor(GUITutor* t)
{
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


