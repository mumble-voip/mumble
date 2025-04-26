// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "HostLinux.h"

#include "mumble_positional_audio_utils.h"

#include <cstring>
#include <libgen.h>
#include <sstream>

#include <sys/uio.h>

HostLinux::HostLinux(const procid_t pid) : m_pid(pid) {
}

HostLinux::~HostLinux() {
}

bool HostLinux::peek(const procptr_t address, void *dst, const size_t size) const {
	iovec in;
	in.iov_base = reinterpret_cast< void * >(address);
	in.iov_len  = size;

	iovec out;
	out.iov_base = dst;
	out.iov_len  = size;

	const auto ret = process_vm_readv(m_pid, &out, 1, &in, 1, 0);

	return (ret != -1 && static_cast< size_t >(ret) == in.iov_len);
}

Modules HostLinux::modules() const {
	std::ostringstream path;
	path << "/proc/";
	path << m_pid;
	path << "/maps";
	const auto maps = readFile(path.str());

	if (maps.size() == 0) {
		return {};
	}

	Modules modules;
	std::string name;
	std::stringstream stream(maps);

	while (stream) {
		auto ret = readUntil(stream, '-');
		if (!ret.second) {
			return modules;
		}

		MemoryRegion region;
		region.address = std::stoull(ret.first, nullptr, 16);

		ret = readUntil(stream, ' ');
		if (!ret.second) {
			return modules;
		}

		region.size = std::stoull(ret.first, nullptr, 16) - region.address;

		ret = readUntil(stream, ' ');
		if (!ret.second) {
			return modules;
		}

		for (const auto character : ret.first) {
			switch (character) {
				case 'r':
					region.readable = true;
					break;
				case 'w':
					region.writable = true;
					break;
				case 'x':
					region.executable = true;
					break;
			}
		}

		// Skip offset, dev and inode
		for (uint8_t i = 0; i < 3; ++i) {
			if (!skipUntil(stream, ' ', false)) {
				return modules;
			}
		}

		// Eat spaces until we're at the beginning of the path
		if (!skipUntil(stream, ' ', true)) {
			return modules;
		}

		// Throw up last eaten character
		stream.unget();

		ret = readUntil(stream, '\n');
		if (!ret.second) {
			return modules;
		}

		if (ret.first.size() == 0) {
			// Anonymous region, let's add it to the module that owns the previous one
			if (!name.empty()) {
				auto iter = modules.find(name);
				iter->second.addRegion(region);
			}

			continue;
		}

		// Only treat path as a real path if it starts with "/"
		if (ret.first[0] != '/') {
			continue;
		}

		const auto lastSlashPos = ret.first.find_last_of('/');
		if (ret.first.size() > lastSlashPos + 1) {
			name            = ret.first.substr(lastSlashPos + 1);
			const auto iter = modules.find(name);
			if (iter != modules.cend()) {
				iter->second.addRegion(region);
			} else {
				Module module(name);
				module.addRegion(region);
				modules.insert(std::make_pair(name, module));
			}
		}
	}

	return modules;
}

bool HostLinux::isWine(const procid_t id) {
	std::ostringstream ss;
	ss << "/proc/";
	ss << id;
	ss << "/exe";

	char *path = realpath(ss.str().c_str(), nullptr);
	if (!path) {
		return false;
	}

	const char *filename = basename(path);
	if (!filename) {
		return false;
	}

	const auto ret = strcmp(filename, "wine-preloader") == 0 || strcmp(filename, "wine64-preloader") == 0;

	// basename() returns a pointer to the basename's position in the string passed as argument.
	// For that reason we cannot free 'path' before performing the check.
	free(path);

	return ret;
}
