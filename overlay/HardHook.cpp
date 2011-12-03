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

void * HardHook::pCodePage = NULL;
unsigned int HardHook::uiCodePageUnusedOffset = 0;

HardHook::HardHook() : bTrampoline(false), call(0) {
	int i;
	baseptr = NULL;
	for (i=0;i<6;i++)
		orig[i]=replace[i]=0;
}

HardHook::HardHook(voidFunc func, voidFunc replacement) {
	int i;
	baseptr = NULL;
	for (i=0;i<6;i++)
		orig[i]=replace[i]=0;
	setup(func, replacement);
}

static unsigned int modrmbytes(unsigned char a, unsigned char b) {
	const unsigned char lower = (a & 0x0f);
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

void* HardHook::cloneCode(void **pOrig) {
	DWORD oldProtect, restoreProtect;
	if (! pCodePage || uiCodePageUnusedOffset > 4000) {
		fods("HardHook: Initializing pCode executable codepage");
		uiCodePageUnusedOffset = 0;
		pCodePage = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	}

	unsigned char *orig = (unsigned char *) *pOrig;
	unsigned char *executableCodePage = (unsigned char *) pCodePage;
	executableCodePage += uiCodePageUnusedOffset;

	if (!VirtualProtect(orig, 16, PAGE_EXECUTE_READ, &oldProtect)) {
		fods("HardHook: Failed vprotect (1)");
		return NULL;
	}

	// Follow jumps until we reach a no-jump operation
	while (*orig == 0xe9) { // JMP
		/*const*/ unsigned char * originalJumpPointer = orig;
		int *iptr = reinterpret_cast<int *>(orig+1);
		// Follow jmp relative to next command. It doesn't make a difference
		// if we actually perform all the jumps or directly jump after the
		// chain.
		orig += *iptr + 5;

		fods("HardHook: Chaining from %p to %p", *pOrig, orig);
		*pOrig = orig;

		// Assume jump took us out of our read enabled zone, get rights for the new one
		VirtualProtect(originalJumpPointer, 16, oldProtect, &restoreProtect);
		if (!VirtualProtect(orig, 16, PAGE_EXECUTE_READ, &oldProtect)) {
			fods("HardHook: Failed vprotect (2)");
			return NULL;
		}
	}

	unsigned int idx = 0;
	do {
		unsigned char opcode = orig[idx];
		unsigned char a = orig[idx+1];
		unsigned char b = orig[idx+2];
		unsigned char c = orig[idx+3];
		unsigned int extra = 0;

		// Clone the opcode value from the original source to the codepage
		executableCodePage[idx] = opcode;
		idx++;

		// Check if we need to increment idx further (multi-byte operations (params etc))
		// For x86 opcodes, see for example http://ref.x86asm.net/coder.html
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
			//0x00 extra = modrmbytes(a,b);
			//TODO is 0x00 always 2 bytes???
			case 0x00: // OR - r/m8 r8 - http://ref.x86asm.net/coder32.html#x00
				extra = 1;
				break;
			case 0x0b: // OR - r32 r/m32 - http://ref.x86asm.net/coder32.html#x0B
				extra = 1;
				break;
			case 0x68: // PUSH immediate
				extra = 4;
				break;
			case 0x6a: // PUSH immediate
				extra = 1;
				break;
			case 0x6f: // OUTS - no idea … - http://ref.x86asm.net/coder32.html#x6F
				extra = 4;
				break;
			case 0x81: // CMP - opcode field + r/m + immediate
				extra = 1 + modrmbytes(a,b) + 4;
				break;
			case 0x83:	// CMP - opcode field + r/m + imm8
				extra = 1 + modrmbytes(a,b) + 1;
				break;
			case 0x8b:	// MOV
				extra = 1 + modrmbytes(a,b);
				break;
			case 0xff:	// INC || DEC || CALL || JMP || PUSH - http://ref.x86asm.net/coder.html#xFF
				extra = 1 + modrmbytes(a,b);
				break;
			default:
				fods("HardHook: Unknown opcode %02x at %d: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x", opcode, idx-1, orig[0], orig[1], orig[2], orig[3], orig[4], orig[5], orig[6], orig[7], orig[8], orig[9], orig[10], orig[11]);
				VirtualProtect(orig, 16, oldProtect, &restoreProtect);
				return NULL;
				break;
		}
		// Copy any potential extra bytes (params etc)
		for (unsigned int i=0;i<extra;++i)
			executableCodePage[idx+i] = orig[idx+i];
		idx += extra;
		//TODO replace ^ with idx += 1 + extra; instead of separate places of incrementation
		//     alternatively, increment idx also while copying params

	} while (idx < 6); //TODO < why only clone 6 bytes?? [doc]
	VirtualProtect(orig, 16, oldProtect, &restoreProtect);

	// Add a relative jmp back to the original code
	executableCodePage[idx] = 0xe9; //JMP
	const int offset = orig - executableCodePage - 5;
	++idx; // idx now indices to where the jump target address goes to

	int *iptr = reinterpret_cast<int *>(&executableCodePage[idx]);
	*iptr = offset; // set the relative jump target address
	idx += 4;

	uiCodePageUnusedOffset += idx;
	FlushInstructionCache(GetCurrentProcess(), executableCodePage, idx);

	return executableCodePage;
}

void HardHook::setup(voidFunc func, voidFunc replacement) {
	int i;
	DWORD oldProtect, restoreProtect;

	if (baseptr)
		return;

	unsigned char *fptr = reinterpret_cast<unsigned char *>(func);
	unsigned char *nptr = reinterpret_cast<unsigned char *>(replacement);

	fods("HardHook: Asked to replace %p with %p", func, replacement);

	call = (voidFunc) cloneCode((void **) &fptr);

	if (call) {
		bTrampoline = true;
	} else {
		bTrampoline = false;
		call = func;
	}

	if (VirtualProtect(fptr, 16, PAGE_EXECUTE_READ, &oldProtect)) {
		unsigned char **iptr = reinterpret_cast<unsigned char **>(&replace[1]);
		*iptr = nptr;
		replace[0] = 0x68; // PUSH (immediate) nptr
		replace[5] = 0xc3; // RETN

		for (i=0;i<6;i++)
			orig[i]=fptr[i];

		baseptr = fptr;
		inject(true);

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

void HardHook::restore(bool force) {
	if (! baseptr)
		return;
	if (! force && bTrampoline)
		return;

	DWORD oldProtect, restoreProtect;
	if (VirtualProtect(baseptr, 6, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		int i;
		for (i=0;i<6;i++)
			baseptr[i] = orig[i];
		VirtualProtect(baseptr, 6, oldProtect, &restoreProtect);
		FlushInstructionCache(GetCurrentProcess(),baseptr, 6);
	}
}

void HardHook::print() {
	fods("HardHook: %02x %02x %02x %02x %02x => %02x %02x %02x %02x %02x (%02x %02x %02x %02x %02x)",
	     orig[0], orig[1], orig[2], orig[3], orig[4],
	     replace[0], replace[1], replace[2], replace[3], replace[4],
	     baseptr[0], baseptr[1], baseptr[2], baseptr[3], baseptr[4]);
}

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
