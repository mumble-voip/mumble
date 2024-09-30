// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "lib.h"

#include <tlhelp32.h>

#include <string>
#include <vector>

/// Find the PROCESSENTRY32 entry for the parent of the process with the |childpid| PID.
///
/// Returns true on success, and fills out |parent| with the correct PROCESSENTRY32.
/// Returns false on failure, and does not touch |parent|.
static bool findParentProcessForChild(DWORD childpid, PROCESSENTRY32 *parent) {
	DWORD parentpid = 0;
	HANDLE hSnap    = nullptr;
	bool done       = false;

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);

	MODULEENTRY32 me;
	me.dwSize = sizeof(me);

	// Find our parent's process ID.
	{
		BOOL ok = FALSE;

		hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnap == INVALID_HANDLE_VALUE) {
			return false;
		}
		ok = Process32First(hSnap, &pe);
		while (ok) {
			if (pe.th32ProcessID == childpid) {
				parentpid = pe.th32ParentProcessID;
				break;
			}
			ok = Process32Next(hSnap, &pe);
		}
		CloseHandle(hSnap);
	}

	if (parentpid == 0) {
		return false;
	}

	// Find our parent's name.
	{
		BOOL ok = FALSE;

		hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnap == INVALID_HANDLE_VALUE) {
			return false;
		}
		ok = Process32First(hSnap, &pe);
		while (ok) {
			if (pe.th32ProcessID == parentpid) {
				memcpy(parent, &pe, sizeof(pe));
				ok   = FALSE;
				done = true;
				break;
			}
			ok = Process32Next(hSnap, &pe);
		}
		CloseHandle(hSnap);
	}

	return done;
}

/// Find the MODULEENTRY32 for the PROCESSENTRY32 |parent|.
/// The MODULEENTRY32 allows us to access the absolute path of the parent executable.
///
/// Returns true on success, and fills out |module| with the proper MODULEENTRY32 entry.
/// Returns false on failure, and does not touch |module|.
static bool getModuleForParent(PROCESSENTRY32 *parent, MODULEENTRY32 *module) {
	HANDLE hSnap = nullptr;
	bool done    = false;

	MODULEENTRY32 me;
	me.dwSize = sizeof(me);

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, parent->th32ProcessID);
	if (hSnap == INVALID_HANDLE_VALUE) {
		goto out;
	}

	if (!Module32First(hSnap, &me)) {
		goto out;
	}

	memcpy(module, &me, sizeof(me));
	done = true;

out:
	CloseHandle(hSnap);
	return done;
}

bool GetProcessAncestorChain(std::vector< std::string > &absAncestorExeNames,
							 std::vector< std::string > &ancestorExeNames) {
	PROCESSENTRY32 parent;
	MODULEENTRY32 module;

	std::vector< std::string > abs;
	std::vector< std::string > rel;

	bool ok        = true;
	DWORD childpid = GetCurrentProcessId();
	while (ok) {
		ok = findParentProcessForChild(childpid, &parent);
		if (ok) {
			ok = getModuleForParent(&parent, &module);
			if (ok) {
				abs.push_back(std::string(module.szExePath));
				rel.push_back(std::string(parent.szExeFile));
				childpid = parent.th32ProcessID;
			}
		}
	}

	ok = abs.size() > 0;
	if (ok) {
		absAncestorExeNames = abs;
		ancestorExeNames    = rel;
	}

	return ok;
}
