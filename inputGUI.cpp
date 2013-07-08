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