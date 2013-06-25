//
//  main.cpp
//  Timetabler
//
//  Created by Charles Baynham on 25/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include <iostream>
#include <vector>

class person {
    char* name;
    int age;
    bool female;
};

class house {
    int _doors;
    int _windows;
    char* _name;
    
    std::vector<person> _inhabs;
    
public:
    house(int noDoors, int noWindows, char* houseName) :
        _doors(noDoors),
        _windows(noWindows),
        _name(houseName)
    {}
    
    
    int getWindows() {
        return _windows;
    }
    
    
    char* name() { return _name; }
    
};


int main(int argc, const char * argv[])
{

    // insert code here...
    std::cout << "Hello, World!\nI'm Charles\n";
    
    house WM = house(2, 10, "Wonastow Mill");
    

    
    std::cout << "My house, " << WM.name() << ", has " <<  WM.getWindows() << " windows.\n";
    
    return 0;
}

