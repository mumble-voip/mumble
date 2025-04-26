// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_POSITIONAL_AUDIO_WIN32_H_
#define MUMBLE_POSITIONAL_AUDIO_WIN32_H_

#ifndef MUMBLE_POSITIONAL_AUDIO_MAIN_H_
#	error "Include mumble_positional_audio_main.h instead of mumble_positional_audio_win32.h"
#endif

#include <stdio.h>
#include <stdlib.h>
// Define "NOMINMAX" only if it isn't already.
// MinGW defines it by default, which results in a redefinition warning.
#ifndef NOMINMAX
#	define NOMINMAX
#endif
#include <windows.h>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdint.h>
#include <tlhelp32.h>

static HANDLE hProcess;

static inline procid_t getProcess(const wchar_t *exename) {
	PROCESSENTRY32 pe;
	procid_t pid = 0;

	pe.dwSize    = sizeof(pe);
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE) {
		BOOL ok = Process32First(hSnap, &pe);

		while (ok) {
			if (wcscmp(pe.szExeFile, exename) == 0) {
				pid = pe.th32ProcessID;
				break;
			}
			ok = Process32Next(hSnap, &pe);
		}
		CloseHandle(hSnap);
	}
	return pid;
}

static inline procptr_t getModuleAddr(const procid_t &pid, const wchar_t *modname) {
	MODULEENTRY32 me;
	procptr_t ret = 0;
	me.dwSize     = sizeof(me);
	HANDLE hSnap  = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (hSnap != INVALID_HANDLE_VALUE) {
		BOOL ok = Module32First(hSnap, &me);

		while (ok) {
			if (wcscmp(me.szModule, modname) == 0) {
				uintptr_t addr = reinterpret_cast< uintptr_t >(me.modBaseAddr);
				ret            = static_cast< procptr_t >(addr);
				break;
			}
			ok = Module32Next(hSnap, &me);
		}
		CloseHandle(hSnap);
	}
	return ret;
}

static inline bool peekProc(const procptr_t &addr, void *dest, const size_t &len) {
	SIZE_T r;
	const BOOL ok = ReadProcessMemory(hProcess, reinterpret_cast< void * >(addr), dest, len, &r);
	return (ok && (r == len));
}

static void generic_unlock() {
	if (hProcess) {
		CloseHandle(hProcess);
		hProcess = nullptr;
	}

	pPid    = 0;
	pModule = 0;
}

static bool initialize(const std::multimap< std::wstring, unsigned long long int > &pids, const wchar_t *procname,
					   const wchar_t *modname = nullptr) {
	hProcess = nullptr;
	pModule  = 0;

	if (!pids.empty()) {
		auto iter = pids.find(std::wstring(procname));

		if (iter != pids.end()) {
			pPid = static_cast< procid_t >(iter->second);
		} else {
			pPid = 0;
		}
	} else {
		pPid = getProcess(procname);
	}

	if (!pPid) {
		return false;
	}

	hProcess = OpenProcess(PROCESS_VM_READ, false, pPid);
	if (!hProcess) {
		pPid = 0;
		return false;
	}

	pModule = getModuleAddr(procname);
	if (!pModule) {
		generic_unlock();
		return false;
	}

	const int8_t result = isProcess64Bit(pModule);
	if (result == -1) {
		generic_unlock();
		return false;
	}

	if (modname) {
		pModule = getModuleAddr(modname);
		if (!pModule) {
			generic_unlock();
			return false;
		}
	}

	// We compare to 1 to prevent the following warning:
	// C4800: 'BOOL': forcing value to bool 'true' or 'false' (performance warning)
	is64Bit = (result == 1);

	return true;
}

#endif
