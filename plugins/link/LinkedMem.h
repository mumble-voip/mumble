// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_PLUGINS_LINK_LINKEDMEM_H_
#define MUMBLE_PLUGINS_LINK_LINKEDMEM_H_

#ifdef _WIN32
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	include "windows.h"
#else
#	include <cstdint>
#	include <cstdio>

#	include <unistd.h>
#endif

// NOTE: To prevent possible undefined behavior in
// `SharedMemory::write` and `SharedMemory::reset`, this
// struct must not have any padding, and no members may
// have indeterminate bits when this struct is
// default-constructed.
struct LinkedMem {
#ifdef _WIN32
	UINT32 uiVersion = 0;
	DWORD uiTick     = 0;
#else
	uint32_t uiVersion    = 0;
	uint32_t uiTick       = 0;
#endif
	float fAvatarPosition[3] = { 0 };
	float fAvatarFront[3]    = { 0 };
	float fAvatarTop[3]      = { 0 };
	wchar_t name[256]        = { 0 };
	float fCameraPosition[3] = { 0 };
	float fCameraFront[3]    = { 0 };
	float fCameraTop[3]      = { 0 };
	wchar_t identity[256]    = { 0 };
#ifdef _WIN32
	UINT32 context_len = 0;
#else
	uint32_t context_len  = 0;
#endif
	unsigned char context[256] = { 0 };
	wchar_t description[2048]  = { 0 };
};

static inline const char *getLinkedMemoryName() {
#ifdef _WIN32
	return "MumbleLink";
#else
	static char name[256] = {};

	snprintf(name, 256, "/MumbleLink.%d", getuid());

	return name;
#endif
}

#endif // MUMBLE_PLUGINS_LINK_LINKEDMEM_H_
