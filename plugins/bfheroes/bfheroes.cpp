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
BYTE *stateptr;

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
	::MessageBox(h, L"Reads audio position information from Battlefield Heroes", L"Mumble BFHeroes Plugin", MB_OK);
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++)
		avatar_pos[i]=avatar_front[i]=avatar_top[i]=0.0f;

	char state;
	char logincheck;
	bool ok;

	ok = peekProc((BYTE *) 0x00A24D6C, &logincheck, 1);
	if (! ok)
		return false;

	if (logincheck == 0)
		return false;

	ok = peekProc(posptr, avatar_pos, 12) &&
	     peekProc(faceptr, avatar_front, 12) &&
	     peekProc(topptr, avatar_top, 12) &&
	     peekProc(stateptr, &state, 1);

	if (! ok)
		return false;

	/*
	   This state value just uses the first memory position. If the memory position is "0," then it means that you are not ingame.
	*/
	if (state == 0)
		return true; // This results in all vectors beeing zero which tells Mumble to ignore them.

	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	return ok;
}

static int trylock() {

	h = NULL;
	posptr = faceptr = topptr = NULL;

	DWORD pid=getProcess(L"BFHeroes.exe");
	if (!pid)
		return false;
	BYTE *mod=getModuleAddr(pid, L"BFAudio.dll");
	if (!mod)
		return false;

	h=OpenProcess(PROCESS_VM_READ, false, pid);
	if (!h)
		return false;

	BYTE *cacheaddr = mod + 0x4745c;
	BYTE *cache = peekProcPtr(cacheaddr);

	posptr = peekProcPtr(cache + 0xc0);
	faceptr = peekProcPtr(cache + 0xc4);
	topptr = peekProcPtr(cache + 0xc8);
	stateptr = peekProcPtr(cache + 0xc0);

	float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
	std::string context;
	std::wstring identity;

	if (fetch(apos, afront, atop, cpos, cfront, ctop, context, identity))
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

static const std::wstring longdesc() {
	return std::wstring(L"Supports Battlefield Heroes. No identity support yet.");
}

static std::wstring description(L"Battlefield Heroes");
static std::wstring shortname(L"Battlefield Heroes");

static MumblePlugin bfheroesplug = {
	MUMBLE_PLUGIN_MAGIC,
	description,
	shortname,
	about,
	NULL,
	trylock,
	unlock,
	longdesc,
	fetch
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &bfheroesplug;
}
