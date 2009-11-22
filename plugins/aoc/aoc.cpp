#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <sstream>

#define _USE_MATH_DEFINES
#include <math.h>

#include "../mumble_plugin.h"

using namespace std;

HANDLE h;
BYTE *posptr;
BYTE *rotptr;
BYTE *stateptr;
BYTE *hostptr;
BYTE *teamptr;

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
	::MessageBox(h, L"Reads audio position information from Age of Chivalry (Build 3943)", L"Mumble AOC Plugin", MB_OK);
}

static bool calcout(float *pos, float *rot, float *opos, float *front, float *top) {
	float h = rot[0];
	float v = rot[1];

	if ((v < -360.0f) || (v > 360.0f) || (h < -360.0f) || (h > 360.0f))
		return false;

	h *= static_cast<float>(M_PI / 180.0f);
	v *= static_cast<float>(M_PI / 180.0f);

	// Seems AOC is in inches. INCHES?!?
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

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, string &context, wstring &identity) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = 0;

	float ipos[3], rot[3];
	bool ok;
	char state;
	char chHostStr[40];
	BYTE bTeam;
	string sTeam;
	wostringstream new_identity;
	ostringstream new_context;

	ok = peekProc(posptr, ipos, 12) &&
	     peekProc(rotptr, rot, 12) &&
	     peekProc(stateptr, &state, 1) &&
	     peekProc(hostptr, chHostStr, 40) &&
	     peekProc(teamptr, &bTeam, 1);
	if (!ok)
		return false;
	chHostStr[39] = 0;


	switch (bTeam) {
		case 9:
			sTeam = "Mason Order";
			break;
		case 10:
			sTeam = "Agathia Knights";
			break;
		default:
			sTeam = "Unknown";
			break;
	}
		
	new_context << "<context>"
			<< "<game>aoc</game>"
			<< "<hostport>" << chHostStr << "</hostport>"
			<< "<team>" << sTeam << "</team>"
		    << "</context>";
	context = new_context.str();
// // 
	
	/* TODO
	new_identity << "<identity>"
			<< "<name>" << "SAS" << "</name>"
		     << "</identity>";
	identity = new_identity.str(); */

	// Check to see if you are spawned
	if (state == 0 || state == 1 || state == 2)
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

static int trylock() {
	h = NULL;
	posptr = rotptr = NULL;

	DWORD pid=getProcess(L"hl2.exe");
	if (!pid)
		return false;
	BYTE *mod=getModuleAddr(pid, L"client.dll");
	if (!mod)
		return false;
	BYTE *mod_engine = getModuleAddr(pid, L"engine.dll");
	if (!mod_engine)
		return false;

	h=OpenProcess(PROCESS_VM_READ, false, pid);
	if (!h)
		return false;

	// Check if we really have AOC running
	/*
		position tuple:		client.dll+0x748e14  (x,y,z, float)
		orientation tuple:	client.dll+0x73dc9c  (v,h float)
		ID string:			client.dll+0x7071e8 = "ageofchivalry" (13 characters, text)
		spawn state:        client.dll+0x6d4334  (0 when at main menu, 1 when at team selection, 2 when not spawned,
		                                          between 6 and 7 when spawned)
	    context offsets:    engine.dll + 0x3bf69f = (ip:port)
							server.dll + 0x71a0a4 = (team info, 9 Mason, 10 Agathia)
	*/

	// Remember addresses for later
	posptr = mod + 0x748e14;
	rotptr = mod + 0x73dc9c;
	stateptr = mod + 0x6d4334;
	hostptr = mod_engine + 0x3bf69f;
	teamptr = mod + 0x71a0a4;

	//Gamecheck 
	char sMagic[13];
	if (!peekProc(mod + 0x7071e8, sMagic, 13) || strncmp("ageofchivalry", sMagic, 13)!=0)
		return false;
		
	// Check if we can get meaningful data from it
	float apos[3], afront[3], atop[3];
	float cpos[3], cfront[3], ctop[3];
	wstring sidentity;
	string scontext;

	if (fetch(apos, afront, atop, cpos, cfront, ctop, scontext, sidentity))
		return true;

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

static const std::wstring longdesc() {
	return std::wstring(L"Supports AOC Build 3943. Context support only.");
}

static std::wstring description(L"Age of Chivalry (Build 3943)");
static std::wstring shortname(L"Age of Chivalry");

static MumblePlugin aocplug = {
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
	return &aocplug;
}
