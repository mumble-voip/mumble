/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "HardHook.h"
#include "ods.h"

void *HardHook::pCode = NULL;
unsigned int HardHook::uiCode = 0;

/**
 * @brief Constructs a new hook without actually injecting.
 */
HardHook::HardHook() : bTrampoline(false), call(0) {
	int i;
	baseptr = NULL;
	for (i=0;i<6;i++)
		orig[i]=replace[i]=0;
}

/**
 * @brief Constructs a new hook by injecting given replacement function into func.
 * @see HardHook::setup
 * @param func Funktion to inject replacement into.
 * @param replacement Function to inject into func.
 */
HardHook::HardHook(voidFunc func, voidFunc replacement) {
	int i;
	baseptr = NULL;
	for (i=0;i<6;i++)
		orig[i]=replace[i]=0;
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
 * A trampoline is called by an injected mumble function to return
 * control flow to the original code.
 *
 * For this to work we have to save all commands overlapping the first 6 bytes
 * of code in the original function. This is needed so the first redirection,
 * to our mumble code, can be inserted in their place.
 *
 * As commands must not be destroyed they have to be disassembled to get their length.
 * All encountered commands will be part of the trampoline and stored in pCode (shared
 * for all trampolines).
 *
 * If code is encountered that can not be moved into the trampoline (conditionals etc.)
 * construction fails and and NULL is returned. If enough commands can be saved the
 * trampoline is finalized by appending a jump back to the original code. The return value
 * in this case will be the address of the newly constructed trampoline.
 *
 * pCode + offset to trampoline:
 *     [SAVED CODE FROM ORIGINAL > 6 bytes][JUMP BACK TO ORIGINAL CODE]
 *
 * @param porig Original code
 * @return Pointer to trampoline on success. NULL if trampoline construction failed.
 */
void *HardHook::cloneCode(void **porig) {


	DWORD oldProtect, restoreProtect;
	if (! pCode || uiCode > 4000) {
		uiCode = 0;
		pCode = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	}

	unsigned char *o = (unsigned char *) *porig;
	unsigned char *n = (unsigned char *) pCode;
	n += uiCode;
	unsigned int idx = 0;

	if (!VirtualProtect(o, 16, PAGE_EXECUTE_READ, &oldProtect)) {
		fods("HardHook: Failed vprotect (1)");
		return NULL;
	}

	while (*o == 0xe9) { // JMP
		unsigned char *tmp = o;
		int *iptr = reinterpret_cast<int *>(o+1);
		// Follow jmp relative to next command. It doesn't make a difference
		// if we actually perform all the jumps or directly jump to the end of
		// the chain. Hence these jumps need not be part of the trampoline.
		o += *iptr + 5;

		fods("HardHook: Chaining from %p to %p", *porig, o);
		*porig = o;

		// Assume jump took us out of our read enabled zone, get rights for the new one
		VirtualProtect(tmp, 16, oldProtect, &restoreProtect);
		if (!VirtualProtect(o, 16, PAGE_EXECUTE_READ, &oldProtect)) {
			fods("HardHook: Failed vprotect (2)");
			return NULL;
		}
	}

	do {
		unsigned char opcode = o[idx];
		unsigned char a = o[idx+1];
		unsigned char b = o[idx+2];
		unsigned int extra = 0;

		n[idx] = opcode;
		idx++;

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
				extra = modrmbytes(a,b) + 5;
				break;
			case 0x83:	// CMP
				extra = modrmbytes(a,b) + 2;
				break;
			case 0x8b:	// MOV
				extra = modrmbytes(a,b) + 1;
				break;
			default: {
				int rmop = ((a>>3) & 7);
				if (opcode == 0xff && rmop == 6) { // PUSH memory
					extra = modrmbytes(a,b) + 1;
					break;
				}

				fods("HardHook: Unknown opcode at %d: %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x", idx-1, o[0], o[1], o[2], o[3], o[4], o[5], o[6], o[7], o[8], o[9], o[10], o[11]);
				VirtualProtect(o, 16, oldProtect, &restoreProtect);
				return NULL;
				break;
			}
		}
		for (unsigned int i=0;i<extra;++i)
			n[idx+i] = o[idx+i];
		idx += extra;

	} while (idx < 6);
	VirtualProtect(o, 16, oldProtect, &restoreProtect);

	n[idx++] = 0xe9; // Add a relative jmp back to the original code
	int offs = o - n - 5;

	int *iptr = reinterpret_cast<int *>(&n[idx]);
	*iptr = offs;
	idx += 4;

	uiCode += idx;
	FlushInstructionCache(GetCurrentProcess(), n, idx);

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

	unsigned char *fptr = reinterpret_cast<unsigned char *>(func);
	unsigned char *nptr = reinterpret_cast<unsigned char *>(replacement);

	fods("HardHook: Asked to replace %p with %p", func, replacement);

	call = (voidFunc) cloneCode((void **) &fptr);

	if (call) {
		bTrampoline = true;
	} else {
		// Could not create a trampoline. Use alternative method instead.
		bTrampoline = false;
		call = func;
	}

	DWORD oldProtect;
	if (VirtualProtect(fptr, 16, PAGE_EXECUTE_READ, &oldProtect)) {
		unsigned char **iptr = reinterpret_cast<unsigned char **>(&replace[1]);
		replace[0] = 0x68; // PUSH immediate        1 Byte
		*iptr = nptr;      // (imm. value = nptr)   4 Byte
		replace[5] = 0xc3; // RETN                  1 Byte

		for (int i=0;i<6;i++) // Save original 6 bytes at start of original function
			orig[i] = fptr[i];

		baseptr = fptr;
		inject(true);

		DWORD restoreProtect;
		VirtualProtect(fptr, 16, oldProtect, &restoreProtect);
	} else {
		fods("HardHook: Failed vprotect");
	}
}

