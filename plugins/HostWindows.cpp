// Copyright 2020-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "HostWindows.h"

#include "mumble_positional_audio_utils.h"

#include <windows.h>
#include <tlhelp32.h>

HostWindows::HostWindows(const procid_t pid) : m_pid(pid) {
	m_handle = OpenProcess(PROCESS_VM_READ, false, m_pid);
}

HostWindows::~HostWindows() {
	if (m_handle) {
		CloseHandle(m_handle);
	}
}

bool HostWindows::peek(const procptr_t address, void *dst, const size_t size) const {
	SIZE_T read;
	const auto ok = ReadProcessMemory(m_handle, reinterpret_cast< void * >(address), dst, size, &read);
	return (ok && read == size);
}

Modules HostWindows::modules() const {
	const auto processHandle = OpenProcess(PROCESS_QUERY_INFORMATION, false, m_pid);
	if (!processHandle) {
		return {};
	}

	const auto snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, m_pid);
	if (snapshotHandle == INVALID_HANDLE_VALUE) {
		return {};
	}

	Modules modules;

	MODULEENTRY32 me;
	me.dwSize = sizeof(me);
	for (auto ok = Module32First(snapshotHandle, &me); ok; ok = Module32Next(snapshotHandle, &me)) {
		const auto name = utf16ToUtf8(reinterpret_cast< char16_t * >(me.szModule));
		if (modules.find(name) != modules.cend()) {
			continue;
		}

		Module module(name);
		MEMORY_BASIC_INFORMATION64 mbi;
		auto address = reinterpret_cast< procptr_t >(me.modBaseAddr);
		while (VirtualQueryEx(processHandle, reinterpret_cast< LPCVOID >(address),
							  reinterpret_cast< PMEMORY_BASIC_INFORMATION >(&mbi), sizeof(mbi))) {
			MemoryRegion region{};
			region.address = address;
			region.size    = mbi.RegionSize;
			switch (mbi.Protect) {
				case PAGE_READWRITE:
					region.writable = true;
				case PAGE_READONLY:
				case PAGE_WRITECOPY:
					region.readable = true;
					break;
				case PAGE_EXECUTE_READWRITE:
					region.writable = true;
				case PAGE_EXECUTE_READ:
				case PAGE_EXECUTE_WRITECOPY:
					region.readable = true;
				case PAGE_EXECUTE:
					region.executable = true;
					break;
			}
			module.addRegion(region);

			address += region.size;
		}

		modules.insert(std::make_pair(name, module));
	}

	CloseHandle(processHandle);
	CloseHandle(snapshotHandle);

	return modules;
}
