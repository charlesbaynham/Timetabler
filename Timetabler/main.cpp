

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


int main()
{

    
    Configuration::getInstance().parseFile("config.txt");

    Configuration::getInstance().dumpTutors();
    
    cout << GaGlobalRandomIntegerGenerator->Generate(9) <<GaGlobalRandomIntegerGenerator->Generate(9) <<GaGlobalRandomIntegerGenerator->Generate(9) <<GaGlobalRandomIntegerGenerator->Generate(9) <<endl;
    
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
    GaGlobalRandomIntegerGenerator = new GaRandomInteger(1622);
	GaGlobalRandomFloatGenerator = new GaRandomFloat(12513);
	GaGlobalRandomDoubleGenerator = new GaRandomDouble(13223);
	GaGlobalRandomBoolGenerator = new GaRandomBool(12372);
    
    TimetablerInst::getInstance().getAlgorithm()->StartSolving(false);
    
    // get the algorithm (set in class as GaAlgorithm), cast to Incremental
    //      (since this has the WaitForThreads method) and then execute
    dynamic_cast<GaIncrementalAlgorithm*>(TimetablerInst::getInstance().getAlgorithm())->WaitForThreads();

    
    printf("Done\n");
    
	return 0;
}
