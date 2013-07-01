
/*! \file Initialization.cpp
    \brief This file contains declaration of functions for initialization and finalization of the library.
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

#ifndef __INITIALIZATION_H__
#define __INITIALIZATION_H__

#include "Platform.h"

/// <summary>This function initializes GAL.
/// Main purpose is to initialize global random generators and operation catalogues.</summary>
/// <remarks>No library feature should be called before calling <c>GaInitialize</c>.</remarks>
GAL_API
void GACALL GaInitialize();

/// <summary><c>GaFinalize</c> should be call before exiting application, and after work with GAL is done.
/// It releases all acquired memory and resources needed for library.</summary>
/// <remarks>Objects which were instantiated by the user,
/// except those objects which was added to global operation catalogues, must be destroyed prior calling <c>GaFinalize</c>.
/// Note that any object of operation which was added to global catalogues will be deleted after <c>GaFinalize</c> call.</remarks>
GAL_API
void GACALL GaFinalize();

#endif // __INITIALIZATION_H__
