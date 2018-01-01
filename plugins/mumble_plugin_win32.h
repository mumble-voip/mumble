// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PLUGIN_WIN32_H_
#define MUMBLE_MUMBLE_PLUGIN_WIN32_H_

#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
// Define "NOMINMAX" only if it isn't already.
// MinGW defines it by default, which results in a redefinition warning.
#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <windows.h>
#include <tlhelp32.h>
#include <math.h>
#include <sstream>
#include <iostream>
#include <stdint.h>

#include "mumble_plugin.h"

DWORD dwPid;
int is64Bit;
static HANDLE hProcess;
static procptr_t pModule;

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

static inline int checkProcessIs64Bit(const DWORD pid)
{
	// This function returns 0 if the process is 32-bit and 1 if it's 64-bit.
	// In case of failure, it returns -1.

	HANDLE handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, pid);
	if (!handle) {
		return -1;
	}

	// IsWow64Process() returns true if the process is 32-bit, running on a 64-bit system.
	// It returns false in case the process is 64-bit, running on a 64-bit system.
	// It also returns false if the process is 32-bit, running on a 32-bit system.

	BOOL isWow64Process;
	if (!IsWow64Process(handle, &isWow64Process)) {
		CloseHandle(handle);
		return -1;
	}
	
	CloseHandle(handle);

	bool is32 = isWow64Process || sizeof(void*) == 4;
	return !is32;
}

static inline procptr_t getModuleAddr(DWORD pid, const wchar_t *modname) {
	MODULEENTRY32 me;
	procptr_t ret = 0;
	me.dwSize = sizeof(me);
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE|TH32CS_SNAPMODULE32, pid);
	if (hSnap != INVALID_HANDLE_VALUE) {
		BOOL ok = Module32First(hSnap, &me);

		while (ok) {
			if (wcscmp(me.szModule, modname)==0) {
				uintptr_t addr = reinterpret_cast<uintptr_t>(me.modBaseAddr);
				ret = static_cast<procptr_t>(addr);
				break;
			}
			ok = Module32Next(hSnap, &me);
		}
		CloseHandle(hSnap);
	}
	return ret;
}

static inline procptr_t getModuleAddr(const wchar_t *modname) {
	return getModuleAddr(dwPid, modname);
}

static inline bool peekProc(procptr_t base, VOID *dest, SIZE_T len) {
	SIZE_T r;
	uintptr_t addr = static_cast<uintptr_t>(base);
	BOOL ok=ReadProcessMemory(hProcess, reinterpret_cast<VOID *>(addr), dest, len, &r);
	return (ok && (r == len));
}

template<class T>
bool peekProc(procptr_t base, T &dest) {
	SIZE_T r;
	uintptr_t addr = static_cast<uintptr_t>(base);
	BOOL ok=ReadProcessMemory(hProcess, reinterpret_cast<VOID *>(addr), reinterpret_cast<VOID *>(& dest), sizeof(T), &r);
	return (ok && (r == sizeof(T)));
}

static inline procptr_t peekProcPtr(procptr_t base) {
	procptr_t v = 0;
	
	if (!peekProc(base, &v, is64Bit ? 8 : 4)) {
		return 0;
	}
	
	return v;
}

static bool inline initialize(const std::multimap<std::wstring, unsigned long long int> &pids, const wchar_t *procname, const wchar_t *modname = NULL) {
	hProcess = NULL;
	pModule = 0;

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

	int result = checkProcessIs64Bit(dwPid);
	if (result == -1) {
		dwPid = 0;
		return false;
	}

	is64Bit = result;

	pModule=getModuleAddr(modname ? modname : procname);
	if (!pModule) {
		dwPid = 0;
		return false;
	}

	hProcess=OpenProcess(PROCESS_VM_READ, false, dwPid);
	if (!hProcess) {
		dwPid = 0;
		pModule = 0;
		return false;
	}

	return true;
}

static void generic_unlock() {
	if (hProcess) {
		CloseHandle(hProcess);
		hProcess = NULL;
		pModule = 0;
		dwPid = 0;
	}
}

#endif
