//
//  main.cpp
//  WtGUI
//
//  Created by Charles Baynham on 02/07/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "GUI.h"
#include <functional>

#include <regex>

using namespace std;

#include <boost/algorithm/string/predicate.hpp>

void TimetablerWebApplication::handlePathChange()
{
    Wt::WApplication *app = Wt::WApplication::instance();
    
    // Handle pages (i.e. diffrent URLs).
    //     functions prefaced with "page" draw pages (e.g. pageOutput() )
    string path = app->internalPath();
    if (path == "/output") {
        root()->clear();
        if (Configuration::getInstance().isEmpty()) { // We haven't yet made the configuration
            root()->addWidget(new WText("Error: this page is not meant for direct access. Redirecting to input..."));
            app->setInternalPath("/", true);
        }
        else
            startSolve();
    }
    else if(path == "/") pageInput();
    else if( boost::starts_with(path, "/input") ) app->setInternalPath("/", true); // redirect to input page if using old input path
    else app->setInternalPath("/", true); // Redirect to main page if page not found
    
    
}

void TimetablerWebApplication::startSolve() {

    root()->clear();
    
    // setup a timer which refreshes the timetable (edit: if it's changed) every 2 seconds, until timer->stop() is called.
    _timer = new WTimer();
    _timer->setInterval(200);
    _timer->timeout().connect(this, &TimetablerWebApplication::refreshStats );
    
    
//    _greeting->setText("Solving...");
//    _button->setHidden(true);
    _timer->start();
    
    ////  Start the algorithm:
    
    char* configfile = "config.txt";
    
    // Stop it if it's already running
    Algorithm::GaAlgorithmState state = TimetablerInst::getInstance()->getAlgorithm()->GetState();
    if (state == Algorithm::GaAlgorithmState::GAS_RUNNING)
        TimetablerInst::getInstance()->getAlgorithm()->StopSolving();
    
    // Read in configuration if not already configured
    if ( Configuration::getInstance().isEmpty() ) {
        cerr << "***\nReading Config from file\n";
        if ( Configuration::getInstance().parseFile( configfile ) ) { cerr << "Error when opening config file \"" << configfile << "\". Does it exist?\n"; exit(EXIT_FAILURE); }
    } else cerr << "***\nUsing current config\n";
    
    
#ifdef DEBUG
    cerr << "About to solve:" << endl;
    Configuration::getInstance().dumpTutors();
    

#endif
    
    // Solve!
    TimetablerInst::getInstance()->getAlgorithm()->StartSolving(false);
    
}

void TimetablerWebApplication::refreshStats() {
    
    GaChromosomePtr result;
    GaAlgorithm* algorithm = TimetablerInst::getInstance()->getAlgorithm();
    algorithm->GetPopulation(0).GetBestChromosomes(&result, 0, 1); // store best chromosone in result
    
    float bestFitness = (*result).GetFitness();
    int generation = algorithm->GetAlgorithmStatistics().GetCurrentGeneration();

#if DEBUG
    cerr << "Current generation: "<<generation<<". Best fitness: "<<bestFitness<< endl;
#endif
    
    char out[100];
    sprintf(out, "Gen %i, fitness %f", generation, bestFitness);
    
    _bestFitness->setText(out);
    
    // Build table
    static finishedTT* lastTT;
    finishedTT* timetable = new finishedTT(result.GetRawPtr());
    
    // If it has changed:
    if (lastTT != timetable) {
        buildTable(timetable , _tutors );
        lastTT = timetable;
    }

    // If maximum fitness reached, stop
    bool optimal = ( abs(1.0f - bestFitness) <= 0.000001 );
    if (optimal)
        TimetablerInst::getInstance()->getAlgorithm()->StopSolving();
    
    //if finished, stop checking
    GaAlgorithmState state = TimetablerInst::getInstance()->getAlgorithm()->GetState();
#if DEBUG
    cerr << "The state is " << state << endl;
#endif
    if (state & GaAlgorithmState::GAS_STOPPED ) {
        _timer->stop();
        _bestFitness->setText( "Non-optimal solution found : Fitness "+to_string(100*bestFitness)+"%" );
        if (optimal)
            _bestFitness->setText( "Optimal solution found!" );
#if DEBUG
        cerr<<"Stopping timer on algorithm completion" << endl;
#endif
    }

}


TimetablerWebApplication::TimetablerWebApplication(const Wt::WEnvironment& env)
: Wt::WApplication(env), _bestFitness(new WText(""))
{
    this->enableInternalPaths();
    this->setInternalPathDefaultValid(false);
    
    setTitle("Charles' Timetabler");
    
    this->useStyleSheet("style.css");

    
    Wt::WApplication *app = Wt::WApplication::instance();
    
    app->internalPathChanged().connect(std::bind([=] () {
        handlePathChange();
    }));
    
    handlePathChange();
    
}


void TimetablerWebApplication::pageInput() {
    
    // set to the correct path in case called from another function and the path is currently wrong
    this->setInternalPath("/");
    
    //wipe display
    root()->clear();
    
    // wipe any existing configuration and stop any existing algorithms
    Configuration::getInstance().clear();
    TimetablerInst::getInstance()->getAlgorithm()->StopSolving();
    
    //reset the inputGUI id counters
    inputGUI::resetIDs();
    
    //reset the instance if present
    TimetablerInst::getInstance()->reset();
    
    
//    if (!_inputGUI) {
    
        _inputGUI = new inputGUI(root());
                
        this->processEvents();
    

    
//    }
//    else {
//        root()->clear();
//        _inputGUI->redraw(root());
//    }

}


