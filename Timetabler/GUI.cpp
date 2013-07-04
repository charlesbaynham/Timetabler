//
//  main.cpp
//  WtGUI
//
//  Created by Charles Baynham on 02/07/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "GUI.h"
#include <functional>

using namespace std;


void TimetablerWebApplication::startSolve() {
    
    // setup a timer which calls MyClass::timeout() every 2 seconds, until timer->stop() is called.
    _timer = new WTimer();
    _timer->setInterval(200);
    _timer->timeout().connect(this, &TimetablerWebApplication::refreshStats );
    
    
    _greeting->setText("Solving...");
    _button->setHidden(true);
    _timer->start();
    
    ////  Start the algorithm:
    
    char* configfile = "config.txt";
    
    // Read in configuration
    if ( Configuration::getInstance().parseFile( configfile ) ) { cerr << "Error when opening config file \"" << configfile << "\". Does it exist?\n"; exit(EXIT_FAILURE); }
    
    // Solve!
    TimetablerInst::getInstance().getAlgorithm()->StartSolving(false);
    
    WApplication *app = WApplication::instance();
    app->processEvents();
    
    // get the algorithm (set in class as GaAlgorithm), cast to Incremental
    //      (since this has the WaitForThreads method) and then wait for it to finish
    //dynamic_cast<GaIncrementalAlgorithm*>(TimetablerInst::getInstance().getAlgorithm())->WaitForThreads();

    
//    ////  Output the finished timetable:
//    
//    printf("Algorithm execution completed in %i generations\n", TimetablerInst::getInstance().getAlgorithm()->GetAlgorithmStatistics().GetCurrentGeneration() );
//    
//    _greeting->setText("Solved.");
//    
//    root()->clear();
//    
//    GaChromosomePtr result;
//    TimetablerInst::getInstance().getAlgorithm()->GetPopulation(0).GetBestChromosomes(&result, 0, 1); // store best chromosone in result
//
//    finishedTT* final = new finishedTT(result.GetRawPtr());
//    buildTable(final, false);
    
//    WText *greeting = webApp->getGreeting();
////    greeting = new WText("Complete!");
////    greeting->setHidden(true);
//    
//    GaChromosomePtr resultPtr;
//    TimetablerInst::getInstance().getAlgorithm()->GetPopulation(0).GetBestChromosomes(&resultPtr, 0, 1); // store best chromosone in result
//    
//    finishedTT* timetable = new finishedTT(resultPtr.GetRawPtr());
//    webApp->buildTable(timetable , false );

    
}

void TimetablerWebApplication::refreshStats() {
    
    GaChromosomePtr result;
    GaAlgorithm* algorithm = TimetablerInst::getInstance().getAlgorithm();
    algorithm->GetPopulation(0).GetBestChromosomes(&result, 0, 1); // store best chromosone in result
    
    float bestFitness = (*result).GetFitness();
    int generation = algorithm->GetAlgorithmStatistics().GetCurrentGeneration();
    printf("Current generation: %i. Best fitness: %f\n", generation, bestFitness);
    
//    char out[100];
//    sprintf(out, "Current generation: %i,   Best fitness:: %f", generation, bestFitness);
//    
//    _status->setText(out);
    
    // Build table
    finishedTT* timetable = new finishedTT(result.GetRawPtr());
    buildTable(timetable , _tutors );
    
    if (bestFitness > 0.999999)
    {
                
//        sprintf(out, "Current generation: %i,   Best fitness:: %f\n\tSolved! Outputting timetable now...", generation, bestFitness);
//        
//        // Process this text update before building table
//        _status->setText(out);
//        WApplication *app = WApplication::instance();
//        app->processEvents();
        
        
        // Build table
        finishedTT* timetable = new finishedTT(result.GetRawPtr());
        buildTable(timetable , _tutors );

        _timer->stop();
//        //debug
        
//        WContainerWidget *container = root();
//        
//        WGridLayout *grid = dynamic_cast<WGridLayout*>( container->layout() );
//        
//        _timer->setInterval(2000);
//        
//        static int griditem=0;
//        griditem++;
//        
//        WLayoutItem* test = grid->itemAt(griditem);
//        
//        WText* lol = dynamic_cast<WText*>( test->widget() );
//        
//        string text = "(" + to_string(grid->columnCount()) + "," + to_string(grid->rowCount()) + ")";
//        
//        lol->setText( text );
//        
//        //debug end

    }
}


TimetablerWebApplication::TimetablerWebApplication(const Wt::WEnvironment& env)
: Wt::WApplication(env)
{
    setTitle("Charles' Timetabler");
    
    this->useStyleSheet("style.css");

    
    Wt::WApplication *app = Wt::WApplication::instance();
    
    app->internalPathChanged().connect(std::bind([=] () {
        handlePathChange();
    }));
    
    handlePathChange();
    
//    TimetablerInst::getInstance().registerObserverFunc( &TimetablerWebApplication::completed ));
    
//    TimetablerWebApplication* webApp = this;
//    
//    TimetablerInst::getInstance().registerObserverFunc( std::bind(&completed, placeholders::_1, webApp ));
    
    
    
//////  Output the finished timetable:
//    
//    printf("Algorithm execution completed in %i generations\n", TimetablerInst::getInstance().getAlgorithm()->GetAlgorithmStatistics().GetCurrentGeneration() );
//
//    title = new WText("Complete!");
//    
//    GaChromosomePtr resultPtr;
//    TimetablerInst::getInstance().getAlgorithm()->GetPopulation(0).GetBestChromosomes(&resultPtr, 0, 1); // store best chromosone in result
//    
//    finishedTT* timetable = new finishedTT(resultPtr.GetRawPtr());
//    buildTable(timetable , false );
}

void TimetablerWebApplication::handlePathChange()
{
    Wt::WApplication *app = Wt::WApplication::instance();
    
    if (app->internalPath() == "/navigation/shop")
        ;
    else
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

void TimetablerWebApplication::pageOutput() {
    
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
        
    //    container->setHeight(700);
        container->setStyleClass("yellow-box");
        
        grid = new WGridLayout();
        container->setLayout(grid);
        
        // (0,0) is blank. edit
        WContainerWidget* toggleButtons = new WContainerWidget;

        WVBoxLayout* buttons = new WVBoxLayout;
        toggleButtons->setLayout(buttons);

        WPushButton *setTutor=new WPushButton("Tutor mode", root());
        WPushButton *setStudent=new WPushButton("Student mode", root());
        
        setTutor->clicked().connect( boost::bind(&TimetablerWebApplication::buildTable, this, timetable, true) );
        setStudent->clicked().connect( boost::bind(&TimetablerWebApplication::buildTable, this, timetable, false) );
        
        buttons->addWidget(setTutor);
        buttons->addWidget(setStudent);
        
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
    
//    
//    for (int row = 0; row < 2; ++row) {
//        for (int column = 0; column < 2; ++column) {
//            Wt::WString cell = Wt::WString("<div class='centerme'>Item ({1}, {2})</div>").arg(row).arg(column);
//            
//            Wt::WText *t = new Wt::WText(cell);
//            if (row == 1 || column == 2)
//                t->setStyleClass("blue-box");
//            else
//                t->setStyleClass("green-box");
//            
//            grid->addWidget(t, row, column);
//        }
//    }
//    
//    grid->setRowStretch(1, 1);
//    grid->setColumnStretch(1, 1);
    
}


Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
    return new TimetablerWebApplication(env);
}
