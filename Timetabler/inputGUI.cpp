//
//  inputGUI.cpp
//  Timetabler
//
//  Created by Charles Baynham on 08/07/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "inputGUI.h"
#include <boost/lexical_cast.hpp>

int GUITutor::_nextID = 1;
int GUISubject::_nextID = 1;

GUITutor::GUITutor(inputGUI* parent) :
    GUIelement(parent)
{    
    _ID = _nextID++;
    printf("Creating new tutor with ID %i\n", _ID);
    _tutor = new Tutor(_ID, "", NULL, NULL);

    _nameLabel = new WText("Name:", _visOutput);
    _name = new WLineEdit(_visOutput);
    _subjectsLabel = new WText("Subjects:",_visOutput);
    _subjects = new WSelectionBox(_visOutput);
        //loop over the index and populate the new selection box
        vector<Subject*> subjectIndex = _parent->getSubjectIndex();
        for (vector<Subject*>::iterator it = subjectIndex.begin(); it != subjectIndex.end(); it++)
            _subjects->addItem( (*it)->getName() );
        //set the selection box to allow multiple selections
        _subjects->setSelectionMode(SelectionMode::ExtendedSelection);


    _notLabel = new WText("Unavailable times:",_visOutput);
    _notSlots = new WSelectionBox(_visOutput);
        //set the selection box to allow multiple selections
        _notSlots->setSelectionMode(SelectionMode::ExtendedSelection);
        // Add the slot options
        for (int i=0; i<SLOTS_IN_DAY; i++) { _notSlots->addItem( to_string(i) ); }
    
    _name->setValidator(new WValidator(true));
    
    addDeleteButton();
    
    //register update handler
    _name->changed().connect( boost::bind( &GUITutor::callUpdate, this ) );
    _notSlots->changed().connect( boost::bind( &GUITutor::callUpdate, this ) );
    _subjects->changed().connect( boost::bind( &GUITutor::callUpdate, this ) );
}
GUISubject::GUISubject(inputGUI* parent) :
    GUIelement(parent)
{
    _ID = _nextID++;
    _subject = new Subject(_ID, "");
    
    _nameLabel = new WText("Name:", _visOutput);
    _name = new WLineEdit(_visOutput);
    
    addDeleteButton();
    
    //register update handler
    _name->changed().connect( boost::bind( &GUISubject::callUpdate, this ) );
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
    //loop over the index and populate the new selection box
        vector<Subject*> subjectIndex = _parent->getSubjectIndex();
        for (vector<Subject*>::iterator it = subjectIndex.begin(); it != subjectIndex.end(); it++)
            _subject->addItem( (*it)->getName() );
    _prevLabel = new WText("Previous tutors:", _visOutput);
    _prevTutors = new WSelectionBox(_visOutput);
        // Make into multi select box
        _prevTutors->setSelectionMode(SelectionMode::ExtendedSelection);
        //loop over the index and populate the new selection box
        vector<Tutor*> tutorIndex = _parent->getTutorIndex();
        for (vector<Tutor*>::iterator it = tutorIndex.begin(); it != tutorIndex.end(); it++)
            _prevTutors->addItem( (*it)->getName() );
    
    // Add the delete button inherited from GUIelement
    addDeleteButton();
    
    //register update handler
    _name->changed().connect( boost::bind( &GUIStudent::callUpdate, this ) );
    _numInterviews->changed().connect( boost::bind( &GUIStudent::callUpdate, this ) );
    _subject->changed().connect( boost::bind( &GUIStudent::callUpdate, this ) );
    _prevTutors->changed().connect( boost::bind( &GUIStudent::callUpdate, this ) );
}

void inputGUI::addBlankTutor () {
    GUITutor* newTut = new GUITutor(this);
    _tutors.push_back(newTut); // Add new GUI element to list...
    _tutorTab->addWidget( (*newTut)() ); // and to the display
    
    // Add to the index list, then
    // loop over all GUI elements in the student tab, adding this new tutor to the options
    Tutor* tutObj = newTut->getTutor();
    _tutorIndex.push_back(tutObj);
    for ( list<GUIStudent*>::iterator it = _students.begin(); it != _students.end(); it++) {
        (*it)->addTutorOption(newTut->getTutor());
    }
}
void inputGUI::addBlankSubject () {
    GUISubject* newSubj = new GUISubject(this);
    _subjects.push_back(newSubj); // Add new GUI element to list...
    _subjectTab->addWidget( (*newSubj)() ); // and to the display
    
    // Add to the index list, then
    _subjectIndex.push_back(newSubj->getSubject());
    // loop over all GUI elements in the student tab, adding this new tutor to the options
    for ( list<GUIStudent*>::iterator it = _students.begin(); it != _students.end(); it++) {
        (*it)->addSubjectOption(newSubj->getSubject() );
    }
    // now over all GUI elements in the tutor tab:
    for ( list<GUITutor*>::iterator it = _tutors.begin(); it != _tutors.end(); it++) {
        (*it)->addSubjectOption(newSubj->getSubject() );
    }
    
}
void inputGUI::addBlankStudent () {
    GUIStudent* newStud = new GUIStudent(this);
    _students.push_back(newStud); // Add new GUI element to list...
    _studentTab->addWidget( (*newStud)() ); // and to the display
}

