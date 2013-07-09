//
//  inputGUI.cpp
//  Timetabler
//
//  Created by Charles Baynham on 08/07/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include "inputGUI.h"

GUITutor::GUITutor()
{
    _visOutput = new WContainerWidget();
    _ID = new WLineEdit(_visOutput);
    _name = new WLineEdit(_visOutput);
}

void inputGUI::addBlankTutor () {
    GUITutor* newTut = new GUITutor;
    _tutors.push_back(newTut); // Add new GUI element to list...
    _tutorTab->addWidget( (*newTut)() ); // and to the display
}

//WContainerWidget* inputGUI::renderTutor() {
//    WContainerWidget* out = new WContainerWidget();
//    
//    // Loop over all the GUITutor objects and add each to the output
//    for (list<GUITutor*>::iterator it=_tutors.begin(); it!=_tutors.end(); it++) {
//        out->addWidget( (**it)() ); // Nasty looking, I know. *it is a GUITutor* pointer => **it is the GUITutor object itself.
//                                    //   (**it) () is calling the overloaded operator() which returns a WContainer for that GUI element
//        cerr << "\n\n***\nAdding GUItutor\n***\n\n"<<endl;
//    }
//    
//    return out;
//}

//void inputGUI::setMenu(Wt::WTabWidget& menu) {
//    
//    _menu = &menu;
//    
//    _tutorTab = renderTutor();
////    _studentTab = renderStudent();
////    _subjectTab = renderSubject();
//    
//    //debug
//    _tutorTab->addWidget(new WText("This is a test"));
//    //end debug
//    
//    _menu->addTab(_tutorTab, "Tutors");
////    _menu->addTab(renderSubject(), "Subjects");
////    _menu->addTab(renderStudent(), "Students");
//}

//void inputGUI::refresh() {
//    _menu->removeTab(_tutorTab);
//    _tutorTab = renderTutor();
//    _menu->addTab(_tutorTab, "Tutors");
////    _studentTab = renderStudent();
////    _subjectTab = renderSubject();
//}

void inputGUI::removeGUITutor(GUITutor* t)
{
    _tutorTab->removeWidget( (*t)() );
    _tutors.remove(t);
}


// Initialise the display elements and store pointers to them in the inputGUI class
inputGUI::inputGUI(WContainerWidget* parent) :
    _tutorTab(new WContainerWidget())
{
    // create a menu
    _menu = new WTabWidget(parent);
    
    _menu->setInternalPathEnabled();
    _menu->setInternalBasePath("/input");
    
    //Add tabs:
    _menu->addTab( new WText("intro"), "Introduction");
    _menu->addTab( new WText("Not yet available"), "Download", WTabWidget::LoadPolicy::PreLoading);
    
    // Add the menu to the display
    _menu->addTab(_tutorTab, "Tutors");
    
    //debug
    WPushButton* removeButton = new WPushButton("Remove first");
    removeButton->clicked().connect( this, &inputGUI::removeFirstTutor );
    _tutorTab->addWidget(removeButton);
    
    //end debug
    
}


