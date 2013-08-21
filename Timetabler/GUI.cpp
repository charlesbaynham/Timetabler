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
//        if (Configuration::getInstance().isEmpty()) { // We haven't yet made the configuration
            root()->addWidget(new WText("Error: this page is not meant for direct access. Redirecting to input..."));
            app->setInternalPath("/", true);
//        }
//        else
//            startSolve();
    }
    else if(path == "/") pageInput();
    else if( boost::starts_with(path, "/input") ) app->setInternalPath("/", true); // redirect to input page if using old input path
    else app->setInternalPath("/", true); // Redirect to main page if page not found
    
    
}

void TimetablerWebApplication::startSolve() {

    root()->clear();
    
    // setup a timer which refreshes the timetable if it's changed every 0.5 seconds, until timer->stop() is called.
    _timer = new WTimer();
    _timer->setInterval(500);
    _timer->timeout().connect(this, &TimetablerWebApplication::refreshStats );
    
    
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
    
    // Start the refresh timer
    _timer->start();
    
    
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
    bool optimal = ( abs(1.0f - bestFitness) <= 0.00001 );
    if (optimal)
        TimetablerInst::getInstance()->getAlgorithm()->StopSolving();
    
    //if finished, stop checking
    GaAlgorithmState state = TimetablerInst::getInstance()->getAlgorithm()->GetState();
#if DEBUG
    cerr << "The state is " << state << endl;
#endif
    if (state & GaAlgorithmState::GAS_STOPPED ) {// the algorithm has found a solution / met its criteria
        _timer->stop();
        _bestFitness->setText( "Non-optimal solution found after " + to_string(generation) + " gens: Fitness "+to_string(100*bestFitness)+"%" );
        _stopButton->setText("Try harder");
        
        if (optimal) {
            _bestFitness->setText( "Optimal solution found in "+to_string(generation)+" generations!" );
            _stopButton->setText( "Resume" );
            _stopButton->disable();
        }

        
#if DEBUG
        cerr<<"Stopping timer on algorithm completion and saving config" << endl;
#endif
        
        // save config and best chromosome to the output file
        GaChromosomePtr result;
        Configuration::getInstance().saveConfig(_filename.c_str());
        TimetablerInst::getInstance()->getAlgorithm()->GetPopulation(0).GetBestChromosomes(&result, 0, 1); // store best chromosone in result
        ( outputSolution::getInstance() )( _filename , *result, true );
        
        // enable download
        _download->enable();
        _saveConfig->enable();
    }

}


TimetablerWebApplication::TimetablerWebApplication(const Wt::WEnvironment& env)
: Wt::WApplication(env), _filename("out.ttcfg")
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
    
    // wipe any existing configuration and stop any existing algorithms
    TimetablerInst::getInstance()->getAlgorithm()->StopSolving();
    // Stop the timer if it has been created
    
    if (_timer && _timer->isActive()) {
        _timer->stop();
        this->processEvents();
    }
    Configuration::getInstance().clear();
    _tableBuilt = false;
    
    //wipe display
    root()->clear();
    
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
    
    _bestFitness = new WText("");
    
    TimetablerWebApplication::startSolve();
    
}

void TimetablerWebApplication::buildTable(finishedTT* timetable, bool tutors)
{
    // If launched in tutor/student mode, save this for next time and rebuild the table
    if (_tutors != tutors ) {
        _tutors = tutors;
        _tableBuilt = false;
    }
    
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
        _saveConfig = new WPushButton("Save configuration");
        _stopButton = new WPushButton("Pause");
        _download = new WPushButton("Download timetable");
        
        setTutor->clicked().connect( boost::bind(&TimetablerWebApplication::buildTable, this, timetable, true) );
        setStudent->clicked().connect( boost::bind(&TimetablerWebApplication::buildTable, this, timetable, false) );
        
        // save config and best chromosome to the output file
//        saveConfig->clicked().connect( std::bind( [this, filename] () {
//            GaChromosomePtr result;
//            Configuration::getInstance().saveConfig(filename.c_str());
//            TimetablerInst::getInstance()->getAlgorithm()->GetPopulation(0).GetBestChromosomes(&result, 0, 1); // store best chromosone in result
//            ( outputSolution::getInstance() )( filename , *result, true );
        
//            string path = this->internalPath();
//            this->setInternalPath("/" + filename);
//            this->setInternalPath(path);
//        }));
        // The above section is commented because out.ttcfg is now generated every time the algorithm stops
        _saveConfig->setLink("/"+_filename);
        _saveConfig->disable();
        
        _stopButton->clicked().connect( this, &TimetablerWebApplication::toggleState );
        
        _download->setLink("/solution.csv");
        _download->disable();
        
        if ( TimetablerInst::getInstance()->getAlgorithm()->GetState() & GAS_STOPPED ) {
            _download->enable();
            _stopButton->setText("Resume");
            _saveConfig->enable();
            
            // check if optimal solution already found:
            GaChromosomePtr result;
            GaAlgorithm* algorithm = TimetablerInst::getInstance()->getAlgorithm();
            // store best chromosone in result
            algorithm->GetPopulation(0).GetBestChromosomes(&result, 0, 1);
            
            float bestFitness = (*result).GetFitness();
            
            if ( abs(1.0f - bestFitness) <= 0.00001 )
                _stopButton->disable();
        }
        
        buttons->addWidget(_bestFitness);
        buttons->addWidget(setTutor);
        buttons->addWidget(setStudent);
        buttons->addWidget(_saveConfig);
        buttons->addWidget(_stopButton);
        buttons->addWidget(_download);
        
        grid->addWidget(toggleButtons, 0, 0);
        
        // (0,1) to (0,i) contain time headers. edit
        for (int i=1; i<=SLOTS_IN_DAY; i++) {
            WText *timeWidget;
            
            timeWidget = new WText("Slot " + to_string(i));
            
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
        
        _tableBuilt = true;
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

void TimetablerWebApplication::buildTablePDF(finishedTT *timetable, bool tutors) {
    
    ;
    
}

void TimetablerWebApplication::toggleState() {

    bool started;
    
    (TimetablerInst::getInstance()->getAlgorithm()->GetState() & GAS_RUNNING) ? started = true : started = false;
    
    if (started) {
        TimetablerInst::getInstance()->getAlgorithm()->PauseSolving();
#ifdef DEBUG
        cerr << "Stopping. State is now " << TimetablerInst::getInstance()->getAlgorithm()->GetState() << endl;
#endif
        _timer->stop();
        _download->enable();
        _saveConfig->enable();
        _stopButton->setText("Resume");
    } else {
        TimetablerInst::getInstance()->getAlgorithm()->StartSolving(true);
        _timer->start();
#ifdef DEBUG
        cerr << "Resuming. State is now " << TimetablerInst::getInstance()->getAlgorithm()->GetState() << endl;
#endif
        _download->disable();
        _saveConfig->disable();
        _stopButton->setText("Pause");
    }

}


Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
    return new TimetablerWebApplication(env);
}