void inputGUI::changeTutorOptions(Tutor* newtut, Tutor* oldtut) {
    // get the index of the tutor
    vector<Tutor*>::iterator search = find(_tutorIndex.begin(), _tutorIndex.end(), oldtut );
    int index = (int)(search - _tutorIndex.begin());
    
    // update the index with the new tutor
    _tutorIndex[index] = newtut;
    
    // loop over all GUI elements in the student tab, changing the tutor to reflect the changes
    for ( list<GUIStudent*>::iterator it = _students.begin(); it != _students.end(); it++) {
        (*it)->changeTutorOption(newtut, index);
//        (*it)->removeTutorOption(index);
//        (*it)->addTutorOption(newtut, index);
    }
}

void inputGUI::changeSubjectOptions( Subject* newsub, Subject* oldsub) {
    // get the index of the subject
    vector<Subject*>::iterator search = find(_subjectIndex.begin(), _subjectIndex.end(), oldsub );
    int index = (int)(search - _subjectIndex.begin());
    
    // update the index with the new tutor
    _subjectIndex[index] = newsub;
    
    // loop over all GUI elements in the student tab, changing the subject to reflect the changes
    for ( list<GUIStudent*>::iterator it = _students.begin(); it != _students.end(); it++) {
        (*it)->changeSubjectOption(newsub, index);
//        (*it)->removeSubjectOption(index);
//        (*it)->addSubjectOption(newsub, index);
        
    }
    // now loop over all GUI elements in the tutor tab
    for ( list<GUITutor*>::iterator it = _tutors.begin(); it != _tutors.end(); it++) {
        (*it)->changeSubjectOption(newsub, index);
//        (*it)->removeSubjectOption(index);
//        (*it)->addSubjectOption(newsub, index);
    }
    
}

