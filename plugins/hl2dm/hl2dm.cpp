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
	::MessageBox(h, L"Reads audio position information from Garry's Mod 11 (Build 3943). IP:Port context without team descriminator.", L"Mumble Gmod Plugin", MB_OK);
}

static bool calcout(float *pos, float *rot, float *opos, float *front, float *top) {
	float h = rot[0];
	float v = rot[1];

	if ((v < -360.0f) || (v > 360.0f) || (h < -360.0f) || (h > 360.0f))
		return false;

	h *= static_cast<float>(M_PI / 180.0f);
	v *= static_cast<float>(M_PI / 180.0f);

	// Seems HL2DM is in inches. INCHES?!?
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
	wostringstream new_identity;
	ostringstream new_context;

	ok = peekProc(posptr, ipos, 12) &&
	     peekProc(rotptr, rot, 12) &&
	     peekProc(stateptr, &state, 1) &&
	     peekProc(hostptr, chHostStr, 40);
	if (!ok)
		return false;
	chHostStr[39] = 0;

	new_context << "<context>"
	<< "<game>hl2dm</game>"
	<< "<hostport>" << chHostStr << "</hostport>"
	<< "</context>";
	context = new_context.str();

	/* TODO
	new_identity << "<identity>"
			<< "<name>" << "Gordon Freeman" << "</name>"
		     << "</identity>";
	identity = new_identity.str(); */

	// Check if the player is spawned
	if (state == 0 || state == 2)
		return true; // Deactivate plugin by leaving position as 0,0,0

	if (calcout(ipos, rot, avatar_pos, avatar_front, avatar_top)) {
		for (int i=0;i<3;++i) {
			camera_pos[i] = avatar_pos[i];
			camera_front[i] = avatar_front[i];
			camera_top[i] = avatar_top[i];
		}
		return true;
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

	// Check if we really have HL2DM running
	/*
		position tuple:		client.dll+0x3dcad4  (x,y,z, float)
		orientation tuple:	client.dll+0x3dcae0  (v,h float)
		ID string:			client.dll+0x3a5674 = "Dm/$" (4 characters, text)
		spawn state:		client.dll+0x37e180  (0 when at main menu, 2 when not spawned, 7 when spawned, byte)
		ip:port string		engine.dll+0x3909c4  (zero terminated ip:port, string)
	*/
	// Remember addresses for later
	posptr = mod + 0x3dcad4;
	rotptr = mod + 0x3dcae0;
	stateptr = mod + 0x37e180;
	hostptr = mod_engine + 0x3909c4;

	// Check if we are really running hl2dm
	char sMagic[4];
	if (!peekProc(mod + 0x3a5674, sMagic, 4) || strncmp("Dm/$", sMagic, 4) !=0)
		return false;

	// Check if we can get meaningfull data from it
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

static const wstring longdesc() {
	return wstring(L"Supports HL2DM build 3945. No identity support yet.");
}

static wstring description(L"Half-Life 2: Deathmatch (Build 3945)");
static wstring shortname(L"Half-Life 2: Deathmatch");

static MumblePlugin hl2dmplug = {
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
	return &hl2dmplug;
}
