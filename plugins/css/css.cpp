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

static void about(HWND h) {
	::MessageBox(h, L"Reads audio position information from Counter-Strike: Source (Build 3698)", L"Mumble CSS Plugin", MB_OK);
}

static bool calcout(float *pos, float *rot, float *opos, float *front, float *top) {
	float h = rot[0];
	float v = rot[1];

	if ((v < -180.0f) || (v > 180.0f) || (h < -180.0f) || (h > 180.0f))
		return false;

	h *= static_cast<float>(M_PI / 180.0f);
	v *= static_cast<float>(M_PI / 180.0f);

	// Seems CSS is in inches. INCHES?!?
	opos[0] = pos[0] / 39.37f;
	opos[1] = pos[2] / 39.37f;
	opos[2] = pos[1] / 39.37f;

	front[0] = cos(v) * cos(h);
	front[1] = -sin(h);
	front[2] = sin(v) * cos(h);

	h -= static_cast<float>(M_PI / 2.0f);

	top[0] = cos(v) * cos(h);
	top[1] = -sin(h);
	top[2] = sin(v) * cos(h);

	return true;
}

static int trylock() {
	h = NULL;
	posptr = rotptr = NULL;

	DWORD pid=getProcess(L"hl2.exe");
	if (!pid)
		return false;
	BYTE *mod=getModuleAddr(pid, L"client.dll");
	if (!mod)
		return false;
	h=OpenProcess(PROCESS_VM_READ, false, pid);
	if (!h)
		return false;

	// Check if we really have CSS running
	/*
		position tuple:		client.dll+0x39b504  (x,y,z, float)
		orientation tuple:	client.dll+0x3ec684  (v,h float)
		ID string:			client.dll+0x39cde9 = "CSSpectatorGUI@@" (16 characters, text)
		spawn state:		client.dll+0x38e050  (0 in main menu, 3 when at team selection menu, 5 when spawned as CT, 6 when spawns as T)
	*/
	char sMagic[16];
	if (!peekProc(mod + 0x39cde9, sMagic, 16) || strncmp("CSSpectatorGUI@@", sMagic, 16)!=0)
		return false;

	// Remember addresses for later
	posptr = mod + 0x39b504;
	rotptr = mod + 0x3ec684;
	stateptr = mod + 0x38e050;

	float pos[3];
	float rot[3];
	float opos[3], top[3], front[3];

	bool ok = peekProc(posptr, pos, 12) &&
	          peekProc(rotptr, rot, 12);

	if (ok)
		return calcout(pos, rot, opos, top, front);
	// If it failed clean up
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
	char state;

	for (int i=0;i<3;i++)
		pos[i] = front[i] = top[i] = 0;

	ok = peekProc(posptr, ipos, 12) &&
	     peekProc(rotptr, rot, 12) &&
	     peekProc(stateptr , &state, 1);
	if (!ok)
		return false;

	//Check to see if you are in a server
	//spawn state: client.dll+0x38e050  (0 in main menu, 3 when at team selection menu, 5 when spawned as CT, 6 when spawns as T)
	if (state == 0 || state == 3)
		return true; // Deactivate plugin

	return calcout(ipos, rot, pos, front, top);
}

static MumblePlugin cssplug = {
	MUMBLE_PLUGIN_MAGIC,
	L"Counter-Strike: Source (Build 3698)",
	L"Counter-Strike: Source",
	about,
	NULL,
	trylock,
	unlock,
	fetch
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &cssplug;
}
