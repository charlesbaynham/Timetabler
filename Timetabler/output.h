//
//  output.h
//  Timetabler
//
//  Created by Charles Baynham on 01/07/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

//CSV output format for now

#ifndef __Timetabler__output__
#define __Timetabler__output__

#include <iostream>
#include <fstream>

#include "TTChromosone.h"

class outputCSV {
    static outputCSV _instance;
    
public:

    // output a chromosome
    void operator ()(char * filename, const GaChromosome& chromo);
    
    static inline outputCSV getInstance(){ return _instance; }
};

#endif /* defined(__Timetabler__output__) */
