#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>
#include <math.h>
#include <string>
#include <sstream>

#include "../mumble_plugin.h"

HANDLE h = NULL;

using namespace std;

BYTE *pos1ptr;
BYTE *pos2ptr;
BYTE *pos3ptr;
BYTE *rot1ptr;
BYTE *rot2ptr;

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
	::MessageBox(h, L"Reads audio position information from Enemy Territory: Quake Wars (v1.50). IP:Port context support. Note that vehicle PA support is not present or may not function as expected", L"Mumble ETQW plugin", MB_OK);
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	char menustate;
	char ccontext[128];
	
	float viewHor, viewVer;
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
	ok = peekProc((BYTE *) 0x00801BA4, &menustate, 1);
	if (! ok)
		return false;
	if (menustate == 0)
		return true;

	ok = peekProc(pos1ptr, avatar_pos+1, 4) &&	//Y
	     peekProc(pos2ptr, avatar_pos, 4) &&	//X
	     peekProc(pos3ptr, avatar_pos+2, 4) && //Z
	     peekProc(rot1ptr, &viewHor, 4) && //Hor
	     peekProc(rot2ptr, &viewVer, 4) && //Ver
		 peekProc((BYTE *) 0x0122E0B8, ccontext, 128);

	if (! ok)
		return false;

	avatar_top[2] = -1; // Head movement is in front vector
	
	ccontext[127] = 0;
	context = std::string(ccontext);
	
	if (context.find(':')==string::npos) 
		context.append(":27733");

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
	pos1ptr = pos2ptr = pos3ptr = rot1ptr = rot2ptr = NULL;
	
	DWORD pid=getProcess(L"etqw.exe");
	if (!pid)
		return false;
	BYTE *mod=getModuleAddr(pid, L"gamex86.dll");
	if (!mod)
		return false;
		
	pos1ptr = mod + 0x74EABC;
	pos2ptr = mod + 0x74EAB4;
	pos3ptr = mod + 0x74EAB8;
	rot1ptr = mod + 0x75F014;
	rot2ptr = mod + 0x75D30C;

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
	return std::wstring(L"Supports Enemy Territory: Quake Wars v1.50. No identity support yet.");
}

static std::wstring description(L"Enemy Territory: Quake Wars v1.50");
static std::wstring shortname(L"Enemy Territory: Quake Wars");

static MumblePlugin etqwplug = {
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
	return &etqwplug;
}
