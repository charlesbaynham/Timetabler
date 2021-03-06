//
//  inputGUI.cpp
//  Timetabler
//
//  Created by Charles Baynham on 08/07/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "inputGUI.h"
#include <boost/lexical_cast.hpp>

// cheat to get file_copy to work:
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>

#include <unordered_set>


int GUITutor::_nextID = 1;
int GUISubject::_nextID = 1;

GUITutor::GUITutor(inputGUI* parent, Tutor* tutor) :
    GUIelement(parent)
{    
    if (tutor != NULL) {
        _ID = tutor->getID();
        _nextID++;
        _tutor = tutor;
    }
    else {
        _ID = _nextID++;
        map<Subject*, float> emptymap;
        _tutor = new Tutor(_ID, "", emptymap, NULL);
    }
    
    _nameLabel = new WText("Name:", _visOutput);
    _name = new WLineEdit(_visOutput);
    _name->setText(_tutor->getName());
    
    _subjectsLabel = new WText("Subjects:",_visOutput);
    
    _subjectsCont = new WContainerWidget(_visOutput);
    _subjectsCont->setInline(true);
    _subjectsCont->setStyleClass("subjectsCont");
    
    //loop over the index and populate the list
    vector<Subject*> subjectIndex = _parent->getSubjectIndex();
    
    for (vector<Subject*>::iterator it = subjectIndex.begin(); it != subjectIndex.end(); it++) {
        subjectChoice newSub(*it, _subjectsCont);
        
        //update handler:
        newSub.dropdown->changed().connect( boost::bind( &GUITutor::callUpdate, this ) );
        
        _subjectsL.push_back(newSub);
    }
    
    
    // select any subjects that are in map<Subject*, float> subjects
    
    map<Subject*, float> subjects = _tutor->getSubjects();
    
    // for each subject, check to see if this tutor already has an entry for it
    for (list<subjectChoice>::iterator it = _subjectsL.begin(); it!= _subjectsL.end(); it++) {
        
        map<Subject*, float>::iterator foundSubject = subjects.find((*it).subject);
        if (foundSubject != subjects.end() ) {
            float prof = (*foundSubject).second;
            if (prof <= PROFICIENCY_SOME-0.01)
                ;
            else if (prof<= PROFICIENCY_PROFICIENT - 0.01)
                (*it).dropdown->setCurrentIndex(1);
            else if (prof<= PROFICIENCY_EXPERT - 0.01)
                (*it).dropdown->setCurrentIndex(2);
            else
                (*it).dropdown->setCurrentIndex(3);
        }
            
    }
    
    
    _notLabel = new WText("Unavailable times:",_visOutput);
    _notSlots = new WSelectionBox(_visOutput);
        //set the selection box to allow multiple selections
        _notSlots->setSelectionMode(SelectionMode::ExtendedSelection);
        // Add the slot options
        for (int i=0; i<SLOTS_IN_DAY; i++) { _notSlots->addItem( to_string(i) ); }
    
    // select any times that are in list<int> notTimes
    set<int> selectedTimes;
    for (int i=0; i < SLOTS_IN_DAY; i++) {
        list<int> notTimes = _tutor->getNotTimes();
        if (find(notTimes.begin(), notTimes.end(), i) != notTimes.end() )
            selectedTimes.insert(i);
    }
    _notSlots->setSelectedIndexes(selectedTimes);

    
    _name->setValidator(new WValidator(true));
    
    addDeleteButton();
    
    //register update handler
    _name->changed().connect( boost::bind( &GUITutor::callUpdate, this ) );
    _notSlots->changed().connect( boost::bind( &GUITutor::callUpdate, this ) );
//    _subjects->changed().connect( boost::bind( &GUITutor::callUpdate, this ) );
}

