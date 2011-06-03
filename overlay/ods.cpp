/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2011-2011, Stefan Hacker <dd0t@users.sourceforge.net>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include "ods.h"

void __cdecl _ods_out(const char *format, va_list *args) {
	char buf[4096], *p = buf + 2;

	buf[0] = 'M'; // Add a prefix
	buf[1] = ':';

	// Format but be aware of space taken by prefix
	int len = _vsnprintf_s(p, sizeof(buf) - 3, _TRUNCATE, format, *args);


	if (len <= 0)
		return;

	p += len;

	// Truncate trailing spaces
	while (p > (buf + 2) && isspace(p[-1]))
		*--p = '\0';

	// Add custom termination
	if (p > (buf + sizeof(buf) - 3)) { // Make sure there's space
		p = buf + sizeof(buf) - 3;
	}
	*p++ = '\r';
	*p++ = '\n';
	*p   = '\0';

	OutputDebugStringA(buf);
}

void __cdecl fods(const char *format, ...) {
	va_list args;

	va_start(args, format);
	_ods_out(format, &args);
	va_end(args);
}
