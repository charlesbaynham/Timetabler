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
#include <Wt/WBorderLayout>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WHBoxLayout>
#include <Wt/WGridLayout>

#include "output.h"
#include "TimetablerInst.h"


using namespace Wt;
using namespace std;

WApplication *createApplication(const Wt::WEnvironment& env);

class TimetablerWebApplication : public Wt::WApplication
{
public:
    TimetablerWebApplication(const Wt::WEnvironment& env);
    
private:
    Wt::WLineEdit *_nameEdit;
    Wt::WText *_greeting;
    
    // Build the output timetable using <timetable> as the info, from the point of view of tutors or students according to <bool tutors>
    void buildTable(finishedTT* timetable, bool tutors);

    void greet();
};


#endif /* defined(__Timetabler__GUI__) */
