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
public:
    char* name;
    int age;
    bool female;

    person(char* newname, int newage, bool newfemale):
        name(newname),
        age(newage),
        female(newfemale)
    {}
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
    
    person& addInhab(char* newname, int newage, bool newfemale)
    {
        person newguy(newname, newage, newfemale);
        _inhabs.push_back(newguy);
        return (_inhabs.back());
    }
    
    char* name() { return _name; }
    
    int newestAge(){
        return _inhabs.back().age;
    }
    person newestPerson(){ return _inhabs.back(); }
    person oldestPerson(){ return _inhabs.front(); }
    
};


int main(int argc, const char * argv[])
{

    // insert code here...
    std::cout << "Hello, World!\nI'm Charles\n";
    
    house WM = house(2, 10, "Wonastow Mill");
    
    person& added = WM.addInhab("Charles", 22, false);
    WM.addInhab("Georgie", 19, true);
    
    std::cout << "My house, " << WM.name() << ", has " <<  WM.getWindows() << " windows.\n";
    
    std::cout << "We just added " << WM.newestPerson().name << " to the house. ";
    if (WM.newestPerson().female) { std::cout << "She"; }
    else { std::cout << "He";}
    std::cout << " is " << WM.newestAge() << " years old.\n";
    
    std::cout << "There's also " << WM.oldestPerson().name << " who is " << WM.oldestPerson().age << " years old.\n";
    
    return 0;
}

