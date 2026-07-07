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

#include <clunk/clunk_ex.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#if defined _MSC_VER
#    ifndef snprintf
#        define snprintf _snprintf
#    endif
#endif

void clunk::Exception::add_message(const char *file, int line) {
	char buf[1024];
	snprintf(buf, sizeof(buf), "[%s:%d] ", file, line);
	message += buf;
}

void clunk::Exception::add_message(const std::string &msg) {
	message += msg;
	message += ' ';
}

void clunk::IOException::add_custom_message() {
	char buf[1024];
	memset(buf, 0, sizeof(buf));

#ifdef _MSC_VER
	strncpy(buf, _strerror(NULL), sizeof(buf));
#else 
	strncpy(buf, strerror(errno), sizeof(buf));
//	if (strerror_r(errno, buf, sizeof(buf)-1) != 0) perror("strerror");
#endif
	add_message(buf);
}

