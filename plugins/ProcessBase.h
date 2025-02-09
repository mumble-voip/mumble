// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef PROCESSBASE_H_
#define PROCESSBASE_H_

#ifdef OS_WINDOWS
#	include "HostWindows.h"
using Host = HostWindows;
#else
#	include "HostLinux.h"
using Host = HostLinux;
#endif

#include <map>
#include <vector>

/// Abstract class.
/// Only defines stuff that can be used with both Linux and Windows processes.
class ProcessBase : public Host {
protected:
	bool m_ok;
	std::string m_name;
	uint8_t m_pointerSize;

public:
	using Host::peek;

	inline bool isOk() const { return m_ok; }

	template< typename T > inline bool peek(const procptr_t address, T &dst) const {
		return peek(address, &dst, sizeof(T));
	}

	template< typename T > T inline peek(const procptr_t address) const {
		T ret;
		if (!peek(address, &ret, sizeof(T))) {
			ret = {};
		}

		return ret;
	}

	procptr_t peekPtr(const procptr_t address) const;

	/// Resolves x64's RIP (Relative Instruction Pointer).
	procptr_t peekRIP(const procptr_t address) const { return address + peek< uint32_t >(address) + 4; }

	/// Reads the specified amount of data at the specified address and returns it as std::string.
	/// An empty std::string is returned in case of error.
	///
	/// If \p length is 0, the function reads one byte at a time and stops when either '\0' is found or 3 seconds have
	/// passed. The successfully read data is returned, also in case of error.
	std::string peekString(const procptr_t address, const size_t length = 0) const;

	template< typename T > std::vector< T > peekVector(const procptr_t address, const size_t elements) const {
		try {
			std::vector< T > var(elements);
			peek(address, &var[0], sizeof(T) * elements);
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
	std::vector< T > peekVector(const procptr_t address, const size_t elements, const size_t realStructSize) const {
		if (realStructSize == sizeof(T)) {
			// If the structure's size is correct, there is no need for this special function.
			return peekVector< T >(address, elements);
		} else if (realStructSize < sizeof(T)) {
			return std::vector< T >();
		}

		// Read the full structures into a vector.
		const auto fullStructs = peekVector< uint8_t >(address, realStructSize * elements);

		// Allocate vector for the known structures.
		std::vector< T > ret(elements);

		// Copy elements from the vector containing the full structures, discarding extra bytes.
		auto seek = &fullStructs[0];

		for (size_t i = 0; i < elements; ++i, seek += realStructSize) {
			memcpy(&ret[i], seek, sizeof(T));
		}

		return ret;
	}

	virtual procptr_t exportedSymbol(const std::string &symbol, const procptr_t module) const = 0;

	procptr_t virtualFunction(const procptr_t classObject, const size_t index) const;

	/// Iterates through all readable memory regions, until findPattern() succeeds.
	/// Returns 0 if the pattern is not found.
	procptr_t findPattern(const std::vector< uint8_t > &pattern, const Module &module);

	/// Reads memory in chunks until either the pattern is found, the end is reached or peek() fails.
	/// Refer to searchInBuffer() for the pattern format.
	/// Returns 0 if the pattern is not found.
	procptr_t findPattern(const std::vector< uint8_t > &pattern, procptr_t address, const size_t size);

	ProcessBase(const procid_t id, const std::string &name);
	virtual ~ProcessBase();
};

#endif // PROCESSBASE_H_
