//
//  main.cpp
//  Timetabler
//
//  Created by Charles Baynham on 25/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include <iostream>

class house {
    int _doors;
    int _windows;
    char* _name;
    
public:
    house(int noDoors, int noWindows, char* houseName) {
        _doors = noDoors;
        _windows = noWindows;
        _name = houseName;
    }
    
};



int main(int argc, const char * argv[])
{

    // insert code here...
    std::cout << "Hello, World!\nI'm Charles\n";
    
    
    
    return 0;
}

