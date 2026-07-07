#ifndef CLUNK_EXCEPTION_H__
#define CLUNK_EXCEPTION_H__

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


#include <exception>
#include <string>
#include <clunk/logger.h>

namespace clunk {
	class CLUNKAPI Exception : public std::exception {
	public: 
		Exception() throw() {} 
		void add_message(const char *file, int line);
		void add_message(const std::string &msg);
		virtual void add_custom_message() {}
		
		virtual ~Exception() throw() {}
		virtual const char* what() const throw() { return message.c_str(); }
	private: 
		std::string message;
	};
	
	class CLUNKAPI IOException : public Exception {
	public: 
		virtual void add_custom_message();
	};
}

#define throw_generic(ex_cl, fmt) { ex_cl e; e.add_message(__FILE__, __LINE__); e.add_message(clunk::format_string fmt ); e.add_custom_message(); throw e; }
#define throw_ex(fmt) throw_generic(clunk::Exception, fmt)
#define throw_io(fmt) throw_generic(clunk::IOException, fmt)

#define TRY try
#define CATCH(where, code) catch(const std::exception &_e) {\
		LOG_ERROR(("%s: %s", where, _e.what())); \
		code;\
	} 

#endif

