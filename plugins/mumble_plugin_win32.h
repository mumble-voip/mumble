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

#ifndef _MUMBLE_PLUGIN_WIN32_H
#define _MUMBLE_PLUGIN_WIN32_H

#include "mumble_plugin.h"

static HANDLE hProcess;

static inline DWORD getProcess(const wchar_t *exename) {
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

static inline BYTE *getModuleAddr(DWORD pid, const wchar_t *modname) {
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

static inline bool peekProc(VOID *base, VOID *dest, SIZE_T len) {
	SIZE_T r;
	BOOL ok=ReadProcessMemory(hProcess, base, dest, len, &r);
	return (ok && (r == len));
}

template<class T>
bool peekProc(VOID *base, T &dest) {
	SIZE_T r;
	BOOL ok=ReadProcessMemory(hProcess, base, reinterpret_cast<VOID *>(& dest), sizeof(T), &r);
	return (ok && (r == sizeof(T)));
}

template<class T>
T peekProc(VOID *base) {
	T v = 0;
	peekProc(base, reinterpret_cast<T *>(&v), sizeof(T));
	return v;
}

static void u8(std::wstring &dst, const std::string &src) {
	int len = MultiByteToWideChar(CP_UTF8, 0, src.data(), src.length(), NULL, 0);
	
	wchar_t *wbuff = reinterpret_cast<wchar_t *>(_alloca(sizeof(wchar_t) * len));
	MultiByteToWideChar(CP_UTF8, 0, src.data(), src.length(), wbuff, len);
	
	dst.assign(wbuff, len);
}

static void u8(std::wstring &dst, const char *src, int srclen) {
	int len = MultiByteToWideChar(CP_UTF8, 0, src, srclen, NULL, 0);
	
	wchar_t *wbuff = reinterpret_cast<wchar_t *>(_alloca(sizeof(wchar_t) * len));
	MultiByteToWideChar(CP_UTF8, 0, src, srclen, wbuff, len);
	
	dst.assign(wbuff, len);
}

#endif
