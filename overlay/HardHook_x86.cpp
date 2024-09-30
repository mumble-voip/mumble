// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "HardHook.h"
#include "ods.h"

void *HardHook::pCode         = nullptr;
unsigned int HardHook::uiCode = 0;

const int HardHook::CODEREPLACESIZE = 6;
const int HardHook::CODEPROTECTSIZE = 16;

/**
 * @brief Constructs a new hook without actually injecting.
 */
HardHook::HardHook() : bTrampoline(false), call(0), baseptr(nullptr) {
	for (int i = 0; i < CODEREPLACESIZE; ++i) {
		orig[i] = replace[i] = 0;
	}

	//	assert(CODEREPLACESIZE == sizeof(orig) / sizeof(orig[0]));
	//	assert(CODEREPLACESIZE == sizeof(replace) / sizeof(replace[0]));
}

/**
 * @brief Constructs a new hook by injecting given replacement function into func.
 * @see HardHook::setup
 * @param func Funktion to inject replacement into.
 * @param replacement Function to inject into func.
 */
HardHook::HardHook(voidFunc func, voidFunc replacement) : bTrampoline(false), call(0), baseptr(nullptr) {
	for (int i = 0; i < CODEREPLACESIZE; ++i)
		orig[i] = replace[i] = 0;
	setup(func, replacement);
}

/**
 * @return Number of extra bytes.
 */
static unsigned int modrmbytes(unsigned char a, unsigned char b) {
	unsigned char lower = (a & 0x0f);
	if (a >= 0xc0) {
		return 0;
	} else if (a >= 0x80) {
		if ((lower == 4) || (lower == 12))
			return 5;
		else
			return 4;
	} else if (a >= 0x40) {
		if ((lower == 4) || (lower == 12))
			return 2;
		else
			return 1;

	} else {
		if ((lower == 4) || (lower == 12)) {
			if ((b & 0x07) == 0x05)
				return 5;
			else
				return 1;
		} else if ((lower == 5) || (lower == 13))
			return 4;
		return 0;
	}
}

/**
 * @brief Tries to construct a trampoline from original code.
 *
 * A trampoline is the replacement code that features the original code plus
 * a jump back to the original instructions that follow.
 * It is called to execute the original behavior. As it is a replacement for
 * the original, the original can then be overwritten.
 * The size of the trampoline is at least CODEREPLACESIZE. Thus, CODEREPLACESIZE
 * bytes of the original code can afterwards be overwritten (and the trampoline
 * called after those instructions for the original logic).
 * CODEREPLACESIZE has to be smaller than CODEPROTECTSIZE.
 *
 * As commands must not be destroyed they have to be disassembled to get their length.
 * All encountered commands will be part of the trampoline and stored in pCode (shared
 * for all trampolines).
 *
 * If code is encountered that can not be moved into the trampoline (conditionals etc.)
 * construction fails and nullptr is returned. If enough commands can be saved the
 * trampoline is finalized by appending a jump back to the original code. The return value
 * in this case will be the address of the newly constructed trampoline.
 *
 * pCode + offset to trampoline:
 *     [SAVED CODE FROM ORIGINAL which is >= CODEREPLACESIZE bytes][JUMP BACK TO ORIGINAL CODE]
 *
 * @param porig Original code
 * @return Pointer to trampoline on success. nullptr if trampoline construction failed.
 */
