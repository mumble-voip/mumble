#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>

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
	::MessageBox(h, L"Read audio position information from BF2 (v 1.02)", L"Mumble BF2 Plugin", MB_OK);
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
	if (cache && posptr && faceptr && topptr && velptr)
		if (tryRead(posptr, 12) && tryRead(faceptr, 12) && tryRead(topptr, 12) && tryRead(velptr, 12))
			return true;
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
