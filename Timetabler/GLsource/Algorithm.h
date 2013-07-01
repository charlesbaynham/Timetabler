
/*! \file Algorithm.h
    \brief This file contains declaration of interfaces, classes and datatypes that are used by implementations of genetic algorithms..
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

#ifndef __GA_ALGORITHM_H__
#define __GA_ALGORITHM_H__

#include "Platform.h"
#include "Threading.h"
#include "Observing.h"
#include "Catalogue.h"
#include "Population.h"
#include "AlgorithmOperations.h"
#include "AlgorithmState.h"

using namespace Threading;
using namespace Observing;
using namespace Common;
using namespace Population;
using namespace Algorithm;

/// <summary>Contains interfaces, classes and datatypes that are used by implementations of genetic algorithms.</summary>
namespace Algorithm
{
	/// <summary>Interface for parameters of genetic algorithm.</summary>
	class GaAlgorithmParams : public GaParameters { }; // END CLASS DEFINITION GaAlgorithmParams

	/// <summary>This enumeration is used to identify type of thread which is used for execution by the genetic algorithm.</summary>
	enum GaAlgorithmThreadType
	{
		/// <summary>represents control thread.</summary>
		GATT_CONTROL = 0, 

		/// <summary>represents worker thread.</summary>
		GATT_WORK = 1
	};

	/// <summary>Interface for genetic algorithms.</summary>
	class GaAlgorithm
	{

	public:

		/// <summary>Because this is base class, virtual destructor must be defined in order to enable calling of right destructor
		/// (destructor of inheriting class).</summary>
		virtual ~GaAlgorithm() { };

		/// <summary>This method starts execution of algorithm from beginning, or resumes previously paused execution.</summary>
		/// <param name="continueSolving">if this parameter is set to <c>false</c>, algorithm starts execution from beginning,
		/// it clears previous state and results. If this parameter is set to <c>true</c> it continues previously paused execution.</param>
		virtual void GACALL StartSolving(bool continueSolving)=0;

		/// <summary>This method stops execution of algorithm. User cannot resume execution after call of this method;
		/// it can only start execution from beginning. </summary>
		virtual void GACALL StopSolving()=0;

		/// <summary>This method pauses execution of algorithm. User can resume, or can start execution from beginning. </summary>
		virtual void GACALL PauseSolving()=0;

		/// <summary>This method should be called by user when changes of algorithm are required, such as change of a genetic operation or its parameters,
		/// stop criteria or other. It blocks thread which called this until it is safe to carry the changes, then it release the thread and blocks execution
		/// of algorithm if it running. This call should be fallowed by <see cref="EndParametersChange" /> call when it is over with changes.</summary>
		virtual void GACALL BeginParameterChange()=0;

		/// <summary>This method should be called by user when changes it is over with changes. It unblocks execution of algorithm if it was running.</summary>
		virtual void GACALL EndParameterChange()=0;

		/// <summary>This method returns reference to object that holds pointer to stop criteria of by the algorithm and its parameters.</summary>
		/// <returns>Method returns reference to object that holds pointer to stop criteria of by the algorithm and its parameters.</returns>
		virtual const GaStopCriteriaPair& GACALL StopCriteria() const=0;

		/// <summary><c>SetStopCriteria</c> method sets stop criteria and its parameters which is used by genetic algorithm.
		/// It makes copy of parameters object by using <see cref="GaParameters::Clone" /> method.</summary>
		/// <param name="criteria">pointer to new stop criteria.</param>
		/// <param name="parameters">pointer to parameters of stop criteria.</param>
		virtual void GACALL SetStopCriteria(GaStopCriteria* criteria,
			GaStopCriteriaParams* parameters)=0;

		/// <summary><c>SetStopCriteriaParams</c> method sets parameters for stop criteria.
		/// It makes copy of parameters object by using <see cref="GaParameters::Clone" /> method. </summary>
		/// <param name="parameters">pointer to parameters of stop criteria.</param>
		virtual void GACALL SetStopCriteriaParams(GaStopCriteriaParams* parameters)=0;

		/// <summary>This method returns reference to algorithm parameters.</summary>
		/// <returns>Method returns reference to algorithm parameters.</returns>
		virtual const GaAlgorithmParams& GACALL GetAlgorithmParameters() const=0;

		/// <summary><c>SetAlgorithmParameters</c> method sets algorithm parameters.</summary>
		/// <param name="parameters">reference to new paremters of algorithm.</param>
		virtual void GACALL SetAlgorithmParameters(const GaAlgorithmParams& parameters)=0;

		/// <summary>This method returns reference to object which contains statistical information about execution of algorithm.</summary>
		/// <returns>Method returns reference to object which contains statistical information about execution of algorithm.</returns>
		virtual const GaStatistics& GACALL GetAlgorithmStatistics() const=0;

		/// <summary><c>GetPopulation</c> method returns reference to population which is used by algorithm with specified index.</summary>
		/// <param name="index">index of population.</param>
		/// <returns>Method returns reference to population with specified index.</returns>
		virtual const GaPopulation& GACALL GetPopulation(int index) const=0;

		/// <summary>This method returns algorithm's execution state.</summary>
		/// <returns>Method returns algorithm's execution state.</returns>
		virtual GaAlgorithmState GACALL GetState() const=0;

		/// <summary>This method subscribe observer to events of algorithm</summary>
		/// <param name="observer">pointer to observer which should be subscribed.</param>
		virtual void GACALL SubscribeObserver(GaObserver* observer)=0;

		/// <summary>This method unsubscribe observer from events of algorithm. </summary>
		/// <param name="observer">pointer to observer which should be unsubscribed.</param>
		virtual void GACALL UnsubscribeObserver(GaObserver* observer)=0;

	protected:

		/// <summary>This method should be called within algorithm when it enters critical section in which it cannot handle any changes of parameters,
		/// operation or other aspects. This call should be fallowed by <see cref="ReleaseParameterChanges" /> call
		/// when algorithm exits critical section.</summary>
		virtual void GACALL BlockParameterChanges()=0;

		/// <summary>This method should be called within algorithm when it exit critical section it is able to handle changes of parameters,
		/// operation or other aspects.</summary>
		virtual void GACALL ReleaseParameterChanages()=0;

	};// END CLASS DEFINITION GaAlgorithm

	/// <summary>This class implements control of algorithm's execution and its state. It also provides synchronization for multithreading control of algorithm.
	///
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class,
	/// but all public method and operators are thread-safe.
	///
	/// Next diagram show possible states of algorithm's execution and transitions.
	///
	/// <img src="graphic/alg_st.png"></img></summary>
	class GaBaseAlgorithm : public GaAlgorithm
	{

	protected:

		/// <summary>Algorithm's execution state.</summary>
		GaAlgorithmState _state;

		/// <summary>Stores referenc to stop criteria and its parameters.</summary>
		GaStopCriteriaPair _stopCriteria;

		/// <summary>Critical section object for synchronization of parameters and operations changes from multiple threads.</summary>
		GaCriticalSection _syncParameterChanges;

		/// <summary>Critical section object is used for synchronization of algorithm's execution state changes from multiple threads.</summary>
		GaCriticalSection _syncStateChange;

		/// <summary>List of observers which are subscribed to events of this algorithm.</summary>
		GaObserversList _observers;

	public:

		/// <summary>This constructor sets initial state of algorithm's execution.</summary>
		GaBaseAlgorithm() : _state(GAS_UNINITIALIZED) { }

		/// <summary>Detailed description is given in specification of <see cref="GaAlgorithm::StartSolving" /> method.
		/// Diagram shows when this method can be called.
		///
		/// This method is thread-safe.</summary>
		GAL_API
		virtual void GACALL StartSolving(bool continueSolving);

		/// <summary>Detailed description is given in specification of <see cref="GaAlgorithm::StopSolving" /> method.
		/// Diagram shows when this method can be called.
		///
		/// This method is thread-safe.</summary>
		GAL_API
		virtual void GACALL StopSolving();

		/// <summary>Detailed description is given in specification of <see cref="GaAlgorithm::PauseSolving" /> method.
		/// Diagram shows when this method can be called.
		///
		/// This method is thread-safe.</summary>
		GAL_API
		virtual void GACALL PauseSolving();

		/// <summary>Detailed description is given in specification of <see cref="GaAlgorithm::BeginParameterChange" /> method.
		///
		/// This method is thread-safe.</summary>
		virtual void GACALL BeginParameterChange() { LOCK( _syncParameterChanges ); }

		/// <summary>Detailed description is given in specification of <see cref="GaAlgorithm::BeginParameterChange" /> method.
		///
		/// This method is thread-safe.</summary>
		virtual void GACALL EndParameterChange() { UNLOCK( _syncParameterChanges ); }

		/// <summary>Detailed description is given in specification of <see cref="GaAlgorithm::StopCriteria" /> method.
		///
		/// This method is thread-safe.</summary>
		virtual const GaStopCriteriaPair& GACALL StopCriteria() const { return _stopCriteria; }

		// Sets stop criteria and it's parameters
		/// <summary>Detailed description is given in specification of <see cref="GaAlgorithm::SetStopCriteria" /> method.
		///
		/// This method is thread-safe.</summary>
		virtual void GACALL SetStopCriteria(GaStopCriteria* criteria,
			GaStopCriteriaParams* parameters) { _stopCriteria.SetOperation( criteria, *parameters ); }

		// Set parameters for stop criteria
		/// <summary>Detailed description is given in specification of <see cref="GaAlgorithm::SetStopCriteriaParams" /> method.
		///
		/// This method is thread-safe.</summary>
		virtual void GACALL SetStopCriteriaParams(GaStopCriteriaParams* parameters) { _stopCriteria.SetParameters( *parameters ); }

		// Returns the state of the evolution and problem solving.
		/// <summary>Detailed description is given in specification of <see cref="GaAlgorithm::GetState" /> method.
		///
		/// This method is thread-safe.</summary>
		virtual GaAlgorithmState GACALL GetState() const { return _state; }

		// Subscribes algorithm's observer
		/// <summary>Detailed description is given in specification of <see cref="GaAlgorithm::SubscribeObserver" /> method.
		///
		/// This method is thread-safe.</summary>
		virtual void GACALL SubscribeObserver(GaObserver* observer) { _observers += *observer; }

		// Subscribes algorithm's observer
		/// <summary>Detailed description is given in specification of <see cref="GaAlgorithm::UnsubscribeObserver" /> method.
		///
		/// This method is thread-safe.</summary>
		virtual void GACALL UnsubscribeObserver(GaObserver* observer) { _observers -= *observer; }

	protected:

		/// <summary>This method checks current state of the algorithm against desired state specified by stop criteria and if the state is reached
		/// it stops execution of algorithm.
		/// 
		/// This method is not thread-safe.</summary>
		GAL_API
		virtual bool GACALL CheckStopCriteria();

		/// <summary>This method is called when user has successfully started new execution. It should restart previous state and results and 
		/// initialize and prepares populations, statistical information and other data and control structures.
		///
		/// This method is not thread-safe.</summary>
		virtual void GACALL Initialize()=0;

		/// <summary><c>OnStart</c> method is called when user has successfully started new execution.
		/// This method is called after <see cref="Initialize" /> method. </summary>
		/// <returns>Method returns <c>true</c> if algorithm is started succesafully.</returns>
		virtual bool GACALL OnStart()=0;

		/// <summary><c>OnStop</c> method is called when user requests stopping of execution.</summary>
		/// <returns>Method returns <c>true</c> if algorithm is stopped succesafully.</returns>
		virtual bool GACALL OnStop()=0;

		/// <summary><c>OnPause</c> method is called when user requests pausing of execution.</summary>
		/// <returns>Method returns <c>true</c> if algorithm is paused succesafully.</returns>
		virtual bool GACALL OnPause()=0;

		/// <summary><c>OnResume</c> method is called when user has successfully resumed previously paused execution.</summary>
		/// <returns>Method returns <c>true</c> if algorithm is resumed succesafully.</returns>
		virtual bool GACALL OnResume()=0;

		/// <summary>Detailed description is given in specification of <see cref="GaAlgorithm::BlockParameterChanges" /> method.
		///
		/// This method is thread-safe.</summary>
		virtual void GACALL BlockParameterChanges() { LOCK( _syncParameterChanges ); }

		/// <summary>Detailed description is given in specification of <see cref="GaAlgorithm::ReleaseParameterChanages" /> method.
		///
		/// This method is thread-safe.</summary>
		virtual void GACALL ReleaseParameterChanages() { UNLOCK( _syncParameterChanges ); }

		/// <summary>This method should be called within algorithm when it enters critical section in which it cannot handle any state changes of its execution.
		/// This call should be fallowed by <see cref="ReleaseStateChange" /> call when algorithm exits critical section.
		///
		/// This method is thread-safe.</summary>
		virtual void GACALL BlockStateChange() { LOCK( _syncStateChange ); }

		/// <summary>This method should be called within algorithm when it exits critical section it is able to handle state changes of its execution.
		///
		/// This method is thread-safe.</summary>
		virtual void GACALL ReleaseStateChange() { UNLOCK( _syncStateChange ); }

	};// END CLASS DEFINITION GaBaseAlgorithm

} // Algorithm

#endif // __GA_ALGORITHM_H__
