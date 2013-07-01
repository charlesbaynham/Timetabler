
/*! \file Initialization.h
    \brief This file contains implementation of functions for initialization and finalization of the library.
*/

/*
 * 
 * website: N/A
 * contact: kataklinger@gmail.com
 *
 */

/*
 * Genetic Algorithms Library
 * Copyright (C) 2007-2012 Mladen Jankovic
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "Initialization.h"

#include "Random.h"

#include "CrossoverOperations.h"
#include "MutationOperations.h"
#include "FitnessComparators.h"

#include "Population.h"

#include "SelectionOperations.h"
#include "CouplingOperations.h"
#include "ReplacementOperations.h"
#include "ScalingOperations.h"

#include "StopCriterias.h"

using namespace Chromosome;
using namespace Chromosome::CrossoverOperations;
using namespace Chromosome::MutationOperations;
using namespace Chromosome::FitnessComparators;

using namespace Population;
using namespace Population::SelectionOperations;
using namespace Population::CouplingOperations;
using namespace Population::ReplacementOperations;
using namespace Population::ScalingOperations;

using namespace Algorithm;
using namespace Algorithm::StopCriterias;

GAL_API Common::GaRandomInteger* GaGlobalRandomIntegerGenerator = 0;
GAL_API Common::GaRandomFloat* GaGlobalRandomFloatGenerator = 0;
GAL_API Common::GaRandomDouble* GaGlobalRandomDoubleGenerator = 0;
GAL_API Common::GaRandomBool* GaGlobalRandomBoolGenerator = 0;

void GaInitialize()
{
	GaGlobalRandomIntegerGenerator = new GaRandomInteger();
	GaGlobalRandomFloatGenerator = new GaRandomFloat();
	GaGlobalRandomDoubleGenerator = new GaRandomDouble();
	GaGlobalRandomBoolGenerator = new GaRandomBool();

	// catalogue of crossover operations
	GaCrossoverCatalogue::MakeInstance();
	GaCrossoverCatalogue::Instance().Register( "GaAddCrossover", new GaAddCrossover() );
	GaCrossoverCatalogue::Instance().Register( "GaMidpointCrossover", new GaMidpointCrossover() );
	GaCrossoverCatalogue::Instance().Register( "GaMultiValueCrossover", new GaMultiValueCrossover() );
	GaCrossoverCatalogue::Instance().Register( "GaSubCrossover", new GaSubCrossover() );

	// catalogue of mutation operations
	GaMutationCatalogue::MakeInstance();
	GaMutationCatalogue::Instance().Register( "GaFlipMutation", new GaFlipMutation() );
	GaMutationCatalogue::Instance().Register( "GaInvertMutation", new GaInvertMutation() );
	GaMutationCatalogue::Instance().Register( "GaSwapMutation", new GaSwapMutation() );

	// catalogue of fitness operations
	GaFitnessComparatorCatalogue::MakeInstance();
	GaFitnessComparatorCatalogue::Instance().Register( "GaMinFitnessComparator", new GaMinFitnessComparator() );
	GaFitnessComparatorCatalogue::Instance().Register( "GaMaxFitnessComparator", new GaMaxFitnessComparator() );

	// catalogue of selection operations
	GaSelectionCatalogue::MakeInstance();
	GaSelectionCatalogue::Instance().Register( "GaSelectBest", new GaSelectBest() );
	GaSelectionCatalogue::Instance().Register( "GaSelectRandom", new GaSelectRandom() );
	GaSelectionCatalogue::Instance().Register( "GaSelectRandomBest", new GaSelectRandomBest() );
	GaSelectionCatalogue::Instance().Register( "GaSelectRouletteWheel", new GaSelectRouletteWheel() );
	GaSelectionCatalogue::Instance().Register( "GaSelectTournament", new GaSelectTournament() );
	GaSelectionCatalogue::Instance().Register( "GaSelectWorst", new GaSelectWorst() );

	// catalogue of coupling operations
	GaCouplingCatalogue::MakeInstance();
	GaCouplingCatalogue::Instance().Register( "GaBestAlwaysCoupling", new GaBestAlwaysCoupling() );
	GaCouplingCatalogue::Instance().Register( "GaCrossCoupling", new GaCrossCoupling() );
	GaCouplingCatalogue::Instance().Register( "GaInverseCoupling", new GaInverseCoupling() );
	GaCouplingCatalogue::Instance().Register( "GaRandomCoupling", new GaRandomCoupling() );
	GaCouplingCatalogue::Instance().Register( "GaSimpleCoupling", new GaSimpleCoupling() );

	// catalogue of replacement operations
	GaReplacementCatalogue::MakeInstance();
	GaReplacementCatalogue::Instance().Register( "GaReplaceBest", new GaReplaceBest() );
	GaReplacementCatalogue::Instance().Register( "GaReplaceParents", new GaReplaceParents() );
	GaReplacementCatalogue::Instance().Register( "GaReplaceRandom", new GaReplaceRandom() );
	GaReplacementCatalogue::Instance().Register( "GaReplaceWorst", new GaReplaceWorst() );

	// catalogue of scaling operations
	GaScalingCatalogue::MakeInstance();
	GaScalingCatalogue::Instance().Register( "GaExponentialScaling", new GaExponentialScaling() );
	GaScalingCatalogue::Instance().Register( "GaLinearScaling", new GaLinearScaling() );
	GaScalingCatalogue::Instance().Register( "GaNormalizationScaling", new GaNormalizationScaling() );
	GaScalingCatalogue::Instance().Register( "GaWindowScaling", new GaWindowScaling() );

	// catalogue of stop criterias for genetica algorithms
	GaStopCriteriaCatalogue::MakeInstance();
	GaStopCriteriaCatalogue::Instance().Register( "GaFitnessCriteria", new GaFitnessCriteria() );
	GaStopCriteriaCatalogue::Instance().Register( "GaFitnessProgressCriteria", new GaFitnessProgressCriteria() );
	GaStopCriteriaCatalogue::Instance().Register( "GaGenerationCriteria", new GaGenerationCriteria() );

	// default setup for population operations
	GaPopulationConfiguration::MakeDefault();
}

void GaFinalize()
{
	delete GaGlobalRandomIntegerGenerator;
	delete GaGlobalRandomFloatGenerator;
	delete GaGlobalRandomDoubleGenerator;
	delete GaGlobalRandomBoolGenerator;

	GaCrossoverCatalogue::FreeInstance();
	GaMutationCatalogue::FreeInstance();
	GaFitnessComparatorCatalogue::FreeInstance();

	GaSelectionCatalogue::FreeInstance();
	GaCouplingCatalogue::FreeInstance();
	GaReplacementCatalogue::FreeInstance();
	GaScalingCatalogue::FreeInstance();

	GaStopCriteriaCatalogue::FreeInstance();

	GaPopulationConfiguration::FreeDefault();
}
