

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
    
    // Seeds for debugging
    GaGlobalRandomIntegerGenerator = new GaRandomInteger(123);
	GaGlobalRandomFloatGenerator = new GaRandomFloat(1253);
	GaGlobalRandomDoubleGenerator = new GaRandomDouble(1223);
	GaGlobalRandomBoolGenerator = new GaRandomBool(1233);
    
    cout << GaGlobalRandomIntegerGenerator->Generate(9) <<GaGlobalRandomIntegerGenerator->Generate(9) <<GaGlobalRandomIntegerGenerator->Generate(9) <<GaGlobalRandomIntegerGenerator->Generate(9) <<endl;
    
    TimetablerInst::getInstance().getAlgorithm()->StartSolving(false);
    
//    GaAlgorithmState state = TimetablerInst::getInstance().getAlgorithm()->GetState();
//    
//    cout << state << endl;
    
    // get the algorithm (set in class as GaAlgorithm), cast to Incremental
    //      (since this has the WaitForThreads method) and execute
    dynamic_cast<GaIncrementalAlgorithm*>(TimetablerInst::getInstance().getAlgorithm())->WaitForThreads();

//    state = TimetablerInst::getInstance().getAlgorithm()->GetState();
//    if (state==GAS_UNINITIALIZED) printf("uninit\n");
//    /// <summary>algorithm execution is stopped by the user.</summary>
//    else if (state==GAS_USER_STOPED) printf("Stoped by user\n");
//    
//    /// <summary>algorithm reached given criteria and it is stopped.</summary>
//    else if (state==GAS_CRITERIA_STOPPED) printf("Stopped by criteria\n");
//    
//    /// <summary>temporary paused, but it can resume execution at any time.</summary>
//    else if (state==GAS_PAUSED) printf("paused\n");
//    
//    /// <summary>algorithm is running.</summary>
//    else if (state==GAS_RUNNING) printf("running\n");
    
    printf("Done\n");
    
	return 0;
}
