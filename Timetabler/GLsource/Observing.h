
/*! \file Observing.h
    \brief This file contains declaration of interfaces and classes needed to observe execution of an genetic algorithm.
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

#ifndef __GA_OBSERVING_H__
#define __GA_OBSERVING_H__

#include <list>

#include "Platform.h"
#include "Threading.h"
#include "Chromosome.h"
#include "Statistics.h"
#include "AlgorithmState.h"

namespace Algorithm
{
	class GaAlgorithm;
}

using namespace std;
using namespace Common;
using namespace Chromosome;
using namespace Algorithm;

/// <summary>Contains interfaces and classes needed to observe execution of an genetic algorithm.</summary>
namespace Observing
{
	/// <summary><c>GaObserver</c> interface is base class for all algorithms' observers. This interface has defined methods to handle each 
	/// event which can be raised by an algorithm.</summary>
	class GaObserver
	{

	public:

		/// <summary>Because this is base class, virtual destructor must be defined in order to enable calling of right destructor 
		/// (destructor of inheriting class).</summary>
		virtual ~GaObserver() { }

		/// <summary>This method handles event which is raised at the end of a generation by the algorithm.</summary>
		/// <param name="statistics">reference to object with statistical information.</param>
		/// <param name="algorithm">reference the algorithm which raised event.</param>
		virtual void GACALL StatisticUpdate(const GaStatistics& statistics,
			const GaAlgorithm& algorithm)=0;

		/// <summary>This method handles event which is raised when algorithm finds new chromosome (solution)
		/// which is better then the best chromosome of previous generation.</summary>
		/// <param name="newChromosome">reference to the chromosome.</param>
		/// <param name="algorithm">reference the algorithm which raised event.</param>
		virtual void GACALL NewBestChromosome(const GaChromosome& newChromosome,
			const GaAlgorithm& algorithm)=0;

		/// <summary>This method handles event which is raised when state of algorithm's execution is change.</summary>
		/// <param name="newState">new state of algorithm's execution</param>
		/// <param name="algorithm">reference the algorithm which raised event.</param>
		virtual void GACALL EvolutionStateChanged(GaAlgorithmState newState,
			const GaAlgorithm& algorithm)=0;

	};// END INTERFACE DEFINITION GaObserver

	/// <summary>This is adapter class for GaObserver interface. It overrides all methods from the interface, 
	/// so classes inherited from this class can implements only necessary methods. Detail description of the methods is given in specification
	/// of <see cref="GaObserver" /> interface.</summary>
	class GaObserverAdapter : public GaObserver
	{

	public:

		virtual void GACALL StatisticUpdate(const GaStatistics& statistics,
			const GaAlgorithm& algorithm) {};

		virtual void GACALL NewBestChromosome(const GaChromosome& newChromosome,
			const GaAlgorithm& algorithm) {};

		virtual void GACALL EvolutionStateChanged(GaAlgorithmState newState,
			const GaAlgorithm& algorithm) {};

	};// END CLASS DEFINITION GaObserverAdapter

	/// <summary>This class manages subscriptions of observers to algorithm's events.
	///
	/// This class has built-in synchronizator, so it is allowed to use <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros with instances of this class.
	/// All public methods are thread-safe.</summary>
	class GaObserversList : public GaObserver
	{
		DEFINE_SYNC_CLASS

	protected:

		/// <summary>Alias for STL list class which contains pointers to <see cref="GaObserver" /> objects.</summary>
		typedef list<GaObserver*> GaObserversListType;

		/// <summary>List of subscribed observers.</summary>
		GaObserversListType _observers;

	public:

		/// <summary>This method notifies all subscribed observer that algorithm has reached end of the current generation. 
		/// 
		/// This method is thread-safe.</summary>
		/// <param name="statistics">reference to object with statistical information.</param>
		/// <param name="algorithm">reference the algorithm which raised event.</param>
		GAL_API
		virtual void GACALL StatisticUpdate(const GaStatistics& statistics,
			const GaAlgorithm& algorithm);

		/// <summary>This method notifies all subscribed observer that algorithm found new chromosome which is better then the best chromosome
		/// form previous population
		///
		/// This method is thread-safe.</summary>
		/// <param name="newChromosome">reference to the chromosome.</param>
		/// <param name="algorithm">reference the algorithm which raised event.</param>
		GAL_API
		virtual void GACALL NewBestChromosome(const GaChromosome& newChromosome,
			const GaAlgorithm& algorithm);

		/// <summary>This method notifies all subscribed observer that state of algorithm's execution has changed. 
		///
		/// This method is thread-safe.</summary>
		/// <param name="newState">new state of algorithm's execution</param>
		/// <param name="algorithm">reference the algorithm which raised event.</param>
		GAL_API
		virtual void GACALL EvolutionStateChanged(GaAlgorithmState newState,
			const GaAlgorithm& algorithm);

		/// <summary><c>operator +=</c> subscribes new observer. If observer is already subscribed call has no affect.
		///
		/// This operator is thread-safe.</summary>
		/// <param name="observer">observer which will be subscribed.</param>
		/// <returns>Method returns reference to <c>this</c> object.</returns>
		GAL_API
		GaObserversList& GACALL operator +=(GaObserver& observer);

		/// <summary><c>operator -=</c> unsubscribes the observer. If observer is not subscribed call has no affect.
		///
		/// This operator is thread-safe.</summary>
		/// <param name="observer">observer which will be unsubscribed.</param>
		/// <returns>Method returns reference to <c>this</c> object.</returns>
		GAL_API
		GaObserversList& GACALL operator -=(GaObserver& observer);

		// Returns the number of subscribed observers
		/// <summary>This method is thread-safe.</summary>
		/// <returns>Method returns number of subscribed observers.</returns>
		GAL_API
		int GACALL GetObserverCount() const;

	};// END CLASS DEFINITION GaObserversList

} // Observing

#endif //__GA_OBSERVING_H__
