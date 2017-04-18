// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_PLUGIN_LINUX_32BIT_H_
#define MUMBLE_PLUGIN_LINUX_32BIT_H_

#include <stdint.h>

typedef uint32_t procptr32_t;

#define PTR_TYPE procptr32_t
#include "mumble_plugin_linux.h"

#endif
