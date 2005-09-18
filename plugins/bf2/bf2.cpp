#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>
#include <math.h>

#include "../mumble_plugin.h"

HANDLE h;
BYTE *posptr;
BYTE *faceptr;
BYTE *topptr;
BYTE *velptr;


static DWORD getProcess(const wchar_t *exename) {
	PROCESSENTRY32 pe;
	DWORD pid = 0;

	pe.dwSize = sizeof(pe);
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE) {
		bool ok = Process32First(hSnap, &pe);

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
		bool ok = Module32First(hSnap, &me);

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
	bool ok=ReadProcessMemory(h, base, dest, len, &r);
	return (ok && (r == len));
}

static BYTE scratchbuffer[128];

static bool tryRead(VOID *base, SIZE_T len) {
	return peekProc(base, scratchbuffer, len);
}

static DWORD peekProc(VOID *base) {
	DWORD v = 0;
	peekProc(base, reinterpret_cast<BYTE *>(&v), sizeof(DWORD));
	return v;
}

static BYTE *peekProcPtr(VOID *base) {
	DWORD v = peekProc(base);
	return reinterpret_cast<BYTE *>(v);
}

static void about(HWND h) {
	::MessageBox(h, L"Reads audio position information from BF2 (v 1.02)", L"Mumble BF2 Plugin", MB_OK);
}

static bool sane(float *pos, float *vel, float *face, float *top, bool initial = false) {
	int i;
	bool ok = true;

	float min = (initial) ? 0.1 : 0.00001;

	// Sanity check #1: Position should be from -1000 to +1000, and not 0.
	for(i=0;i<3;i++) {
		ok = ok && (fabs(pos[i]) > min);
		ok = ok && (fabs(pos[i]) < 1000.0);
	}
	if (! ok) {
		return false;
	}

	// Sanity check #2: Directional vectors should have length 1. (and 1 * 1 == 1, so no sqrt)
	double sqdist;
	sqdist=face[0] * face[0] + face[1] * face[1] + face[2] * face[2];
	if (fabs(sqdist - 1.0) > 0.1) {
		return false;
	}


	sqdist=top[0] * top[0] + top[1] * top[1] + top[2] * top[2];
	if (fabs(sqdist - 1.0) > 0.1) {
		return false;
	}

	if (! initial)
		return true;

	// .. and it's not looking STRAIGHT ahead...
	if (face[2] == 1.0)
		return false;
	if (top[1] == 1.0)
		return false;

	// Sanity check #3: Initial speed vector should be >0.2 (BF2 gravity) and < 1.0
	sqdist=vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2];
	if ((sqdist < 0.2) || (sqdist > 1.0))
		return false;

	// We're good!
	return true;
}

static int trylock() {

	h = NULL;
	posptr = faceptr = topptr = velptr = NULL;

	DWORD pid=getProcess(L"BF2.exe");
	if (!pid)
		return false;
	BYTE *mod=getModuleAddr(pid, L"BF2Audio.dll");
	if (!mod)
		return false;

	h=OpenProcess(PROCESS_VM_READ, false, pid);
	if (!h)
		return false;

	BYTE *cacheaddr= mod + 0x4645c;
	BYTE *cache = peekProcPtr(cacheaddr);

	posptr = peekProcPtr(cache + 0xb4);
	faceptr = peekProcPtr(cache + 0xb8);
	topptr = peekProcPtr(cache + 0xbc);
	velptr = peekProcPtr(cache + 0xc0);
	if (cache && posptr && faceptr && topptr && velptr) {
		float pos[3];
		float vel[3];
		float face[3];
		float top[3];

		bool ok = peekProc(posptr, pos, 12) &&
			peekProc(velptr, vel, 12) &&
			peekProc(faceptr, face, 12) &&
			peekProc(topptr, top, 12);

		if (ok)
			ok = sane(pos, vel, face, top, true);

		if (ok)
			return true;
	}
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

static int fetch(float *pos, float *vel, float *front, float *top) {
	bool ok;
	ok = peekProc(posptr, pos, 12) &&
		peekProc(velptr, vel, 12) &&
		peekProc(faceptr, front, 12) &&
		peekProc(topptr, top, 12);

	if (ok) {
		ok = sane(pos, vel, front, top);
	}

	return ok;
}

static MumblePlugin bf2plug = {
	MUMBLE_PLUGIN_MAGIC,
	L"Battlefield 2 v1.02",
	L"Battlefield 2",
	about,
	NULL,
	trylock,
	unlock,
	fetch
};

extern "C" __declspec(dllexport) __cdecl MumblePlugin *getMumblePlugin() {
	return &bf2plug;
}
