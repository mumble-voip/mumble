/* Copyright (C) 2009-2010, Snares <snares@users.sourceforge.net>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
	::MessageBox(h, L"Reads audio position information from Team Fortress 2 (Build 4057). IP:Port context support.", L"Mumble TF2 Plugin", MB_OK);
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

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, string &context, wstring &identity) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = 0;

	float ipos[3], rot[3];
	bool ok;
	char state;
	char chHostStr[40];
	string sHost;
	wostringstream new_identity;
	ostringstream new_context;

	ok = peekProc(posptr, ipos, 12) &&
	     peekProc(rotptr, rot, 12) &&
	     peekProc(stateptr, &state, 1) &&
	     peekProc(hostptr, chHostStr, 40);

	if (!ok)
		return false;

	chHostStr[39] = 0;

	sHost.assign(chHostStr);
	if (sHost.find(':')==string::npos)
		sHost.append(":27015");

	new_context << "<context>"
	<< "<game>tf2</game>"
	<< "<hostport>" << chHostStr << "</hostport>"
	<< "</context>";
	context = new_context.str();

	/* TODO
	new_identity << "<identity>"
			<< "<name>" << "SAS" << "</name>"
		     << "</identity>";
	identity = new_identity.str(); */

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

static int trylock() {

	h = NULL;
	posptr = rotptr = NULL;

	DWORD pid=getProcess(L"hl2.exe");
	if (!pid)
		return false;
	BYTE *mod=getModuleAddr(pid, L"client.dll");
	if (!mod)
		return false;
	BYTE *mod_engine=getModuleAddr(pid, L"engine.dll");
	if (!mod_engine)
		return false;

	h=OpenProcess(PROCESS_VM_READ, false, pid);
	if (!h)
		return false;

	// Check if we really have TF2 running
	/*
		position tuple:		client.dll+0x5753d8  (x,y,z, float)
		orientation tuple:	client.dll+0x4b691c  (v,h float)
		ID string:			client.dll+0x4eb30b = "teamJet@@" (9 characters, text)
		spawn state:        client.dll+0x49db9c  (0 when at main menu, 1 when spectator, 3 when at team selection menu, and 6 or 9 when on a team (depending on the team side and gamemode), byte)
		host string: 		engine.dll+0x3c8124  (ip:port zero-terminated string; localhost:27015 if create a server ingame)
			note that memory addresses in this comment are for example only; the real ones are defined below
	*/

	// Remember addresses for later
	posptr = mod + 0x5022D0;
	rotptr = mod + 0x502244;
	stateptr = mod + 0x4E7BCC;
	hostptr = mod_engine + 0x3C91A4;

	// Gamecheck
	char sMagic[9];
	if (!peekProc(mod + 0x4F83E3, sMagic, 9) || strncmp("teamJet@@", sMagic, 9)!=0)
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
	return std::wstring(L"Supports TF2 build 4057. No identity support yet.");
}

static std::wstring description(L"Team Fortress 2 (Build 4057)");
static std::wstring shortname(L"Team Fortress 2");

static MumblePlugin tf2plug = {
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
	return &tf2plug;
}
