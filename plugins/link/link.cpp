// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef QT_NO_DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif
#include <windows.h>
#include <math.h>

#include "../mumble_plugin.h"

static wchar_t pluginname[256] = L"Link";
static wchar_t description[2048] = L"Reads audio position information from linked game";

struct LinkedMem {
	UINT32  uiVersion;
	DWORD   dwcount;
	float	fAvatarPosition[3];
	float	fAvatarFront[3];
	float	fAvatarTop[3];
	wchar_t	name[256];
	float	fCameraPosition[3];
	float	fCameraFront[3];
	float	fCameraTop[3];
	wchar_t	identity[256];
	UINT32 context_len;
	unsigned char context[256];
	wchar_t description[2048];
};

static HANDLE hMapObject = NULL;
static LinkedMem *lm = NULL;
static DWORD last_count = 0;
static DWORD last_tick = 0;

static void unlock() {
	lm->dwcount = last_count = 0;
	lm->uiVersion = 0;
	lm->name[0] = 0;
	wcscpy(pluginname, L"Link");
	wcscpy(description, L"Reads audio position information from linked game");
}

static int trylock(const MumblePIDLookup, const MumblePIDLookupContext) {
	if ((lm->uiVersion == 1) || (lm->uiVersion == 2)) {
		if (lm->dwcount != last_count) {
			last_count = lm->dwcount;
			last_tick = GetTickCount();

			errno_t err = 0;

			if (lm->name[0]) {
				err = wcscpy_s(pluginname, 256, lm->name);
			}
			if (!err && lm->description[0]) {
				err = wcscpy_s(description, 2048, lm->description);
			}
			if (err) {
				wcscpy(pluginname, L"Link");
				wcscpy(description, L"Reads audio position information from linked game");
				return false;
			}
			return true;
		}
	}
	return false;
}


static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, MumbleString *context, MumbleWideString *identity) {
	if (lm->dwcount != last_count) {
		last_count = lm->dwcount;
		last_tick = GetTickCount();
	} else 	if ((GetTickCount() - last_tick) > 5000)
		return false;

	if ((lm->uiVersion != 1) && (lm->uiVersion != 2))
		return false;

	for (int i=0;i<3;++i) {
		avatar_pos[i]=lm->fAvatarPosition[i];
		avatar_front[i]=lm->fAvatarFront[i];
		avatar_top[i]=lm->fAvatarTop[i];
	}

	if (lm->uiVersion == 2) {
		for (int i=0;i<3;++i) {
			camera_pos[i]=lm->fCameraPosition[i];
			camera_front[i]=lm->fCameraFront[i];
			camera_top[i]=lm->fCameraTop[i];
		}

		MumbleStringAssign(context, std::string(reinterpret_cast<const char *>(lm->context), std::min(255u, lm->context_len)));
		lm->identity[255] = 0;
		MumbleStringAssign(identity, lm->identity);
	} else {
		for (int i=0;i<3;++i) {
			camera_pos[i]=lm->fAvatarPosition[i];
			camera_front[i]=lm->fAvatarFront[i];
			camera_top[i]=lm->fAvatarTop[i];
		}
		MumbleStringClear(context);
		MumbleStringClear(identity);
	}

	return true;
}

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID) {
	bool bCreated = false;
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			hMapObject = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"MumbleLink");
			if (hMapObject == NULL) {
				hMapObject = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(LinkedMem), L"MumbleLink");
				bCreated = true;
				if (hMapObject == NULL)
					return false;
			}
			lm = static_cast<LinkedMem *>(MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, 0));
			if (lm == NULL) {
				CloseHandle(hMapObject);
				hMapObject = NULL;
				return false;
			}
			if (bCreated)
				memset(lm, 0, sizeof(LinkedMem));
			break;
		case DLL_PROCESS_DETACH:
			if (lm) {
				UnmapViewOfFile(lm);
				lm = NULL;
			}
			if (hMapObject) {
				CloseHandle(hMapObject);
				hMapObject = NULL;
			}
			break;
	}
	return true;
}

static MumblePlugin linkplug = {
	MUMBLE_PLUGIN_MAGIC,
	120,
	false,
	pluginname,
	L"Universal",
	description,
	fetch,
	trylock,
	unlock,
};

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &linkplug;
}