void TimetablerWebApplication::pageReady() {
    
    //clear page
    root()->clear();
    
    // set to the correct path in case called from another function and the path is currently wrong
    this->setInternalPath("/output");
    
    _greeting = new WText("Ready");
    _greeting->setStyleClass("titleText");
    
    root()->addWidget(_greeting);
    root()->addWidget(new WBreak);
    
    _button = new Wt::WPushButton("Go", root());
    root()->addWidget(new Wt::WBreak());
    
    _status = new WText("");
    root()->addWidget(_status);
    
    _button->clicked().connect(this, &TimetablerWebApplication::startSolve );
    
}

void TimetablerWebApplication::buildTable(finishedTT* timetable, bool tutors)
{
    // If launched in tutor/student mode, save this for next time
    _tutors = tutors;
    
    int entries;
    
    hash_map<int, Tutor*> tutorMap = timetable->getAllTutors();
    hash_map<int, Student*> studentMap = timetable->getAllStudents();
    
    if (tutors) entries = (int)tutorMap.size();
    else entries = (int)studentMap.size();
    
    WContainerWidget *container = root();
    WGridLayout *grid;
    if (!_tableBuilt) {
        
        container->setStyleClass("yellow-box");
        
        grid = new WGridLayout();
        container->setLayout(grid);
        
        // (0,0) contains the status and the controls
        WContainerWidget* toggleButtons = new WContainerWidget;

        WVBoxLayout* buttons = new WVBoxLayout;
        toggleButtons->setLayout(buttons);

        WPushButton *setTutor=new WPushButton("Tutor mode");
        WPushButton *setStudent=new WPushButton("Student mode");
        WPushButton *stop = new WPushButton("Stop");
        
        setTutor->clicked().connect( boost::bind(&TimetablerWebApplication::buildTable, this, timetable, true) );
        setStudent->clicked().connect( boost::bind(&TimetablerWebApplication::buildTable, this, timetable, false) );
        stop->clicked().connect( boost::bind( &GaAlgorithm::StopSolving, TimetablerInst::getInstance()->getAlgorithm() ) );
        stop->clicked().connect( _timer, &WTimer::stop );
        
        buttons->addWidget(_bestFitness);
        buttons->addWidget(setTutor);
        buttons->addWidget(setStudent);
        buttons->addWidget(stop);
        
        grid->addWidget(toggleButtons, 0, 0);
        
        // (0,1) to (0,i) contain time headers. edit
        for (int i=1; i<=SLOTS_IN_DAY; i++) {
            WText *timeWidget = new WText(to_string(i));
            timeWidget->setHeight(75);
            timeWidget->setStyleClass("blue-box");
            timeWidget->addStyleClass("centerme");
            grid->addWidget(timeWidget, 0, i);
            grid->setColumnStretch(i, 1);
        }
        
        // (1,0) to (1,j) contain Student / Tutor names
        WText *nameWidget;
        int j=1;
        if (tutors) {
            for (hash_map<int, Tutor*>::iterator it = tutorMap.begin(); it != tutorMap.end(); it++, j++) {
                // j holds the row
                
                nameWidget = new WText( (*it).second->getName() );
                nameWidget->setStyleClass("green-box");
                nameWidget->setHeight(50);
                grid->addWidget(nameWidget, j, 0);
                grid->setRowStretch(j, 1);
            }
        }
        else {
            for (hash_map<int, Student*>::iterator it = studentMap.begin(); it != studentMap.end(); it++, j++) {
                // j holds the row
                
                nameWidget = new WText( (*it).second->getName() );
                nameWidget->setStyleClass("green-box");
                grid->addWidget(nameWidget, j, 0);
                grid->setRowStretch(j, 1);
                nameWidget->resize(120, 75);
            }
        }
    } else {
        
        grid = dynamic_cast<WGridLayout*>( container->layout() );
        
    }
    
    // fill in the actual appointments
    int j=1;
    if (tutors) {
        for (hash_map<int, Tutor*>::iterator it = tutorMap.begin(); it != tutorMap.end(); it++, j++ ) {
//            static int j=0;
            //Count which row we're on:
//            j++;
            
            for (int time = 0; time < SLOTS_IN_DAY; time++) {
                // This slot's student
                Student* student;
                // the text for this slot
                WText* appt;
                
                // If we found a student, get their name. Else, the tutor has a free slot
                if ( (student = timetable->getTutorOrientated()->getTutorApt( (*it).second, time )) )
                    appt = new WText( student->getName() );
                else
                    appt = new WText( "-" );
                
                // add to the output grid
                appt->setStyleClass("appt");
                grid->addWidget(appt, j, time+1);
            }
        }
        
    }
    else {
        for (hash_map<int, Student*>::iterator it = studentMap.begin(); it != studentMap.end(); it++, j++) {
            //j holds which row we're on.
            
            for (int time = 0; time < SLOTS_IN_DAY; time++) {
                // This slot's tutor
                Tutor* tutor;
                // the text for this slot
                WText* appt;
                
                // If we found a tutor, get their name. Else, the student has a free slot
                if ( (tutor = timetable->getStudentOrientated()->getStudentApt( (*it).second, time )) )
                    appt = new WText( tutor->getName() );
                else
                    appt = new WText( "-" );
                
                // add to the output grid
                appt->setStyleClass("appt");
                grid->addWidget(appt, j, time+1);
            }
        }
        
    }    
}


Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
    return new TimetablerWebApplication(env);
}
