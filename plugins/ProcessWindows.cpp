// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ProcessWindows.h"

#include "mumble_positional_audio_win32_internals.h"

ProcessWindows::ProcessWindows(const procid_t id, const std::string &name) : ProcessBase(id, name) {
	const auto mods = modules();
	const auto iter = mods.find(name);
	if (iter == mods.cend()) {
		return;
	}

	const auto address = iter->second.baseAddress();
	if (!address) {
		return;
	}

	const auto dos = peek< ImageDosHeader >(address);
	if (!(dos.magic[0] == 'M' && dos.magic[1] == 'Z')) {
		return;
	}

	const auto nt = peek< ImageNtHeadersNoOptional >(address + dos.addressOfNtHeader);
	if (!(nt.signature[0] == 'P' && nt.signature[1] == 'E' && nt.signature[2] == '\0' && nt.signature[3] == '\0')) {
		return;
	}

	switch (nt.fileHeader.machine) {
		case 0x14c: // IMAGE_FILE_MACHINE_I386
			m_pointerSize = 4;
			break;
		default:
			m_pointerSize = 8;
	}

	m_ok = true;
}

ProcessWindows::~ProcessWindows() {
}

template< typename ImageNtHeaders >
static procptr_t exportedSymbol(const ProcessBase &proc, const std::string &symbol, const procptr_t module) {
	const auto dos = proc.peek< ImageDosHeader >(module);
	if (!(dos.magic[0] == 'M' && dos.magic[1] == 'Z')) {
		return 0;
	}

	const auto nt = proc.peek< ImageNtHeaders >(module + dos.addressOfNtHeader);
	if (!(nt.signature[0] == 'P' && nt.signature[1] == 'E' && nt.signature[2] == '\0' && nt.signature[3] == '\0')) {
		return 0;
	}

	const auto dataAddress = nt.optionalHeader.dataDirectory[0].virtualAddress;
	if (!dataAddress) {
		return 0;
	}

	const auto exportDir = proc.peek< ImageExportDirectory >(module + dataAddress);

	const auto funcs = proc.peekVector< uint32_t >(module + exportDir.addressOfFunctions, exportDir.numberOfFunctions);
	const auto names = proc.peekVector< uint32_t >(module + exportDir.addressOfNames, exportDir.numberOfNames);
	const auto ords  = proc.peekVector< uint16_t >(module + exportDir.addressOfNameOrdinals, exportDir.numberOfNames);

	for (uint32_t i = 0; i < exportDir.numberOfNames; ++i) {
		if (names[i]) {
			const auto name = proc.peekString(module + names[i], symbol.size());

			if (name == symbol) {
				return module + funcs[ords[i]];
			}
		}
	}

	return 0;
}

procptr_t ProcessWindows::exportedSymbol(const std::string &symbol, const procptr_t module) const {
	if (m_pointerSize > 4) {
		return ::exportedSymbol< ImageNtHeaders64 >(*this, symbol, module);
	} else {
		return ::exportedSymbol< ImageNtHeaders32 >(*this, symbol, module);
	}
}
