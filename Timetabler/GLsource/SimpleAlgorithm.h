
/*! \file SimpleAlgorithm.h
    \brief This file declares classes of simple genetic algorithm with non-overlapping populations and its parameters.
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

#ifndef __GA_SIMPLE_ALGORITHM_H__
#define __GA_SIMPLE_ALGORITHM_H__

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
	/// <summary>Contains implementation of simple genetic algorithms.</summary>
	namespace SimpleAlgorithms
	{

		/// <summary><c>GaPopulationType</c> is used to enumerate two non-overlapping populations used by <see cref="GaSimpleAlgorithm" />.</summary>
		enum GaPopulationType
		{

			/// <summary>first (initial) population.</summary>
			GAPT_POPULATION_A = 0, 

			/// <summary>second (helper) population. </summary>
			GAPT_POPULATION_B = 1

		};

		// Parameters for simple algorithm which use non-overlapping populations.
		/// <summary>This class is used by <see cref="GaSimpleAlgorithm" /> as parameters.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		///No public or private methods are thread-safe.</summary>
		class GaSimpleAlgorithmParams : public GaMultithreadingAlgorithmParams
		{

		private:

			/// <summary>Number of best chromosomes in current generation which survives to the next.</summary>
			int _elitism;

		public:

			/// <summary>This constructor initializes parameters with user-defined elitism and number of workers.</summary>
			/// <param name="elitism">number of best chromosomes in current generation which survives to the next.</param>
			/// <param name="numberOfWorkers">number of working threads.</param>
			GaSimpleAlgorithmParams(int elitism,
				int numberOfWorkers) : GaMultithreadingAlgorithmParams(numberOfWorkers),
				_elitism(elitism) { }

			/// <summary>This constructor initializes parameters default values. Default number of working threads is 1 and elitism size is 0.</summary>
			GaSimpleAlgorithmParams() : _elitism(0) { }

			/// <summary>More details are given in specification of <see cref="GaParameters::Clone" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual GaParameters* GACALL Clone() const { return new GaSimpleAlgorithmParams( *this ); }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns number of best chromosomes in current generation which survives to the next.</returns>
			inline int GACALL GetElitism() const { return _elitism; }

			/// <summary><c>SetElitism</c> method sets number of best chromosomes in current generation which survives to the next.
			///
			/// This method is not-thread-safe.</summary>
			/// <param name="elitism">number of chromosomes.</param>
			inline void GACALL SetElitism(int elitism) { _elitism = elitism; }

		};// END CLASS DEFINITION GaSimpleAlgorithmParams

		/// <summary>Simple algorithm creates entirely new population of chromosomes at the end of each generation,
		/// only specified number of best chromosomes survive and they are copied into new generation. This algorithm uses two populations to
		/// avoid overlapping of generations which are used alternately.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class,
		/// but all public method and operators are thread-safe.</summary>
		class GaSimpleAlgorithm : public GaMultithreadingAlgorithm
		{

		private:

			/// <summary>Indicates which population, of two, is used for current generation.</summary>
			GaPopulationType _currentPopulation;

			/// <summary>Pointer to array which is used to store best chromosomes which will survive to the next generation.</summary>
			GaChromosomePtr* _elitismBuffer;

			/// <summary>Number of chromosomes which are stored in elitism buffer.</summary>
			int _savedChromosomes;

			/// <summary>Pointers to coupling result sets (and selection result sets) which are used for storing produced offspring
			/// which are going to be inserted in new generation. Each population has its own coupling and selection sets.</summary>
			GaCouplingResultSet* _buffers[ 2 ];

			/// <summary>Array of pointers to two non-overlapping populations which are used by algorithm.</summary>
			GaPopulation* _populations[ 2 ];

			/// <summary>Parameters of genetic algorithm.</summary>
			GaSimpleAlgorithmParams _parameters;

			/// <summary>Pointer to the best chromosome found so far by the algorithm.</summary>
			GaChromosomePtr _bestChromosome;

		public:

			/// <summary>This constructor initializes algorithm with population on which evolution is performed and with parameters of algorithm.
			/// It also makes helper population with exactly the same setup as the original population which is used to
			/// prevent overlapping of generations.</summary>
			/// <param name="population">pointer to population on which evolution is performed.</param>
			/// <param name="parameters">reference to parameters of algorithm.</param>
			GAL_API
			GaSimpleAlgorithm(GaPopulation* population,
				const GaSimpleAlgorithmParams& parameters);

			/// <summary>The destructor destroys helper population and frees memory used by elitism buffer and result sets.</summary>
			GAL_API
			virtual ~GaSimpleAlgorithm();

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns reference to parameters of algorithm.</returns>
			virtual const GaAlgorithmParams& GACALL GetAlgorithmParameters() const { return _parameters; }

			/// <summary>This method should be called from classes which are inheriting this class when user changes algorithm parameters.
			///
			/// More details are given in specification of <see cref="GaAlgorithm::SetAlgorithmParameters" /> method.
			/// 
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL SetAlgorithmParameters(const GaAlgorithmParams& parameters);

			/// <summary>Population with index 0 is original population, and population with index 1 is helper population
			///
			/// More details are given in specification of <see cref="GaAlgorithm::SetPopulation" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual const GaPopulation& GACALL GetPopulation(int index) const { return *_populations[ _currentPopulation ]; }

			/// <summary><c>GetAlgorithmStatistics</c> method returns reference to object which stores statistical information
			/// about population which handles current generation.
			///
			/// More details are given in specification of <see cref="GaAlgorithm::SetAlgorithmStatistics" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual const GaStatistics& GACALL GetAlgorithmStatistics() const { return _populations[ _currentPopulation ]->GetStatistics(); }

		protected:

			/// <summary>This method initializes original population by calling <see cref="GaPopulation::Initialize" /> method,
			/// and clears helper population.</summary>
			GAL_API
			virtual void GACALL Initialize();

			/// <summary>This operation performs selection of chromosomes from population of current generation.
			/// It uses selection operation provided by the population.</summary>
			GAL_API
			virtual void GACALL BeforeWorkers();

			/// <summary>This operation stores best chromosome in elitism buffer and performs coupling operation and produces offspring chromosomes
			/// by using previously selected chromosomes as parents. It uses coupling operation provided by the population.</summary>
			/// <param name="workerId">identification number of working thread.</param>
			GAL_API
			virtual void GACALL WorkStep(int workerId);

			/// <summary>This method inserts offspring chromosomes and chromosomes from elitism buffer into population which will handle next generation.
			/// It also transfers statistics from current population to next.</summary>
			GAL_API
			virtual void GACALL AfterWorkers();

		};// END CLASS DEFINITION GaSimpleAlgorithm

	} // SimpleAlgorithms
} // Algorithm

#endif // __GA_SIMPLE_ALGORITHM_H__
