#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "../mumble_plugin.h"

HANDLE h;
BYTE *posptr;
BYTE *rotptr;

static DWORD getProcess(const wchar_t *exename) {
	PROCESSENTRY32 pe;
	DWORD pid = 0;

	pe.dwSize = sizeof(pe);
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE) {
		BOOL ok = Process32First(hSnap, &pe);

		while (ok) {
			if (wcscmp(pe.szExeFile, exename)==0) {
				pid = pe.th32ProcessID;
				break;
			}
			ok = Process32Next(hSnap, &pe);
		}
		CloseHandle(hSnap);
	}
	return pid;
}

static BYTE *getModuleAddr(DWORD pid, const wchar_t *modname) {
	MODULEENTRY32 me;
	BYTE *addr = NULL;
	me.dwSize = sizeof(me);
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (hSnap != INVALID_HANDLE_VALUE) {
		BOOL ok = Module32First(hSnap, &me);

		while (ok) {
			if (wcscmp(me.szModule, modname)==0) {
				addr = me.modBaseAddr;
				break;
			}
			ok = Module32Next(hSnap, &me);
		}
		CloseHandle(hSnap);
	}
	return addr;
}


static bool peekProc(VOID *base, VOID *dest, SIZE_T len) {
	SIZE_T r;
	BOOL ok=ReadProcessMemory(h, base, dest, len, &r);
	return (ok && (r == len));
}

static void about(HWND h) {
	::MessageBox(h, L"Reads audio position information from Left 4 Dead (Build 3730)", L"Mumble L4D Plugin", MB_OK);
}

static bool calcout(float *pos, float *rot, float *opos, float *front, float *top) {
		top[0] = 0.0f;
		top[1] = 0.0f;
		top[2] = 0.0f;

		float h = rot[0];
		float v = rot[1];

		if ((v < -180.0f) || (v > 180.0f) || (h < -180.0f) || (h > 180.0f))
			return false;

		h *= static_cast<float>(M_PI / 180.0f);
		v *= static_cast<float>(M_PI / 180.0f);

		// Seems L4D is in inches. INCHES?!?
		opos[0] = pos[0] / 39.37f;
		opos[1] = pos[2] / 39.37f;
		opos[2] = pos[1] / 39.37f;

		front[0] = cos(v);
		front[1] = 0.0f;
		front[2] = sin(v);

		return true;
}

static int trylock() {

	h = NULL;
	posptr = rotptr = NULL;

	DWORD pid=getProcess(L"left4dead.exe");
	if (!pid)
		return false;

	BYTE *mod=getModuleAddr(pid, L"client.dll");
	if (!mod)
		return false;

	h=OpenProcess(PROCESS_VM_READ, false, pid);
	if (!h)
		return false;

	posptr = mod + 0x576240;
	rotptr = mod + 0x576240 + 24;

	float pos[3];
	float rot[3];
	float opos[3], top[3], front[3];

	bool ok = peekProc(posptr, pos, 12) &&
			  peekProc(rotptr, rot, 12);

	if (ok)
		return calcout(pos, rot, opos, top, front);

	CloseHandle(h);
	h = NULL;
	return false;
}

static void unlock() {
	if (h) {
		CloseHandle(h);
		h = NULL;
	}
	return;
}

static int fetch(float *pos, float *front, float *top) {
	float ipos[3], rot[3];
	bool ok;
	ok = peekProc(posptr, ipos, 12) &&
	     peekProc(rotptr, rot, 12);

	if (ok)
		return calcout(ipos, rot, pos, top, front);

	return false;
}

static MumblePlugin l4dplug = {
	MUMBLE_PLUGIN_MAGIC,
	L"Left 4 Dead (Build 3730)",
	L"Left 4 Dead",
	about,
	NULL,
	trylock,
	unlock,
	fetch
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &l4dplug;
}
