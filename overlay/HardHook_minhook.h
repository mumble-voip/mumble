// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_HARDHOOK_MINHOOK_H_
#define MUMBLE_HARDHOOK_MINHOOK_H_

#include <cmath>
#include <ctype.h>
#include <map>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <vector>

#include <windows.h>
#include <unknwn.h>

typedef void *(*voidFunc)();

struct HardHook {
	voidFunc m_func;
	voidFunc m_replacement;
	voidFunc call;

	HardHook();
	HardHook(voidFunc func, voidFunc replacement);
	void setup(voidFunc func, voidFunc replacement);
	void setupInterface(IUnknown *intf, LONG funcoffset, voidFunc replacement);
	void reset();
	void inject(bool force = false);
	void restore(bool force = false);
	void print();
	void check();
};

#endif
