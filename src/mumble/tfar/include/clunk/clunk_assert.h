#ifndef CLUNK_STATIC_ASSERT_H__
#define CLUNK_STATIC_ASSERT_H__

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


#define CLUNK_JOIN(x, y) CLUNK_JOIN_AGAIN(x, y)
#define CLUNK_JOIN_AGAIN(x, y) x ## y

#define clunk_static_assert(e) \
typedef char CLUNK_JOIN(assertion_failed_at_line_, __LINE__) [(e) ? 1 : -1]

#endif

