// Copyright 2005-2016 The Mumble Developers. All rights reserved.
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

static std::wstring wsPluginName;
static std::wstring wsDescription;

#ifndef PLUTOVR_BUILD
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
#else
struct LinkedMem {
	UINT32  uiVersion;
	DWORD   dwcount;
	float	fAvatarPosition[3];
	float	fAvatarFront[3];
	float	fAvatarTop[3];
	float	fCameraPosition[3];
	float	fCameraFront[3];
	float	fCameraTop[3];
	wchar_t	identity[256];
};
#endif

static void about(HWND h) {
	::MessageBox(h, L"Reads audio position information from linked game", L"Mumble Link Plugin", MB_OK);
}

static HANDLE hMapObject = NULL;
static LinkedMem *lm = NULL;
static DWORD last_count = 0;
static DWORD last_tick = 0;

static void unlock() {
	lm->dwcount = last_count = 0;
	lm->uiVersion = 0;
#ifndef PLUTOVR_BUILD
	lm->name[0] = 0;
	wsPluginName.assign(L"Link");
#else
	wsPluginName.assign(L"PlutoLink");
#endif
	wsDescription.clear();
}

static int trylock() {
	if ((lm->uiVersion == 1) || (lm->uiVersion == 2)) {
		if (lm->dwcount != last_count) {
			last_count = lm->dwcount;
			last_tick = GetTickCount();

			errno_t err = 0;
#ifndef PLUTOVR_BUILD
			wchar_t buff[2048];

			if (lm->name[0]) {
				err = wcscpy_s(buff, 256, lm->name);
				if (! err)
					wsPluginName.assign(buff);
			}
			if (!err && lm->description[0]) {
				err = wcscpy_s(buff, 2048, lm->description);
				if (! err)
					wsDescription.assign(buff);
			}
#else
			wsPluginName.assign(L"PlutoLink");
			wsDescription.clear();
#endif
			if (err) {
				wsPluginName.assign(L"Link");
				wsDescription.clear();
				return false;
			}
			return true;
		}
	}
	return false;
}


static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	if (lm->dwcount != last_count) {
		last_count = lm->dwcount;
		last_tick = GetTickCount();
#ifndef PLUTOVR_BUILD
	} else 	if ((GetTickCount() - last_tick) > 5000)
#else
	} else 	if ((GetTickCount() - last_tick) > 600000)
#endif
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

#ifndef PLUTOVR_BUILD
		if (lm->context_len > 255)
			lm->context_len = 255;
#endif
		lm->identity[255] = 0;

#ifndef PLUTOVR_BUILD
		context.assign(reinterpret_cast<const char *>(lm->context), lm->context_len);
#else
		context = "Pluto";
		if (identity.empty())
#endif
		identity.assign(lm->identity);
	} else {
		for (int i=0;i<3;++i) {
			camera_pos[i]=lm->fAvatarPosition[i];
			camera_front[i]=lm->fAvatarFront[i];
			camera_top[i]=lm->fAvatarTop[i];
		}
		context.clear();
		identity.clear();
	}

	return true;
}

static const std::wstring getdesc() {
	return wsDescription;
}

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID) {
	bool bCreated = false;
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
#ifndef PLUTOVR_BUILD
			wsPluginName.assign(L"Link");
			hMapObject = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"MumbleLink");
#else
			wsPluginName.assign(L"PlutoLink");
			hMapObject = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"PlutoLink");
#endif
			if (hMapObject == NULL) {
#ifndef PLUTOVR_BUILD
				hMapObject = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(LinkedMem), L"MumbleLink");
#else
				hMapObject = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(LinkedMem), L"PlutoLink");
#endif
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

static std::wstring description(L"Link v1.2.0");

static MumblePlugin linkplug = {
	MUMBLE_PLUGIN_MAGIC,
	description,
	wsPluginName,
	about,
	NULL,
	trylock,
	unlock,
	getdesc,
	fetch
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &linkplug;
}
