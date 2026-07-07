/*
 *	Simple Source Common Header (header)
 *
 *  File		: SimpleHeader.h
 *	Library		: SimpleSource
 *  Version		: 1.12
 *
 *	© 2006, ChunkWare Music Software, OPEN-SOURCE
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a
 *	copy of this software and associated documentation files (the "Software"),
 *	to deal in the Software without restriction, including without limitation
 *	the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *	and/or sell copies of the Software, and to permit persons to whom the
 *	Software is furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in
 *	all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *	DEALINGS IN THE SOFTWARE.
 */


#ifndef __SIMPLE_HEADER_H__
#define __SIMPLE_HEADER_H__

#if _MSC_VER > 1000			// MS Visual Studio
#define INLINE __forceinline	// forces inline
#ifndef NOMINMAX
#define NOMINMAX				// for standard library min(), max()
#endif
#define _USE_MATH_DEFINES		// for math constants
#else						// other IDE's
#define INLINE inline
#endif

#include <algorithm>	// for min(), max()
#include <cassert>		// for assert()
#include <cmath>

#endif	// end __SIMPLE_HEADER_H__
