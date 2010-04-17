/* Copyright (C) 2009-2010, Imre Botos <zokny@users.sourceforge.net>
   Copyright (C) 2009-2010, Snares <snares@users.sourceforge.net>
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

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <sstream>

#define _USE_MATH_DEFINES
#include <math.h>

#include "mumble_plugin.h"

using namespace std;

HANDLE h;
BYTE *pClient;
BYTE *pEngine;

/*	DESCRIPTION		ADDRESS			TYPE	VALUE

	* Client based addresses
	Connected to server:	client.dll+0xFC228	byte	(0 - not; 1 - connected)
	Position tuple:		client.dll+0x11D470	float	(x, y, z in inches)
	Orientation tuple:	client.dll+0x11D47C	float	(v, h in degrees)
	Check string:		client.dll+0xE46F5	char	("CSSpectatorGUI@@" | 16 bytes)
	Player state:		client.dll+0xFBC2C	byte	(0 - died; 1 - stand; 2 - duck; 3 - walk; 4 - run; 5 - crouchmove; 6 - fall)
	Team state:		client.dll+0xFC94C	byte	(0 - Spectator; 1 - Terrorist; 2 - Counter-Terrorist)
	ALT+TAB:		client.dll+0xFD05C	byte	(0 - tabbed; 1 - ingame)
	Map string:		client.dll+0x11DCE4	str	("maps/de_nuke.bsp")

	* Engine based addresses
	IP:Port string		hw.dll+0x697E60		str	(ip:port)
	ALT+TAB:		hw.dll+0x7486B4		byte	(0 - tabbed; 1 - ingame)
*/

void __cdecl ods(const char *format, ...) {
	char    buf[4096], *p = buf;
	va_list args;

	va_start(args, format);
	int len = _vsnprintf_s(p, sizeof(buf) - 1, _TRUNCATE, format, args);
	va_end(args);

	if (len <= 0)
		return;

	p += len;

	while (p > buf  &&  isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p   = '\0';

	OutputDebugStringA(buf);
}

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
	::MessageBox(h, L"Reads audio position information from Counter-Strike 1.6. IP:Port context support.", L"Mumble CS Plugin", MB_OK);
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	bool	ok;
	bool	bConnected;
	char	cPlayerState;
	char	cHostAddr[40];
	float	fViewHor, fViewVer;

	/*	Dust2
		INC		DEC		|	Ver			Hor	
	X - hosszú fele		B fele		|	0	közép		0/360	hosszú fele
	Y - CT kezdõ fele	T kezdõ fele	|	-89	felnéz		90	CT kezdõ fele
	Z - felfele		lefele		|	89	lenéz		180	B fele
						|				270	T kezdõ fele
	*/

	ok = 	peekProc((BYTE*) pClient + 0x11D470, avatar_pos, 12) &&
		peekProc((BYTE*) pClient + 0x11D47C, &fViewHor, 4) &&
		peekProc((BYTE*) pClient + 0x11D480, &fViewVer, 4) &&
		peekProc((BYTE*) pClient + 0xFC228, &bConnected, 1) &&
		peekProc((BYTE*) pClient + 0xFBC2C, &cPlayerState, 1) &&
		peekProc((BYTE*) pEngine + 0x697E60, cHostAddr, 40);
	if(!ok){
		//ods("MUMBLECSPLUGIN Error: peekProc failed\n");
		return false;}

/*	// FOR TEST ONLY - constant position (d2 CT middle, looking T spawn through doorgap)
	avatar_pos[0] = -380.359375f;
	avatar_pos[1] = 2188.1328125f;
	avatar_pos[2] = -91.96875f;
	fViewHor = 269,703369140625;
	fViewVer = -4,45587635040283;
*/
	if(!bConnected){
		//ods("MUMBLECSPLUGIN: Not connected!\n");
		return true;}

	context = cHostAddr;

	if(!cPlayerState){
		//ods("MUMBLECSPLUGIN: Player dead!\n");
		return true;}

	//ods("MUMBLECSPLUGIN Active: %d, %s, %d", bConnected, cHostAddr, cPlayerState);

	for (int i=0;i<3;i++)
		avatar_pos[i] /= 39.37f;

	fViewHor *= static_cast<float>(M_PI / 180.0f);
	fViewVer *= static_cast<float>(M_PI / 180.0f);

	avatar_front[0] = cos(fViewVer) * cos(fViewHor);
	avatar_front[1] = -sin(fViewHor);
	avatar_front[2] = sin(fViewVer) * cos(fViewHor);

	fViewHor -= static_cast<float>(M_PI / 2.0f);
	avatar_top[0] = cos(fViewVer) * cos(fViewHor);
	avatar_top[1] = -sin(fViewHor);
	avatar_top[2] = sin(fViewVer) * cos(fViewHor);

	for (int i=0;i<3;++i) {
		camera_pos[i]	= avatar_pos[i];
		camera_front[i]	= avatar_front[i];
		camera_top[i]	= avatar_top[i];
	}

	return true;
}

static int trylock() {

	h = NULL;

	DWORD pid = getProcess(L"hl.exe");
	if (!pid){
		//ods("MUMBLECSPLUGIN: GetProcess(\"hl.exe\") failed!\n");
		return false;}

	pClient = getModuleAddr(pid, L"client.dll");
	if (!pClient){
		//ods("MUMBLECSPLUGIN: GetModule(\"client.dll\") failed!\n");
		return false;}
	pEngine = getModuleAddr(pid, L"hw.dll");
	if (!pEngine){
		//ods("MUMBLECSPLUGIN: GetModule(\"hw.dll\") failed!\n");
		return false;}

	h = OpenProcess(PROCESS_VM_READ, false, pid);
	if (!h){
		//ods("MUMBLECSPLUGIN: OpenProccess() failed!\n");
		return false;}

	float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
	std::string context;
	std::wstring identity;

	if (fetch(apos, afront, atop, cpos, cfront, ctop, context, identity))
		return true;

	//ods("MUMBLECSPLUGIN: fetch() failed!\n");
	
	CloseHandle(h);
	h = NULL;
	return false;
}

static void unlock() {
	//ods("MUMBLECSPLUGIN: unlock()\n");
	if (h) {
		CloseHandle(h);
		h = NULL;
	}
	return;
}


static const std::wstring longdesc() {
	return std::wstring(L"Supports Counter-Strike 1.6 (Steam). Context support based on server address. No identity support.");
}

static std::wstring description(L"Counter-Strike 1.6");
static std::wstring shortname(L"Counter-Strike");

static MumblePlugin csplug = {
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
	return &csplug;
}
