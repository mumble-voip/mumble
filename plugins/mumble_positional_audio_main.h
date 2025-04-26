// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// This is the main header for process/OS stuff; it's included in most plugins
// because values can rarely be retrieved without accessing the process' memory.
//
// Various functions are provided for common tasks when reading from memory.
// The most important are getModuleAddr() and peekProc(); the first returns the
// base address of a module (e.g. shared library), the latter reads memory at the
// specified address and stores it in a variable.

#ifndef MUMBLE_POSITIONAL_AUDIO_MAIN_H_
#define MUMBLE_POSITIONAL_AUDIO_MAIN_H_

#if !defined(OS_WINDOWS) && !defined(OS_LINUX)
#	error "Please define either OS_WINDOWS or OS_LINUX"
#endif

#include "mumble_positional_audio_win32_internals.h"

#include <chrono>
#include <cstring>
#include <vector>

#ifdef OS_WINDOWS
static const bool isWin32 = true;
#else
static bool isWin32;
#endif

static bool is64Bit;
static procid_t pPid;
static procptr_t pModule;

#define GET_POINTER_SIZE (is64Bit ? 8 : 4)

static inline bool peekProc(const procptr_t &addr, void *dest, const size_t &len);
static inline procptr_t getModuleAddr(const procid_t &pid, const wchar_t *modname);

static inline procptr_t getModuleAddr(const wchar_t *modname) {
	return getModuleAddr(pPid, modname);
}

template< class T > static inline bool peekProc(const procptr_t &addr, T &dest) {
	return peekProc(addr, &dest, sizeof(T));
}

template< class T > static inline T peekProc(const procptr_t &addr) {
	T ret;

	if (!peekProc(addr, &ret, sizeof(T))) {
		memset(&ret, 0, sizeof(ret));
	}

	return ret;
}

template< typename T > static inline std::vector< T > peekProcVector(const procptr_t addr, const size_t elements) {
	try {
		std::vector< T > var(elements);
		peekProc(addr, &var[0], sizeof(T) * elements);
		return var;
	} catch (std::bad_alloc &) {
		return std::vector< T >();
	}
}

/// This function is very useful when there is an array of structures and we only need to store a part of each.
/// The Source Engine plugin needs it because there is a structure that happens to be larger in the Linux version of
/// Left 4 Dead 2. Since the extra members are unknown, we simply discard the extra bytes when reading the array of
/// structures.
template< typename T >
static inline std::vector< T > peekProcVector(const procptr_t addr, const size_t elements,
											  const size_t realStructSize) {
	if (realStructSize == sizeof(T)) {
		// If the structure's size is correct, there is no need for this special function.
		return peekProcVector< T >(addr, elements);
	} else if (realStructSize < sizeof(T)) {
		return std::vector< T >();
	}

	// Read the full structures into a vector.
	const std::vector< uint8_t > fullStructs = peekProcVector< uint8_t >(addr, realStructSize * elements);

	// Allocate vector for the known structures.
	std::vector< T > ret(elements);

	// Copy elements from the vector containing the full structures, discarding extra bytes.
	const uint8_t *seek = &fullStructs[0];

	for (size_t i = 0; i < elements; ++i, seek += realStructSize) {
		memcpy(&ret[i], seek, sizeof(T));
	}

	return ret;
}

/// Reads the specified amount of data at the specified address and returns it as std::string.
/// An empty std::string is returned in case of error.
///
/// If \p length is 0, the function reads one byte at a time and stops when either '\0' is found or 3 seconds have
/// passed. The successfully read data is returned, also in case of error.
static inline std::string peekProcString(const procptr_t addr, const size_t length = 0) {
	std::string string;

	if (length > 0) {
		string.resize(length);

		if (!peekProc(addr, &string[0], length)) {
			return std::string();
		}
	} else {
		auto now       = std::chrono::steady_clock::now();
		const auto end = now + std::chrono::seconds(3);

		for (procptr_t i = 0; now < end; ++i) {
			char ch = 0;
			if (!peekProc(addr + i, &ch, sizeof(ch)) || ch == '\0') {
				break;
			}

			string += ch;

			// Update current time.
			now = std::chrono::steady_clock::now();
		}
	}

	return string;
}

