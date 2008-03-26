#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

#include "../mumble_plugin.h"

struct LinkedMem {
	DWORD	dwVersion;
	DWORD	dwTick;
	float	fPosition[3];
	float	fFront[3];
	float	fTop[3];
};

static void about(HWND h) {
	::MessageBox(h, L"Reads audio position information from linked game", L"Mumble Link Plugin", MB_OK);
}

static HANDLE hMapObject = NULL;
LinkedMem *lm = NULL;

static void unlock() {
	lm->dwVersion = 0;
	return;
}

static int trylock() {
	if (lm->dwVersion == 1) {
		if ((GetTickCount() - lm->dwTick) < 5000)
			return true;
	}
	return false;
}


static int fetch(float *pos, float *front, float *top) {
	if ((GetTickCount() - lm->dwTick) > 5000)
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
	bool bCreated = true;
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			hMapObject = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(LinkedMem), L"MumbleLink");
			if (hMapObject == NULL) {
				bCreated = false;
				hMapObject = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"MumbleLink");
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
	L"Link v1.00",
	L"Link",
	about,
	NULL,
	trylock,
	unlock,
	fetch
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &linkplug;
}
