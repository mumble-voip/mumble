// Copyright 2021-2022 The Mumble Developers. All rights reserved.
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

struct LinkedMem {
#ifdef _WIN32
	UINT32 uiVersion;
	DWORD uiTick;
#else
	uint32_t uiVersion;
	uint32_t uiTick;
#endif
	float fAvatarPosition[3];
	float fAvatarFront[3];
	float fAvatarTop[3];
	wchar_t name[256];
	float fCameraPosition[3];
	float fCameraFront[3];
	float fCameraTop[3];
	wchar_t identity[256];
#ifdef _WIN32
	UINT32 context_len;
#else
	uint32_t context_len;
#endif
	unsigned char context[256];
	wchar_t description[2048];
};

static const char *getLinkedMemoryName() {
#ifdef _WIN32
	return "MumbleLink";
#else
	static char name[256] = {};

	snprintf(name, 256, "/MumbleLink.%d", getuid());

	return name;
#endif
}

#endif // MUMBLE_PLUGINS_LINK_LINKEDMEM_H_
