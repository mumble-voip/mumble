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

static BYTE scratchbuffer[128];

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
	::MessageBox(h, L"Reads audio position information from BF2142 (v1.50)", L"Mumble BF2142 Plugin", MB_OK);
}

static bool sane(float *avatar_pos, float *vel, float *avatar_face, float *avatar_top, bool initial = false) {
	int i;
	bool ok = true;

	float min = (initial) ? 0.1f : 0.00001f;

	// Sanity check #1: Position should be from -2000 to +2000, and not 0.
	for (i=0;i<3;i++) {
		ok = ok && (fabs(avatar_pos[i]) > min);
		ok = ok && (fabs(avatar_pos[i]) < 2000.0);
	}
	if (! ok) {
		return false;
	}

	// Sanity check #2: Directional vectors should have length 1. (and 1 * 1 == 1, so no sqrt)
	double sqdist;
	sqdist=avatar_face[0] * avatar_face[0] + avatar_face[1] * avatar_face[1] + avatar_face[2] * avatar_face[2];
	if (fabs(sqdist - 1.0) > 0.1) {
		return false;
	}


	sqdist=avatar_top[0] * avatar_top[0] + avatar_top[1] * avatar_top[1] + avatar_top[2] * avatar_top[2];
	if (fabs(sqdist - 1.0) > 0.1) {
		return false;
	}

	if (! initial)
		return true;

	// .. and it's not looking STRAIGHT ahead...
	if (avatar_face[2] == 1.0)
		return false;
	if (avatar_top[1] == 1.0)
		return false;

	// Sanity check #3: Initial speed vector should be >0.2 (BF2142 gravity) and < 1.0
	sqdist=vel[0] * vel[0] + vel[1] * vel[1] + vel[2] * vel[2];
	if ((sqdist < 0.2) || (sqdist > 1.0))
		return false;

	// We're good!
	return true;
}

static int trylock() {

	h = NULL;
	posptr = faceptr = topptr = velptr = NULL;

	DWORD pid=getProcess(L"BF2142.exe");
	if (!pid)
		return false;
	BYTE *mod=getModuleAddr(pid, L"BF2142Audio.dll");
	if (!mod)
		return false;

	h=OpenProcess(PROCESS_VM_READ, false, pid);
	if (!h)
		return false;

	BYTE *cacheaddr= mod + 0x4745c;
	BYTE *cache = peekProcPtr(cacheaddr);

	posptr = peekProcPtr(cache + 0xc0);
	faceptr = peekProcPtr(cache + 0xc4);
	topptr = peekProcPtr(cache + 0xc8);
	velptr = peekProcPtr(cache + 0xcc);
	if (cache && posptr && faceptr && topptr && velptr) {
		float avatar_pos[3];
		float vel[3];
		float avatar_face[3];
		float avatar_top[3];

		bool ok = peekProc(posptr, avatar_pos, 12) &&
		          peekProc(velptr, vel, 12) &&
		          peekProc(faceptr, avatar_face, 12) &&
		          peekProc(topptr, avatar_top, 12);

		if (ok)
			ok = sane(avatar_pos, vel, avatar_face, avatar_top, true);

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

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	float vel[3];
	bool ok;
	ok = peekProc(posptr, avatar_pos, 12) &&
	     peekProc(velptr, vel, 12) &&
	     peekProc(faceptr, avatar_front, 12) &&
	     peekProc(topptr, avatar_top, 12);

		 for (int i=0;i<3;i++) {
            camera_pos[i] = avatar_pos[i];
            camera_front[i] = avatar_front[i];
            camera_top[i] = avatar_top[i];
		 }

	if (ok) {
		ok = sane(avatar_pos, vel, avatar_front, avatar_top);
	}

	return ok;
}

static const std::wstring longdesc() {
    return std::wstring(L"Supports Battlefield 2142 v1.50. No context or identity support yet.");
}

static std::wstring description(L"Battlefield 2142 v1.50");
static std::wstring shortname(L"Battlefield 2142");

static MumblePlugin bf2142plug = {
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
	return &bf2142plug;
}
