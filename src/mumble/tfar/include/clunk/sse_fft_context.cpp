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

#include <stdlib.h>
#include <stdio.h>
#include <new>
#include <clunk/fft_context.h>
#include <clunk/sse_fft_context.h>

using namespace clunk;

void * aligned_allocator::allocate(size_t size, size_t alignment) {
	void * ptr;
#ifdef _WINDOWS
	if ((ptr = _aligned_malloc(size, alignment)) == NULL)
		throw std::bad_alloc();
#else
	if (posix_memalign(&ptr, alignment, size) != 0)
		throw std::bad_alloc();
#endif
	return ptr;
}

void aligned_allocator::deallocate(void *ptr) {
#ifdef _WINDOWS
	_aligned_free(ptr);
#else
	free(ptr);
#endif
}
