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


void TimetablerWebApplication::completed() {
    
    ////  Output the finished timetable:
    
    printf("Algorithm execution completed in %i generations\n", TimetablerInst::getInstance().getAlgorithm()->GetAlgorithmStatistics().GetCurrentGeneration() );
    
    _greeting->setText("Solved.");
    
    GaChromosomePtr result;
    TimetablerInst::getInstance().getAlgorithm()->GetPopulation(0).GetBestChromosomes(&result, 0, 1); // store best chromosone in result

    finishedTT* final = new finishedTT(result.GetRawPtr());
    buildTable(final, false);
    
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


TimetablerWebApplication::TimetablerWebApplication(const Wt::WEnvironment& env)
: Wt::WApplication(env)
{
    setTitle("Charles' Timetabler");
    
    this->useStyleSheet("style.css");
    
    _greeting = new WText("Solving...");
    _greeting->setStyleClass("titleText");

    root()->addWidget(_greeting);
    root()->addWidget(new WBreak);
    
    Wt::WPushButton *button = new Wt::WPushButton("Refresh", root());
    root()->addWidget(new Wt::WBreak());

    button->clicked().connect(this, &TimetablerWebApplication::completed );
    
//    TimetablerWebApplication* webApp = this;
//    
//    TimetablerInst::getInstance().registerObserverFunc( std::bind(&completed, placeholders::_1, webApp ));
    
////  Start the algorithm:
    
    char* configfile = "config.txt";
    
    // Read in configuration
    if ( Configuration::getInstance().parseFile( configfile ) ) { cerr << "Error when opening config file \"" << configfile << "\". Does it exist?\n"; exit(EXIT_FAILURE); }
    
    // Solve!
    TimetablerInst::getInstance().getAlgorithm()->StartSolving(false);
    
    // get the algorithm (set in class as GaAlgorithm), cast to Incremental
    //      (since this has the WaitForThreads method) and then wait for it to finish
//    dynamic_cast<GaIncrementalAlgorithm*>(TimetablerInst::getInstance().getAlgorithm())->WaitForThreads();
    
    
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


void TimetablerWebApplication::buildTable(finishedTT* timetable, bool tutors)
{
    int entries;
    
    hash_map<int, Tutor*> tutorMap = timetable->getAllTutors();
    hash_map<int, Student*> studentMap = timetable->getAllStudents();
    
    if (tutors) entries = (int)tutorMap.size();
    else entries = (int)studentMap.size();
    
    WContainerWidget *container = root();
    
//    container->setHeight(700);
    container->setStyleClass("yellow-box");
    
    WGridLayout *grid = new WGridLayout();
    container->setLayout(grid);
    
    // (0,0) is blank
    grid->addWidget(new WBreak, 0, 0);
    
    // (0,1) to (0,i) contain time headers. edit
    for (int i=1; i<=SLOTS_IN_DAY; i++) {
        WText *timeWidget = new WText(to_string(i));
        timeWidget->setHeight(75);
        timeWidget->setStyleClass("blue-box");
        grid->addWidget(timeWidget, 0, i);
        grid->setColumnStretch(i, 1);
    }
    
    // (1,0) to (1,j) contain Student / Tutor names
    WText *nameWidget;
    if (tutors) {
        for (hash_map<int, Tutor*>::iterator it = tutorMap.begin(); it != tutorMap.end(); it++) {
            
            static int j=0;
            //Count which row we're on:
            j++;
            
            nameWidget = new WText( (*it).second->getName() );
            nameWidget->setStyleClass("green-box");
            nameWidget->setHeight(50);
            grid->addWidget(nameWidget, j, 0);
            grid->setRowStretch(j, 1);
        }
    }
    else {
        for (hash_map<int, Student*>::iterator it = studentMap.begin(); it != studentMap.end(); it++) {
            
            static int j=0;
            //Count which row we're on:
            j++;
            
            nameWidget = new WText( (*it).second->getName() );
            nameWidget->setStyleClass("green-box");
            grid->addWidget(nameWidget, j, 0);
            grid->setRowStretch(j, 1);
            nameWidget->resize(120, 75);
        }
    }
    
    
    // fill in the actual appointments
    if (tutors) {
        for (hash_map<int, Tutor*>::iterator it = tutorMap.begin(); it != tutorMap.end(); it++) {
            static int j=0;
            //Count which row we're on:
            j++;
            
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
                grid->addWidget(appt, j, time+1);
            }
        }
        
    }
    else {
        for (hash_map<int, Student*>::iterator it = studentMap.begin(); it != studentMap.end(); it++) {
            static int j=0;
            //Count which row we're on:
            j++;
            
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


void TimetablerWebApplication::greet()
{
    _greeting->setText("Hello there, " + _nameEdit->text());
}

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
    return new TimetablerWebApplication(env);
}
