// Copyright 2020-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Process.h"

#include "mumble_positional_audio_utils.h"

#include <chrono>

Process::Process(const procid_t id, const std::string &name) : Host(id), m_ok(false), m_name(name), m_pointerSize(0) {
}

Process::~Process() {
}

procptr_t Process::peekPtr(const procptr_t address) const {
	procptr_t v = 0;

	if (!peek(address, &v, m_pointerSize)) {
		return 0;
	}

	return v;
}

std::string Process::peekString(const procptr_t address, const size_t length) const {
	std::string string;

	if (length > 0) {
		string.resize(length);

		if (!peek(address, &string[0], length)) {
			return std::string();
		}
	} else {
		auto now       = std::chrono::steady_clock::now();
		const auto end = now + std::chrono::seconds(3);

		for (procptr_t i = 0; now < end; ++i) {
			char ch = 0;
			if (!peek(address + i, &ch, sizeof(ch)) || ch == '\0') {
				break;
			}

			string += ch;

			// Update current time.
			now = std::chrono::steady_clock::now();
		}
	}

	return string;
}

procptr_t Process::virtualFunction(const procptr_t classObject, const size_t index) const {
	const auto vTable = peekPtr(classObject);
	if (!vTable) {
		return 0;
	}

	return peekPtr(vTable + (index * m_pointerSize));
}

procptr_t Process::findPattern(const std::vector< uint8_t > &pattern, const Module &module) {
	for (const auto &region : module.regions()) {
		if (!region.readable) {
			continue;
		}

		const auto ret = findPattern(pattern, region.address, region.size);
		if (ret) {
			return ret;
		}
	}

	return 0;
}

procptr_t Process::findPattern(const std::vector< uint8_t > &pattern, procptr_t address, const size_t size) {
	// 32 KiB appears to be a good balance
	constexpr uint16_t bufferSize = 32768;
	std::vector< uint8_t > buffer(bufferSize);

	const auto chunks = size / buffer.size();
	for (size_t i = 0; i < chunks; ++i) {
		if (!peek(address, &buffer[0], buffer.size())) {
			return 0;
		}

		const auto ret = searchInBuffer(pattern, buffer);
		if (ret != SIZE_MAX) {
			return address + ret;
		}

		address += buffer.size();
	}

	const auto remainder = size % buffer.size();
	if (remainder >= pattern.size()) {
		buffer.resize(remainder);
		if (!peek(address, &buffer[0], buffer.size())) {
			return 0;
		}

		const auto ret = searchInBuffer(pattern, buffer);
		if (ret != SIZE_MAX) {
			return address + ret;
		}
	}

	return 0;
}

procid_t Process::find(const std::string &name, const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (pids.empty()) {
		return 0;
	}

	const auto iter = pids.find(utf8ToUtf16(name));
	if (iter == pids.cend()) {
		return 0;
	}

	return static_cast< procid_t >(iter->second);
}
