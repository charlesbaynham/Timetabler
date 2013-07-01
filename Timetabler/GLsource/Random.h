
/*! \file Random.h
    \brief This file contains declaration of interfaces, classes and datatypes of random number generators.
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

#ifndef __GA_RANDOM_H__
#define __GA_RANDOM_H__

#include <time.h>

#include "Platform.h"
#include "Threading.h"

namespace Common
{
	/// <summary>This union is used for conversation from 32-bits long integer random number
	/// to single precision floating point number in interval (0, 1).</summary>
	union GaUnsignedIntToFloat
	{

		/// <summary>This field is used to store 32-bit long integer number.</summary>
		unsigned int bits;

		/// <summary>field is used to read 32-bit long integer number as mantissa of single precision floating point number.</summary>
		float number;

	};

	/// <summary>This union is used for conversation from 64-bits long integer random number
	/// to double precision floating point number in interval (0, 1).</summary>
	union GaUnsignedIntToDouble
	{

		/// <summary>This field is used to store 64-bit long integer number.</summary>
		unsigned int bits[ 2 ];

		/// <summary>This field is used to read 64-bit long integer number as mantissa of single precision floating point number.</summary>
		double number;

	};

	/// <summary><c>GaRandomGenerator</c> class implements algorithm for generating 64-bits wide random unsigned integers and floating-point numbers.
	/// It takes care of architecture's endianness, but underlying CPU architecture must support floating-point by IEEE 754 standard.
	/// <c>GaRandomGenerator</c> class does not implement <see cref="GaRandom" /> interface. Primary purpose of this class is to provide service
	/// for generating random numbers for classes which implement <see cref="GaRandom" /> interface.
	///
	/// This class has built-in synchronizator so it is allowed to use <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros with instances of this class.
	/// All public methods are thread-safe.</summary>
	class GaRandomGenerator
	{

		DEFINE_SYNC_CLASS

	private:

		/// <summary>Defines representations of random generator's state.</summary>
		struct GaState
		{

			/// <summary>The first part of random generator state.</summary>
			unsigned int _w;

			/// <summary>The second part of random generator state.</summary>
			unsigned int _z;

		};

		/// <summary>Current state of random generator.</summary>
		GaState _currentState;

		/// <summary>This attribute indicates endianness of architecture. If it is set to <c>true</c>, the architecture is little-endian,
		/// if the architecture is big-endian this attribute is set to <c>false</c>.</summary>
		bool _littleEndian;

	public:

		/// <summary>This constructor initialize random generator with current time as seed.</summary>
		GaRandomGenerator()
		{
			unsigned long long x = (unsigned long long)time( NULL );
			Initalization( (unsigned int)( x >> 16 ), (unsigned int)x );
		}

		/// <summary>This constructor initialize random generator with user-defined seed.</summary>
		/// <param name="seed">user-defined seed.</param>
		GaRandomGenerator(unsigned int seed) { Initalization( seed, 0 ); }

		/// <summary><c>Generate</c> method generates and returns 32-bit wide unsigned integer.
		///
		/// This method is thread-safe.</summary>
		/// <returns>Method returns generated number.</returns>
		GAL_API
		unsigned int GACALL Generate();
		
		/// <summary><c>GeneratrFloat</c> method generates single precision floating point number i interval (0, 1).
		///
		/// This method is thread-safe.</summary>
		/// <returns>Method returns generated number.</returns>
		GAL_API
		float GACALL GenerateFloat();

		/// <summary><c>GeneratrFloat</c> method generates double precision floating point number i interval (0, 1).
		///
		/// This method is thread-safe.</summary>
		/// <returns>Method returns generated number.</returns>
		GAL_API
		double GACALL GenerateDouble();

		/// <summary>Initializes random generator with specified seed. <c>Initialization</c> method is called by constructor.</summary>
		/// <param name="seed1">seed used to initialize the first part of generator's state.</param>
		/// <param name="seed2">seed used to initialize the second part of generator's state.</param>
		GAL_API
			void GACALL Initalization(unsigned int seed1,
			unsigned int seed2);

	};// END CLASS DEFINITION GaRandomGenerator

	/// <summary>Interface for random value generators.</summary>
	/// <param name="TYPE">type of generated values.</param>
	template <typename TYPE>
	class GaRandom
	{

	public:

		/// <summary>This method generates random values of <c>TYPE</c> with no specific range.</summary>
		/// <returns>Returns generate random value.</returns>
		virtual TYPE GACALL Generate()=0;

		/// <summary>This method generates random value of <c>TYPE</c> with specified maximum.</summary>
		/// <param name="max">maximal value which can be generated.</param>
		/// <returns>Returns generate random value.</returns>
		virtual TYPE GACALL Generate(const TYPE& max)=0;

		/// <summary>This method generates random value of <c>TYPE</c> within specified range of values.</summary>
		/// <param name="min">minimal value which can be generated.</param>
		/// <param name="max">maximal value which can be generated.</param>
		/// <returns>Returns generate random value.</returns>
		virtual TYPE GACALL Generate(const TYPE& min,
			const TYPE& max)=0;

	};// END CLASS DEFINITION GaRandom

	/// <summary><c>GaRandomInteger</c> class generates random 32-bits wide integer numbers. The class implements <see cref="GaRandom" /> interface.
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class,
	/// but all public methods are thread-safe.</summary>
	class GaRandomInteger : public GaRandom<int>
	{

	private:

		/// <summary>Instance of algorithm for generating random numbers.</summary>
		GaRandomGenerator _generator;

	public:

		/// <summary>This constructor initializes random generator with current time as seed.</summary>
		GaRandomInteger() { }

		/// <summary>This constructor initialize random generator with user-defined seed.</summary>
		/// <param name="seed">user-defined seed.</param>
		GaRandomInteger(unsigned long seed) : _generator(seed) { }

		/// <summary>This method generates random values in interval(0, 2147483647).
		///
		/// This method is thread-safe.</summary>
		/// <returns>Returns generate random value.</returns>
		virtual int GACALL Generate()
		{
			unsigned int w1 = _generator.Generate();
			return (int)w1 < 0 ? -(int)w1 : (int)w1;
		}

		/// <summary>This method generates random values in interval(0, <c>max</c>).
		///
		/// This method is thread-safe.</summary>
		/// <param name="max">maximal value which can be generated.</param>
		/// <returns>Returns generate random value.</returns>
		virtual int GACALL Generate(const int& max) { return (int)( _generator.GenerateDouble() * ( max + 1 ) ) % ( max + 1 ); }

		/// <summary>This method generates random values in interval(<c>min</c>, <c>max</c>).
		///
		/// This method is thread-safe.</summary>
		/// <param name="min">minimal value which can be generated.</param>
		/// <param name="max">maximal value which can be generated.</param>
		/// <returns>Returns generate random value.</returns>
		virtual int GACALL Generate(const int& min,
			const int& max) { return min + Generate( max - min ); }

	};// END CLASS DEFINITION GaRandomInteger

	/// <summary><c>GaRandomFloat</c> class generates random single  precision floating-point numbers. Targeted architecture must support IEEE 754 standard.
	/// The class implements <see cref="GaRandom" /> interface. This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c>
	/// macros cannot be used with instances of this class, but all public methods are thread-safe.</summary>
	class GaRandomFloat : public GaRandom<float>
	{

	private:

		/// <summary>Instance of algorithm for generating random numbers.</summary>
		GaRandomGenerator _generator;

	public:

		/// <summary>This constructor initializes random generator with current time as seed.</summary>
		GaRandomFloat() { }

		/// <summary>This constructor initialize random generator with user-defined seed.</summary>
		/// <param name="seed">user-defined seed.</param>
		GaRandomFloat(unsigned long seed) : _generator(seed) { }

		/// <summary>This method generates random values in interval(0, 1).
		///
		/// This method is thread-safe.</summary>
		/// <returns>Returns generate random value.</returns>
		virtual float GACALL Generate() { return _generator.GenerateFloat(); }

		/// <summary>This method generates random values in interval(0, <c>max</c>).
		///
		/// This method is thread-safe.</summary>
		/// <param name="max">maximal value which can be generated.</param>
		/// <returns>Returns generate random value.</returns>
		virtual float GACALL Generate(const float& max) { return max * _generator.GenerateFloat(); }

		/// <summary>This method generates random values in interval(<c>min</c>, <c>max</c>).
		///
		/// This method is thread-safe.</summary>
		/// <param name="min">minimal value which can be generated.</param>
		/// <param name="max">maximal value which can be generated.</param>
		/// <returns>Returns generate random value.</returns>
		virtual float GACALL Generate(const float& min,
						 const float& max) { return min + Generate( max - min ); }

	};// END CLASS DEFINITION GaRandomFloat

	/// <summary><c>GaRandomDouble</c> class generates random double precision floating-point numbers. Class takes care about endianness of the architecture.
	/// Targeted architecture must support IEEE 754 standard. The class implements <see cref="GaRandom" /> interface. This class has no built-in synchronizator,
	/// so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class, but all public methods are thread-safe.</summary>
	class GaRandomDouble : public GaRandom<double>
	{

	private:

		/// <summary>Instance of algorithm for generating random numbers.</summary>
		GaRandomGenerator _generator;

	public:

		/// <summary>This constructor initializes random generator with current time as seed.</summary>
		GaRandomDouble() { }

		/// <summary>This constructor initialize random generator with user-defined seed.</summary>
		/// <param name="seed">user-defined seed.</param>
		GaRandomDouble(unsigned long seed) : _generator(seed) { }

		/// <summary>This method generates random values in interval(0, 1).
		///
		/// This method is thread-safe.</summary>
		/// <returns>Returns generate random value.</returns>
		virtual double GACALL Generate() { return _generator.GenerateDouble(); }

		/// <summary>This method generates random values in interval(0, <c>max</c>).
		///
		/// This method is thread-safe.</summary>
		/// <param name="max">maximal value which can be generated.</param>
		/// <returns>Returns generate random value.</returns>
		virtual double GACALL Generate(const double& max) { return max * _generator.GenerateDouble(); }

		/// <summary>This method generates random values in interval(<c>min</c>, <c>max</c>).
		///
		/// This method is thread-safe.</summary>
		/// <param name="min">minimal value which can be generated.</param>
		/// <param name="max">maximal value which can be generated.</param>
		/// <returns>Returns generate random value.</returns>
		virtual double GACALL Generate(const double& min,
			const double& max) { return min + Generate( max - min ); }

	};// END CLASS DEFINITION GaRandomDouble

	/// <summary><c>GaRandomBool</c> class generates random boolean values. It supports generating boolean with defined probabilities of 
	/// <c>true</c> and <c>false</c> states. The class implements <see cref="GaRandom" /> interface. This class has no built-in synchronizator,
	/// so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class, but all public methods are thread-safe.</summary>
	class GaRandomBool : public GaRandom<bool>
	{

	private:

		/// <summary>Instance of algorithm for generating random numbers.</summary>
		GaRandomGenerator _generator;

	public:

		/// <summary>This constructor initializes random generator with current time as seed.</summary>
		GaRandomBool() { }

		/// <summary>This constructor initialize random generator with user-defined seed.</summary>
		/// <param name="seed">user-defined seed.</param>
		GaRandomBool(unsigned long seed) : _generator(seed) { }

		/// <summary>This method generates random Boolean values.
		///
		/// This method is thread-safe.</summary>
		/// <returns>Returns generate random value.</returns>
		virtual bool GACALL Generate() { return ( _generator.Generate() & 1 ) == 1; }

		/// <summary>This method generates random Boolean values.
		///
		/// This method is thread-safe.</summary>
		/// <param name="max">this parameter is ignored.</param>
		/// <returns>Returns generate random value.</returns>
		virtual bool GACALL Generate(const bool& max) { return Generate(); }

		/// <summary>This method generates random Boolean values.
		///
		/// This method is thread-safe.</summary>
		/// <param name="min">this parameter is ignored.</param>
		/// <param name="max">this parameter is ignored.</param>
		/// <returns>Returns generate random value.</returns>
		virtual bool GACALL Generate(const bool& min,
			const bool& max) { return Generate(); }

		// Generates boolean with p probability of TRUE and 1-p probability of FALSE
		/// <summary>This method generates Boolean value with <c>p</c> probability of <c>true</c> value.
		///
		/// This method is thread safe.</summary>
		/// <param name="p">probability of <c>true</c> value (0, 1).</param>
		/// <returns>Returns generate random value.</returns>
		inline bool GACALL Generate(double p) { return _generator.GenerateFloat() < p; }

		// Generates boolean with p probability of TRUE and 100-p probability of FALSE. p is expressed in 
		/// <summary>This method generates Boolean value with <c>p</c> probability of <c>true</c> and <c>100-p</c> of <c>false</c> value.
		///
		/// This method is thread safe.</summary>
		/// <param name="p">probability in percents of <c>true</c> value (0 - 100).</param>
		/// <returns>Returns generate random value.</returns>
		inline bool GACALL Generate(int p) { return (int)( _generator.GenerateDouble() * 100 ) < p; }

	};// END CLASS DEFINITION GaRandomBool

} // Common

#endif // __GA_RANDOM_H__
