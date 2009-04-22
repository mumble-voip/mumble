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
	::MessageBox(h, L"Reads audio position information from Team Fortress 2 (Build 3771)", L"Mumble TF2 Plugin", MB_OK);
}

static bool calcout(float *pos, float *rot, float *opos, float *front, float *top) {
	float h = rot[0];
	float v = rot[1];

	if ((v < -360.0f) || (v > 360.0f) || (h < -360.0f) || (h > 360.0f))
		return false;

	h *= static_cast<float>(M_PI / 180.0f);
	v *= static_cast<float>(M_PI / 180.0f);

	// Seems TF2 is in inches. INCHES?!?
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

	// Check if we really have TF2 running
	/*
		position tuple:		client.dll+0x47ef90  (x,y,z, float)
		orientation tuple:	client.dll+0x47ef10  (v,h float)
		ID string:			client.dll+0x47826b = "teamJet@@" (9 characters, text)
		spawn state:        client.dll+0x467b84  (0 when at main menu, 1 when spectator, 3 when at team selection menu, and 6 or 9 when on a team (depending on the team side and gamemode), byte)
	*/
	char sMagic[9];
	if (!peekProc(mod + 0x47826b, sMagic, 9) || strncmp("teamJet@@", sMagic, 9)!=0)
		return false;

	// Remember addresses for later
	posptr = mod + 0x47ef90;
	rotptr = mod + 0x47ef10;
	stateptr = mod + 0x467b84;

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

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = 0;

	float ipos[3], rot[3];
	bool ok;
	char state;

	ok = peekProc(posptr, ipos, 12) &&
	     peekProc(rotptr, rot, 12) &&
	     peekProc(stateptr, &state, 1);
	if (!ok)
		return false;

	// Check to see if you are in a server
	if (state == 0 || state == 1 || state == 3)
		return true; // Deactivate plugin

	if (ok) {
		int res = calcout(ipos, rot, avatar_pos, avatar_front, avatar_top);
		if (res) {
			for (int i=0;i<3;++i) {
				camera_pos[i] = avatar_pos[i];
				camera_front[i] = avatar_front[i];
				camera_top[i] = avatar_top[i];
			}
			return res;
		}
	}

	return false;
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports TF2 build 3771. No identity or context support yet.");
}

static MumblePlugin tf2plug = {
	MUMBLE_PLUGIN_MAGIC,
	std::wstring(L"Team Fortress 2 (Build 3771)"),
	std::wstring(L"Team Fortress 2"),
	about,
	NULL,
	trylock,
	unlock,
	longdesc,
	fetch
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &tf2plug;
}
