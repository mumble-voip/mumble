/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef QT_NO_DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif
#include <windows.h>
#include <math.h>

#include "../mumble_plugin.h"

static std::wstring wsPluginName;
static std::wstring wsDescription;

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
	lm->name[0] = 0;
	wsPluginName.assign(L"Link");
	wsDescription.clear();
}

static int trylock() {
	if ((lm->uiVersion == 1) || (lm->uiVersion == 2)) {
		if (lm->dwcount != last_count) {
			last_count = lm->dwcount;
			last_tick = GetTickCount();

			errno_t err = 0;
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

		if (lm->context_len > 255)
			lm->context_len = 255;
		lm->identity[255] = 0;

		context.assign(reinterpret_cast<const char *>(lm->context), lm->context_len);
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
			wsPluginName.assign(L"Link");
			hMapObject = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"MumbleLink");
			if (hMapObject == NULL) {
				hMapObject = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(LinkedMem), L"MumbleLink");
				bCreated = true;
				if (hMapObject == NULL)
					return false;
			}
			lm = (LinkedMem *) MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, 0);
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