void inputGUI::removeGUITutor(GUITutor* t)
{
    // get the index of the tutor, then remove from the index
    vector<Tutor*>::iterator search = find(_tutorIndex.begin(), _tutorIndex.end(), t->getTutor());
    int currIndex = (int)(search - _tutorIndex.begin());
    _tutorIndex.erase(search);
    
    if (currIndex != t->getTutor()->getID() - 1) {printf("\nSeems that you were wrong.\n"); exit(1); }
    
    // loop over all GUI elements in the student tab, removing this new tutor from the options
    for ( list<GUIStudent*>::iterator it = _students.begin(); it != _students.end(); it++) {
        (*it)->removeTutorOption(currIndex);
    }
    
    //loop over the rest of the tutors, decrementing their IDs to ensure that we still have sequential IDs
    for ( list<GUITutor*>::iterator it = _tutors.begin(); it != _tutors.end(); it++) {
        int loopID = (*it)->getTutor()->getID();
        if ( loopID > t->getTutor()->getID() )
            (*it)->decrementID();
    }
    
    //reduce the next id
    GUITutor::decrementGlobalID();
    
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
    // get the index of the subject, then remove from the index
    vector<Subject  *>::iterator search = find(_subjectIndex.begin(), _subjectIndex.end(), s->getSubject());
    int currIndex = (int)(search - _subjectIndex.begin());
    _subjectIndex.erase(search);
    
    // loop over all GUI elements in the student tab, removing this subject from the options
    for ( list<GUIStudent*>::iterator it = _students.begin(); it != _students.end(); it++) {
        (*it)->removeSubjectOption(currIndex);
    }
    
    // loop over all GUI elements in the tutor tab, removing this subject from the options
    for ( list<GUITutor*>::iterator it = _tutors.begin(); it != _tutors.end(); it++) {
        (*it)->removeSubjectOption(currIndex);
    }
    
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
    _submitLabel = new WText("");
    _submitTab->addWidget(_submitLabel);
    
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


void GUITutor::callUpdate() {
    
    Tutor* n = new Tutor(*_tutor);
    
    n->setID( _ID );
    n->setName( _name->valueText().narrow() );
    
    
    if (!_parent->getSubjectIndex().empty())
    {
        // get the set of indexes that are selected
        set<int> subjects = _subjects->selectedIndexes();
        
        //clear this tutor's subjects
        n->clearSubjects();
        
        //lookup each index in the reference and add the resulting subject to this tutor
        for ( set<int>::iterator it=subjects.begin(); it!=subjects.end(); it++)
        {
            // debug
            cerr << "Adding subject " << _parent->getSubjectIndex()[ *it ]->getName() << " to tutor " << n->getName() << ".\n";
            
            n->addSubject( _parent->getSubjectIndex()[ *it ] );
        }
    }
    
    // clear the tutor's notTimes, then...
    n->clearNotTimes();
    // Loop over all the selected notTimes and add them to the tutor
    set<int> selection = _notSlots->selectedIndexes();
    for (set<int>::iterator it=selection.begin(); it!=selection.end(); it++) {
        n->addNotTime(*it);
    }
    //debug
    list<int> nottimes = n->getNotTimes();
    
    // Update the other elements that use this tutor
    _parent->changeTutorOptions(n, _tutor);
    
    // update the _tutor object in this element
    _tutor = n;
}

void GUISubject::callUpdate() {
    
    Subject* n = new Subject(*_subject);
    
    n->setID( _ID );
    n->setName( _name->valueText().narrow() );
    
    // Update the other elements that use this tutor
    _parent->changeSubjectOptions(n, _subject);
    
    // update the object in this element
    _subject = n;
}

void GUIStudent::callUpdate() {
    
    Student* n = new Student(*_student);

    n->setName( _name->valueText().narrow() );
    n->setNumInterviews( boost::lexical_cast<int>( _numInterviews->valueText().narrow() ));
    
    // If we have any subjects, update the subject
    if (!_parent->getSubjectIndex().empty())
        n->setSubject( _parent->getSubjectIndex()[_subject->currentIndex()] );

    // if we have any tutors, update the prevtutor
    if (!_parent->getTutorIndex().empty()) {
        // Clear tutors and then re add from the index
        set<int> selection = _prevTutors->selectedIndexes();
        n->clearPrevTutors();
        for (set<int>::iterator it = selection.begin(); it!=selection.end(); it++) {
            n->addPrevTutor( _parent->getTutorIndex()[ *it ] );
        }
    }

    // update the object in this element
    _student = n;
}

void inputGUI::resetIDs() {
    GUITutor::resetID();
    GUISubject::resetID();
}


void inputGUI::submit() {
    
    // If we have at least one tutor and at least one subject
    if ( !_subjectIndex.empty() && !_tutorIndex.empty() ) {
        _submitLabel->setText("Submitting...");
        
        // declare containers
        hash_map<int, Tutor*> tutors; hash_map<int, Subject*> subjects; list<Student*> students;
        
        // loop over all the subjects
        for (list<GUISubject*>::iterator it=_subjects.begin(); it!=_subjects.end(); it++) {
            int id = (*it)->getSubject()->getID();
            Subject* subject = (*it)->getSubject();
            
            subjects[id] = subject;
            
            //debug edit
            static hash_map<int, bool> test;
            if (it==_subjects.begin())
                test.clear();

            if (test[id]==true) { printf("Subject fail\n\n\n\n"); exit(-1); }
            else test[id]=true;
        }
        
        // loop over all the tutors
        for (list<GUITutor*>::iterator it=_tutors.begin(); it!=_tutors.end(); it++) {
            int id = (*it)->getTutor()->getID();
            Tutor* tutor = (*it)->getTutor();
            
            tutors[id] = tutor;
            
#ifdef DEBUG
            Tutor* debugtest = tutors[id];
            
            static hash_map<int, bool> test;
            if (it==_tutors.begin()) test.clear();
            if (test[id]==true) {
                printf("Tutor fail\n\n\n\n");
                exit(-1);
            }
            else test[id]=true;
#endif
        }
        
        // loop over all the students
        for (list<GUIStudent*>::iterator it=_students.begin(); it!=_students.end(); it++) {
            int id = (*it)->getStudent()->getID();
            Student* student = (*it)->getStudent();
            
            students.push_back(student);
            
            // Create duplicates for each interview required over 1
            for (int i=1; i < student->getNoInterviews(); i++) {
                students.push_back( new Student(student) );
            }
            
#ifdef DEBUG
            static hash_map<int, bool> test;
            if (it==_students.begin()) test.clear();
            if (test[id]==true) { printf("Student fail\n\n\n\n"); exit(-1); }
            else test[id]=true;
#endif
        }
        
        Configuration::getInstance().setup( tutors, subjects, students );
        
#ifdef DEBUG
        Configuration::getInstance().dumpTutors();
#endif
        
        // Redirect to the output page
        TimetablerWebApplication* app = dynamic_cast<TimetablerWebApplication*>( WApplication::instance() );
        app->pageReady();
        
    } else {
        _submitLabel->setText("You must have at least one tutor and subject!");
    }
        
        
}
