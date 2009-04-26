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

static bool peekProc(VOID *base, VOID *dest, SIZE_T len) {
	SIZE_T r;
	BOOL ok=ReadProcessMemory(h, base, dest, len, &r);
	return (ok && (r == len));
}

static void about(HWND h) {
	::MessageBox(h, L"Reads audio position information from cod2 (v 1.3)", L"Mumble cod2 Plugin", MB_OK);
}


static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	float viewHor, viewVer;
	char state;

	for (int i=0;i<3;i++)
		avatar_pos[i]=avatar_front[i]=avatar_top[i]=0.0f;

	bool ok;

	/*
		This plugin uses the following Variables:

			Address			Type	Description
			===================================
			0x01516608		float	Z-Coordinate (=0 out-of-game)
			0x0151660C		float	X-Coordinate (=0 out-of-game)
			0x01516610		float	Y-Coordinate (=0 out-of-game)
			0x0151A114		float	Horizontal view (degrees) (=0 out-of-game)
			0x0151A110		float	Vertical view (degrees) (=0 out-of-game)
			0x0096B688		byte	Magic value (0=ingame/out-of-game, 4=spectator)
	*/
	ok = peekProc((BYTE *) 0x0096B688, &state, 1);
	if (! ok)
		return false;

	if (state == 4)
		return true; // If this magic value is 4 we are spectating, so switch of PA

	ok = peekProc((BYTE *) 0x01516608, avatar_pos+2, 4) &&	//Z
	     peekProc((BYTE *) 0x0151660C, avatar_pos, 4) &&	//X
	     peekProc((BYTE *) 0x01516610, avatar_pos+1, 4) && //Y
	     peekProc((BYTE *) 0x0151A114, &viewHor, 4) && //Hor
	     peekProc((BYTE *) 0x0151A110, &viewVer, 4); //Ver

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

	// Calculate view unit vector
	/*
	   Vertical view 0 when centered
					85	when looking down
				   275 when looking up
	   Decreasing when looking up.

	   Horizontal is 0 when facing North
					90 when facing West
				   180 when facing South
				   270 when facing East
	   Increasing when turning left.
	*/
	viewVer *= static_cast<float>(M_PI / 180.0f);
	viewHor *= static_cast<float>(M_PI / 180.0f);

	avatar_front[0] = -sin(viewHor) * cos(viewVer);
	avatar_front[1] = -sin(viewVer);
	avatar_front[2] = cos(viewHor) * cos(viewVer);

	return ok;
}

static int trylock() {
	h = NULL;
	DWORD pid=getProcess(L"CoD2MP_s.exe");
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
        return std::wstring(L"Supports Call of Duty 2 v1.3. No context or identity support yet.");
}

static std::wstring description(L"Call of Duty 2 v1.3");
static std::wstring shortname(L"Call of Duty 2");

static MumblePlugin cod2plug = {
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
	return &cod2plug;
}
