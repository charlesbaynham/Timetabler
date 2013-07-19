//
//  GUI.h
//  Timetabler
//
//  Created by Charles Baynham on 02/07/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#ifndef __Timetabler__GUI__
#define __Timetabler__GUI__

#include <iostream>

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
//#include <Wt/WBorderLayout>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
//#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WTimer>
#include <Wt/WGridLayout>
#include <Wt/WMenu>
#include <Wt/WMenuItem>
#include <Wt/WTabWidget>

#include "output.h"
#include "TimetablerInst.h"

using namespace std;

using namespace Wt;

using namespace Algorithm;
using namespace Algorithm::StopCriterias;
using namespace Algorithm::SimpleAlgorithms;

// This is declared here since inputGUI depends on the following definitions: therefore I #include it's head files at the end of this one, after the GUI.h declarations
class inputGUI;

WApplication *createApplication(const WEnvironment& env);

class TimetablerWebApplication : public WApplication
{
public:
    TimetablerWebApplication(const WEnvironment& env);

    // toggle the state of the algorithm, resuming if stopped
    void toggleState();

    // Stop the algorithm if it's already running on web session removal
    ~TimetablerWebApplication()
    {
        cerr << "*** Aborting solution ***\n\n";
        if (TimetablerInst::getInstance()->getAlgorithm()->GetState() == Algorithm::GaAlgorithmState::GAS_RUNNING)
            TimetablerInst::getInstance()->getAlgorithm()->StopSolving();
        
        cerr << "*** Killing Inst ***\n";
        TimetablerInst::getInstance()->reset();
    }
    
private:
    WPushButton *_button;
    WPushButton *_stopButton;
    WText* _greeting;
    WTimer* _timer;
    WText* _status;
    WText* _bestFitness;
    bool _tableBuilt=false;
    bool _tutors=false;
    void handlePathChange();
    
    inputGUI* _inputGUI;

public:
    // ready to solve
    void pageReady();
    // Set up the configuration
    void pageInput();
    
    // Build the output timetable using <timetable> as the info, from the point of view of tutors or students according to <bool tutors>
    void buildTable(finishedTT* timetable, bool tutors);
    // Refresh table
    void refreshStats();
    
    void startSolve();
    
};

#include "inputGUI.h"

#endif /* defined(__Timetabler__GUI__) */
