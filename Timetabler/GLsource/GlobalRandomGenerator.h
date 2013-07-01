
/*! \file GlobalRandomGenerator.h
    \brief This file contains declaration of global random number generator objects.
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

#include "Platform.h"
#include "Random.h"

#if defined(GAL_API_EXPORTING)

/// <summary>Global object used by GAL to generate random integer numbers. It cannot be used prior calling <see cref="GaInitialize" />
/// and after calling <see cref="GaFinalize" />. For more details see <see cref="GaRandomInteger" /> class.</summary>
extern Common::GaRandomInteger* GaGlobalRandomIntegerGenerator;

/// <summary>Global object used by GAL to generate random single precision floating point numbers. It cannot be used prior calling <see cref="GaInitialize" />
/// and after calling <see cref="GaFinalize" />. For more details see <see cref="GaRandomInteger" /> class.</summary>
extern Common::GaRandomFloat* GaGlobalRandomFloatGenerator;

/// <summary>Global object used by GAL to generate random double precision floating point numbers. It cannot be used prior calling <see cref="GaInitialize" />
/// and after calling <see cref="GaFinalize" />. For more details see <see cref="GaRandomInteger" /> class.</summary>
extern Common::GaRandomDouble* GaGlobalRandomDoubleGenerator;

/// <summary>Global object used by GAL to generate random Boolean values. It cannot be used prior calling <see cref="GaInitialize" />
/// and after calling <see cref="GaFinalize" />. For more details see <see cref="GaRandomInteger" /> class.</summary>
extern Common::GaRandomBool* GaGlobalRandomBoolGenerator;

#else

#if defined(GAL_PLATFORM_WIN)

GAL_API Common::GaRandomInteger* GaGlobalRandomIntegerGenerator;
GAL_API Common::GaRandomFloat* GaGlobalRandomFloatGenerator;
GAL_API Common::GaRandomDouble* GaGlobalRandomDoubleGenerator;
GAL_API Common::GaRandomBool* GaGlobalRandomBoolGenerator;

#elif defined(GAL_PLATFORM_NIX)

extern Common::GaRandomInteger* GaGlobalRandomIntegerGenerator;
extern Common::GaRandomFloat* GaGlobalRandomFloatGenerator;
extern Common::GaRandomDouble* GaGlobalRandomDoubleGenerator;
extern Common::GaRandomBool* GaGlobalRandomBoolGenerator;

#endif

#endif
