/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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
#include <math.h>

#include "../mumble_plugin.h"

HANDLE h;

BYTE *posptr;
BYTE *faceptr;
BYTE *topptr;

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
	::MessageBox(h, L"Reads audio position information from BF2 (v1.50). IP:Port context support.", L"Mumble BF2 Plugin", MB_OK);
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	char ccontext[128];
	char state;
	char logincheck;
	bool ok;

	ok = peekProc((BYTE *) 0x30058642, &logincheck, 1);
	if (! ok)
		return false;

	if (logincheck == 0)
		return false;


	/*
		state value is:
		0						while not in game
		usually 1, never 0		if you create your own server ingame; this value will switch to 1 the instant you click "Join Game"
		usually 3, never 0		if you load into a server; this value will switch to 3 the instant you click "Join Game"
	*/
	ok = peekProc((BYTE *) 0x00A1D0A8, &state, 1); // Magical state value
	if (! ok)
		return false;

	ok = peekProc(posptr, avatar_pos, 12) &&
	     peekProc(faceptr, avatar_front, 12) &&
	     peekProc(topptr, avatar_top, 12) &&
	     peekProc((BYTE *) 0x009A80B8, ccontext, 128);

	if (! ok)
		return false;

	/*
	    Get context string; in this plugin this will be an
	    ip:port (char 256 bytes) string
	*/
	ccontext[127] = 0;
	context = std::string(ccontext);

	if (state == 0)
		return true; // This results in all vectors beeing zero which tells Mumble to ignore them.

	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	return ok;
}

static int trylock() {
	h = NULL;
	posptr = faceptr = topptr = NULL;

	DWORD pid=getProcess(L"BF2.exe");
	if (!pid)
		return false;
	BYTE *mod=getModuleAddr(pid, L"BF2Audio.dll");
	if (!mod)
		return false;

	h=OpenProcess(PROCESS_VM_READ, false, pid);
	if (!h)
		return false;

	BYTE *cacheaddr = mod + 0x4645c;
	BYTE *cache = peekProcPtr(cacheaddr);

	posptr = peekProcPtr(cache + 0xb4);
	faceptr = peekProcPtr(cache + 0xb8);
	topptr = peekProcPtr(cache + 0xbc);

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
	return;
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Battlefield 2 v1.50. No identity support yet.");
}

static std::wstring description(L"Battlefield 2 v1.50");
static std::wstring shortname(L"Battlefield 2");

static MumblePlugin bf2plug = {
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
	return &bf2plug;
}
