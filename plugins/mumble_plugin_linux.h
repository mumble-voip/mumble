// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_PLUGIN_LINUX_H_
#define MUMBLE_PLUGIN_LINUX_H_

#ifndef PTR_TYPE_CONCRETE
# define PTR_TYPE_CONCRETE PTR_TYPE
#endif

#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#include "mumble_plugin.h"

pid_t pPid;
static PTR_TYPE_CONCRETE pModule;

static inline std::string readAll(std::string fn) {
	std::ifstream ifs;
	ifs.open(fn.c_str(), std::ifstream::binary);

	std::string content;

	char buf[256];
	while (ifs.good()) {
		ifs.read(&buf[0], sizeof(buf));
		size_t nread = ifs.gcount();
		if (nread > 0) {
			content.append(&buf[0], nread);
		}
	}

	return content;
}

static inline PTR_TYPE_CONCRETE getModuleAddr(pid_t pid, const wchar_t *modname) {
	std::wstring modnameWide(modname);
	std::string modnameNonWide(modnameWide.begin(), modnameWide.end());

	std::stringstream ss;
	ss << std::string("/proc/");
	ss << static_cast<unsigned long>(pid);
	ss << std::string("/maps");
	std::string mapsFn = ss.str();
	std::string maps = readAll(mapsFn);

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
			baseaddr.push_back(static_cast<char>(ch));
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
			pathname.push_back(static_cast<char>(ch));
		};

		// OK, we found 'em!
		// Only treat path as a real path if it starts with /.
		if (pathname.size() > 0 && pathname.at(0) == '/') {
			// Find the basename.
			size_t lastSlash = pathname.find_last_of('/');
			if (pathname.size() > lastSlash + 1) {
				std::string basename = pathname.substr(lastSlash + 1);
				if (basename == modnameNonWide) {
					unsigned long addr = strtoul(baseaddr.c_str(), NULL, 16);
					return addr;
				}
			}
		}
	}

	return 0;
}

static inline PTR_TYPE_CONCRETE getModuleAddr(const wchar_t *modname) {
	return getModuleAddr(pPid, modname);
}

static inline bool peekProc(PTR_TYPE base, void *dest, size_t len) {
	struct iovec in;
	in.iov_base = reinterpret_cast<void *>(base); // Address from target process
	in.iov_len = len; // Length

	struct iovec out;
	out.iov_base = dest;
	out.iov_len = len;

	ssize_t nread = process_vm_readv(pPid, &out, 1, &in, 1, 0);

	return (nread != -1 && static_cast<size_t>(nread) == in.iov_len);
}

template<class T>
T peekProc(PTR_TYPE base) {
	T v = 0;
	if (!peekProc(base, reinterpret_cast<T *>(&v), sizeof(T))) {
		return 0;
	}
	return v;
}

template<class T>
bool peekProc(PTR_TYPE base, T &dest) {
	struct iovec in;
	in.iov_base = reinterpret_cast<void *>(base); // Address from target process
	in.iov_len = sizeof(T); // Length

	struct iovec out;
	out.iov_base = &dest;
	out.iov_len = sizeof(T);

	ssize_t nread = process_vm_readv(pPid, &out, 1, &in, 1, 0);

	return (nread != -1 && static_cast<size_t>(nread) == in.iov_len);
}

static bool inline initialize(const std::multimap<std::wstring, unsigned long long int> &pids, const wchar_t *procname, const wchar_t *modname = NULL) {
	pModule = 0;

	if (! pids.empty()) {
		std::multimap<std::wstring, unsigned long long int>::const_iterator iter = pids.find(std::wstring(procname));

		if (iter != pids.end())
			pPid = static_cast<pid_t>(iter->second);
		else
			pPid = 0;
	} else {
		pPid = 0;
	}

	if (pPid == 0)
		return false;

	pModule = getModuleAddr(modname ? modname : procname);

	if (pModule == 0) {
		pPid = 0;
		return false;
	}

	return true;
}

static void generic_unlock() {
	pModule = 0;
	pPid = 0;
}

#endif
