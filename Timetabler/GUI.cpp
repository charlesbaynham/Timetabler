//
//  main.cpp
//  WtGUI
//
//  Created by Charles Baynham on 02/07/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//


#include "GUI.h"

TimetablerWebApplication::TimetablerWebApplication(const Wt::WEnvironment& env)
: Wt::WApplication(env)
{
    setTitle("Hello world");
    
    this->useStyleSheet("style.css");
    
    WText *title = new WText("Hello");
    title->setStyleClass("titleText");
    
    root()->addWidget(title);
    root()->addWidget(new WBreak);
    
    GaChromosomePtr resultPtr;
    TimetablerInst::getInstance().getAlgorithm()->GetPopulation(0).GetBestChromosomes(&resultPtr, 0, 1); // store best chromosone in result
    
    finishedTT* timetable = new finishedTT(resultPtr.GetRawPtr());
    buildTable(timetable , false );
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
