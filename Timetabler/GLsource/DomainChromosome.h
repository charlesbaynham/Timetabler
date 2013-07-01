
/*! \file DomainChromosome.h
    \brief This file contains declaration and implementation of abstract class for chromosomes and CCB that have value sets.
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

#ifndef __GA_DOMAIN_CHROMOSOME_H__
#define __GA_DOMAIN_CHROMOSOME_H__

#include "Chromosome.h"
#include "Platform.h"
#include "ValueSets.h"

using namespace Chromosome;
using namespace Chromosome::Representation;

namespace Chromosome
{
	namespace Representation
	{

		/// <summary>This class is CCB for chromosomes which have codes with values which must be in defined domains. This CCB stores pointer to value sets.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		/// <param name="T">type of data used by value set.</param>
		template <typename T>
		class GaChromosomeDomainBlock : public GaChromosomeOperationsBlock
		{

		protected:

			/// <summary>Pointer to values sets.</summary>
			std::vector<GaValueSet<T>*> _domains;

		public:

			/// <summary>This constructor initializes CCB with pointer to chromosomes' parameters, extern genetic operations and value set.</summary>
			/// <param name="domain">pointer to values set.</param>
			/// <param name="crossover">pointer to extern crossover operation.</param>
			/// <param name="mutation">pointer to extern mutation operation.</param>
			/// <param name="fitnessOperation">pointer to extern fitness operation.</param>
			/// <param name="fitnessComparator">pointer to extern fitness comparator.</param>
			/// <param name="parameters">pointer to chromosomes' parameters.</param>
			GaChromosomeDomainBlock(GaValueSet<T>* domain,
				GaCrossoverOperation* crossover,
				GaMutationOperation* mutation,
				GaFitnessOperation* fitnessOperation,
				GaFitnessComparator* fitnessComparator,
				GaChromosomeParams* parameters) : GaChromosomeOperationsBlock(crossover, mutation, fitnessOperation, fitnessComparator, parameters),
				_domains(1) { _domains[ 0 ] = domain; }

			/// <summary>This constructor initializes CCB with pointer to chromosomes' parameters, extern genetic operations and value set.</summary>
			/// <param name="domains">pointer to values sets.</param>
			/// <param name="domainCount">number of value sets.</param>
			/// <param name="crossover">pointer to extern crossover operation.</param>
			/// <param name="mutation">pointer to extern mutation operation.</param>
			/// <param name="fitnessOperation">pointer to extern fitness operation.</param>
			/// <param name="fitnessComparator">pointer to extern fitness comparator.</param>
			/// <param name="parameters">pointer to chromosomes' parameters.</param>
			GaChromosomeDomainBlock(GaValueSet<T>** domains,
				int domainCount,
				GaCrossoverOperation* crossover,
				GaMutationOperation* mutation,
				GaFitnessOperation* fitnessOperation,
				GaFitnessComparator* fitnessComparator,
				GaChromosomeParams* parameters) : GaChromosomeOperationsBlock(crossover, mutation, fitnessOperation, fitnessComparator, parameters),
				_domains(domainCount)
			{
				// store value sets
				for( int i = domainCount - 1; i >= 0; i-- )
					_domains[ i ] = domains[ i ];
			}


			/// <summary>This is copy constructor. The constructor doesn't create copy of parameters', operations' and value set's objects,
			///it only copies pointer to chromosomes' parameters, operations and value set.</summary>
			/// <param name="rhs">reference to CCB which is copied.</param>
			GaChromosomeDomainBlock(const GaChromosomeDomainBlock& rhs) : GaChromosomeOperationsBlock(rhs),
				_domains(rhs._domains) { }

			/// <summary>This constructor initializes empty CCB.</summary>
			GaChromosomeDomainBlock() { }

			/// <summary><c>GetValueSet</c> method returns value set for specified value position.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="pos">position for which the value set is queried.</param>
			/// <returns>Method returns pointer to value set.</returns>
			inline const GaValueSet<T>* GACALL GetValueSet(int pos) const { return _domains.size() ? _domains[ pos % _domains.size() ] : NULL; }

			/// <summary><c>SetValueSet</c> method stores pointer to value set for specified position in CCB.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="pos">position for which the value set is stored.</param>
			/// <param name="domain">pointer to new value set.</param>
			inline void GACALL SetValueSet(GaValueSet<T>* domain,
				int pos)
			{
				if( _domains.size() > 0 )
					_domains[ pos % _domains.size() ] = domain;
			}

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns number of value sets stored in CCB.</returns>
			inline int GACALL GetValueSetCount() const { return (int)_domains.size(); }

		};// END CLASS DEFINITION GaChromosomeDomainBlock

		/// <summary><c>GaDomainChromosome</c> class should be base for chromosomes which have codes with values which must bi in defined domain. 
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		/// <param name="T">type of data used by value set.</param>
		template <typename T>
		class GaDomainChromosome : public GaDynamicOperationChromosome
		{

		public:

			/// <summary>This constructor initializes chromosome with CCB.</summary>
			/// <param name="configBlock">pointer to CCB.</param>
			GaDomainChromosome(GaChromosomeDomainBlock<T>* configBlock) : GaDynamicOperationChromosome(configBlock) { }

			/// <summary>This is copy constructor.</summary>
			/// <param name="c">reference to chromosome which is copied.</param>
			/// <param name="setupOnly">if this parameter is <c>true</c>, only pointer to CCB is copied.
			/// If this parameter is <c>false</c>, chromosome's data and CCB is copied.</param>
			GaDomainChromosome(const GaDomainChromosome<T>& c,
				bool setupOnly) : GaDynamicOperationChromosome(c, setupOnly) { }

			/// <summary><c>GetClosestValue</c> method returns nearest value which can be found in value set to specified value.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="value">value which nearest neighbor should be found.</param>
			/// <param name="pos">position of the value in the chromosome's code.</param>
			/// <returns>Method returns reference to nearest value in value set to specified value.</returns>
			inline const T& GetClosestValue(const T& value,
				int pos) const
			{
				const GaValueSet<T>* d = ( (GaChromosomeDomainBlock<T>*)_configBlock )->GetValueSet( pos );
				return d ? d->ClosestValue( value ) : value;
			}

		};// END CLASS DEFINITION GaDomainChromosome

	} // Representation
} // Chromosome

#endif // __GA_DOMAIN_CHROMOSOME_H__
