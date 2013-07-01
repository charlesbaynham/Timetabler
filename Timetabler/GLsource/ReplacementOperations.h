
/*! \file ReplacementOperations.h
    \brief This file contains declaration of class of replacement operation that replaces the worst chromosomes.
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

#ifndef __GA_REPLACEMENT_OPERATIONS_H__
#define __GA_REPLACEMENT_OPERATIONS_H__

#include "PopulationOperations.h"

using namespace Population;

namespace Population
{
	/// <summary>Contains implementation of some basic replacement operations.</summary>
	namespace ReplacementOperations
	{

		/// <summary><c>GaReplaceWorst</c> replaces chromosomes with worst fitness values in population. If population is sorted this operation can
		/// replace any number of chromosomes, if it is not sorted this operation can only replace chromosomes which are stored in the worst chromosome 
		/// sorted group of population. This replacement operation use <see cref="GaReplacementParams" /> class for parameters. 
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaReplaceWorst : public GaReplacementOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaReplacementOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(GaPopulation& population,
				const GaReplacementParams& parameters,
				const GaCouplingResultSet& newChromosomes) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaReplacementParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaReplacementParams(); }

			/// <summary>Valid parameters must have number of chromosomes which should be replaced greater then 0. 
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const
			{
				return ( (const GaReplacementParams&) parameters ).GetReplacementSize() > 0;
			}

		};// END CLASS DEFINITION GaReplaceWorst

		/// <summary>This class should be used (directly or by inheritance) by replacement operations which can unintentionally remove the best chromosomes
		/// from population. 
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		class GaReplaceElitismParams : public GaReplacementParams
		{

		private:

			/// <summary>Number of best chromosomes that will not be removed from population by replacement operation.</summary>
			int _elitism;

		public:

			/// <summary>This constructor initializes parameters with user defined values.</summary>
			/// <param name="replacementSize">number of chromosomes which replaced by the operation.</param>
			/// <param name="elitism">number of the best chromosomes that will not be removed from population by replacement operation.</param>
			GaReplaceElitismParams(int replacementSize,
				int elitism) : GaReplacementParams(replacementSize),
				_elitism(elitism) { }

			/// <summary>This constructor initializes parameters with default values. Default number of replaced chromosomes is 2,
			/// and default elitism size is 0.</summary>
			GaReplaceElitismParams() : _elitism(0) { }

			/// <summary>More details are given in specification of <see cref="GaParameters::Clone" /> method.
			///
			/// This method is not thread-safe.</summary>
			virtual GaParameters* GACALL Clone() const { return new GaReplaceElitismParams( *this ); }

			// Returns number of the best chromosome which will certainly surivive to the next generation
			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns number of the best chromosomes that will not be removed from population by replacement operation.</returns>
			inline int GACALL GetElitism() const { return _elitism; }

			// Sets number of the best chromosome which will certainly surivive to the next generation
			/// <summary><c>SetElitism</c> method sets number of the best chromosomes that will not be removed from population by replacement operation. 
			///
			/// This method is not thread-safe.</summary>
			/// <param name="elitism">number of the best chromosomes that will not be removed.</param>
			inline void GACALL SetElitism(int elitism) { _elitism = elitism; }

		};// END CLASS DEFINITION GaReplaceElitismParams

		/// <summary><c>GaReplaceRandom</c> randomly chooses chromosomes which are going to be replaced. This operation saves best chromosomes from replacement
		/// if specified in parameters. This replacement operation use <see cref="GaReplaceElitismParams" /> class for parameters.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaReplaceRandom : public GaReplacementOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaReplacementOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(GaPopulation& population,
				const GaReplacementParams& parameters,
				const GaCouplingResultSet& newChromosomes) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaReplaceElitismParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaReplaceElitismParams(); }

			/// <summary>Valid parameters must have number of chromosomes which should be replaced greater then 0. 
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const
			{
				return ( (const GaReplacementParams&) parameters ).GetReplacementSize() > 0;
			}

		};// END CLASS DEFINITION GaReplaceRandom

		/// <summary><c>GaReplaceParents</c> replaces chromosomes which are marked as parents by coupling operation to offspring chromosomes.
		/// This operation saves the best chromosomes from replacement if specified in parameters. This replacement operation use
		/// <see cref="GaReplaceElitismParams" /> class for parameters.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaReplaceParents : public GaReplacementOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaReplacementOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(GaPopulation& population,
				const GaReplacementParams& parameters,
				const GaCouplingResultSet& newChromosomes) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaReplaceElitismParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaReplaceElitismParams(); }

			/// <summary>Valid parameters must have number of chromosomes which should be replaced greater then 0. 
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const
			{
				return ( (const GaReplacementParams&) parameters ).GetReplacementSize() > 0;
			}

		};// END CLASS DEFINITION GaReplaceParents

		/// <summary><c>GaReplaceBest</c> replaces chromosomes with best fitness values in population. If population is sorted this operation can replace any
		/// number of chromosomes, if it is not sorted this operation can only replace chromosomes which are stored in the best chromosome sorted group
		/// of population. This replacement operation use <see cref="GaReplacementParams" /> class for parameters. 
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// Because this genetic operation is stateless all public method are thread-safe.</summary>
		class GaReplaceBest : public GaReplacementOperation
		{

		public:

			/// <summary>More details are given in specification of <see cref="GaReplacementOperation::operator ()" /> method.
			///
			/// This method is thread-safe.</summary>
			GAL_API
			virtual void GACALL operator ()(GaPopulation& population,
				const GaReplacementParams& parameters,
				const GaCouplingResultSet& newChromosomes) const;

			/// <summary>More details are given in specification of <see cref="GaOperation::MakeParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			/// <returns>Method returns new instance of <see cref="GaReplacementParams" /> class.</returns>
			virtual GaParameters* GACALL MakeParameters() const { return new GaReplacementParams(); }

			/// <summary>Valid parameters must have number of chromosomes which should be replaced greater then 0.
			///
			/// More details are given in specification of <see cref="GaOperation::CheckParameters" /> method.
			///
			/// This method is thread-safe.</summary>
			virtual bool GACALL CheckParameters(const GaParameters& parameters) const
			{
				return ( (const GaReplacementParams&) parameters ).GetReplacementSize() > 0;
			}

		};// END CLASS DEFINITION GaReplaceBest

	} // ReplacementOperations
} // Population

#endif // __GA_REPLACEMENT_OPERATIONS_H__
