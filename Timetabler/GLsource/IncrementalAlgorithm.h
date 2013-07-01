
/*! \file IncrementalAlgorithm.h
    \brief This file declares classes of incremental genetic algorithm with overlapping population and its parameters.
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

#ifndef __GA_INCREMENTAL_ALGORITHM_H__
#define __GA_INCREMENTAL_ALGORITHM_H__

#include <vector>
#include "Platform.h"
#include "Threading.h"
#include "MultithreadingAlgorithm.h"

namespace Population
{
	class GaPopulation;
}

using namespace std;
using namespace Threading;
using namespace Population;
using namespace Algorithm;

namespace Algorithm
{
	namespace SimpleAlgorithms
	{

		/// <summary>Incremental algorithm replaces only specified number of chromosomes at the end of each generation,
		/// thus allowing majority of chromosomes to survive.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class,
		/// but all public method and operators are thread-safe.</summary>
		class GaIncrementalAlgorithm : public GaMultithreadingAlgorithm
		{

		private:

			/// <summary>Parameters of genetic algorithm.</summary>
			GaMultithreadingAlgorithmParams _parameters;

			/// <summary>Pointer to population on which evolution is performed.</summary>
			GaPopulation* _population;

			/// <summary>Pointer to coupling result set (and selection result set) which is used for storing produced offspring
			/// which are going to replace chromosomes from current generation. </summary>
			GaCouplingResultSet* _buffer;

			/// <summary>Pointer to the best chromosome found so far by the algorithm.</summary>
			GaChromosomePtr _bestChromosome;

		public:

			/// <summary>This constructor initializes algorithm with population on which evolution is performed and with parameters of algorithm.</summary>
			/// <param name="population">pointer to population on which evolution is performed.</param>
			/// <param name="parameters">reference to parameters of algorithm.</param>
			GaIncrementalAlgorithm(GaPopulation* population,
				const GaMultithreadingAlgorithmParams& parameters) : GaMultithreadingAlgorithm(parameters),
				_population(population),
				_parameters(parameters)
			{
				_buffer = new GaCouplingResultSet( 0, new GaSelectionResultSet( 0, _population ) );
			}

			/// <summary>The destructor frees memory used by result sets.</summary>
			virtual ~GaIncrementalAlgorithm()
			{
				delete &_buffer->GetSelectionResultSet();
				delete _buffer;
			}

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns reference to parameters of algorithm</returns>
			virtual const GaAlgorithmParams& GACALL GetAlgorithmParameters() const { return _parameters; }

			/// <summary>This method should be called from classes which are inheriting this class when user changes algorithm parameters.
			///
			/// More details are given in specification of <see cref="GaAlgorithm::SetAlgorithmParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL SetAlgorithmParameters(const GaAlgorithmParams& parameters);

			/// <summary><c>GetPopulation</c> method always returns reference to population which is used by the algorithm and
			/// which is by the user when algorithm is constructed.
			///
			/// More details are given in specification of <see cref="GaAlgorithm::GetPopulation" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual const GaPopulation& GACALL GetPopulation(int index) const { return *_population; }

			/// <summary><c>GetAlgorithmStatistics</c> method returns reference to object which stores statistical information about population.
			///
			/// More details are given in specification of <see cref="GaAlgorithm::GetAlgorithmStatistics" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual const GaStatistics& GACALL GetAlgorithmStatistics() const { return _population->GetStatistics(); }

		protected:

			/// <summary>This method initializes population by calling <see cref="GaPopulation::Initialize" /> method.</summary>
			virtual void GACALL Initialize() { _population->InitializePopulation(); }

			/// <summary>This operation performs selection of chromosomes from population of current generation.
			/// It uses selection operation provided by the population.</summary>
			GAL_API
			virtual void GACALL BeforeWorkers();

			/// <summary>This operation performs coupling operation and produces offspring chromosomes by using previously selected chromosomes as parents.
			/// It uses coupling operation provided by the population.</summary>
			/// <param name="workerId">identification number of working thread.</param>
			GAL_API
			virtual void GACALL WorkStep(int workerId);

			/// <summary>This operation performs replacement of chromosomes from current generation with new offspring chromosomes produced
			/// in coupling operation. It uses replacement operation provided by the population.</summary>
			GAL_API
			virtual void GACALL AfterWorkers();

		};// END CLASS DEFINITION GaIncrementalAlgorithm

	} // SimpleAlgorithms
} // Algorithm

#endif // __GA_INCREMENTAL_ALGORITHM_H__
