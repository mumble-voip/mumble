// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PLUGIN_WIN32_H_
#define MUMBLE_MUMBLE_PLUGIN_WIN32_H_

#ifdef USE_PLUGIN_LEGACY_PTR
# define PTR_TYPE VOID *
# define PTR_TYPE_CONCRETE BYTE *
# include "mumble_plugin_win32_ptr_type.h"
#else
# error The mumble_plugin_win32.h header is no longer supported
#endif

#endif
