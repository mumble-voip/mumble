/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

void *HardHook::pCode = NULL;
unsigned int HardHook::uiCode = 0;

static void __cdecl ods(const char *format, ...) {
	char    buf[4096], *p = buf;
	va_list args;

	va_start(args, format);
	int len = _vsnprintf_s(p, sizeof(buf) - 1, _TRUNCATE, format, args);
	va_end(args);

	if (len <= 0)
		return;

	p += len;

	while (p > buf  &&  isspace(static_cast<unsigned char>(p[-1])))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p   = '\0';

	OutputDebugStringA(buf);
}


HardHook::HardHook() {
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

void *HardHook::cloneCode(void **porig) {
	if (! pCode || uiCode > 4000) {
		uiCode = 0;
		pCode = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	}

	unsigned char *o = (unsigned char *) *porig;
	unsigned char *n = (unsigned char *) pCode;
	n += uiCode;
	unsigned int idx = 0;

	while (*o == 0xe9) {
		int *iptr = reinterpret_cast<int *>(o+1);
		o += *iptr + 5;

		ods("HardHook: Chaining from %p to %p", *porig, o);
		*porig = o;
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
			case 0x68:
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
			default:
				ods("HardHook: Unknown opcode at %d: %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x", idx-1, o[0], o[1], o[2], o[3], o[4], o[5], o[6], o[7], o[8], o[9], o[10], o[11]);
				return NULL;
				break;
		}
		for (unsigned int i=0;i<extra;++i)
			n[idx+i] = o[idx+i];
		idx += extra;

	} while (idx < 6);

	n[idx++] = 0xe9;
	int offs = o - n - 5;

	int *iptr = reinterpret_cast<int *>(&n[idx]);
	*iptr = offs;
	idx += 4;

	uiCode += idx;
	FlushInstructionCache(GetCurrentProcess(), n, idx);

	return n;
}

void HardHook::setup(voidFunc func, voidFunc replacement) {
	int i;
	DWORD oldProtect, restoreProtect;

	if (baseptr)
		return;

	unsigned char *fptr = reinterpret_cast<unsigned char *>(func);
	unsigned char *nptr = reinterpret_cast<unsigned char *>(replacement);

	ods("HardHook: Asked to replace %p with %p", func, replacement);

	if (VirtualProtect(fptr, 16, PAGE_EXECUTE_READ, &oldProtect)) {
		call = (voidFunc) cloneCode((void **) &fptr);

		if (call) {
			bTrampoline = true;
		} else {
			bTrampoline = false;
			call = func;
		}

		unsigned char **iptr = reinterpret_cast<unsigned char **>(&replace[1]);
		*iptr = nptr;
		replace[0] = 0x68;
		replace[5] = 0xc3;

		for (i=0;i<6;i++)
			orig[i]=fptr[i];

		baseptr = fptr;
		inject(true);

		VirtualProtect(fptr, 16, oldProtect, &restoreProtect);
	} else {
		ods("Failed initial vprotect");
	}
}

void HardHook::setupInterface(IUnknown *unkn, LONG funcoffset, voidFunc replacement) {
	ods("HardHook: Replacing %p function #%ld", unkn, funcoffset);
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
			baseptr[i] = replace[i];
		VirtualProtect(baseptr, 6, oldProtect, &restoreProtect);
		FlushInstructionCache(GetCurrentProcess(),baseptr, 6);
	}
	for (i=0;i<6;i++)
		if (baseptr[i] != replace[i])
			ods("HH: Injection failure at byte %d", i);
}

void HardHook::restore(bool force) {
	DWORD oldProtect, restoreProtect;
	int i;

	if (! baseptr)
		return;
	if (! force && bTrampoline)
		return;

	if (VirtualProtect(baseptr, 6, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		for (i=0;i<6;i++)
			baseptr[i] = orig[i];
		VirtualProtect(baseptr, 6, oldProtect, &restoreProtect);
		FlushInstructionCache(GetCurrentProcess(),baseptr, 6);
	}
}

void HardHook::print() {
	ods("HardHook: %02x %02x %02x %02x %02x => %02x %02x %02x %02x %02x (%02x %02x %02x %02x %02x)",
	    orig[0], orig[1], orig[2], orig[3], orig[4],
	    replace[0], replace[1], replace[2], replace[3], replace[4],
	    baseptr[0], baseptr[1], baseptr[2], baseptr[3], baseptr[4]);
}

void HardHook::check() {
	if (memcmp(baseptr, replace, 6) != 0) {
		if (memcmp(baseptr, orig, 6) == 0) {
			ods("HH: Restoring function %p", baseptr);
			inject(true);
		} else {
			ods("HH: Function %p replaced by third party. Lost.");
		}
	}
}
