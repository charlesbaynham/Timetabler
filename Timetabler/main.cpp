

#include <iostream>

#include "GLsource/Initialization.h"
#include "GLsource/ChromosomeOperations.h"
#include "GLsource/MultiValueChromosome.h"
#include "GLsource/Population.h"
#include "GLsource/StopCriterias.h"
#include "GLsource/SimpleAlgorithm.h"

using namespace std;

using namespace Algorithm;
using namespace Algorithm::StopCriterias;
using namespace Algorithm::SimpleAlgorithms;


#include "TimetablerInst.h"
#include "output.h"

#include <Wt/WApplication>
#include "GUI.h"


int main(int argc, char **argv)
{
    char* configfile = "config.txt";
    
    if ( Configuration::getInstance().parseFile( configfile ) ) { cerr << "Error when opening config file \"" << configfile << "\". Does it exist?\n"; exit(EXIT_FAILURE); }

    Configuration::getInstance().dumpTutors();
        
//    // debug
//    GaChromosomePtr no1;
//    
//    no1 = TimetablerInst::getInstance().getPrototype()->MakeNewFromPrototype();
//    
////    outputCSV::getInstance().operator()("test1.txt", *no1);
////    outputCSV::getInstance().operator()("test2.txt", *no2);
//    
////    ( outputRaw::getInstance() )("testUnmutated.txt", *no1);
////    (*no1).Mutation();
////    ( outputRaw::getInstance() )("testMutated.txt", *no1);
//    
////    outputCSV::getInstance().operator()("testCrossed.txt", *crossed);
//    //end debug

    // Seeds for debugging
    GaGlobalRandomIntegerGenerator = new GaRandomInteger(7456);
	GaGlobalRandomFloatGenerator = new GaRandomFloat(345);
	GaGlobalRandomDoubleGenerator = new GaRandomDouble(3568);
	GaGlobalRandomBoolGenerator = new GaRandomBool(2354768);

    // debug web interface :
//    TimetablerInst::getInstance().getAlgorithm()->StartSolving(false);
    
    // Print some random numbers to make sure the seeds worked
    cout << GaGlobalRandomIntegerGenerator->Generate(9) <<GaGlobalRandomIntegerGenerator->Generate(9) <<GaGlobalRandomIntegerGenerator->Generate(9) <<GaGlobalRandomIntegerGenerator->Generate(9) <<endl;
    
    
    // get the algorithm (set in class as GaAlgorithm), cast to Incremental
    //      (since this has the WaitForThreads method) and then execute
    dynamic_cast<GaIncrementalAlgorithm*>(TimetablerInst::getInstance().getAlgorithm())->WaitForThreads();

    
    printf("Done genetic\n");
    
    ////////
    
    cout << "***\nTry GUI with args \"";
    for (int i = 1; i<argc; i++) { cout << " " << argv[i]; }
    cout << "\"" << endl;
    
    return Wt::WRun(argc, argv, &createApplication );
    
}


