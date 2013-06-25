//
//  main.cpp
//  Timetabler
//
//  Created by Charles Baynham on 25/06/2013.
//  Copyright (c) 2013 Charles Baynham. All rights reserved.
//

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <time.h>

using namespace std;


class config {
    float _probMutate = 0.03;
    float _probMix = 0.2;
    float _mixMax = 1;
    
    float _searchMin = -50;
    float _searchMax = 50;
    
public:
    void setProbMutate(float prob) { _probMutate = prob; }
    void setProbMix(float prob) { _probMix = prob; }
    void setMixMax(float dist) { _mixMax = dist; }
    void setSearchMin(float min){ _searchMin = min; }
    void setSearchMax(float max){ _searchMax = max; }
    float getProbMutate(){return _probMutate;}
    float getProbMix(){return _probMix;}
    float getMixMax(){return _mixMax;}
    float getSearchMin(){return _searchMin;}
    float getSearchMax(){return _searchMax;}
};

class chromosone {
    float _fitness;
    float _x;
    float _y;
    config* _myconfig;
    
    float _fnEval(float x, float y){
        return ( (x-5.2)*(x-5.2) + (y-6)*(y-6) );
    }
    
    void _doMutate(){
        //
    }
    
    chromosone* _doMix(chromosone* c){
        //
        return NULL;
    }
    
public:
    
    chromosone(float x, float y, config* theconfig):
    _x(x),
    _y(y),
    _myconfig(theconfig)
    { _fitness = 0; }
    
    void mutate(){
        float random = (float)rand() / RAND_MAX;
        if (random <= _myconfig->getProbMutate()) {
            _doMutate();
        }
    }
    
    chromosone* mix(chromosone* c) {
        float random = (float)rand() / RAND_MAX;
        if (random <= _myconfig->getProbMix()) {
            _doMix(c);
        }
        return NULL;
    }
    
    float updateFitness() {
        _fitness = _fnEval(_x,_y);
        return _fitness;
    }
};

class algorithm {

    vector<chromosone> _population;
    config mainConfig;
    
public:

    void start() {
        //
    }
    void stop() {
        //
    }
};


int main() {
    
//    srand(time(NULL));
    
    cout << "RAND_MAX is " << RAND_MAX << endl;
    cout << "Some random numbers: " << rand() << "  " << rand() << "  " << rand() << "  " << endl;
    
    config theConfig;
    
    chromosone test(1,32.1,&theConfig);
    
    test.mutate();
    theConfig.setProbMix(0.4);
    test.mutate();

}