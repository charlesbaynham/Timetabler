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
    
    buildTable( 2, 2 );
}

void TimetablerWebApplication::buildTable(int times, int entries)
{
    WContainerWidget *container = new WContainerWidget();
    
    container->setHeight(700);
    container->setStyleClass("yellow-box");
    
    WGridLayout *grid = new WGridLayout();
    container->setLayout(grid);
    
    // (0,0) is blank
    grid->addWidget(new WBreak, 0, 0);
    
    // (0,1) to (0,i) contain time headers. edit
    for (int i=1; i<=times; i++) {
        WText *timeWidget = new WText(to_string(i));
        timeWidget->setHeight(150);
        timeWidget->setStyleClass("blue-box");
        grid->addWidget(timeWidget, 0, i);
    }
    
    // (1,0) to (1,j) contain Student / Tutor names
    for (int j=1; j<=entries; j++) {
        WText *nameWidget = new WText("Name x");
        nameWidget->setStyleClass("green-box");
        grid->addWidget(nameWidget, j, 0);
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
    
    root()->addWidget(container);
}


void TimetablerWebApplication::greet()
{
    _greeting->setText("Hello there, " + _nameEdit->text());
}

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
    return new TimetablerWebApplication(env);
}
