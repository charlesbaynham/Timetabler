
/*! \file Operation.h
    \brief This file declares interface for genetic operations and parameters. It also delcares and implements datatype which store operation-parameters pair. 
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

#ifndef __GA_OPERATION_H__
#define __GA_OPERATION_H__

#include "Platform.h"

namespace Common
{
	/// <summary>Interface for parameters of genetic operations.</summary>
	class GaParameters
	{

	public:

		/// <summary>It is used to allocate memory, create copy of parameters' object on heap.</summary>
		/// <remarks>Caller is responsible for allocated memory, freeing of new object should be done by <c>delete</c> keyword.
		/// User-defined parameters classes inherited by built-in classes should override this method to perform custom copying.</remarks>
		/// <returns>Returns pointer to newly created object.</returns>
		virtual GaParameters* GACALL Clone() const=0;

	};// END CLASS DEFINITION GaParameters

	/// <summary>Interface for genetic operations.</summary>
	class GaOperation
	{

	public:
		
		/// <summary>Allocates memory and makes corresponding operation's parameters object on heap.</summary>
		/// <remarks>Caller is responsible for allocated memory, freeing of new object should be done by delete keyword. 
		/// User-defined operation classes inherited by built-in classes should override this method to perform creation
		/// of custom operation's parameters objects.</summary>
		/// <returns>Returns pointer to newly created object.</returns>
		virtual GaParameters* GACALL MakeParameters() const=0;

		/// <summary>Method checks correctness of parameters.</summary>
		/// <param name="parameters">constant reference to parameters' object on which correctness checking is performed.</param>
		/// <returns>Returns <c>true</c> if parameters are correct, otherwise it returns <c>false</c>.</returns>
		virtual bool GACALL CheckParameters(const GaParameters& parameters) const=0;

	};// END CLASS DEFINITION GaOperation

	/// <summary>This template class is used for storing genetic operations and their parameters.
	/// This class has no built-in synchronizator, so <c>LOCK_OBJECT</c> and <c>LOCK_THIS_OBJECT</c> macros cannot be used with instances of this class.
	///
	/// No public or private methods are thread-safe.</summary>
	/// <param name="OTYPE">Type of operation.</param>
	/// <param name="PTYPE">Type of parameters.</param>
	template <typename OTYPE, typename PTYPE>
	class GaOperationParametersPair
	{

	private:

		/// <summary>Pointer to operation's object.</summary>
		OTYPE* _operation;

		/// <summary>Pointer to parameters' object</summary>
		PTYPE* _parameters;

	public:

		/// <summary>This constructor initializes operation's and parameters' pointers with user-defined values.
		/// Parameter's object is cloned and pointer to the new object is stored in the pair.</summary>
		/// <param name="operation">pointer to operation's object</param>
		/// <param name="parameters">pointer to the object of operation's parameters</param>
		GaOperationParametersPair(OTYPE* operation,
			PTYPE& parameters) : _operation(operation) { _parameters = &parameters ? (PTYPE*)parameters.Clone() : 0; }

		/// <summary>This is copy constructor. It copies pointer to used operation int new pair object and clones parameters' object and stores it.</summary>
		/// <param name="rhs">reference to pair object which is being copied.</param>
		GaOperationParametersPair(const GaOperationParametersPair& rhs) : _operation(rhs._operation)
			{ _parameters = rhs._parameters ? (PTYPE*)rhs._parameters->Clone() : 0; }

		/// <summary>This constructor initializes operation's and parameters' pointers with <c>NULL</c> values. </summary>
		GaOperationParametersPair() : _operation(0),
			_parameters(0) { };

		/// <summary>Destructor frees memory allocated by parameters' object which was made by cloning parameters' object which user specified
		/// by calling <see cref="SetParameters" />, <see cref="SetOperation" /> or when pair's object is constructed.</summary>
		~GaOperationParametersPair()
		{
			if( _parameters )
				delete _parameters;
		}

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Returns reference to operation's object.</returns>
		inline OTYPE& GACALL GetOperation() const { return *_operation; }

		/// <summary><c>SetOperation</c> method stores pointer to operation's object. It stores parameters stored by calling
		/// <see cref="SetParameters" /> method.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="operation">pointer to operation's object which will be stored in the pair.</param>
		/// <param name="parameters">reference to parameters' object who's copy will be stored in the pair.</param>
		inline void GACALL SetOperation(OTYPE* operation, PTYPE& parameters)
		{
			_operation = operation;

			SetParameters( parameters );
		}

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Returns reference to parameter's object.</returns>
		inline PTYPE& GACALL GetParameters() const { return *_parameters; }

		/// <summary><c>SetParameters</c> operation makes copy of (clones) user specified parameter's object and stores pointer to that object.
		/// If pointer to parameters/ object was defined prior the call, this method deletes object to which it was pointing.
		///
		/// This method is not thread-safe.</summary>
		/// <param name="parameters">reference to parameters' object who's copy will be stored in the pair.</param>
		void GACALL SetParameters(PTYPE& parameters)
		{
			if( _parameters )
				delete _parameters;

			_parameters = &parameters ? (PTYPE*)parameters.Clone() : 0;
		}

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Returns <c>true</c> if pair contains pointer to an operation's object, otherwise it returns <c>false</c>.</returns>
		inline bool GACALL HasOperation() const { return _operation != 0; }

		/// <summary>This method is not thread-safe.</summary>
		/// <returns>Returns <c>true</c> if pair contains pointer to an operation's object, otherwise it returns <c>false</c>.</returns>
		inline bool GACALL HasParameters() const { return _parameters != 0; }

	};// END CLASS DEFINITION GaOperationParametersPair

} // Common

#endif //__GA_OPERATION_H__
