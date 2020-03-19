// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_HARDHOOK_X86_H_
#define MUMBLE_HARDHOOK_X86_H_

#define _UNICODE
#ifndef _WIN32_WINNT
#define  _WIN32_WINNT 0x0501
#endif
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <windows.h>
#include <cmath>
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
