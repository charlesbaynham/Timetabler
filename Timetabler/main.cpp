
/*
 *
 * website: N/A
 * contact: kataklinger@gmail.com
 *
 */

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

const char pattern[] =
"        GGGGGGGGGGGGG               AAA               LLLLLLLLLLL             "
"     GGG::::::::::::G              A:::A              L:::::::::L             "
"   GG:::::::::::::::G             A:::::A             L:::::::::L             "
"  G:::::GGGGGGGG::::G            A:::::::A            LL:::::::LL             "
" G:::::G       GGGGGG           A:::::::::A             L:::::L               "
"G:::::G                        A:::::A:::::A            L:::::L               "
"G:::::G                       A:::::A A:::::A           L:::::L               "
"G:::::G    GGGGGGGGGG        A:::::A   A:::::A          L:::::L               "
"G:::::G    G::::::::G       A:::::A     A:::::A         L:::::L               "
"G:::::G    GGGGG::::G      A:::::AAAAAAAAA:::::A        L:::::L               "
"G:::::G        G::::G     A:::::::::::::::::::::A       L:::::L               "
" G:::::G       G::::G    A:::::AAAAAAAAAAAAA:::::A      L:::::L         LLLLLL"
"  G:::::GGGGGGGG::::G   A:::::A             A:::::A   LL:::::::LLLLLLLLL:::::L"
"   GG:::::::::::::::G  A:::::A               A:::::A  L::::::::::::::::::::::L"
"     GGG::::::GGG:::G A:::::A                 A:::::A L::::::::::::::::::::::L"
"        GGGGGG   GGGGAAAAAAA                   AAAAAAALLLLLLLLLLLLLLLLLLLLLLLL";

const int patternSize = sizeof( pattern ) - 1;

class pFitness : public GaFitnessOperation
{
    
public:
    
	virtual float GACALL operator ()(const GaChromosome* chromosome) const
	{
		const vector<char>& v = dynamic_cast<const GaMultiValueChromosome<char>*>( chromosome )->GetCode();
        
		int score = 0;
		for( int i = 0; i < patternSize; i++ )
		{
			if( v[ i ] == pattern[ i ] )
				score++;
		}
        
		return (float)score / patternSize * 100;
	}
    
	virtual GaParameters* GACALL MakeParameters() const { return NULL; }
    
	virtual bool GACALL CheckParameters(const GaParameters& parameters) const { return true; }
};

volatile bool stop = false;

class pObserver : public GaObserverAdapter
{
public:
    
	virtual void GACALL NewBestChromosome(const GaChromosome& newChromosome, const GaAlgorithm& algorithm)
	{
		static int a = 0;
		a++;
		//if(a%100==0)
		{
			const vector<char>& v = dynamic_cast<const GaMultiValueChromosome<char>&>( newChromosome ).GetCode();
            
			cout << "Generation: " << algorithm.GetAlgorithmStatistics().GetCurrentGeneration() << endl;
			cout << "Fitness: " << newChromosome.GetFitness();
			cout << "\n-------------------------\n";
            
			for( int i = 0; i < (int)v.size(); i++ )
			{
				if( i % 78 == 0 )
					cout << endl;
                
				cout << v[ i ];
			}
            
			cout << "\n-------------------------\n";
		}
	}
    
	virtual void GACALL EvolutionStateChanged(GaAlgorithmState newState, const GaAlgorithm& algorithm)
	{
		if( newState == GAS_CRITERIA_STOPPED )
			cout << " end.";
	}
};


int main()
{
	GaInitialize();
    
	GaChromosomeParams chromosomeParams( 0.1F, 10, true, 0.8F, 2 );
    
	GaMultiValueSet<char> valueSet( false );
	valueSet.Add( "GAL: ", "     ", 5 );
    
	pFitness fitnessOperation;
	GaChromosomeDomainBlock<char> configBlock( &valueSet,
                                              GaCrossoverCatalogue::Instance().GetEntryData( "GaMultiValueCrossover" ),
                                              GaMutationCatalogue::Instance().GetEntryData( "GaFlipMutation" ),
                                              &fitnessOperation, GaFitnessComparatorCatalogue::Instance().GetEntryData( "GaMaxFitnessComparator" ),
                                              &chromosomeParams );
    
	GaMultiValueChromosome<char> prototype( patternSize, &configBlock );
    
	GaPopulationConfiguration populationConfig;
	GaPopulationParameters populationParams( 30, true, true, false, 0, 0 );
    
	populationConfig.SetParameters( populationParams );
	populationConfig.SetSortComparator( &configBlock.GetFitnessComparator() );
	populationConfig.Selection().GetParameters().SetSelectionSize( 6 );
	
	GaPopulation population( &prototype, &populationConfig );
    
	GaSimpleAlgorithmParams algorithmParams( 10, 2 );
	GaSimpleAlgorithm algorithm( &population, algorithmParams );
    
	GaFitnessCriteriaParams criteriaParams( 100, GFC_EQUALS_TO, GSV_BEST_FITNESS );
	algorithm.SetStopCriteria( GaStopCriteriaCatalogue::Instance().GetEntryData( "GaFitnessCriteria" ), 
                              &criteriaParams );
    
	pObserver observer;
	algorithm.SubscribeObserver( &observer );
    
	algorithm.StartSolving( false );
    
	algorithm.WaitForThreads();
    
	GaFinalize();
    
    printf("Done\n");
    
	return 0;
}
