
/*! \file BinaryChromosome.h
    \brief This file contains declaration of binary chromosome and its parameters classes.
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

#ifndef __GA_BINARY_CHROMOSOME_H__
#define __GA_BINARY_CHROMOSOME_H__

#include "Platform.h"
#include "Chromosome.h"
#include "RepresentationInterfaces.h"

using namespace Chromosome;
using namespace Chromosome::Representation;

namespace Chromosome
{
	namespace Representation
	{

		/// <summary>This class is used by <see cref="GaBinaryChromosome" /> class for extracting of single values from chromosome's code
		/// and other manipulation on chromosome's code.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		class GaBit : public GaCodeValue
		{

		private:

			/// <summary>This attribute stores state of the bit.</summary>
			bool _bitState;

		public:

			/// <summary>Detailed description can be found in specification of <see cref="GaCodeValue::Initialize" />.</summary>
			virtual void GACALL Initialize() { _bitState = GaGlobalRandomBoolGenerator->Generate(); }

			/// <summary>Detailed description can be found in specification of <see cref="GaCodeValue::FromBuffer" />.</summary>
			virtual void GACALL FromBuffer(const GaCodeValuesBuffer& buffer,
				int pos)
			{
				_bitState = ( ( (unsigned int*)buffer.GetBuffer() )[ pos / sizeof(unsigned int) ] >> pos % sizeof(unsigned int) ) & 1;
			}

			/// <summary>This constructor initializes bit with user-defined state.</summary>
			/// <param name="state">state of the bit.</param>
			GaBit(bool state) : _bitState(state) { }

			/// <summary>This constructor initializes bit with random state. It calls <see cref="Initialize" /> method.</summary>
			GaBit() { Initialize(); }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns state of bit.</returns>
			inline bool GACALL GetBitState() const { return _bitState; }

			/// <summary><c>SetBitState</c> method sets state of the bit. 
			///
			/// This method is not thread-safe.</summary>
			/// <param name="state">new state.</param>
			inline void GACALL SetBitState(bool state) { _bitState = state; }

		};// END CLASS DEFINITION GaBit

		/// <summary>This class is used by <see cref="GaBinaryChromosome" /> class as chromosomes' parameters.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		class GaBinaryChromosomeParams : public Chromosome::GaChromosomeParams
		{

		private:

			/// <summary>Probability in interval (0, 1) of bit’s set state.</summary>
			float _probabilityOfSetState;

		public:

			/// <summary>This constructor initializes parameters with user-defined values.</summary>
			/// <param name="probabiltyOfSetState">probability of bit's set state (0,1).</param>
			/// <param name="mutationProbability">mutation probability in interval (0, 1).</param>
			/// <param name="mutationSize">maximal number of affected values in chromosome's code by mutation operation.</param>
			/// <param name="improvingOnlyMutations">state of improving only mutation flag.</param>
			/// <param name="crossoverProbability">crossover probability in interval (0, 1).</param>
			/// <param name="numberOfCrossoverPoints">number of crossover points.</param>
			GaBinaryChromosomeParams(float probabiltyOfSetState,
				float mutationProbability,
				int mutationSize,
				bool improvingOnlyMutations,
				float crossoverProbability,
				int numberOfCrossoverPoints) : GaChromosomeParams(mutationProbability, mutationSize,
				improvingOnlyMutations, crossoverProbability, numberOfCrossoverPoints),
				_probabilityOfSetState(probabiltyOfSetState) { }

			/// <summary>This constructor initializes parameters with default values. Default probability of set state is 0.5,
			/// for othere values see specification of <see cref="GaChromosomeParams" /> class.</summary>
			GaBinaryChromosomeParams() : _probabilityOfSetState(0.5) { }

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns probability in interval (0, 1) of bit's set state.</returns>
			inline float GetProbabilityOfSetState() const { return _probabilityOfSetState; }

			/// <summary><c>SetProbabilityOfSetState</c> method sets probability of bit's set state.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="probability">probability in interval (0, 1).</param>
			inline void SetProbabilityOfSetState(float probability) { _probabilityOfSetState = probability; }

		};// END CLASS DEFINITION GaBinaryChromosomeParams

		/// <summary>This class can be used for solutions of problems which can be encoded as array of bits.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		class GaBinaryChromosome : public GaDynamicOperationChromosome,
			public GaSwapableCode,
			public GaSizableCode,
			public GaMutableCode,
			public GaMultiValueCode
		{

		protected:

			/// <summary>Array of booleans which represents binary code. </summary>
			bool* _bits;

			/// <summary>Number of bits in code.</summary>
			int _codeSize;

			/// <summary>Array of backed-up bits of chromosome's code.</summary>
			bool* _backupBits;

			/// <summary>Number of bits in backed-up code.</summary>
			int _backupSize;

		public:

			/// <summary>More details are given in specification of <see cref="GaSizableCode::Remove" /> method.
			///
			/// This method is not thread-safe.</summary>
			GAL_API
			virtual void GACALL Remove(int start,
				int size);

			/// <summary>More details are given in specification of <see cref="GaSizableCode::Insert" /> method.
			///
			/// This method is not thread-safe.</summary>
			GAL_API
			virtual void GACALL Insert(int start,
				GaCodeValue* data,
				int size);

			/// <summary>More details are given in specification of <see cref="GaSwapableCode::Swap" /> method.
			///
			/// This method is not thread-safe.</summary>
			GAL_API
			virtual void GACALL Swap(int start1,
				int size1,
				int start2,
				int size2);

			/// <summary>More details are given in specification of <see cref="GaMutableCode::Flip" /> method.
			///
			/// This method is not thread-safe.</summary>
			GAL_API
			virtual void GACALL Flip(int start,
				int size);

			/// <summary>More details are given in specification of <see cref="GaMutableCode::Invert" /> method.
			///
			/// This method is not thread-safe.</summary>
			GAL_API
			virtual void GACALL Invert(int start,
				int size);

			/// <summary>More details are given in specification of <see cref="GaMultiValueCode::MakeBuffer" /> method.
			///
			/// This method is not thread-safe.</summary>
			GAL_API
			virtual GaCodeValuesBuffer* GACALL MakeBuffer(int size) const;

			/// <summary>More details are given in specification of <see cref="GaMultiValueCode::FillBuffer" /> method.
			///
			/// This method is not thread-safe.</summary>
			GAL_API
			virtual void GACALL FillBuffer(int pos,
				int size,
				GaCodeValuesBuffer& buffer) const;

			/// <summary>More details are given in specification of <see cref="GaMultiValueCode::FromBuffer" /> method.
			///
			/// This method is not thread-safe.</summary>
			GAL_API
			virtual void GACALL FromBuffer(const GaCodeValuesBuffer& buffer);

			/// <summary>This constructor initializes chromosome with CCB.</summary>
			/// <param name="configBlock">pointer to CCB.</param>
			GAL_API
			GaBinaryChromosome(GaChromosomeOperationsBlock* configBlock);

			/// <summary>This constructor initializes chromosome with CCB and makes random chromosome's code with defined size.</summary>
			/// <param name="size">size of chromosome's code.</param>
			/// <param name="configBlock">pointer to CCB.</param>
			GAL_API
			GaBinaryChromosome(int size,
				GaChromosomeOperationsBlock* configBlock);

			/// <summary>This constructor initializes chromosome with CCB and its code.</summary>
			/// <param name="code">array of bits which is copied to chromosome's code.</param>
			/// <param name="size">size of chromosome's code.</param>
			/// <param name="configBlock">pointer to CCB.</param>
			GAL_API
			GaBinaryChromosome(bool* code,
				int size,
				GaChromosomeOperationsBlock* configBlock);

			/// <summary>This is copy constructor.</summary>
			/// <param name="c">reference to chromosome which is copied.</param>
			/// <param name="setupOnly">if this parameter is <c>true</c>, only pointer to CCB is copied. If this parameter is <c>false</c>,
			/// chromosome's data and CCB is copied.</param>
			GAL_API
			GaBinaryChromosome(const GaBinaryChromosome& c,
				bool setupOnly);

			/// <summary>Destructor frees memory used by chromosome's code and backup code.</summary>
			GAL_API
			virtual ~GaBinaryChromosome();

			/// <summary>More details are given in specification of <see cref="GaChromosome::MakeCopy" /> method.
			///
			/// This method is not thread-safe.</summary>
			GAL_API
			virtual GaChromosomePtr GACALL MakeCopy(bool setupOnly) const { return new GaBinaryChromosome( *this, setupOnly ); }

			/// <summary>This method uses <see cref="MakeCopy" /> method to create chromosome's object, so if additional steps are not needed
			/// for creating chromosome from prototype, classes which inherits this class don't have to override this method.
			/// 
			/// More details are given in specification of <see cref="GaChromosome::MakeNewFromPrototype" /> method.</summary>
			GAL_API
			virtual GaChromosomePtr GACALL MakeNewFromPrototype() const;

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns number of bits in chromosome's code.</returns>
			virtual int GACALL GetCodeSize() const { return _codeSize; }

			/// <summary><c>SetBitState</c> method sets state of bit at specified position.
			///
			/// This method is not thread safe.</summary>
			/// <param name="bit">position of bit.</param>
			/// <param name="state">new state of the bit.</param>
			GAL_API
			void GACALL SetBitState(int bit,
				bool state);

			/// <summary><c>GetBit</c> method returns state of specified bit.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="bit">position of bit which state is queried.</param>
			/// <returns>Method returns state of specified bit.</returns>
			GAL_API
			bool GACALL GetBit(int bit);

			/// <summary>This method is not thread-safe.</summary>
			/// <returns>Method returns pointer to array of booleans which represent binary code of chromosome.</returns>
			inline bool* GACALL GetCode() { return _bits; }

			/// <summary>More details are given in specification of <see cref="GaChromosome::operator =" />.
			///
			/// This method is not thread-safe.</summary>
			GAL_API
			virtual GaChromosome& GACALL operator =(const GaChromosome& rhs);

			/// <summary>More details are given in specification of <see cref="GaChromosome::operator ==" />.
			///
			/// This method is not thread-safe.</summary>
			GAL_API
			virtual bool GACALL operator ==(const GaChromosome& c) const;

			/// <summary><c>GetChar</c> method use <c>sizeof(char)*8</c> number of bits from <c>pos</c> position of chromosome's code to create <c>char</c> value. 
			///
			/// This method is not thread-safe.</summary>
			/// <param name="pos">starting position in chromosome's code array of bits.</param>
			/// <returns>Methods returns created value from array of bits.</returns>
			GAL_API
			char GACALL GetChar(int pos = 0) const;

			/// <summary><c>GetChar</c> method fills array of <c>char</c> values by extracting them from chromosome's code.
			/// If output array is bigger then chromosome's code, remaining values in output are set to 0. 
			///
			/// This method is not thread-safe.</summary>
			/// <param name="output">output array of values.</param>
			/// <param name="pos">starting position in chromosome's code array of bits.</param>
			/// <param name="size">number of values which should be extracted.</param>
			GAL_API
			void GACALL GetChar(char* output,
				int pos = 0,
				int size = 0) const;

			/// <summary><c>GetShort</c> method use <c>sizeof(short)*8</c> number of bits from <c>pos</c> position of chromosome's code
			/// to create <c>short</c> value.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="pos">starting position in chromosome's code array of bits.</param>
			/// <returns>Methods returns created value from array of bits.</returns>
			GAL_API
			short GACALL GetShort(int pos = 0) const;

			/// <summary><c>GetShort</c> method fills array of <c>short</c> values by extracting them from chromosome's code.
			/// If output array is bigger then chromosome's code, remaining values in output are set to 0. 
			///
			/// This method is not thread-safe.</summary>
			/// <param name="output">output array of values.</param>
			/// <param name="pos">starting position in chromosome's code array of bits.</param>
			/// <param name="size">number of values which should be extracted.</param>
			GAL_API
			void GACALL GetShort(short* output,
				int pos = 0,
				int size = 0) const;

			/// <summary><c>GetInt</c> method use <c>sizeof(int)*8</c> number of bits from <c>pos</c> position of chromosome's code
			/// to create <c>int</c> value.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="pos">starting position in chromosome's code array of bits.</param>
			/// <returns>Methods returns created value from array of bits.</returns>
			GAL_API
			int GACALL GetInt(int pos = 0) const;

			/// <summary><c>GetInt</c> method fills array of int values by extracting them from chromosome's code.
			/// If output array is bigger then chromosome's code, remaining values in output are set to 0. 
			///
			/// This method is not thread-safe.</summary>
			/// <param name="output">output array of values.</param>
			/// <param name="pos">starting position in chromosome's code array of bits.</param>
			/// <param name="size">number of values which should be extracted.</param>
			GAL_API
			void GACALL GetInt(int* output,
				int pos = 0,
				int size = 0) const;

			/// <summary><c>GetFloat</c> method use <c>sizeof(float)*8</c> number of bits from <c>pos</c> position of chromosome's code
			/// to create <c>float</c> value.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="pos">starting position in chromosome's code array of bits.</param>
			/// <returns>Methods returns created value from array of bits.</returns>
			GAL_API
			float GACALL GetFloat(int pos = 0) const;

			/// <summary><c>GetFloat</c> method fills array of float values by extracting them from chromosome's code.
			/// If output array is bigger then chromosome's code, remaining values in output are set to 0. 
			///
			/// This method is not thread-safe.</summary>
			/// <param name="output">output array of values.</param>
			/// <param name="pos">starting position in chromosome's code array of bits.</param>
			/// <param name="size">number of values which should be extracted.</param>
			GAL_API
			void GACALL GetFloat(float* output,
				int pos = 0,
				int size = 0) const;

			/// <summary><c>GetDouble</c> method use <c>sizeof(double)*8</c> number of bits from <c>pos</c> position of chromosome's code
			/// to create <c>double</c> value.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="pos">starting position in chromosome's code array of bits.</param>
			/// <returns>Methods returns created value from array of bits.</returns>
			GAL_API
			double GACALL GetDouble(int pos = 0) const;

			/// <summary><c>GetDouble</c> method fills array of double values by extracting them from chromosome's code.
			/// If output array is bigger then chromosome's code, remaining values in output are set to 0. 
			///
			/// This method is not thread-safe.</summary>
			/// <param name="output">output array of values.</param>
			/// <param name="pos">starting position in chromosome's code array of bits.</param>
			/// <param name="size">number of values which should be extracted.</param>
			GAL_API
			void GACALL GetDouble(double* output,
				int pos = 0,
				int size = 0) const;

			/// <summary><c>FromChar</c> method creates array of booleans which is equal to binary representation of array of input values of <c>char</c> type.
			/// Produced array can be used as chromosome's code of binary chromosome. Caller is responsible for memory allocated by this method for output array. 
			///
			/// This method is not thread-safe.</summary>
			/// <param name="input">pointer to array of input values.</param>
			/// <param name="size">number of values in input array.</param>
			/// <returns>Method returns pointer to output array. Size of output array is <c>sizeof(char)*8</c>.</returns>
			GAL_API
			static bool* GACALL FromChar(char* input,
				int size);

			/// <summary><c>FromShort</c> method creates array of booleans which is equal to binary representation of array of input values of <c>short</c> type.
			/// Produced array can be used as chromosome's code of binary chromosome. Caller is responsible for memory allocated by this method for output array.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="input">pointer to array of input values.</param>
			/// <param name="size">number of values in input array.</param>
			/// <returns>Method returns pointer to output array. Size of output array is <c>sizeof(short)*8</c>.</returns>
			GAL_API
			static bool* GACALL FromShort(short* input,
				int size);

			/// <summary><c>FromInt</c> method creates array of booleans which is equal to binary representation of array of input values of <c>int</c> type.
			/// Produced array can be used as chromosome's code of binary chromosome. Caller is responsible for memory allocated by this method for output array.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="input">pointer to array of input values.</param>
			/// <param name="size">number of values in input array.</param>
			/// <returns>Method returns pointer to output array. Size of output array is <c>sizeof(int)*8</c>.</returns>
			GAL_API
			static bool* GACALL FromInt(int* input,
				int size);

			/// <summary><c>FromFloat</c> method creates array of booleans which is equal to binary representation of array of input values of <c>float</c> type.
			/// Produced array can be used as chromosome's code of binary chromosome. Caller is responsible for memory allocated by this method for output array.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="input">pointer to array of input values.</param>
			/// <param name="size">number of values in input array.</param>
			/// <returns>Method returns pointer to output array. Size of output array is <c>sizeof(float)*8</c>.</returns>
			GAL_API
			static bool* GACALL FromFloat(float* input,
				int size);

			/// <summary><c>FromDouble</c> method creates array of booleans which is equal to binary representation of array of input values of <c>double</c> type.
			/// Produced array can be used as chromosome's code of binary chromosome. Caller is responsible for memory allocated by this method for output array.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="input">pointer to array of input values.</param>
			/// <param name="size">number of values in input array.</param>
			/// <returns>Method returns pointer to output array. Size of output array is <c>sizeof(double)*8</c>.</returns>
			GAL_API
			static bool* GACALL FromDouble(double* input,
				int size);

		protected:

			/// <summary><c>PreapareForMutation</c> method creates backup of current code and saves its size.
			///
			/// This method is not thread-safe.</summary>
			GAL_API
			virtual void GACALL PreapareForMutation();

			/// <summary>This method only deletes backup copy.
			///
			/// This method is not thread-safe.</summary>
			GAL_API
			virtual void GACALL AcceptMutation();

			/// <summary><c>RejectMutation</c> method deletes current chromosome's code and restores it from backup copy.
			///
			/// This method is not thread-safe.</summary>
			GAL_API
			virtual void GACALL RejectMutation();

		};// END CLASS DEFINITION GaBinaryChromosome

	} // Representation
} // Chromosome

#endif // __GA_BINARY_CHROMOSOME_H__
