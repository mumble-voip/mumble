// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_HARDHOOK_H_
#define MUMBLE_HARDHOOK_H_

#ifdef USE_MINHOOK
#	include "HardHook_minhook.h"
#else
#	include "HardHook_x86.h"
#endif

#endif