GUISubject::GUISubject(inputGUI* parent, Subject* subject) :
    GUIelement(parent)
{
    if (subject != NULL) {
        _ID = subject->getID();
        _subject = subject;
    }
    else
    {
        _ID = _nextID++;
        _subject = new Subject(_ID, "");
    }
    
    _nameLabel = new WText("Name:", _visOutput);
    _name = new WLineEdit(_visOutput);
    _name->setText(_subject->getName());
    
    addDeleteButton();
    
    //register update handler
    _name->changed().connect( boost::bind( &GUISubject::callUpdate, this ) );
}
GUIStudent::GUIStudent(inputGUI* parent, Student* student) :
    GUIelement(parent)
{
    if (student==NULL)
        _student = new Student( 0 , "", NULL, 2, NULL, NULL);
    else
        _student = student;
    
    _nameLabel = new WText("Name:", _visOutput);
    _name = new WLineEdit(_visOutput);
    _name->setText(_student->getName());
    
    _interviewsLabel = new WText("Number of interviews:", _visOutput);
    _numInterviews = new WComboBox(_visOutput);
        _numInterviews->addItem("2");
        _numInterviews->addItem("4");
        if (_student->getNoInterviews() == 2) _numInterviews->setCurrentIndex(0);
        else _numInterviews->setCurrentIndex(1);
    
    _subjectLabel = new WText("Subject:", _visOutput);
    _subject = new WComboBox(_visOutput);
    //loop over the index and populate the new selection box
        vector<Subject*> subjectIndex = _parent->getSubjectIndex();
        for (vector<Subject*>::iterator it = subjectIndex.begin(); it != subjectIndex.end(); it++)
            _subject->addItem( (*it)->getName() );
    
    // loop over subject index to find the student's subject then select it
    for (int i =0; i < subjectIndex.size(); i++) {
        Subject* searchSubj = _student->getSubject();
        if (subjectIndex[i] == searchSubj ) {
            _subject->setCurrentIndex(i);
            break;
        }
    }
    
    _prevLabel = new WText("Previous tutors:", _visOutput);
    _prevTutors = new WSelectionBox(_visOutput);
        // Make into multi select box
        _prevTutors->setSelectionMode(SelectionMode::ExtendedSelection);
        //loop over the index and populate the new selection box
        vector<Tutor*> tutorIndex = _parent->getTutorIndex();
        for (vector<Tutor*>::iterator it = tutorIndex.begin(); it != tutorIndex.end(); it++)
            _prevTutors->addItem( (*it)->getName() );
    
    // select any tutors that are in list<Tutor*> prevTutors
    set<int> selectedPrev;
    list<Tutor*> tutors = _student->getPrevTutors();
    for (int i=0; i < tutorIndex.size(); i++) {
        Tutor* searchTut = tutorIndex[i];
        
        if (find(tutors.begin(), tutors.end(), searchTut ) != tutors.end() )
            selectedPrev.insert(i);
    }
    _prevTutors->setSelectedIndexes(selectedPrev);

    
    _notLabel = new WText("Unavailable times:",_visOutput);
        _notSlots = new WSelectionBox(_visOutput);
        //set the selection box to allow multiple selections
        _notSlots->setSelectionMode(SelectionMode::ExtendedSelection);
        // Add the slot options
        for (int i=0; i<SLOTS_IN_DAY; i++) { _notSlots->addItem( to_string(i) ); }
    
    // select any times that are in list<int> notTimes
    set<int> selectedTimes;
    for (int i=0; i < SLOTS_IN_DAY; i++) {
        list<int> notTimes = _student->getNotTimes();
        if (find(notTimes.begin(), notTimes.end(), i) != notTimes.end() )
            selectedTimes.insert(i);
    }
    _notSlots->setSelectedIndexes(selectedTimes);

    
    // Add the delete button inherited from GUIelement
    addDeleteButton();
    
    //register update handler
    _name->changed().connect( boost::bind( &GUIStudent::callUpdate, this ) );
    _numInterviews->changed().connect( boost::bind( &GUIStudent::callUpdate, this ) );
    _subject->changed().connect( boost::bind( &GUIStudent::callUpdate, this ) );
    _prevTutors->changed().connect( boost::bind( &GUIStudent::callUpdate, this ) );
    _notSlots->changed().connect( boost::bind( &GUIStudent::callUpdate, this ) );
}

void inputGUI::addBlankTutor () {
    
    addFilledTutor(NULL);
}

GUITutor* inputGUI::addFilledTutor (Tutor* tutor) {
    GUITutor* newTut = new GUITutor(this, tutor);
    _tutors.push_back(newTut); // Add new GUI element to list...
    _tutorTab->addWidget( (*newTut)() ); // and to the display
    
    // Add to the index list, then
    // loop over all GUI elements in the student tab, adding this new tutor to the options
    Tutor* tutObj = newTut->getTutor();
    _tutorIndex.push_back(tutObj);
    for ( list<GUIStudent*>::iterator it = _students.begin(); it != _students.end(); it++) {
        (*it)->addTutorOption(newTut->getTutor());
    }
    
    return newTut;
}

void inputGUI::addBlankSubject () {
    
    addFilledSubject(NULL);
    
}

GUISubject* inputGUI::addFilledSubject (Subject* subject) {
    GUISubject* newSubj = new GUISubject(this, subject);
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
    
    return newSubj;
}