static inline procptr_t peekProcPtr(const procptr_t &addr) {
	procptr_t v = 0;

	if (!peekProc(addr, &v, GET_POINTER_SIZE)) {
		return 0;
	}

	return v;
}

static inline procptr_t getVirtualFunction(const procptr_t classObject, const size_t index) {
	const auto vTable = peekProcPtr(classObject);
	if (!vTable) {
		return 0;
	}

	return peekProcPtr(vTable + (index * GET_POINTER_SIZE));
}

#ifdef OS_WINDOWS
static inline procptr_t getExportedSymbol(const std::string &symbol, const procptr_t module) {
#else
/// We use a different name because the function is called by the Linux version
/// of getExportedSymbol() in case the process is running through Wine.
static inline procptr_t getWin32ExportedSymbol(const std::string &symbol, const procptr_t module) {
#endif
	const auto dos = peekProc< ImageDosHeader >(module);
	if (!(dos.magic[0] == 'M' && dos.magic[1] == 'Z')) {
		// Invalid DOS signature
		return -1;
	}

	procptr_t dataAddress;

	if (is64Bit) {
		const auto nt = peekProc< ImageNtHeaders64 >(module + dos.addressOfNtHeader);
		dataAddress   = nt.optionalHeader.dataDirectory[0].virtualAddress;
	} else {
		const auto nt = peekProc< ImageNtHeaders32 >(module + dos.addressOfNtHeader);
		dataAddress   = nt.optionalHeader.dataDirectory[0].virtualAddress;
	}

	if (!dataAddress) {
		return 0;
	}

	const auto exportDir = peekProc< ImageExportDirectory >(module + dataAddress);

	const auto funcs = peekProcVector< uint32_t >(module + exportDir.addressOfFunctions, exportDir.numberOfFunctions);
	const auto names = peekProcVector< uint32_t >(module + exportDir.addressOfNames, exportDir.numberOfNames);
	const auto ords  = peekProcVector< uint16_t >(module + exportDir.addressOfNameOrdinals, exportDir.numberOfNames);

	for (uint32_t i = 0; i < exportDir.numberOfNames; ++i) {
		if (names[i]) {
			const auto name = peekProcString(module + names[i], symbol.size());

			if (name == symbol) {
				return module + funcs[ords[i]];
			}
		}
	}

	return 0;
}

// This function returns:
// -1 in case of failure.
// 0 if the process is 32-bit.
// 1 if the process is 64-bit.
#ifdef OS_WINDOWS
static inline int8_t isProcess64Bit(const procptr_t &baseAddress) {
#else
// We use a different name because the function is called by the Linux version
// of isProcess64Bit() in case the process is running through Wine.
static inline int8_t isWin32Process64Bit(const procptr_t &baseAddress) {
#endif
	const auto dos = peekProc< ImageDosHeader >(baseAddress);
	if (!(dos.magic[0] == 'M' && dos.magic[1] == 'Z')) {
		// Invalid DOS signature
		return -1;
	}

	const auto nt = peekProc< ImageNtHeadersNoOptional >(baseAddress + dos.addressOfNtHeader);
	if (!(nt.signature[0] == 'P' && nt.signature[1] == 'E' && nt.signature[2] == '\0' && nt.signature[3] == '\0')) {
		// Invalid NT signature
		return -1;
	}

	switch (nt.fileHeader.machine) {
		case 0x14c: // IMAGE_FILE_MACHINE_I386
			return 0;
		default:
			return 1;
	}
}

#ifdef WIN32
#	include "mumble_positional_audio_win32.h"
#else
#	include "mumble_positional_audio_linux.h"
#endif

#endif
