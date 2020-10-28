// Copyright 2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "HostLinux.h"

#include "mumble_plugin_utils.h"

#include <cstring>
#include <sstream>

#include <sys/uio.h>

HostLinux::HostLinux(const procid_t pid) : m_ok(true), m_pid(pid) {
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

procptr_t HostLinux::module(const std::string &module) const {
	std::stringstream ss;
	ss << "/proc/";
	ss << m_pid;
	ss << "/maps";
	const auto maps = readFile(ss.str());

	if (maps.size() == 0) {
		return 0;
	}

	std::stringstream ssPath(maps);
	while (ssPath.good()) {
		std::string baseaddr;

		int ch;
		while (1) {
			ch = ssPath.get();
			if (ch == '-') {
				break;
			} else if (ch == EOF) {
				return 0;
			}
			baseaddr.push_back(static_cast< char >(ch));
		}

		// seek to perms
		do {
			ch = ssPath.get();
			if (ch == EOF) {
				return 0;
			}
		} while (ch != ' ');

		// seek to offset
		do {
			ch = ssPath.get();
			if (ch == EOF) {
				return 0;
			}
		} while (ch != ' ');

		// seek to dev
		do {
			ch = ssPath.get();
			if (ch == EOF) {
				return 0;
			}
		} while (ch != ' ');

		// seek to inode
		do {
			ch = ssPath.get();
			if (ch == EOF) {
				return 0;
			}
		} while (ch != ' ');

		// seek to pathname
		do {
			ch = ssPath.get();
			if (ch == EOF) {
				return 0;
			}
		} while (ch != ' ');

		// eat spaces until we're at the beginning of pathname.
		while (ch == ' ') {
			if (ch == EOF) {
				return 0;
			}
			ch = ssPath.get();
		}
		ssPath.unget();

		std::string pathname;
		while (1) {
			ch = ssPath.get();
			if (ch == '\n') {
				break;
			} else if (ch == EOF) {
				return 0;
			}
			pathname.push_back(static_cast< char >(ch));
		};

		// OK, we found 'em!
		// Only treat path as a real path if it starts with /.
		if (pathname.size() > 0 && pathname.at(0) == '/') {
			// Find the basename.
			size_t lastSlash = pathname.find_last_of('/');
			if (pathname.size() > lastSlash + 1) {
				std::string basename = pathname.substr(lastSlash + 1);
				if (basename == module) {
					unsigned long addr = strtoul(baseaddr.c_str(), nullptr, 16);
					return addr;
				}
			}
		}
	}

	return 0;
}

bool HostLinux::isWine(const procid_t id) {
	std::stringstream ss;
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
