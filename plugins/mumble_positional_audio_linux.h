// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_POSITIONAL_AUDIO_LINUX_H_
#define MUMBLE_POSITIONAL_AUDIO_LINUX_H_

#ifndef MUMBLE_POSITIONAL_AUDIO_MAIN_H_
#	error "Include mumble_positional_audio_main.h instead of mumble_positional_audio_linux.h"
#endif

#include "mumble_positional_audio_utils.h"

#include <cstring>
#include <elf.h>
#include <iostream>
#include <libgen.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/uio.h>

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
	ss << static_cast< unsigned long >(pid);
	ss << "/exe";

	char *path = realpath(ss.str().c_str(), nullptr);
	if (!path) {
		return -1;
	}

	const char *filename = basename(path);

	if (strcmp(filename, "wine-preloader") == 0 || strcmp(filename, "wine64-preloader") == 0) {
		free(path);
		return 1;
	}

	// basename() returns a pointer to the basename's position in the string passed as argument.
	// For that reason we cannot free 'path' before the if statement.
	free(path);

	return 0;
}

static inline procptr_t getModuleAddr(const procid_t &pid, const wchar_t *modname) {
	std::wstring modnameWide(modname);
	std::string modnameNonWide(modnameWide.begin(), modnameWide.end());

	std::stringstream ss;
	ss << std::string("/proc/");
	ss << static_cast< unsigned long >(pid);
	ss << std::string("/maps");
	std::string mapsFn = ss.str();
	std::string maps   = readFile(mapsFn);

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
	in.iov_base = reinterpret_cast< void * >(addr); // Address from target process
	in.iov_len  = len;                              // Length

	struct iovec out;
	out.iov_base = dest;
	out.iov_len  = len;

	ssize_t nread = process_vm_readv(pPid, &out, 1, &in, 1, 0);

	return (nread != -1 && static_cast< size_t >(nread) == in.iov_len);
}

template< typename Elf_Ehdr, typename Elf_Phdr, typename Elf_Dyn, typename Elf_Sym >
static inline procptr_t getExportedSymbolInternal(const std::string &symbol, const procptr_t module) {
	procptr_t hashTable = 0;
	procptr_t strTable  = 0;
	procptr_t symTable  = 0;

	const auto ehdr = peekProc< Elf_Ehdr >(module);
	const auto phdr = peekProcVector< Elf_Phdr >(module + ehdr.e_phoff, ehdr.e_phnum);

	for (size_t i = 0; i < phdr.size(); ++i) {
		if (phdr[i].p_type == PT_DYNAMIC) {
			const auto dyn = peekProcVector< Elf_Dyn >(module + phdr[i].p_vaddr, phdr[i].p_memsz / sizeof(Elf_Dyn));

			for (size_t j = 0; j < dyn.size(); ++j) {
				switch (dyn[j].d_tag) {
					case DT_HASH:
						hashTable = dyn[j].d_un.d_ptr;
						break;
					case DT_STRTAB:
						strTable = dyn[j].d_un.d_ptr;
						break;
					case DT_SYMTAB:
						symTable = dyn[j].d_un.d_ptr;
						break;
				}

				if (hashTable && strTable && symTable) {
					break;
				}
			}

			break;
		}
	}

	// Hash table pseudo-struct:
	// uint32_t nBucket;
	// uint32_t nChain;
	// uint32_t bucket[nBucket];
	// uint32_t chain[nChain];
	const auto nChain = peekProc< uint32_t >(hashTable + sizeof(uint32_t));

	for (uint32_t i = 0; i < nChain; ++i) {
		const auto sym  = peekProc< Elf_Sym >(symTable + sizeof(Elf_Sym) * i);
		const auto name = peekProcString(strTable + sym.st_name, symbol.size());

		if (name == symbol) {
			return module + sym.st_value;
		}
	}

	return 0;
}

static inline procptr_t getExportedSymbol(const std::string &symbol, const procptr_t module) {
	if (isWin32) {
		return getWin32ExportedSymbol(symbol, module);
	}

	if (is64Bit) {
		return getExportedSymbolInternal< Elf64_Ehdr, Elf64_Phdr, Elf64_Dyn, Elf64_Sym >(symbol, module);
	} else {
		return getExportedSymbolInternal< Elf32_Ehdr, Elf32_Phdr, Elf32_Dyn, Elf32_Sym >(symbol, module);
	}
}

static void generic_unlock() {
	pModule = 0;
	pPid    = 0;
}

static bool initialize(const std::multimap< std::wstring, unsigned long long int > &pids, const wchar_t *procname,
					   const wchar_t *modname = nullptr) {
	pModule = 0;

	if (!pids.empty()) {
		auto iter = pids.find(std::wstring(procname));
		if (iter != pids.end()) {
			pPid = static_cast< procid_t >(iter->second);
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
