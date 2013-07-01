
/*! \file SortedGroupType.h
    \brief This file declares <c>GaSortedGroupType</c> datatypes.
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

#ifndef __GA_SORTED_GROUP_TYPE_H__
#define __GA_SORTED_GROUP_TYPE_H__

namespace Population
{
	/// <summary><c>GaSortedGroupType</c> defines types of sorted groups used in library. Types are defined by usage of sorted group in library.</summary>
	enum GaSortedGroupType
	{

		/// <summary>type is not defined.</summary>
		GASGT_NONE = 0x0, 

		/// <summary>sorted group is used to store best chromosomes.</summary>
		GASGT_BEST = 0x1, 

		/// <summary>sorted group is used to store worst chromosome.</summary>
		GASGT_WORST = 0x2,

		/// <summary>sorted group is used general purpose (i.e. to store results of selection operation).</summary>
		GASGT_OTHER = 0x4 

	};

}

#endif // __GA_SORTED_GROUP_TYPE_H__
