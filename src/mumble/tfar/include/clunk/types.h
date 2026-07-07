#ifndef CLUNK_TYPES_H
#define CLUNK_TYPES_H

/* clunk - cross-platform 3D audio API built on top SDL library
 * Copyright (C) 2007-2014 Netive Media Group
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <stdint.h>

#ifndef _USE_MATH_DEFINES
#	define _USE_MATH_DEFINES
#endif

#include <math.h>
#include <clunk/config.h>

namespace clunk
{
	typedef uint8_t		u8;
	typedef uint16_t	u16;
	typedef uint32_t	u32;

	typedef int8_t		s8;
	typedef int16_t		s16;
	typedef int32_t		s32;
}

#if !(defined(__GNUC__) || defined(__GNUG__) || defined(__attribute__))
#	define __attribute__(p) /* nothing */
#endif

#endif
