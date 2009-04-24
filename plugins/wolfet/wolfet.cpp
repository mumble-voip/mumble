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
	::MessageBox(h, L"Reads audio position information from Wolfenstein: Enemy Territory (v 2.60b)", L"Mumble W:ET plugin", MB_OK);
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
			0x013F79CC		float	Y-Coordinate
			0x013E8CF4		float	X-Coordinate
			0x013E8CF8		float	Z-Coordinate
			0x013F9E20		float	Horizontal view
			0x013F9E1C		float	Vertical view
			0x013E8D18		byte	Magic value (32 ingame / 0 spectating)
	*/
	ok = peekProc((BYTE *) 0x013E8D18, &state, 1);
	if (! ok)
		return false;

	if (state != 32)
		return true; // If this magic value is !=32 we are spectating, so switch off PA

	ok = peekProc((BYTE *) 0x013F79CC, avatar_pos+1, 4) &&	//Y
	     peekProc((BYTE *) 0x013E8CF4, avatar_pos, 4) &&	//X
	     peekProc((BYTE *) 0x013E8CF8, avatar_pos+2, 4) && //Z
	     peekProc((BYTE *) 0x013F9E20, &viewHor, 4) && //Hor
	     peekProc((BYTE *) 0x013F9E1C, &viewVer, 4); //Ver

	if (! ok)
		return false;

	/*
	   Z-Value is increasing when heading north
				  decreasing when heading south
	   X-Value is increasing when heading east
				  decreasing when heading west
	   Y-Value is increasing when going up
				  decreasing when going down
	   1 unit = 1 meter (not confirmed)
	*/

	// Calculate view unit vector
	/*
	   Vertical view 0 when centered
				   -90 when looking up
				   +90 when looking down
	   Increasing when looking down.

	   Horizontal is 90 when facing north
					  0 when facing east
					-90 when facing south
					+/-180 when facing west
	   Increasing when turning left.
	*/
	viewVer *= static_cast<float>(M_PI / 180.0f);
	viewHor *= static_cast<float>(M_PI / 180.0f);

	avatar_front[0] = cos(viewVer) * cos(viewHor);
	avatar_front[1] = -sin(viewVer);
	avatar_front[2] = cos(viewVer) * sin(viewHor);
	
	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
    }

	return ok;
}

static int trylock() {
	h = NULL;
	DWORD pid=getProcess(L"ET.exe");
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
    return std::wstring(L"Supports Wolfenstien: Enemy Territory 2.60b. No context or identity support yet.");
}

static MumblePlugin wolfetplug = {
	MUMBLE_PLUGIN_MAGIC,
	std::wstring(L"Wolfenstein: Enemy Territory 2.60b"),
	std::wstring(L"Wolfenstein: Enemy Territory"),
	about,
	NULL,
	trylock,
	unlock,
	longdesc,
	fetch
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &wolfetplug;
}
