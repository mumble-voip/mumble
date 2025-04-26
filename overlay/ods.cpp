// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ods.h"
#include <windows.h>
#include <ctype.h>
#include <stdio.h>

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
