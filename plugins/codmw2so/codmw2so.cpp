#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>
#include <math.h>

#include "../mumble_plugin.h"

HANDLE h = NULL;

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
	::MessageBox(h, L"Reads audio position information from Call of Duty: Modern Warfare 2 Special Ops(v1.0)", L"Mumble CoDMW2SO Plugin", MB_OK);
}


static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	float viewHor, viewVer;
	char state;
	char specops;

	for (int i=0;i<3;i++)
		avatar_pos[i]=avatar_front[i]=avatar_top[i]=0.0f;

	bool ok;
	bool so;

	/*
		This plugin uses the following Variables:

			Address			Type	Description
			===================================
			0x00782A64		float	Z-Coordinate
			0x00782A68		float	X-Coordinate
			0x00782A6C		float	Y-Coordinate
			0x00782A34		float	Horizontal view (degrees)
			0x00782A30		float	Vertical view (degrees)

			0x01597682		byte	Magical state value
	*/

	so = peekProc((BYTE *) 0x019703A0, &specops, 1); // Magical state value
	if (! so)
		return false;

	if (specops != 2)
		return false; // 2 value indicates you are playing Special Ops, 1 indicates SP, 0 indicates at three-way selection menu

	ok = peekProc((BYTE *) 0x01B12BBB, &state, 1); // Magical state value
	if (! ok)
		return false;

	// /*
	//	state value is:
	//	0		while not in game
	//	4 to 5	while playing

	//	This value is used for disabling pa for spectators
	//	or people not on a server.
	// */

	if (state == 0)
		return true; // This results in all vectors beeing zero which tells mumble to ignore them.

	ok = peekProc((BYTE *) 0x00782A64, avatar_pos+2, 4) &&	//Z
	     peekProc((BYTE *) 0x00782A68, avatar_pos, 4) &&	//X
	     peekProc((BYTE *) 0x00782A6C, avatar_pos+1, 4) && //Y
	     peekProc((BYTE *) 0x00782A34, &viewHor, 4) && //Hor
	     peekProc((BYTE *) 0x00782A30, &viewVer, 4); //Ver

	if (! ok)
		return false;

	// Scale Coordinates
	/*
	   Z-Value is increasing when heading north
				  decreasing when heading south
	   X-Value is increasing when heading west
				  decreasing when heading east
	   Y-Value is increasing when going up
				  decreasing when going down
	   40 units = 1 meter (not confirmed)
	*/
	for (int i=0;i<3;i++)
		avatar_pos[i]/=40.0f; // Scale to meters
	avatar_pos[0]*=(-1.0f); // Convert right to left handed

	avatar_top[2] = -1; // Head movement is in front vector

	// Calculate view unit vector
	/*
	   Vertical view 0° when centered
					85°	when looking down
				   275° when looking up
	   Decreasing when looking up.

	   Horizontal is 0° when facing North
					90° when facing West
				   180° when facing South
				   270° when facing East
	   Increasing when turning left.
	*/
	viewVer *= static_cast<float>(M_PI / 180.0f);
	viewHor *= static_cast<float>(M_PI / 180.0f);

	avatar_front[0] = -sin(viewHor) * cos(viewVer);
	avatar_front[1] = -sin(viewVer);
	avatar_front[2] = cos(viewHor) * cos(viewVer);

	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	return ok;
}

static int trylock() {
	h = NULL;
	DWORD pid=getProcess(L"iw4sp.exe");
	if (!pid)
		return false;

	h=OpenProcess(PROCESS_VM_READ, false, pid);
	if (!h)
		return false;

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
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Call of Duty: Modern Warfare 2 Special Ops v1.0 only. No context or identity support yet.");
}

static std::wstring description(L"Call of Duty: Modern Warfare 2 Special Ops v1.0");
static std::wstring shortname(L"Call of Duty: Modern Warfare 2 Special Ops");

static MumblePlugin codmw2soplug = {
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
	return &codmw2soplug;
}
