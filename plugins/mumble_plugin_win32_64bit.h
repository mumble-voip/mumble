// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PLUGIN_WIN32_64BIT_H_
#define MUMBLE_MUMBLE_PLUGIN_WIN32_64BIT_H_

#if defined(__linux__)
 #include "mumble_plugin_linux_64bit.h"
#else

typedef unsigned long long procptr64_t;

#define PTR_TYPE procptr64_t
#include "mumble_plugin_win32_ptr_type.h"

#endif

#endif
