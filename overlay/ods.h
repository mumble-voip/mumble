// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_ODS_H_
#define MUMBLE_ODS_H_

#include <stdarg.h>

void __cdecl _ods_out(const char *format, va_list *args);
void __cdecl fods(const char *format, ...);

#endif
