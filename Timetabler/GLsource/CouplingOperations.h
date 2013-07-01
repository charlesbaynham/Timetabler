
/*! \file CouplingOperations.h
    \brief This file contains declaration of classes that implement coupling operations.
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

#ifndef __GA_COUPLING_OPERATION_H__
#define __GA_COUPLING_OPERATION_H__

#include "PopulationOperations.h"

using namespace Population;

namespace Population
{
	/// <summary>Contains implementation of some basic coupling operations.</summary>
	namespace CouplingOperations
	{

		/// <summary><c>GaSimpleCoupling</c> coupling takes first two parents and it produces two child using crossover operations,
		/// and each parent is bound to one child, then it takes next two parents, and so on... If all parents are used, but more children should be produced,
		/// this coupling operation wraps-around parent counter and starts from beginning until enough children is produced.
		/// This coupling use <see cref="GaCouplingParams" /> class for parameters.
		///
		/// <img src="graphic/s_cp.png"></img>
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaSimpleCoupling : public GaCouplingOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaCouplingOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(const GaPopulation& population,
				GaCouplingResultSet& output,
				const GaCouplingParams& parameters,
				int workerId,
				int numberOfWorkers) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaCouplingParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaCouplingParams(); }

			/// <summary>Valid parameters must have number of offspring for production grater then 0.
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const
			{
				return ( (const GaCouplingParams&) parameters ).GetNumberOfOffsprings() > 0;
			}

		};// END CLASS DEFINITION GaSimpleCoupling

		/// <summary>This class should be used (directly or by inheritance) by coupling operations which produce more then one child per parent pair before
		/// it chooses best child.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		class GaMulitpleCrossoverCouplingParams : public GaCouplingParams
		{

		private:

			/// <summary>Number of chromosomes which are produced per one pair of parents.</summary>
			int _offspringsPerParentPair;

		public:

			/// <summary>This constructor initializes parameters with user defined values.</summary>
			/// <param name="numberOfOffsprings">number of chromosomes which should be produced.</param>
			/// <param name="checkForDuplicates">this parameter should be set to <c>true</c> if operation should check whether the produced offspring already exist in population
			/// before it is inserted in couling result set.</param>
			/// <param name="offspringsPerParentPair">number of chromosomes which are produced per one pair of parents.</param>
			GaMulitpleCrossoverCouplingParams(int numberOfOffsprings,
				bool checkForDuplicates,
				int offspringsPerParentPair) : GaCouplingParams(numberOfOffsprings, checkForDuplicates),
				_offspringsPerParentPair(offspringsPerParentPair) { }

			/// <summary>This constructor initializes parameters with default values.
			/// Default number of produced chromosome is 2 and coupling operation should produce one child per parent pair, operation does not check for duplicates.</summary>
			GaMulitpleCrossoverCouplingParams() : _offspringsPerParentPair(1) { }

			/// <summary>More details are given in specification of <see cref="GaParameters::Clone" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual GaParameters* GACALL Clone() const { return new GaMulitpleCrossoverCouplingParams( *this ); }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns number of chromosomes which are produced per one pair of parents.</returns>
			inline int GACALL GetOffspringsPerParentPair() const { return _offspringsPerParentPair; }

			/// <summary><c>SetOffspringsPerParentPair</c> method sets number of chromosomes which are produced per one pair of parents.
			///
			/// This method is no thread-safe.</summary>
			/// <param name="number">number of chromosomes which are produced per one pair of parents.</param>
			inline void GACALL SetOffspringsPerParentPair(int number) { _offspringsPerParentPair = number; }

		};// END CLASS DEFINITION GaMulitpleCrossoverCouplingParams

		/// <summary><c>GaCrossCoupling</c> coupling takes first parents sequentially from selection result set, and second parents are the ones
		/// who are right below the first. If all parents are used, but more children should be produced, this coupling operation wraps-around
		/// parent counter and starts from beginning until enough children is produced. When two parents are chosen this operation produces specified number
		/// of children using crossover operation and then it chooses child with best fitness value among produced children and first parent is bound to it.
		/// This coupling use <see cref="GaMultipleCrossoverCouplingParams" /> class for parameters.
		///
		/// <img src="graphic/c_cp.png"></img>
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaCrossCoupling : public GaCouplingOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaCouplingOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(const GaPopulation& population,
				GaCouplingResultSet& output,
				const GaCouplingParams& parameters,
				int workerId,
				int numberOfWorkers) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaMulitpleCrossoverCouplingParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaMulitpleCrossoverCouplingParams(); }

			/// <summary>Valid parameters must have number of offspring for production and number of produced children per parent pair grater then 0.
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const
			{
				return ( (const GaMulitpleCrossoverCouplingParams& ) parameters ).GetNumberOfOffsprings() > 0 &&
					( (const GaMulitpleCrossoverCouplingParams& ) parameters ).GetOffspringsPerParentPair() > 0;
			}

		};// END CLASS DEFINITION GaCrossCoupling

		/// <summary><c>GaInverseCoupling</c> coupling takes first parents sequentially from selection results, and second parents are the ones who are at
		/// the distance from the last chromosome in selection results which is equal to distance of first parent form first chromosome in result set.
		/// If all parents are used, but more children should be produced, this coupling operation wraps-around parent counter and starts from beginning
		/// until enough children is produced. When two parents are chosen this operation produces specified number of children using crossover operation and
		/// then it chooses child with best fitness value among produced children and first parent is bound to it.
		/// This coupling use <see cref="GaMultipleCrossoverCouplingParams" /> class for parameters. 
		///
		/// <img src="graphic/i_cp.png"></img>
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaInverseCoupling : public GaCouplingOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaCouplingOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(const GaPopulation& population,
				GaCouplingResultSet& output,
				const GaCouplingParams& parameters,
				int workerId,
				int numberOfWorkers) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaMulitpleCrossoverCouplingParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaMulitpleCrossoverCouplingParams(); }

			/// <summary>Valid parameters must have number of offspring for production and number of produced children per parent pair grater then 0.
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const
			{
				return ( (const GaMulitpleCrossoverCouplingParams& ) parameters ).GetNumberOfOffsprings() > 0 &&
					( (const GaMulitpleCrossoverCouplingParams& ) parameters ).GetOffspringsPerParentPair() > 0;
			}

		};// END CLASS DEFINITION GaInverseCoupling

		/// <summary><c>GaRandomCoupling</c> coupling takes first parents sequentially from selection result set, and second parents are chosen randomly.
		/// If all parents are used as firs parent, but more children should be produced, this coupling operation wraps-around parent counter for the first parent
		/// and starts from beginning until enough children is produced. When two parents are chosen this operation produces specified number of children
		/// using crossover operation and then it chooses child with best fitness value among produced children and first parent is bound to it.
		/// This coupling use <see cref="GaMultipleCrossoverCouplingParams" /> class for parameters.
		///
		/// <img src="graphic/r_cp.png"></img>
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaRandomCoupling : public GaCouplingOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaCouplingOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(const GaPopulation& population,
				GaCouplingResultSet& output,
				const GaCouplingParams& parameters,
				int workerId,
				int numberOfWorkers) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaMulitpleCrossoverCouplingParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaMulitpleCrossoverCouplingParams(); }

			/// <summary>Valid parameters must have number of offspring for production and number of produced children per parent pair grater then 0.
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const
			{
				return ( (const GaMulitpleCrossoverCouplingParams& ) parameters ).GetNumberOfOffsprings() > 0 &&
					( (const GaMulitpleCrossoverCouplingParams& ) parameters ).GetOffspringsPerParentPair() > 0;
			}

		};// END CLASS DEFINITION GaRandomCoupling

		/// <summary><c>GaBestAlwaysCoupling</c> coupling always takes chromosome with the best fitness value from selection result set, and the second parents
		/// are sequentially taken from. If all parents are used, but more children should be produced, this coupling operation wraps-around parent counter
		/// for second the parents and starts from beginning until enough children is produced. When two parents are chosen this operation produces
		/// specified number of children using crossover operation and then it chooses child with best fitness value among produced children and the second parent
		/// is bound to it. This coupling use <see cref="GaMultipleCrossoverCouplingParams" /> class for parameters.
		///
		/// <img src="graphic/ab_cp.png"></img>
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaBestAlwaysCoupling : public GaCouplingOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaCouplingOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(const GaPopulation& population,
				GaCouplingResultSet& output,
				const GaCouplingParams& parameters,
				int workerId,
				int numberOfWorkers) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaMulitpleCrossoverCouplingParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaMulitpleCrossoverCouplingParams(); }

			/// <summary>Valid parameters must have number of offspring for production and number of produced children per parent pair grater then 0.
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const
			{
				return ( (const GaMulitpleCrossoverCouplingParams& ) parameters ).GetNumberOfOffsprings() > 0 &&
					( (const GaMulitpleCrossoverCouplingParams& ) parameters ).GetOffspringsPerParentPair() > 0;
			}
		};

	} // CouplingOperations
} // Population

#endif // __GA_COUPLING_OPERATION_H__
