/*
 Copyright (c) 2009-2010 Ilmar 'Ingaras' Kruis (seaeagle1@users.sourceforge.net)

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

#if _DEBUG
#include <iostream>
#endif

#include <QtGui/QMessageBox>
#include "../mumble_plugin.h"

HANDLE h;

BYTE *lotroclient;

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

static const std::wstring longdesc() {
	return std::wstring(L"Supports Lord of the Rings Online (Codemasters Edition, Vol II Book 9). Context support based on region and instance. No Identity support.");
}

static std::wstring description(L"Lord of the Rings Online (EU), Vol II Book 9");
static std::wstring shortname(L"Lord of the Rings Online");

static void about(HWND h) {
	QMessageBox msg;
	msg.setWindowTitle(QString::fromStdWString(description));
	msg.setText(QString::fromStdWString(longdesc()));
	msg.exec();
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++)
		avatar_pos[i]=avatar_front[i]=avatar_top[i]=0.0f;

	bool ok;

	byte l[2];
	byte r,i;
	float o[3];
	float h;

	/*
		Position as represented by /loc command

		lx, ly = 8 * (byte)l;
		ox, oy, oz = (float)o;

		0 < ox < 160

		x = north
		y = west
		z = altitude

		r = region
		i = instance nr
	*/

	ok = peekProc((BYTE *) 0x0109F8F0, o, 12) &&
	     peekProc((BYTE *) 0x0109F8E8, l, 2) &&
	     peekProc((BYTE *) 0x0109F8E4, &r, 1) &&
	     peekProc((BYTE *) 0x0109F8EC, &i, 1) &&
	     peekProc((BYTE *)(lotroclient + 0x00D85EC0 + 0x00000034), &h, 4);

	if (! ok)
		return false;

	// Use region as context since each region has its own coordinate system
	if (r == 1)
		context = "Eriador";
	else if (r == 2)
		context = "Rhovannion";
	else
		return true;

	// If we're in an instance, append the instance id
	if (i != 0)
		context += i;

	// Heading should be between 0 and 360
	if (h < 0 || h > 360)
		return true;

	// Limit coordinates to byte-values, otherwise we probably have a read error
	if (l[0] == 255 && l[1] == 255)
		return true;

	avatar_pos[0] = -((float)l[1] * 160.0 + o[1]);
	avatar_pos[1] = o[2];
	avatar_pos[2] = (float)l[0] * 160.0 + o[0];

	avatar_front[0] = sin(h * 3.1415926 / 180.0);
	avatar_front[1] = 0.0f;
	avatar_front[2] = cos(h * 3.1415926 / 180.0);

	avatar_top[0] = 0.0;
	avatar_top[1] = 1.0;
	avatar_top[2] = 0.0;

	// TODO: 3rd person camera support
	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	//printf("P %f %f %f -- %f %f %f \n", avatar_pos[0], avatar_pos[1], avatar_pos[2], avatar_front[0], avatar_front[1], avatar_front[2]);

	return ok;
}

static int trylock() {

	lotroclient = NULL;
	h = NULL;

	DWORD pid=getProcess(L"lotroclient.exe");
	if (!pid)
		return false;

	lotroclient = getModuleAddr(pid, L"lotroclient.exe");
	if (!lotroclient)
		return false;

	h=OpenProcess(PROCESS_VM_READ, false, pid);
	if (!h) {
#ifdef _DEBUG
		DWORD dwError;
		wchar_t errBuf[256];

		dwError = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)errBuf, sizeof(errBuf),NULL);
		std::cout << "Error in OpenProcess: ";
		std::wcout << errBuf << std::endl;
#endif
		return false;
	}

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

static MumblePlugin lotroplug = {
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
	return &lotroplug;
}
