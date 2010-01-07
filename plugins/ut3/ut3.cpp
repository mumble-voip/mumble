/* Copyright (C) 2009-2010, Snares <snares@users.sourceforge.net>
   Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

//#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>
#include <math.h>
//#include <string>
//#include <sstream>

#include "../mumble_plugin.h"

HANDLE h = NULL;

using namespace std;

BYTE *pos0ptr;
BYTE *pos1ptr;
BYTE *pos2ptr;
BYTE *faceptr;
BYTE *top0ptr;
BYTE *top1ptr;
BYTE *top2ptr;
//BYTE *stateptr;

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
	::MessageBox(h, L"Reads audio position information from Unreal Tournament 3 (v2.1).", L"Mumble UT3 plugin", MB_OK);
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	char state;
	//char ccontext[128];
	bool ok;

	float face_corrector[3];
	float top0_corrector;
	float top1_corrector;
	float top2_corrector;

	for (int i=0;i<3;i++)
		avatar_pos[i]=avatar_front[i]=avatar_top[i]=0.0f;

	ok = peekProc((BYTE *) 0x01DEAFD9, &state, 1);
	if (! ok)
		return false;
		
	if (state == 1)
		return true;

	/*
	   Z-Value is increasing when heading north
				  decreasing when heading south
	   X-Value is increasing when heading east
				  decreasing when heading west
	   Y-Value is increasing when going up
				  decreasing when going down
	*/

	//Convert to left-handed coordinate system

	ok = peekProc(pos2ptr, avatar_pos, 4) &&	//X
		 peekProc(pos1ptr, avatar_pos+1, 4) &&	//Y
		 peekProc(pos0ptr, avatar_pos+2, 4) &&  //Z
		 peekProc(faceptr, &face_corrector, 12) &&
		 peekProc(top0ptr, &top0_corrector, 4) &&
		 peekProc(top1ptr, &top1_corrector, 4) &&
		 peekProc(top2ptr, &top2_corrector, 4);
		 //peekProc((BYTE *) 0x0122E0B8, ccontext, 128);

	if (! ok)
		return false;
	
	//Find north by playing on a Warfare game type - center view on the up arrow on the mini map
	avatar_front[0] = face_corrector[2]; 
	avatar_front[1] = face_corrector[1]; 
	avatar_front[2] = face_corrector[0];
	
	avatar_top[0] = top2_corrector;
	avatar_top[1] = top1_corrector;
	avatar_top[2] = top0_corrector;

	//ccontext[127] = 0;
	//context = std::string(ccontext);
	
	//if (context.find(':')==string::npos) 
	//	context.append(":UT3PORT");

	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	return ok;
}

static int trylock() {
	h = NULL;
	pos0ptr = pos1ptr = pos2ptr = faceptr = top0ptr = top1ptr = top2ptr = NULL;
	
	DWORD pid=getProcess(L"UT3.exe");
	if (!pid)
		return false;
	BYTE *mod=getModuleAddr(pid, L"wrap_oal.dll");
	if (!mod)
		return false;

	h=OpenProcess(PROCESS_VM_READ, false, pid);
	if (!h)
		return false;

	BYTE *ptraddress = mod + 0x8A740;
	BYTE *baseptr = peekProcPtr(ptraddress);

	pos0ptr = baseptr;
	pos1ptr = baseptr + 0x4;
	pos2ptr = baseptr + 0x8;
	faceptr = baseptr + 0x18;
	top0ptr = baseptr + 0x24;
	top2ptr = baseptr + 0x2C;

	BYTE *ptraddress2 = peekProcPtr((BYTE *) 0x01DD1FF4);
	BYTE *ptr1 = peekProcPtr(ptraddress2 + 0x230);

	top1ptr = ptr1 + 0xA8;

    //stateptr = mod + 0xC4;

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
	return std::wstring(L"Supports Unreal Tournament 3 (v2.1). No context or identity support yet.");
}

static std::wstring description(L"Unreal Tournament 3 (v2.1)");
static std::wstring shortname(L"Unreal Tournament 3");

static MumblePlugin ut3plug = {
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
	return &ut3plug;
}
