
/*! \file RepresentationInterfaces.h
    \brief This file declares interfaces for manipulation of chromosomes' codes.
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

#ifndef __GA_REPRESENTATION_INTERFACES_H__
#define __GA_REPRESENTATION_INTERFACES_H__

#include <vector>
#include "Chromosome.h"
#include "Platform.h"
#include "ValueSets.h"

using namespace std;
using namespace Chromosome;
using namespace Chromosome::Representation;

namespace Chromosome
{
	/// <summary>Contains implementation of some basic chromosome representations.</summary>
	namespace Representation
	{

		/// <summary>This class represents buffer for storing values of chromosome's code. It is also used for building chromosomes' codes.
		/// <c>GaCodeValuesBuffer</c> class also keeps track of current position for filling the buffer, but interpretation of it is left to users.
		///
		/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
		/// No public or private methods are thread-safe.</summary>
		class GaCodeValuesBuffer
		{

		private:

			/// <summary>Pointer to memory which is allocated for the buffer.</summary>
			void* _buffer;

			/// <summary>Size in bytes of allocated memory.</summary>
			int _size;

			/// <summary>This attribute keeps track of current position for filling the buffer. </summary>
			int _currentPosition;

		public:

			/// <summary>This constructor allocates memory for buffer. </summary>
			/// <param name="size">size of the buffer</param>
			GaCodeValuesBuffer(int size) : _buffer(new char[ size ]),
				_size(size),
				_currentPosition(0) { }
				

			/// <summary>The destructor frees memory used by the buffer.</summary>
			~GaCodeValuesBuffer()
			{
				if( _buffer )
					delete[] _buffer;
			}

			/// <summary>Move method moves current position for filling the buffer.
			///
			/// This method is not thread-safe.</summary>
			/// <param name="delta">value by which the current position is moved. This parameter can be positive or negative.</param>
			inline void GACALL Move(int delta) { _currentPosition += delta; }

			/// <summary>This method is not thread safe.</summary>
			/// <returns>Method returns pointer to memory used by the buffer.</returns>
			inline void* GACALL GetBuffer() const { return _buffer; }

			/// <summary>This method is not thread safe.</summary>
			/// <returns>Method returns size of buffer's memory in bytes.</returns>
			inline int GACALL GetSize() const { return _size; }

			/// <summary>This method is not thread safe.</summary>
			/// <returns>Method returns current position for filling the buffer.</returns>
			inline int GACALL GetPosition() const { return _currentPosition; }

		};// END CLASS DEFINITION GaCodeValuesBuffer

		/// <summary><c>GaCodeValue</c> is interface for values which are parts of chromosome's code.
		/// It is used by interfaces which manipulate chromosome's code.</summary>
		class GaCodeValue
		{

		public:

			/// <summary>This method initializes value randomly.</summary>
			virtual void GACALL Initialize()=0;

			/// <summary>This method extract value at given position form buffer of chromosome's code.</summary>
			/// <param name="buffer">reference to buffer from which value is extracted.</param>
			/// <param name="pos">position of extracting value in buffer.</param>
			virtual void GACALL FromBuffer(const GaCodeValuesBuffer& buffer,
				int pos)=0;

		};// END INTERFACE DEFINITION GaCodeValue

		/// <summary><c>GaMutableCode</c> interface should be implemented by chromosomes' classes which
		/// code supports random changes and inversion of its parts.</summary>
		class GaMutableCode
		{

		public:

			/// <summary>This method changes values of chromosome's code randomly from <c>start</c> position to <c>start + size</c> position.</summary>
			/// <param name="start">position of first value which is going to be changed.</param>
			/// <param name="size">number of values which are going to be changed.</param>
			virtual void GACALL Flip(int start,
				int size)=0;

			/// <summary>This method inverts values of chromosome's code from <c>start</c> position to <c>start + size</c> position.</summary>
			/// <param name="start">position of first value which is going to be inverted.</param>
			/// <param name="size">number of values which are going to be inverted.</param>
			virtual void GACALL Invert(int start,
				int size)=0;

		};// END INTERFACE DEFINITION GaMutableCode

		/// <summary><c>GaSwapableCode</c> interface should be implemented by chromosomes' classes which
		/// code supports swapping of positions of its positions.</summary>
		class GaSwapableCode
		{

		public:

			/// <summary>This method swaps positions of parts of chromosome's code.</summary>
			/// <param name="start1">position of the first part of code.</param>
			/// <param name="size1">number of values of first part which is going to be swapped.</param>
			/// <param name="start2">position of the second part of code.</param>
			/// <param name="size2">number of values of second part which is going to be swapped.</param>
			virtual void GACALL Swap(int start1,
				int size1,
				int start2,
				int size2)=0;

		};// END INTERFACE DEFINITION GaSwapableCode

		/// <summary><c>GaSizableCode</c> interface should be implemented by chromosomes' classes which
		/// code supports adding of new values into it or removing from it.</summary>
		class GaSizableCode
		{

		public:

			/// <summary>This method removes values <c>start</c> position to <c>start + size</c> position.</summary>
			/// <param name="start">position of first value which is going to be removed.</param>
			/// <param name="size">number of values which is going to be removed.</param>
			virtual void GACALL Remove(int start,
				int size)=0;

			/// <summary>This method inserts new values into chromosome's code.</summary>
			/// <param name="start">position at which new values are going to be inserted into code.</param>
			/// <param name="data">array of new values which is going to be inserted.</param>
			/// <param name="size">number of values which are going to be inserted.</param>
			virtual void GACALL Insert(int start,
				GaCodeValue* data,
				int size)=0;

		};// END INTERFACE DEFINITION GaSizableCode

		/// <summary><c>GaMultiValueCode</c> interface should be implemented by chromosomes' classes which code contains multiple values.</summary>
		class GaMultiValueCode
		{

		public:

			/// <summary>This method makes new buffer for storing values of chromosomes' codes but doesn't fill it.
			/// User is responsible for memory allocated for buffer object.</summary>
			/// <param name="size">size of buffer (number of values it can store).</param>
			/// <returns>Method returns pointer to newly created buffer.</returns>
			virtual GaCodeValuesBuffer* GACALL MakeBuffer(int size) const=0;

			/// <summary>This method copies values of chromosome's code from <c>pos</c> to <c>pos + size</c> into buffer at current buffer's position.</summary>
			/// <param name="pos">position of the first value which is going to be copied.</param>
			/// <param name="size">number of values which are going to be copied.</param>
			/// <param name="buffer">reference to buffer to which values are copied.</param>
			virtual void GACALL FillBuffer(int pos,
				int size,
				GaCodeValuesBuffer& buffer) const=0;

			/// <summary>This method makes chromosome's code and stores it in chromosome form buffer of values.</summary>
			/// <param name="buffer">reference to buffer which contains values for making chromosome's code.</param>
			virtual void GACALL FromBuffer(const GaCodeValuesBuffer& buffer)=0;

		};// END INTERFACE DEFINITION GaMultiValueCode

		/// <summary><c>GaArithmeticalCode</c> interface should be implemented by chromosomes' classes which
		/// code supports arithmetical operations over it or its values.</summary>
		class GaArithmeticalCode
		{

		public:

			/// <summary>This method operator new chromosome which code has values which are calculated form corresponding values of codes of
			/// <c>this</c> and <c>c</c> chromosomes by summing of those values.</summary>
			/// <param name="rhs">the second code for summing.</param>
			/// <returns>Method returns smart pointer to newly created chromosome.</returns>
			virtual GaChromosomePtr GACALL operator +(const GaArithmeticalCode& rhs) const=0;

			/// <summary>This method operator new chromosome which code has values which are calculated form corresponding values of codes of
			/// <c>this</c> and <c>c</c> chromosomes by subtracting of those values.</summary>
			/// <param name="rhs">the second code for subtracting.</param>
			/// <returns>Method returns smart pointer to newly created chromosome.</returns>
			virtual GaChromosomePtr GACALL operator -(const GaArithmeticalCode& rhs) const=0;

			/// <summary>This method operator new chromosome which code has values which are calculated form corresponding values of codes of
			/// <c>this</c> and <c>c</c> chromosomes by multiplying of those values.</summary>
			/// <param name="rhs">the second code for multiplying.</param>
			/// <returns>Method returns smart pointer to newly created chromosome.</returns>
			virtual GaChromosomePtr GACALL operator *(const GaArithmeticalCode& rhs) const=0;

			/// <summary>This method operator new chromosome which code has values which are calculated form corresponding values of codes of
			/// <c>this</c> and <c>c</c> chromosomes by dividing of those values.</summary>
			/// <param name="rhs">the second code for dividing.</param>
			/// <returns>Method returns smart pointer to newly created chromosome.</returns>
			virtual GaChromosomePtr GACALL operator /(const GaArithmeticalCode& rhs) const=0;

			/// <summary>This method produces new chromosome which code has values which are calculated form corresponding values of codes of
			/// <c>this</c> and <c>c</c> chromosomes by midpoint algorithm.</summary>
			/// <param name="c">the second code for calculating midpoint.</param>
			/// <returns>Method returns smart pointer to newly created chromosome.</returns>
			virtual GaChromosomePtr GACALL Midpoint(const GaArithmeticalCode& c) const=0;

		};// END INTERFACE DEFINITION GaArithmeticalCode

	} // Representation
} // Chromosome

#endif // __GA_REPRESENTATION_INTERFACES_H__
