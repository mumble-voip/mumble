#ifndef QT_NO_DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif
#include <windows.h>
#include <math.h>

#include "../mumble_plugin.h"

static wchar_t wcPluginName[256];

struct LinkedMem {
	UINT32  uiVersion;
	DWORD   dwTick;
	float	fPosition[3];
	float	fFront[3];
	float	fTop[3];
	wchar_t	name[256];
};

static void about(HWND h) {
	::MessageBox(h, L"Reads audio position information from linked game", L"Mumble Link Plugin", MB_OK);
}

static HANDLE hMapObject = NULL;
static LinkedMem *lm = NULL;

static void unlock() {
	lm->dwTick = 0;
	lm->uiVersion = 0;
	lm->name[0] = 0;
	wcscpy_s(wcPluginName, 256, L"Link");
	return;
}

static int trylock() {
	if (lm->uiVersion == 1) {
		if ((GetTickCount() - lm->dwTick) < 5000) {
			if (lm->name[0]) {
				errno_t err = wcscpy_s(wcPluginName, 256, lm->name);
				if (err != 0) {
					wcscpy_s(wcPluginName, 256, L"Link");
					return false;
				}
			}
			return true;
		}
	}
	return false;
}


static int fetch(float *pos, float *front, float *top) {
	if ((GetTickCount() - lm->dwTick) > 5000)
		return false;

	if (lm->uiVersion != 1)
		return false;

	for (int i=0;i<3;i++)
		pos[i]=lm->fPosition[i];
	for (int i=0;i<3;i++)
		front[i]=lm->fFront[i];
	for (int i=0;i<3;i++)
		top[i]=lm->fTop[i];

	return true;
}

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID) {
	bool bCreated = false;
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			wcscpy_s(wcPluginName, 256, L"Link");
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

static MumblePlugin linkplug = {
	MUMBLE_PLUGIN_MAGIC,
	L"Link v1.0.1",
	wcPluginName,
	about,
	NULL,
	trylock,
	unlock,
	fetch
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &linkplug;
}
