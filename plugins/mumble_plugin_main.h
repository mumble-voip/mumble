// Copyright 2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// This is the main header for process/OS stuff; it's included in most plugins
// because values can rarely be retrieved without accessing the process' memory.
//
// Various functions are provided for common tasks when reading from memory.
// The most important are getModuleAddr() and peekProc(); the first returns the
// base address of a module (e.g. shared library), the latter reads memory at the
// specified address and stores it in a variable.

#ifndef MUMBLE_PLUGIN_MAIN_H_
#define MUMBLE_PLUGIN_MAIN_H_

#include "mumble_plugin.h"
#include "mumble_plugin_win32_internals.h"

#include <cstring>

#ifdef WIN32
static const bool isWin32 = true;
#else
static bool isWin32;
#endif

static bool is64Bit;
static procid_t pPid;
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

template<class T>
static inline T peekProc(const procptr_t &addr) {
	T ret;

	if (!peekProc(addr, &ret, sizeof(T))) {
		memset(&ret, 0, sizeof(ret));
	}

	return ret;
}

static inline procptr_t peekProcPtr(const procptr_t &addr) {
	procptr_t v = 0;

	if (!peekProc(addr, &v, is64Bit ? 8 : 4)) {
		return 0;
	}

	return v;
}

// This function returns:
// -1 in case of failure.
// 0 if the process is 32-bit.
// 1 if the process is 64-bit.
#ifdef WIN32
static inline int8_t isProcess64Bit(const procptr_t &baseAddress) {
#else
// We use a different name because the function is called by the Linux version
// of isProcess64Bit() in case the process is running through Wine.
static inline int8_t isWin32Process64Bit(const procptr_t &baseAddress) {
#endif
	const auto dos = peekProc<ImageDosHeader>(baseAddress);
	if (!(dos.magic[0] == 'M' && dos.magic[1] == 'Z')) {
		// Invalid DOS signature
		return -1;
	}

	const auto nt = peekProc<ImageNtHeadersNoOptional>(baseAddress + dos.addressOfNtHeader);
	if (!(nt.signature[0] == 'P' && nt.signature[1] == 'E' && nt.signature[2] == '\0' && nt.signature[3] == '\0')) {
		// Invalid NT signature
		return -1;
	}

	switch (nt.fileHeader.machine) {
		case 0x14c: // IMAGE_FILE_MACHINE_I386
			return 0;
		default:
			return 1;
	}
}

#ifdef WIN32
# include "../mumble_plugin_win32.h"
#else
# include "../mumble_plugin_linux.h"
#endif

#endif