GUIStudent* inputGUI::addBlankStudent () {
    
    return addFilledStudent(NULL);
}

GUIStudent* inputGUI::addFilledStudent (Student* student) {
    
    GUIStudent* newStud = new GUIStudent(this, student);
    
    _students.push_back(newStud); // Add new GUI element to list...
    _studentTab->addWidget( (*newStud)() ); // and to the display
    
    return newStud;
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
    
    // update the index with the new subject
    _subjectIndex[index] = newsub;
    
    // loop over all GUI elements in the student tab, changing the subject to reflect the changes
    for ( list<GUIStudent*>::iterator it = _students.begin(); it != _students.end(); it++) {
        (*it)->changeSubjectOption(newsub, index);
//        (*it)->removeSubjectOption(index);
//        (*it)->addSubjectOption(newsub, index);
        
    }
    // now loop over all GUI elements in the tutor tab
    for ( list<GUITutor*>::iterator it = _tutors.begin(); it != _tutors.end(); it++) {
        (*it)->changeSubjectOption(oldsub, newsub);
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
    
    
    // loop over all GUI elements in the student tab, removing this tutor from the options
    for ( list<GUIStudent*>::iterator it = _students.begin(); it != _students.end(); it++) {
        (*it)->removeTutorOption(currIndex);
    }
    
    //loop over the rest of the tutors, decrementing their IDs to ensure that we still have sequential IDs
    for ( list<GUITutor*>::iterator it = _tutors.begin(); it != _tutors.end(); it++) {
        int loopID = (*it)->getTutor()->getID();
        if ( loopID > t->getTutor()->getID() )
            (*it)->decrementID();
    }
    
    // If we're using a previous solution, remove this tutor from it
    if (Configuration::getInstance().prevSolutionLoaded())
        Configuration::getInstance().removeTutorFromPrev(t->getTutor());
    
    
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
    
    // get the index of the subject
    vector<Subject*>::iterator search = find(_subjectIndex.begin(), _subjectIndex.end(), s->getSubject());
    int currIndex = (int)(search - _subjectIndex.begin());
    
    // loop over all GUI elements in the student tab, removing this subject from the options
    for ( list<GUIStudent*>::iterator it = _students.begin(); it != _students.end(); it++) {
        (*it)->removeSubjectOption(currIndex);
    }
    
    // loop over all GUI elements in the tutor tab, removing this subject from the options
    for ( list<GUITutor*>::iterator it = _tutors.begin(); it != _tutors.end(); it++) {
        (*it)->removeSubjectOption(*search);
    }
    
    // remove the subject from the index
    _subjectIndex.erase(search);
    
    // remove the visible widget
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
//    if
    
    string filename = "out.ttcfg";
    // If previous config file is detected, ask user if they want to use it
    if (boost::filesystem::exists(filename) ) {
        
        WContainerWidget* topbox = new WContainerWidget(parent);
        
        topbox->addWidget(new WText("Previous config detected. Would you like to load it? The timetabler will attempt to minimise alterations. "));
        
        WPushButton* usePreviousConfig = new WPushButton("Yes", topbox);
        WPushButton* nusePreviousConfig = new WPushButton("No", topbox);
        topbox->addWidget(new WText("  Upload other:"));
        WFileUpload* useOther = new WFileUpload(topbox);
        WPushButton* erase = new WPushButton("Erase previous", topbox);
        
        useOther->uploaded().connect( boost::bind<void> ( [=](){
            useOther->disable();
            this->usePrevious(useOther->spoolFileName());
            //boost::filesystem::copy_file(useOther->spoolFileName(), filename, boost::filesystem::copy_option::overwrite_if_exists);
            parent->removeWidget(topbox);
        }));
        useOther->changed().connect( boost::bind<void> ( [=]() { useOther->upload(); }));
        
        usePreviousConfig->clicked().connect( boost::bind( &inputGUI::usePrevious, this, filename ));
        usePreviousConfig->clicked().connect(boost::bind<void> ( [=](){ parent->removeWidget(topbox);  }));
        nusePreviousConfig->clicked().connect(boost::bind<void> ( [=](){ parent->removeWidget(topbox);  }));
        erase->clicked().connect(boost::bind<void> ( [=](){ remove(filename.c_str()); parent->removeWidget(topbox);  }));
    
    } else {
        WContainerWidget* topbox = new WContainerWidget(parent);
        
        topbox->addWidget(new WText("Would you like to load a previous configuration? The timetabler will attempt to minimise alterations. "));
        
        WFileUpload* useOther = new WFileUpload(topbox);
        WPushButton* nusePreviousConfig = new WPushButton("No", topbox);

        
        useOther->uploaded().connect( boost::bind<void> ( [=](){
            useOther->disable();
            this->usePrevious(useOther->spoolFileName());
            boost::filesystem::copy_file(useOther->spoolFileName(), filename, boost::filesystem::copy_option::overwrite_if_exists);
            parent->removeWidget(topbox);
        }));
        useOther->changed().connect( boost::bind<void> ( [=]() { useOther->upload(); }));
        
        nusePreviousConfig->clicked().connect(boost::bind<void> ( [=](){ parent->removeWidget(topbox);  }));
    }
    
    // create a menu
    _menu = new WTabWidget(parent);
    
//    _menu->setInternalPathEnabled();
//    _menu->setInternalBasePath("/input");
        
    // Add the tabs to the menu
    _menu->addTab(_subjectTab, "Subjects");
    _menu->addTab(_tutorTab, "Tutors");
    _menu->addTab(_studentTab, "Students");
    
    _menu->setStyleClass("menu");

// Submit tab: 
    _menu->addTab(_submitTab, "Submit");
    WPushButton* submit = new WPushButton("Submit");
    _submitTab->addWidget(submit);
    submit->clicked().connect( this, &inputGUI::submit );
    _submitLabel = new WText("");
    _submitTab->addWidget(_submitLabel);
    
    this->createAddButtons();
    
}

void inputGUI::createAddButtons() {
    
    //Add explainatory label
    _tutorTab->addWidget(new WText("Hold cmd to select / deselect multiple choices"));
    _tutorTab->addWidget(new WBreak());
    _studentTab->addWidget(new WText("Hold cmd to select / deselect multiple choices"));
    _studentTab->addWidget(new WBreak());
    
    //Add "new" buttons
    WPushButton* newTut = new WPushButton("Add new");
    WPushButton* newStud = new WPushButton("Add new");
    WPushButton* newSubj = new WPushButton("Add new");
    
    newTut->clicked().connect( this, &inputGUI::addBlankTutor );
    newStud->clicked().connect(std::bind([=] () { this->addBlankStudent(); }));
    newSubj->clicked().connect( this, &inputGUI::addBlankSubject );
    
    _tutorTab->addWidget(newTut);
    _studentTab->addWidget(newStud);
    _subjectTab->addWidget(newSubj);
    
    // Add "import from CSV" buttons and upload areas :
    
    // Students
    WFileUpload* impStudUp = new WFileUpload(_studentTab);
    impStudUp->hide();
    
    WProgressBar* impStudProg = new WProgressBar(_studentTab);
    impStudUp->setProgressBar(impStudProg);
    impStudProg->hide();
    
    WText* impStudOut = new WText("", _studentTab);
    
    WPushButton* impStud = new WPushButton("Import from CSV list");
    impStud->clicked().connect(boost::bind( &inputGUI::setupUpload, this, impStud, impStudUp, impStudProg, impStudOut, TYPE_STUDENT ) );
    _studentTab->addWidget(impStud);
    
    // Tutors
    WFileUpload* impTutUp = new WFileUpload(_tutorTab);
    impTutUp->hide();
    
    WProgressBar* impTutProg = new WProgressBar(_tutorTab);
    impTutUp->setProgressBar(impTutProg);
    impTutProg->hide();
    
    WText* impTutOut = new WText("", _tutorTab);
    
    WPushButton* impTut = new WPushButton("Import from CSV list");
    impTut->clicked().connect(boost::bind( &inputGUI::setupUpload, this, impTut, impTutUp, impTutProg, impTutOut, TYPE_TUTOR ) );
    _tutorTab->addWidget(impTut);
    
    // Subjects
    WFileUpload* impSubUp = new WFileUpload(_subjectTab);
    impSubUp->hide();
    
    WProgressBar* impSubProg = new WProgressBar(_subjectTab);
    impSubUp->setProgressBar(impSubProg);
    impSubProg->hide();
    
    WText* impSubOut = new WText("", _subjectTab);
    
    WPushButton* impSub = new WPushButton("Import from CSV list");
    impSub->clicked().connect(boost::bind( &inputGUI::setupUpload, this, impSub, impSubUp, impSubProg, impSubOut, TYPE_SUBJECT ) );
    _subjectTab->addWidget(impSub);
    
}

void inputGUI::setupUpload(WPushButton* button, WFileUpload* upload, WProgressBar* prog, WText* out, int type ) {
    
    button->hide();
    upload->show();
    prog->show();
    
    //set max filesize
    upload->setFileTextSize(256);
    
    // Upload automatically when the user entered a file.
    upload->changed().connect( upload, &WFileUpload::upload );
    
    // React to a successful upload.
    upload->uploaded().connect(std::bind([=] () {
        out->setText("File upload is finished.");
        upload->disable();
    }));
    
    if (type == 1) upload->uploaded().connect(std::bind([=] () { this->importSubjects(upload->spoolFileName()); }));
    else if (type == 2) upload->uploaded().connect(std::bind([=] () { this->importTutors(upload->spoolFileName()); }));
    else if (type == 3) upload->uploaded().connect(std::bind([=] () { this->importStudents(upload->spoolFileName()); }));
    
    
    // React to a file upload problem.
    upload->fileTooLarge().connect(std::bind([=] () {
        upload->show();
        out->setText("File is too large.");
    }));
    
}

int inputGUI::importStudents(string filename) {
    
    submit(true);
    
//    list<string> names = parseCSV(filename);
    list<Student*> students = parseCSVStudent(filename);
    
    // add these new students
    for (list<Student*>::iterator it = students.begin(); it!=students.end(); it++) {
        if (*it != NULL) {
            // add the GUI object
            GUIStudent* newstud = addFilledStudent(*it);
            //update the Student object with the contents of the student just created
            newstud->callUpdate();
        }
    }
    
    return 0;
}

int inputGUI::importTutors(string filename) {
    
    submit(true);
    
//    list<string> names = parseCSV(filename);
    list<Tutor*> tutors = parseCSVTutor(filename);
    
    
    // add these new tutors
    for (list<Tutor*>::iterator it = tutors.begin(); it!=tutors.end(); it++) {

        if (*it != NULL) {
            // add the GUI object
            GUITutor* newtut = addFilledTutor(*it);
            //update the Student object with the contents of the tutor just added
            newtut->callUpdate();
        }
    }
    
    return 0;
}

int inputGUI::importSubjects(string filename) {

    submit(true);
    
    list<string> names = parseCSV(filename);
    
    // add these new subjects
    for (list<string>::iterator it = names.begin(); it!=names.end(); it++) {
        *it = Configuration::TrimString(*it);
        if (*it != "") {
            // add the GUI object
            GUISubject* newsubj = addFilledSubject(new Subject(0, *it));
            //update the Student object witht he contents of the text fields just created
            newsubj->callUpdate();
        }
    }
    
    return 0;
}

list<string> inputGUI::parseCSV(string filename) {
    
    ifstream in(filename.c_str());
    if (!in.is_open()) return NULL;
    
    typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;
    
    // setup the CSV parsing: use '/' for escape, ',' ';' TAB or '\r\n' for seperators and ' or " for quotation marks
    boost::escaped_list_separator<char> sep("\\", ",;\t\r\n", "\"\'");
    
    vector< string > vec;
    list<string> names;
    string line;
    
    while (getline(in,line))
    {
        Tokenizer tok(line, sep);
        
        copy(tok.begin(), tok.end(), back_inserter(names) );
        
#ifdef DEBUG
        copy(tok.begin(), tok.end(),
             ostream_iterator<string>(cout, "|"));
#endif
        
    }
    
    return names;
}

list<Tutor*> inputGUI::parseCSVTutor(string filename) {
    
    ifstream in(filename.c_str());
    if (!in.is_open()) return NULL;
    
    typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;
    
    // setup the CSV parsing: use '/' for escape, ';' or TAB for seperators and ' or " for quotation marks
    boost::escaped_list_separator<char> sep("\\", ";\t", "\"\'");
    
    // for lists
    boost::escaped_list_separator<char> sepComma("\\", ",", "\"\'");
    
    vector< string > vec;
    list<Tutor*> tutors;
    string line;
    
    while (getline(in,line))
    {
        Tutor* n;
        
        Tokenizer tok(line, sep);
        
        //         get the tutor object, formatted as:
        //         Name ; Subject1,prof1, subject2, prof2... ; Unavailable t1, t2, t3
        //        eg
        //         Charles ; Physics,100,Maths,75,"Fluid Mechanics",75 ; 0,1,2,3
        
        Tokenizer::iterator it = tok.begin();
        
        string name;
        string subjectsStr;
        string timesStr;
        list<int> times;
        
        map<Subject*, float> empty;
        if (it != tok.end()) {
            name = *it;
            n = new Tutor(0, Configuration::TrimString(name), empty, NULL);
            
            
            if (++it != tok.end()) {
                subjectsStr = *it;
                subjectsStr = Configuration::TrimString(subjectsStr);
                
                
                if (++it != tok.end()) {
                    timesStr = *it;
                    timesStr = Configuration::TrimString(timesStr);
                }
            }
        }
        
        
        //Split up the subjects:
        Tokenizer tokSub(subjectsStr, sepComma);
        string subName;
        
        for (Tokenizer::iterator itSub = tokSub.begin(); itSub!=tokSub.end(); itSub++) {
            if (subName == "") {
                subName = *itSub;
                subName = Configuration::TrimString(subName);
            }
            else {
                string profStr = (*itSub);
                float prof = boost::lexical_cast<int>(Configuration::TrimString( profStr) );
                Subject* subject = Configuration::getInstance().getSubject(subName);
                if (subject!=NULL) {
                    n->addSubject(subject, prof/100);
#ifdef DEBUG
                    cout << "Adding subject " << subject->getName() << ", prof " << prof << "%" << endl;
#endif
                }
                subName = "";
            }
        }
        
        //Split up the times:
        Tokenizer tokTime(timesStr, sepComma);
        
        for (Tokenizer::iterator itTime = tokTime.begin(); itTime!=tokTime.end(); itTime++) {
            string time = *itTime;
            time = Configuration::TrimString(time);
            
#ifdef DEBUG
            cout << "Adding notTime " << boost::lexical_cast<int>(time) << endl;
#endif
            
            n->addNotTime( boost::lexical_cast<int>(time) );
            
        }
        
        tutors.push_back(n);
        
    }
    
    return tutors;
}

list<Student*> inputGUI::parseCSVStudent(string filename) {
    
    ifstream in(filename.c_str());
    if (!in.is_open()) return NULL;
    
    typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;
    
    // setup the CSV parsing: use '/' for escape, ';' or TAB for seperators and ' or " for quotation marks
    boost::escaped_list_separator<char> sep("\\", ";\t", "\"\'");
    
    // for lists
    boost::escaped_list_separator<char> sepComma("\\", ",", "\"\'");
    
    vector< string > vec;
    list<Student*> students;
    string line;
    
    while (getline(in,line))
    {
        Student* n;
        
        Tokenizer tok(line, sep);
        
        //         get the student object, formatted as:
        //         Name ; Subject ; Num Interviews ; Unavailable t1, t2, t3 ; prevtut1, prevtut2, prevtut3...
        //        eg
        //         Charles ; Physics ; 4 ; ; "Jack Fraser"
        
        Tokenizer::iterator it = tok.begin();
        
        string name;
        string subjectStr;
        string numIntStr;
        string timesStr;
        string prevStr;
        Subject* subject;
        list<int> times;
        int numInterviews=0;
        
        map<Subject*, float> empty;
        if (it != tok.end()) { // import the name
            name = *it;
            name = Configuration::TrimString(name);
            
            if (++it != tok.end()) { // import the subject
                subjectStr = *it;
                subjectStr = Configuration::TrimString(subjectStr);
                
                subject = Configuration::getInstance().getSubject(subjectStr);
                
                if (++it != tok.end()) { // get numInterviews
                    numIntStr = *it;
                    numIntStr = Configuration::TrimString(numIntStr);
                    numInterviews = boost::lexical_cast<int>(numIntStr);
                    
                    if (++it != tok.end()) { // get NotTimes
                        timesStr = *it;
                        timesStr = Configuration::TrimString(timesStr);
                    
                        if (++it != tok.end()) { // get prevTutors
                            prevStr = *it;
                            prevStr = Configuration::TrimString(prevStr);
                        }
                    }
                }
            }
        }
        
        // skip if essential info not provided
        if ( name == "" || subject == NULL || numInterviews == 0 )
            break;
        
        // else, initialise the new student
        n = new Student(0, name, subject, numInterviews, NULL, NULL);
        
        //Split up the tutors and add them:
        Tokenizer tokSub(prevStr, sepComma);
        
        for (Tokenizer::iterator itTut = tokSub.begin(); itTut!=tokSub.end(); itTut++) {
            
            string tutName = *itTut;
            tutName = Configuration::TrimString(tutName);
        
            Tutor* tutor = Configuration::getInstance().getTutor(tutName);
            if (tutor!=NULL) {
                n->addPrevTutor(tutor);
#ifdef DEBUG
                cout << "Adding tutor " << tutor->getName() << endl;
#endif
            }
        }
        
        //Split up the times and add them:
        Tokenizer tokTime(timesStr, sepComma);
        
        for (Tokenizer::iterator itTime = tokTime.begin(); itTime!=tokTime.end(); itTime++) {
            string time = *itTime;
            time = Configuration::TrimString(time);
            
#ifdef DEBUG
            cout << "Adding notTime " << boost::lexical_cast<int>(time) << endl;
#endif
            
            n->addNotTime( boost::lexical_cast<int>(time) );
            
        }
        
        students.push_back(n);
        
    }
    
    return students;
}

void GUITutor::callUpdate() {
    
    Tutor* n = new Tutor(*_tutor);
    
    n->setID( _ID );
    n->setName( _name->valueText().narrow() );
    
    // if we have any subjects
    if (!_parent->getSubjectIndex().empty())
    {
        //clear this tutor's subjects
        n->clearSubjects();

        // loop over all the subject choices
        for (list<subjectChoice>::iterator it = _subjectsL.begin(); it!=_subjectsL.end(); it++) {
            // if there's a non-zero proficiency, add it to the tutor
            int profIndex = (*it).dropdown->currentIndex();
            
            if (profIndex == 0) ;
            else if (profIndex == 1) n->addSubject((*it).subject, PROFICIENCY_SOME);
            else if (profIndex == 2) n->addSubject((*it).subject, PROFICIENCY_PROFICIENT);
            else n->addSubject((*it).subject, PROFICIENCY_EXPERT);
            
        }
        
//        // get the set of indexes that are selected
//        set<int> subjects = _subjects->selectedIndexes();
//        
//                
//        //lookup each index in the reference and add the resulting subject to this tutor
//        for ( set<int>::iterator it=subjects.begin(); it!=subjects.end(); it++)
//        {
//            // debug
//            cerr << "Adding subject " << _parent->getSubjectIndex()[ *it ]->getName() << " to tutor " << n->getName() << ".\n";
//            
//            n->addSubject( _parent->getSubjectIndex()[ *it ], 1.0 ); // edit debug
//        }
    }
    
    // clear the tutor's notTimes, then...
    n->clearNotTimes();
    // Loop over all the selected notTimes and add them to the tutor
    set<int> selection = _notSlots->selectedIndexes();
    for (set<int>::iterator it=selection.begin(); it!=selection.end(); it++) {
        n->addNotTime(*it);
    }
//    //debug
//    list<int> nottimes = n->getNotTimes();
    
    // Update the other elements that use this tutor
    _parent->changeTutorOptions(n, _tutor);
    
    // update the _tutor object in this element
    _tutor = n;
}

subjectChoice::subjectChoice(Subject* newSub, WContainerWidget* parent) {
    elementContainer = new WContainerWidget(parent);
    elementContainer->setInline(true);
    elementContainer->setStyleClass("subjectChoice");
    label = new WText(newSub->getName() + ":", elementContainer);
    dropdown = new WComboBox(elementContainer);
    subject = newSub;
    
    dropdown->addItem("None");
    dropdown->addItem("Some");
    dropdown->addItem("Proficient");
    dropdown->addItem("Expert");
}

void GUITutor::addSubjectOption(Subject* n) {
    
    subjectChoice newChoice(n, _subjectsCont);
    _subjectsL.push_back(newChoice);
    
}

void GUITutor::removeSubjectOption(Subject* subject){
    
    // loop over the SubjectChoices and remove the appropriate one
    for (list<subjectChoice>::iterator it=_subjectsL.begin(); it!=_subjectsL.end(); it++) {
        if ((*it).subject == subject) {
            _subjectsCont->removeWidget( (*it).elementContainer );
            _subjectsL.erase(it);
        }
    }
    
    // remove this subject from the tutor object
    _tutor->removeSubject(subject);
}

// changes the subject o to subject n
void GUITutor::changeSubjectOption(Subject* o, Subject* n) {
    //        bool selected = ( _subjects->currentIndex() == index );
    //        removeSubjectOption(index);
    //        addSubjectOption(n, index);
    //        if (selected) _subjects->setCurrentIndex(index);
    
    // search subjectChoice list for this subject
    list<subjectChoice>::iterator search = std::find_if( _subjectsL.begin(), _subjectsL.end(), [o](subjectChoice x) {
        return x.subject == o;
    });
    
    if (search != _subjectsL.end()) {
        (*search).subject = n;
        (*search).label->setText(n->getName() + ":");
    }
    
    //        for (list<subjectChoice>::iterator it = _subjectL.begin(); it!=_subjectL.end(); it++) {
    //
    //        }
    
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
    
    // clear the student's notTimes, then...
    n->clearNotTimes();
    // Loop over all the selected notTimes and add them to the student
    set<int> selection = _notSlots->selectedIndexes();
    for (set<int>::iterator it=selection.begin(); it!=selection.end(); it++) {
        n->addNotTime(*it);
    }

    // update the object in this element
    _student = n;
}

void inputGUI::resetIDs() {
    GUITutor::resetGlobalID();
    GUISubject::resetGlobalID();
}


void inputGUI::submit(bool noSolve) {
    
    // If we have at least one tutor and at least one subject
    if ( (!_subjectIndex.empty() && !_tutorIndex.empty()) || noSolve ) {

        if (!noSolve) _submitLabel->setText("Submitting...");
        
        // declare containers
        hash_map<int, Tutor*> tutors;
        hash_map<int, Subject*> subjects;
        list<Student*> students;
        
        
        // loop over all the subjects
        for (list<GUISubject*>::iterator it=_subjects.begin(); it!=_subjects.end(); it++) {
            // update with contents of text boxes
            (*it)->callUpdate();
            
            int id = (*it)->getSubject()->getID();
            Subject* subject = (*it)->getSubject();
            
            subjects[id] = subject;
            
        }
        
        // loop over all the tutors
        for (list<GUITutor*>::iterator it=_tutors.begin(); it!=_tutors.end(); it++) {
            // update with contents of text boxes
            (*it)->callUpdate();
            
            int id = (*it)->getTutor()->getID();
            Tutor* tutor = (*it)->getTutor();
            
            tutors[id] = tutor;
            
#ifdef DEBUG
            static hash_map<int, bool> test;
            if (it==_tutors.begin()) test.clear();
            cout << "Tutor : " << tutor->getName() << " (ID = " << id << ")\n";
            if (test[id]==true) {
                printf("Tutor repeat ID found\n\n\n\n");
                exit(-1);
            }
            else test[id]=true;
#endif
        }
        
        // loop over all the students
        for (list<GUIStudent*>::iterator it=_students.begin(); it!=_students.end(); it++) {
            // update with contents of text boxes
            (*it)->callUpdate();
            
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
        Configuration::getInstance().dumpStudents();
#endif
        
        // Redirect to the output page
        if (!noSolve) {
            TimetablerWebApplication* app = dynamic_cast<TimetablerWebApplication*>( WApplication::instance() );
            app->pageReady();
        }
        
    } else {
        _submitLabel->setText("You must have at least one tutor and subject!");
    }
}


void inputGUI::usePrevious(string filename) {
    
    Configuration::getInstance().parseFile(filename.c_str());
    
#ifdef DEBUG
    Configuration::getInstance().dumpTutors();
    Configuration::getInstance().dumpStudents();
    Configuration::getInstance().dumpSolution();
#endif
    
    // Update the GUI to match the Configuration
    updateFromConfig();
}

void inputGUI::updateFromConfig() {
    
    _students.clear();
    _subjects.clear();
    _tutors.clear();
    _subjectIndex.clear();
    _tutorIndex.clear();
    
    _tutorTab->clear();
    _subjectTab->clear();
    _studentTab->clear();
    
    this->createAddButtons();
    
    int nextSubjectID=1;
    int nextTutorID=1;
    
    ///
    
    // input the subjects
    
    hash_map<int, Subject*> subjects = Configuration::getInstance().getSubjects();
    hash_map<int, Tutor*> tutors = Configuration::getInstance().getTutors();
    list<Student*> students = Configuration::getInstance().getStudents();
    
    for (hash_map<int, Subject*>::iterator it = subjects.begin(); it != subjects.end(); it++) {
        
        // Keep the next subject ID 1 ahead of the current highest
        if ( (*it).second->getID() >= nextSubjectID )
            nextSubjectID = (*it).second->getID() + 1;
        
        addFilledSubject( (*it).second );
        
    }
    
    // input the tutors
    for (hash_map<int, Tutor*>::iterator it = tutors.begin(); it != tutors.end(); it++) {
        
        // Keep the next Tutor ID 1 ahead of the current highest
        nextTutorID++;
        addFilledTutor( (*it).second );
        
    }
    
    // input the students

    //      build of list of which baseIDs have been done so that we don't duplicate:
    unordered_set<int> doneIDs;

    for (list<Student*>::iterator it = students.begin(); it != students.end(); it++) {

        int baseID = (*it)->getBaseID();
        if ( doneIDs.find( baseID ) != doneIDs.end() ) continue;
        doneIDs.insert(baseID);
        
        addFilledStudent( (*it) );
        
    }
    
    // update the nextIDs
    GUITutor::resetGlobalID(nextTutorID);
    GUISubject::resetGlobalID(nextSubjectID);
    
}
