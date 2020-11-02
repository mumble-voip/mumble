// Copyright 2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "HostWindows.h"

#include "mumble_plugin_utils.h"

#include <windows.h>
#include <tlhelp32.h>

HostWindows::HostWindows(const procid_t pid) : m_ok(true), m_pid(pid) {
}

HostWindows::~HostWindows() {
}

bool HostWindows::peek(const procptr_t address, void *dst, const size_t size) const {
	SIZE_T read;
	const auto ok = Toolhelp32ReadProcessMemory(m_pid, reinterpret_cast< void * >(address), dst, size, &read);
	return (ok && read == size);
}

procptr_t HostWindows::module(const std::string &module) const {
	const auto handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, m_pid);
	if (handle == INVALID_HANDLE_VALUE) {
		return 0;
	}

	procptr_t ret = 0;

	MODULEENTRY32 me;
	me.dwSize = sizeof(me);
	for (BOOL ok = Module32First(handle, &me); ok; ok = Module32Next(handle, &me)) {
		if (me.szModule == utf8ToUtf16(module)) {
			ret = reinterpret_cast< procptr_t >(me.modBaseAddr);
			break;
		}
	}

	CloseHandle(handle);

	return ret;
}
