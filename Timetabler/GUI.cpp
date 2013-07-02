//
//  GUI.cpp
//  Timetabler
//
//  Created by Charles Baynham on 02/07/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "GUI.h"

HelloApplication::HelloApplication(const Wt::WEnvironment& env)
: Wt::WApplication(env)
{
    setTitle("Hello world");
    
    root()->addWidget(new Wt::WText("Your name, please ? "));
    nameEdit_ = new Wt::WLineEdit(root());
    Wt::WPushButton *button = new Wt::WPushButton("Greet me.", root());
    root()->addWidget(new Wt::WBreak());
    greeting_ = new Wt::WText(root());
    button->clicked().connect(this, &HelloApplication::greet);
}

void HelloApplication::greet()
{
    greeting_->setText("Hello there, " + nameEdit_->text());
}



// This is run in main:
//     return Wt::WRun(argc, argv, &createApplication);