void HardHook::setupInterface(IUnknown *unkn, LONG funcoffset, voidFunc replacement) {
	fods("HardHook: Replacing %p function #%ld", unkn, funcoffset);
	void **ptr = reinterpret_cast<void **>(unkn);
	ptr = reinterpret_cast<void **>(ptr[0]);
	setup(reinterpret_cast<voidFunc>(ptr[funcoffset]), replacement);
}

/**
 * @brief Injects redirection code into the target function.
 *
 * Replaces the first 6 Bytes of the function indicated by baseptr
 * with the replacement code previously generated (usually a jump
 * to mumble code). If a trampoline is available this injection is not needed
 * as control flow was already permanently redirected by HardHook::setup .
 *
 * @param force If true injection will be performed even when trampoline is available.
 */
void HardHook::inject(bool force) {
	DWORD oldProtect, restoreProtect;
	int i;

	if (! baseptr)
		return;
	if (! force && bTrampoline)
		return;

	if (VirtualProtect(baseptr, 6, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		for (i=0;i<6;i++)
			baseptr[i] = replace[i]; // Replace with jump to new code
		VirtualProtect(baseptr, 6, oldProtect, &restoreProtect);
		FlushInstructionCache(GetCurrentProcess(),baseptr, 6);
	}
	for (i=0;i<6;i++)
		if (baseptr[i] != replace[i])
			fods("HH: Injection failure at byte %d", i);
}

/**
 * @brief Restores the original code in a target function.
 *
 * Restores the first 6 Bytes of the function indicated by baseptr
 * from previously stored original code in orig. If a trampoline is available this
 * restoration is not needed as trampoline will correctly restore control
 * flow.
 *
 * @param force If true injection will be performed even when trampoline is available.
 */
void HardHook::restore(bool force) {

	if (! baseptr)
		return;
	if (! force && bTrampoline)
		return;

	DWORD oldProtect;
	if (VirtualProtect(baseptr, 6, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		for (int i=0;i<6;i++)
			baseptr[i] = orig[i];
		DWORD restoreProtect;
		VirtualProtect(baseptr, 6, oldProtect, &restoreProtect);
		FlushInstructionCache(GetCurrentProcess(), baseptr, 6);
	}
}

void HardHook::print() {
	fods("HardHook: %02x %02x %02x %02x %02x => %02x %02x %02x %02x %02x (%02x %02x %02x %02x %02x)",
	     orig[0], orig[1], orig[2], orig[3], orig[4],
	     replace[0], replace[1], replace[2], replace[3], replace[4],
	     baseptr[0], baseptr[1], baseptr[2], baseptr[3], baseptr[4]);
}

/**
 * @brief Checks whether injected code is in good shape and injects if not yet injected.
 *
 * If injected code is not found injection is attempted unless 3rd party overwrote
 * original code at injection location.
 */
void HardHook::check() {
	if (memcmp(baseptr, replace, 6) != 0) {
		if (memcmp(baseptr, orig, 6) == 0) {
			fods("HH: Restoring function %p", baseptr);
			inject(true);
		} else {
			fods("HH: Function %p replaced by third party. Lost.");
		}
	}
}