void *HardHook::cloneCode(void **porig) {
	if (!pCode || uiCode > 4000) {
		pCode  = VirtualAlloc(nullptr, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		uiCode = 0;
	}
	// If we have no memory to clone to, return.
	if (!pCode) {
		return nullptr;
	}

	unsigned char *o = (unsigned char *) *porig;

	DWORD origProtect;
	if (!VirtualProtect(o, CODEPROTECTSIZE, PAGE_EXECUTE_READ, &origProtect)) {
		fods("HardHook: CloneCode failed; failed to make original code read and executable");
		return nullptr;
	}

	// Follow relative jumps to next instruction. On execution it doesn't make
	// a difference if we actually perform all the jumps or directly jump to the
	// end of the chain. Hence these jumps need not be part of the trampoline.
	while (*o == 0xe9) { // JMP
		unsigned char *tmp = o;
		int *iptr          = reinterpret_cast< int * >(o + 1);
		o += *iptr + 5;

		fods("HardHook: CloneCode: Skipping jump from %p to %p", *porig, o);
		*porig = o;

		// Assume jump took us out of our read enabled zone, get rights for the new one
		DWORD tempProtect;
		VirtualProtect(tmp, CODEPROTECTSIZE, origProtect, &tempProtect);
		if (!VirtualProtect(o, CODEPROTECTSIZE, PAGE_EXECUTE_READ, &origProtect)) {
			fods("HardHook: CloneCode failed; failed to make jump target code read and executable");
			return nullptr;
		}
	}

	unsigned char *n = (unsigned char *) pCode;
	n += uiCode;
	unsigned int idx = 0;

	do {
		unsigned char opcode = o[idx];
		unsigned char a      = o[idx + 1];
		unsigned char b      = o[idx + 2];
		unsigned int extra   = 0;

		switch (opcode) {
			case 0x50: // PUSH
			case 0x51:
			case 0x52:
			case 0x53:
			case 0x54:
			case 0x55:
			case 0x56:
			case 0x57:
			case 0x58: // POP
			case 0x59:
			case 0x5a:
			case 0x5b:
			case 0x5c:
			case 0x5d:
			case 0x5e:
			case 0x5f:
				break;
			case 0x6a: // PUSH immediate
				extra = 1;
				break;
			case 0x68: // PUSH immediate
				extra = 4;
				break;
			case 0x81: // CMP immediate
				extra = modrmbytes(a, b) + 5;
				break;
			case 0x83: // CMP
				extra = modrmbytes(a, b) + 2;
				break;
			case 0x8b: // MOV
				extra = modrmbytes(a, b) + 1;
				break;
			default: {
				int rmop = ((a >> 3) & 7);
				if (opcode == 0xff && rmop == 6) { // PUSH memory
					extra = modrmbytes(a, b) + 1;
					break;
				}

				fods("HardHook: CloneCode failed; Unknown opcode %02x at %d: %02x %02x %02x %02x %02x %02x %02x %02x "
					 "%02x %02x %02x %02x",
					 opcode, idx, o[0], o[1], o[2], o[3], o[4], o[5], o[6], o[7], o[8], o[9], o[10], o[11]);
				DWORD tempProtect;
				VirtualProtect(o, CODEPROTECTSIZE, origProtect, &tempProtect);
				return nullptr;
				break;
			}
		}

		n[idx] = opcode;
		++idx;

		for (unsigned int i = 0; i < extra; ++i)
			n[idx + i] = o[idx + i];
		idx += extra;

	} while (idx < CODEREPLACESIZE);

	DWORD tempProtect;
	VirtualProtect(o, CODEPROTECTSIZE, origProtect, &tempProtect);

	// Add a relative jmp back to the original code, to after the copied code
	n[idx++]              = 0xe9;
	int *iptr             = reinterpret_cast< int * >(&n[idx]);
	const int JMP_OP_SIZE = 5;
	int offs              = o - n - JMP_OP_SIZE;
	*iptr                 = offs;
	idx += 4;

	uiCode += idx;

	FlushInstructionCache(GetCurrentProcess(), n, idx);

	fods("HardHook: trampoline creation successful at %p", n);

	return n;
}

/**
 * @brief Makes sure the given replacement function is run once func is called.
 *
 * Tries to construct a trampoline for the given function (@see HardHook::cloneCode)
 * and then injects replacement function calling code into the first 6 bytes of the
 * original function (@see HardHook::inject).
 *
 * @param func Pointer to function to redirect.
 * @param replacement Pointer to code to redirect to.
 */
void HardHook::setup(voidFunc func, voidFunc replacement) {
	if (baseptr)
		return;

	fods("HardHook: Setup: Asked to replace %p with %p", func, replacement);

	unsigned char *fptr = reinterpret_cast< unsigned char * >(func);
	unsigned char *nptr = reinterpret_cast< unsigned char * >(replacement);

	call = (voidFunc) cloneCode((void **) &fptr);

	if (call) {
		bTrampoline = true;
	} else {
		// Could not create a trampoline. Use alternative method instead.
		// This alternative method is dependant on the replacement code
		// restoring before calling the original. Otherwise we get a jump recursion
		bTrampoline = false;
		call        = func;
	}

	DWORD origProtect;
	if (VirtualProtect(fptr, CODEPROTECTSIZE, PAGE_EXECUTE_READ, &origProtect)) {
		replace[0]           = 0x68; // PUSH immediate        1 Byte
		unsigned char **iptr = reinterpret_cast< unsigned char ** >(&replace[1]);
		*iptr                = nptr; // (imm. value = nptr)   4 Byte
		replace[5]           = 0xc3; // RETN                  1 Byte

		// Save original 6 bytes at start of original function
		for (int i = 0; i < CODEREPLACESIZE; ++i)
			orig[i] = fptr[i];

		baseptr = fptr;

		inject(true);

		DWORD tempProtect;
		VirtualProtect(fptr, CODEPROTECTSIZE, origProtect, &tempProtect);
	} else {
		fods("HardHook: setup failed; failed to make original code read and executable");
	}
}

void HardHook::setupInterface(IUnknown *unkn, LONG funcoffset, voidFunc replacement) {
	fods("HardHook: setupInterface: Replacing %p function #%ld", unkn, funcoffset);
	void **ptr = reinterpret_cast< void ** >(unkn);
	ptr        = reinterpret_cast< void ** >(ptr[0]);
	setup(reinterpret_cast< voidFunc >(ptr[funcoffset]), replacement);
}

void HardHook::reset() {
	baseptr     = 0;
	bTrampoline = false;
	call        = nullptr;
	for (int i = 0; i < CODEREPLACESIZE; ++i) {
		orig[i] = replace[i] = 0;
	}
}

/**
 * @brief Injects redirection code into the target function.
 *
 * Replaces the first 6 Bytes of the function indicated by baseptr
 * with the replacement code previously generated (usually a jump
 * to mumble code). If a trampoline is available this injection is not needed
 * as control flow was already permanently redirected by HardHook::setup .
 *
 * @param force Perform injection even when trampoline is available.
 */
void HardHook::inject(bool force) {
	if (!baseptr)
		return;
	if (!force && bTrampoline)
		return;

	DWORD origProtect;
	if (VirtualProtect(baseptr, CODEREPLACESIZE, PAGE_EXECUTE_READWRITE, &origProtect)) {
		for (int i = 0; i < CODEREPLACESIZE; ++i) {
			baseptr[i] = replace[i]; // Replace with jump to new code
		}

		DWORD tempProtect;
		VirtualProtect(baseptr, CODEREPLACESIZE, origProtect, &tempProtect);

		FlushInstructionCache(GetCurrentProcess(), baseptr, CODEREPLACESIZE);
	}

	// Verify that the injection was successful
	for (int i = 0; i < CODEREPLACESIZE; ++i) {
		if (baseptr[i] != replace[i]) {
			fods("HardHook: Injection failure noticed at byte %d", i);
		}
	}
}

/**
 * @brief Restores the original code in a target function.
 *
 * Restores the first 6 Bytes of the function indicated by baseptr
 * from previously stored original code in orig. If a trampoline is available this
 * restoration is not needed as trampoline will correctly restore control
 * flow.
 *
 * @param force If true injection will be reverted even when trampoline is available.
 */
void HardHook::restore(bool force) {
	if (!baseptr)
		return;
	if (!force && bTrampoline)
		return;

	DWORD origProtect;
	if (VirtualProtect(baseptr, CODEREPLACESIZE, PAGE_EXECUTE_READWRITE, &origProtect)) {
		for (int i = 0; i < CODEREPLACESIZE; ++i)
			baseptr[i] = orig[i];
		DWORD tempProtect;
		VirtualProtect(baseptr, CODEREPLACESIZE, origProtect, &tempProtect);

		FlushInstructionCache(GetCurrentProcess(), baseptr, CODEREPLACESIZE);
	}
}

void HardHook::print() {
	fods("HardHook: code replacement: %02x %02x %02x %02x %02x => %02x %02x %02x %02x %02x (currently effective: %02x "
		 "%02x %02x %02x %02x)",
		 orig[0], orig[1], orig[2], orig[3], orig[4], replace[0], replace[1], replace[2], replace[3], replace[4],
		 baseptr[0], baseptr[1], baseptr[2], baseptr[3], baseptr[4]);
}

/**
 * @brief Checks whether injected code is in good shape and injects if not yet injected.
 *
 * If injected code is not found injection is attempted unless 3rd party overwrote
 * original code at injection location.
 */
void HardHook::check() {
	if (memcmp(baseptr, replace, CODEREPLACESIZE) != 0) {
		// The instructions do not match our replacement instructions
		// If they match the original code, inject our hook.
		if (memcmp(baseptr, orig, CODEREPLACESIZE) == 0) {
			fods("HardHook: Reinjecting hook into function %p", baseptr);
			inject(true);
		} else {
			fods("HardHook: Function %p replaced by third party. Lost injected hook.");
		}
	}
}
