

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
    
#ifdef DEBUG
    cerr << "Program is running in DEBUG mode" << endl;
#endif
    
    
//    // Seeds for debugging
//    GaGlobalRandomIntegerGenerator = new GaRandomInteger(7456);
//	GaGlobalRandomFloatGenerator = new GaRandomFloat(345);
//	GaGlobalRandomDoubleGenerator = new GaRandomDouble(3568);
//	GaGlobalRandomBoolGenerator = new GaRandomBool(2354768);

//    char* configfile = "config.txt";
//    
//    // Read in configuration
//    if ( Configuration::getInstance().parseFile( configfile ) ) { cerr << "Error when opening config file \"" << configfile << "\". Does it exist?\n"; exit(EXIT_FAILURE); }
//    
//    // Solve!
//    TimetablerInst::getInstance().getAlgorithm()->StartSolving(false);
//    
//    // get the algorithm (set in class as GaAlgorithm), cast to Incremental
//    //      (since this has the WaitForThreads method) and then wait for it to finish
//    dynamic_cast<GaIncrementalAlgorithm*>(TimetablerInst::getInstance().getAlgorithm())->WaitForThreads();
//    
//    printf("Algorithm execution completed in %i generations\n", TimetablerInst::getInstance().getAlgorithm()->GetAlgorithmStatistics().GetCurrentGeneration() );
//    
    
    // To hold the command line arguemnts that would normally be passed to the server
    char *params[6];
    params[0] = argv[0];
    
    params[1] = (char*)"--http-address";
    params[2] = (char*)"0.0.0.0";
    params[3] = (char*)"--http-port";
    params[4] = (char*)"8080";
    params[5] = (char*)"--docroot=.;.,/style.css,/resources,/solution.csv,/favicon.ico,/out.ttcfg";
    
    int num = 6;
    
    return Wt::WRun(num, params, &createApplication );
    
}


