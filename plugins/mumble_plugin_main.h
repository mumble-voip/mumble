// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_PLUGIN_MAIN_H_
#define MUMBLE_PLUGIN_MAIN_H_

#include "mumble_plugin.h"

static procid_t pPid;
static bool is64Bit;
static procptr_t pModule;

static inline bool peekProc(const procptr_t &addr, void *dest, const size_t &len);
static inline procptr_t getModuleAddr(const procid_t &pid, const wchar_t *modname);

static inline procptr_t getModuleAddr(const wchar_t *modname) {
	return getModuleAddr(pPid, modname);
}

template<class T>
static inline bool peekProc(const procptr_t &addr, T &dest) {
	return peekProc(addr, &dest, sizeof(T));
}

static inline procptr_t peekProcPtr(const procptr_t &addr) {
	procptr_t v = 0;

	if (!peekProc(addr, &v, is64Bit ? 8 : 4)) {
		return 0;
	}

	return v;
}

#ifdef WIN32
# include "../mumble_plugin_win32.h"
#else
# include "../mumble_plugin_linux.h"
#endif

#endif
