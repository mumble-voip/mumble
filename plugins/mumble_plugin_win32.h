// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PLUGIN_WIN32_H_
#define MUMBLE_MUMBLE_PLUGIN_WIN32_H_

#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#define NOMINMAX
#include <windows.h>
#include <tlhelp32.h>
#include <math.h>
#include <sstream>
#include <iostream>

#include "mumble_plugin.h"

DWORD dwPid;
static HANDLE hProcess;
static BYTE *pModule;

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

static inline BYTE *getModuleAddr(const wchar_t *modname) {
	return getModuleAddr(dwPid, modname);
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

static bool inline initialize(const std::multimap<std::wstring, unsigned long long int> &pids, const wchar_t *procname, const wchar_t *modname = NULL) {
	hProcess = NULL;
	pModule = NULL;

	if (! pids.empty()) {
		std::multimap<std::wstring, unsigned long long int>::const_iterator iter = pids.find(std::wstring(procname));

		if (iter != pids.end())
			dwPid = static_cast<DWORD>(iter->second);
		else
			dwPid = 0;
	} else {
		dwPid=getProcess(procname);
	}

	if (!dwPid)
		return false;

	pModule=getModuleAddr(modname ? modname : procname);
	if (!pModule) {
		dwPid = 0;
		return false;
	}

	hProcess=OpenProcess(PROCESS_VM_READ, false, dwPid);
	if (!hProcess) {
		dwPid = 0;
		pModule = NULL;
		return false;
	}

	return true;
}

static void generic_unlock() {
	if (hProcess) {
		CloseHandle(hProcess);
		hProcess = NULL;
		pModule = NULL;
		dwPid = 0;
	}
}

#endif
