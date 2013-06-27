

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

#include "TTChromosone.h"

int main()
{
	GaInitialize();
    
    Configuration config = Configuration::getInstance();
    
    if (!config.parsed()) {
        printf("Config file is unparsed\nParsing now...\n");
        config.parseFile("/Users/Charles/config.txt");
    }

    printf("We have %i tutors, %i students and %i subjects.\n", config.numTutors(), config.numStudents(), config.numSubjects());
    
    Tutor * first = config.getTutor(0);
    cout << "The first tutor: " << first->getName();
    printf("'s ID is %i\n", first->getID());
    
    Tutor * second = config.getTutor(1);
    cout << "The second tutor: " << second->getName();
    printf("'s ID is %i\n", second->getID());
    
    cout << "The first is: " << (first->isProcessed() ? "processed" : "not processed") << endl;
      
	GaFinalize();
    
    printf("Done\n");
    
	return 0;
}
