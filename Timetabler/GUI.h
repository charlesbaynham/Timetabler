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
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>

class HelloApplication : public Wt::WApplication
{
public:
    HelloApplication(const Wt::WEnvironment& env);
    
private:
    Wt::WLineEdit *nameEdit_;
    Wt::WText *greeting_;
    
    void greet();
};

#endif /* defined(__Timetabler__GUI__) */
