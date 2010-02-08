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

BYTE *identptr;
BYTE *contextptr;
BYTE *posptr;

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
	::MessageBox(h, L"Reads audio position information from Star Trek Online", L"Mumble STO Plugin", MB_OK);
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
		char identblock[0x200];
		char contextblock[0x80];
		float posblock[64];

		if (! peekProc(identptr, identblock, sizeof(identblock)) ||
		    ! peekProc(contextptr, contextblock, sizeof(contextblock)) ||
		    ! peekProc(posptr, posblock, sizeof(posblock))) 
		    return false;
		    
		wchar_t widentbuff[0x200];
		std::string ident = std::string(identblock+0x188, strnlen(identblock + 0x188, 0x78)) + std::string("@") + std::string(identblock, strnlen(identblock, 0x80));
		int len = MultiByteToWideChar(CP_UTF8, 0, ident.c_str(), ident.length(), widentbuff, 0x200);
		identity.assign(widentbuff, len);
#ifdef PLUGIN_DEBUG
		printf("%ls\n", identity.c_str());
#endif

		context.assign(reinterpret_cast<char *>(contextblock + 0x30), 4);
#ifdef PLUGIN_DEBUG
		DWORD *ctx = reinterpret_cast<DWORD *>(contextblock + 0x30);
		printf("%08x\n", *ctx);
#endif
		
#ifdef PLUGIN_DEBUG
		printf("Avatar   %8.3f %8.3f %8.3f\n", posblock[0], posblock[1], posblock[2]);
		printf("AvatarF  %8.3f %8.3f %8.3f\n", posblock[15], posblock[16], posblock[17]);
		printf("Camera   %8.3f %8.3f %8.3f\n", posblock[3], posblock[4], posblock[5]);
		printf("CameraF  %8.3f %8.3f %8.3f\n", posblock[18], posblock[19], posblock[20]);
		printf("SpawnST  %d\n", * reinterpret_cast<DWORD *>(& posblock[52]));
#endif
		if (* reinterpret_cast<DWORD *>(& posblock[52]) == 1) {
			for(int i=0;i<3;++i) {
				avatar_pos[i] = posblock[i];
				avatar_front[i] = posblock[i+15];
				camera_pos[i] = posblock[i+3];
				camera_front[i] = posblock[i+18];
				
				avatar_top[i] = camera_top[i] = 0.0f;
			}
		} else {
			for (int i=0;i<3;++i) {
				avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;
			}
		}

	return true;
}

static int trylock() {
	h = NULL;
	identptr = contextptr = posptr = NULL;

	DWORD pid=getProcess(L"GameClient.exe");
	if (!pid)
		return false;

	BYTE *mod=getModuleAddr(pid, L"GameClient.exe");
	if (!mod)
		return false;

	h=OpenProcess(PROCESS_VM_READ, false, pid);
	if (!h)
		return false;

	BYTE *versionptr = mod + 0x15E8778;
	char version[17];
	version[16]=0;
	
	peekProc(versionptr, version, 16);
	
	
	if (memcmp(version, "ST.0.20100202a.7", 16) == 0) {
#ifdef PLUGIN_DEBUG
		printf("STO: WANTLINK %s\n", version);
#endif
		identptr = mod + 0x1675E80;
		contextptr = mod + 0x1675a30;
		posptr = mod + 0x1885da0;
		
		return true;
	}


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
	return std::wstring(L"Supports Star Trek Online");
}

static std::wstring description(L"Star Trek Online ST.0.20100202a.7");
static std::wstring shortname(L"Star Trek Online");

static MumblePlugin stoplug = {
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
	return &stoplug;
}
