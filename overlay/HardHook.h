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

#ifndef MUMBLE_HARDHOOK_H_
#define MUMBLE_HARDHOOK_H_

#define _UNICODE
#ifndef _WIN32_WINNT
#define  _WIN32_WINNT 0x0501
#endif
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <windows.h>
#include <math.h>
#include <map>
#include <vector>
#include <string>

typedef void *(*voidFunc)();

struct HardHook {
	// Pointer to executable code page that holds all trampoline codes
	static void *pCode;
	// Number of bytes used in pCode.
	static unsigned int uiCode;

	// Pointer to original code (which is hooked/replaced)
	unsigned char *baseptr;
	// The original instructions that are replaced by the hook
	unsigned char orig[6];
	// Hook-Code; Replacement instructions for function call jump
	unsigned char replace[6];
	// Remembers whether there is a trampoline in place in the target code or
	// whether restore -> call orig. -> replace has to be used for every intercepted call
	bool bTrampoline;
	// Points to the (rest of the) original function when used from the injected function
	voidFunc call;

	HardHook();
	HardHook(voidFunc func, voidFunc replacement);
	void setup(voidFunc func, voidFunc replacement);
	void setupInterface(IUnknown *intf, LONG funcoffset, voidFunc replacement);
	void reset();
	void inject(bool force = false);
	void restore(bool force = false);
	void print();
	void check();

private:
	static const int CODEREPLACESIZE;
	static const int CODEPROTECTSIZE;

	void *cloneCode(void **orig);
};

#endif
