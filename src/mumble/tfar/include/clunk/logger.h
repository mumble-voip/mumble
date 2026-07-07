#ifndef CLUNK_LOGGER_H__
#define CLUNK_LOGGER_H__

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

#include <clunk/export_clunk.h>
#include <string>
#include <exception>

#if !(defined(__GNUC__) || defined(__GNUG__) || defined(__attribute__))
#	define __attribute__(p) /* nothing */
#endif

namespace clunk {
	void CLUNKAPI log_debug(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
	const std::string CLUNKAPI format_string(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
}

#ifdef DEBUG
#	define LOG_DEBUG(fmt) clunk::log_debug fmt
#	define LOG_ERROR(fmt) clunk::log_debug fmt
#else
#	define LOG_DEBUG(fmt) do {} while(0)
#	define LOG_ERROR(fmt) clunk::log_debug fmt
#endif

#endif

