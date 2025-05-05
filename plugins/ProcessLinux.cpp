// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ProcessLinux.h"

#include <elf.h>

struct GnuHash {
	uint32_t nBucket;
	uint32_t symOffset;
	uint32_t nBloom;
	uint32_t bloomShift;
	// uintptr_t bloom[nBloom];
	// uint32_t  bucket[nBucket];
	// uint32_t  chain[];
};

ProcessLinux::ProcessLinux(const procid_t id, const std::string &name) : ProcessBase(id, name) {
	const auto mods = modules();
	const auto iter = mods.find(name);
	if (iter == mods.cend()) {
		return;
	}

	const auto address = iter->second.baseAddress();
	if (!address) {
		return;
	}

	// We can know the process architecture by looking at its ELF header.
	const auto elf = peekVector< int8_t >(address, 5);

	// The first 4 bytes constitute the magical number in ASCII: 0x7F 45 4c 46.
	if (!(elf[0] == 0x7f && elf[1] == 'E' && elf[2] == 'L' && elf[3] == 'F')) {
		return;
	}

	// The fifth byte is 1 in case the process is 32 bit or 2 in case it's 64 bit.
	m_pointerSize = elf[4] == 1 ? 4 : 8;

	m_ok = true;
}

ProcessLinux::~ProcessLinux() {
}

template< typename Elf_Ehdr, typename Elf_Phdr, typename Elf_Dyn, typename Elf_Sym >
static procptr_t exportedSymbol(const ProcessBase &proc, const std::string &symbol, const procptr_t module) {
	procptr_t hashTable = 0;
	procptr_t strTable  = 0;
	procptr_t symTable  = 0;

	bool gnuHash = false;

	const auto ehdr  = proc.peek< Elf_Ehdr >(module);
	const auto phdrs = proc.peekVector< Elf_Phdr >(module + ehdr.e_phoff, ehdr.e_phnum);

	for (const auto &phdr : phdrs) {
		if (phdr.p_type != PT_DYNAMIC) {
			continue;
		}

		const auto dyns = proc.peekVector< Elf_Dyn >(module + phdr.p_vaddr, phdr.p_memsz / sizeof(Elf_Dyn));
		for (const auto &dyn : dyns) {
			switch (dyn.d_tag) {
				case DT_GNU_HASH:
					gnuHash = true;
					// Fall-through
				case DT_HASH:
					if (!hashTable) {
						hashTable = dyn.d_un.d_ptr;
					}
					break;
				case DT_STRTAB:
					strTable = dyn.d_un.d_ptr;
					break;
				case DT_SYMTAB:
					symTable = dyn.d_un.d_ptr;
			}

			if (hashTable && strTable && symTable) {
				break;
			}
		}

		break;
	}

	if (!(hashTable && strTable && symTable)) {
		return 0;
	}

	if (!gnuHash) {
		// Hash table pseudo-struct:
		// uint32_t nBucket;
		// uint32_t nChain;
		// uint32_t bucket[nBucket];
		// uint32_t chain[nChain];
		const auto nChain = proc.peek< uint32_t >(hashTable + sizeof(uint32_t));

		for (uint32_t i = 0; i < nChain; ++i) {
			const auto sym  = proc.peek< Elf_Sym >(symTable + sizeof(Elf_Sym) * i);
			const auto name = proc.peekString(strTable + sym.st_name, symbol.size());

			if (name == symbol) {
				return module + sym.st_value;
			}
		}
	} else {
		const auto table = proc.peek< GnuHash >(hashTable);

		procptr_t ptr     = hashTable + sizeof(Elf_Sym().st_value) * table.nBloom;
		const auto bucket = proc.peekVector< uint32_t >(ptr, table.nBucket);

		ptr += sizeof(uint32_t) * bucket.size();

		for (uint32_t i = 0; i < bucket.size(); ++i) {
			auto symIndex = bucket[i];
			if (symIndex < table.symOffset) {
				continue;
			}

			do {
				const auto sym  = proc.peek< Elf_Sym >(symTable + sizeof(Elf_Sym) * symIndex);
				const auto name = proc.peekString(strTable + sym.st_name, symbol.size());

				if (name == symbol) {
					return module + sym.st_value;
				}
			} while (proc.peek< uint32_t >(ptr + sizeof(uint32_t) * symIndex++ - table.symOffset) & 1);
		}
	}

	return 0;
}

procptr_t ProcessLinux::exportedSymbol(const std::string &symbol, const procptr_t module) const {
	if (m_pointerSize > 4) {
		return ::exportedSymbol< Elf64_Ehdr, Elf64_Phdr, Elf64_Dyn, Elf64_Sym >(*this, symbol, module);
	} else {
		return ::exportedSymbol< Elf32_Ehdr, Elf32_Phdr, Elf32_Dyn, Elf32_Sym >(*this, symbol, module);
	}
}
