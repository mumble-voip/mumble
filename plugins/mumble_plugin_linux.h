// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_PLUGIN_LINUX_H_
#define MUMBLE_PLUGIN_LINUX_H_

# ifndef MUMBLE_PLUGIN_MAIN_H_
#  error "Include mumble_plugin_main.h instead of mumble_plugin_linux.h"
# endif

#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

static inline std::string readAll(const std::string &fn) {
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

// This function returns:
// -1 in case of failure.
// 0 if the process is 32-bit.
// 1 if the process is 64-bit.
static inline int isProcess64Bit(const procptr_t &baseAddress) {
	if (isWin32) {
		return isWin32Process64Bit(baseAddress);
	}

	// We can know the process architecture by looking at its ELF header.
	uint8_t elf[5];

	peekProc(baseAddress, elf, sizeof(elf));

	// The first 4 bytes constitute the magical number in ASCII: 0x7F 45 4c 46.
	if (!(elf[0] == 0x7f && elf[1] == 'E' && elf[2] == 'L' && elf[3] == 'F')) {
		return -1;
	}

	// The fifth byte is 1 in case the process is 32-bit or 2 in case it's 64-bit.
	return elf[4] != 1;
}

// This function returns:
// -1 in case of failure.
// 0 if the process is not running through Wine.
// 1 if the process is running through Wine.
static inline int8_t isProcessWin32(const procid_t &pid) {
	std::stringstream ss;
	ss << "/proc/";
	ss << static_cast<unsigned long>(pid);
	ss << "/exe";

	char *path = realpath(ss.str().c_str(), nullptr);
	if (!path) {
		return -1;
	}

	const char *filename = basename(path);

	free(path);

	if (strcmp(filename, "wine-preloader") == 0 || strcmp(filename, "wine64-preloader") == 0) {
		return 1;
	}

	return 0;
}

static inline procptr_t getModuleAddr(const procid_t &pid, const wchar_t *modname) {
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
					unsigned long addr = strtoul(baseaddr.c_str(), nullptr, 16);
					return addr;
				}
			}
		}
	}

	return 0;
}

static inline bool peekProc(const procptr_t &addr, void *dest, const size_t &len) {
	struct iovec in;
	in.iov_base = reinterpret_cast<void *>(addr); // Address from target process
	in.iov_len = len; // Length

	struct iovec out;
	out.iov_base = dest;
	out.iov_len = len;

	ssize_t nread = process_vm_readv(pPid, &out, 1, &in, 1, 0);

	return (nread != -1 && static_cast<size_t>(nread) == in.iov_len);
}

static void generic_unlock() {
	pModule = 0;
	pPid = 0;
}

static bool initialize(const std::multimap<std::wstring, unsigned long long int> &pids, const wchar_t *procname, const wchar_t *modname = nullptr) {
	pModule = 0;

	if (!pids.empty()) {
		auto iter = pids.find(std::wstring(procname));
		if (iter != pids.end()) {
			pPid = static_cast<procid_t>(iter->second);
		} else {
			pPid = 0;
		}
	} else {
		pPid = 0;
	}

	if (!pPid) {
		return false;
	}

	pModule = getModuleAddr(procname);
	if (!pModule) {
		pPid = 0;
		return false;
	}

	int8_t ret = isProcessWin32(pPid);
	if (ret == -1) {
		generic_unlock();
		return false;
	}

	isWin32 = ret;

	ret = isProcess64Bit(pModule);
	if (ret == -1) {
		generic_unlock();
		return false;
	}

	is64Bit = ret;

	if (modname) {
		pModule = getModuleAddr(modname);
		if (!pModule) {
			pPid = 0;
			return false;
		}
	}

	return true;
}

#endif
