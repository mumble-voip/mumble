// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PLUGIN_SE_COMMON_
#define MUMBLE_MUMBLE_PLUGIN_SE_COMMON_

typedef std::map< std::string, procptr_t > Interfaces;

struct InterfaceReg {
	uint32_t createFunction;
	uint32_t name;
	uint32_t next;
};

static Interfaces getInterfaces(const procptr_t module) {
	Interfaces interfaces;

	// s_pInterfaceRegs is exported on Linux
	auto s_pInterfaceRegs = proc->exportedSymbol("s_pInterfaceRegs", module);
	if (!s_pInterfaceRegs) {
		const auto CreateInterface = proc->exportedSymbol("CreateInterface", module);
		if (CreateInterface == 0) {
			return interfaces;
		}

		bool jmpOnly;

		if (proc->peek< uint8_t >(CreateInterface) == 0xE9) {
			// Left 4 Dead:
			// E9 ?? ?? ?? ??    jmp    CreateInterface_0
			jmpOnly = true;
		} else {
			// Other games:
			// 55                push    ebp
			// 8B EC             mov     ebp, esp
			// 5D                pop     ebp
			// E9 ?? ?? ?? ??    jmp     CreateInterfaceInternal
			jmpOnly = false;
		}

		const auto jmpTarget               = proc->peek< int32_t >(CreateInterface + (jmpOnly ? 1 : 5));
		const auto jmpInstructionEnd       = CreateInterface + (jmpOnly ? 5 : 9);
		const auto CreateInterfaceInternal = jmpInstructionEnd + jmpTarget;

		// Left 4 Dead:
		// 56                   push    esi
		// 8B 35 ?? ?? ?? ??    mov     esi, s_pInterfaceRegs
		// 85 F6                test    esi, esi
		// 57                   push    edi
		//
		// Other games:
		// 55                   push    ebp
		// 8B EC                mov     ebp, esp
		// 56                   push    esi
		// 8B 35 ?? ?? ?? ??    mov     esi, s_pInterfaceRegs
		if (proc->peek< uint16_t >(CreateInterfaceInternal + (jmpOnly ? 1 : 4)) != 0x358B) {
			return interfaces;
		}

		s_pInterfaceRegs = proc->peek< uint32_t >(CreateInterfaceInternal + (jmpOnly ? 3 : 6));
	}

	auto iface = proc->peek< InterfaceReg >(proc->peekPtr(s_pInterfaceRegs));

	do {
		const auto name    = proc->peekString(iface.name);
		const auto address = proc->peek< uint32_t >(iface.createFunction + (isWin32 ? 1 : 2));

		interfaces.insert(Interfaces::value_type(name, address));
	} while (iface.next && proc->peek(iface.next, iface));

	return interfaces;
}

static procptr_t getInterfaceAddress(const std::string &name, const Interfaces &interfaces) {
	const auto iface = interfaces.find(name);
	if (iface == interfaces.end()) {
		return 0;
	}

	return iface->second;
}

#endif
