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

#include <clunk/logger.h>
#include <stdio.h>
#include <stdarg.h>
#include <clunk/buffer.h>

void clunk::log_debug(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
}

#define FORMAT_BUFFER_SIZE 1024
#ifdef _WINDOWS
#define vsnprintf _vsnprintf
#endif

const std::string clunk::format_string(const char *fmt, ...) {
	va_list ap;

	//try static buffer on the stack to avoid malloc calls
	char static_buf[FORMAT_BUFFER_SIZE];

    va_start(ap, fmt);    
   	int r = vsnprintf (static_buf, FORMAT_BUFFER_SIZE - 1, fmt, ap);
    va_end(ap);

    if (r > -1 && r <= FORMAT_BUFFER_SIZE) 
   		return std::string(static_buf, r);

	int size = FORMAT_BUFFER_SIZE * 2;

	clunk::Buffer buf;

    while(true) {
		buf.set_size(size);
	    va_start(ap, fmt);    
    	int r = vsnprintf (static_cast<char *>(buf.get_ptr()), size - 1, fmt, ap);
	    va_end(ap);
	    if (r > -1 && r <= size) 
    		return std::string(static_cast<char *>(buf.get_ptr()), r);
    	size *= 2;
    }
}
