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

#ifndef MUMBLE_LIB_H_
#define MUMBLE_LIB_H_

#define _UNICODE
#define  _WIN32_WINNT 0x0501
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <windows.h>
#include <math.h>
#include <map>
#include <vector>
#include <string>
#include "overlay.h"
#include "HardHook.h"
#include "ods.h"

#define lround(x) static_cast<long int>((x) + (((x) >= 0.0) ? 0.5 : -0.5))

#define ARRAY_SIZE_BYTES(x) sizeof(x)
#define ARRAY_NUM_ELEMENTS(x) (sizeof(x)/sizeof((x)[0]))

using namespace std;

void __cdecl ods(const char *format, ...);

const int MODULEFILEPATH_BUFLEN = 2048;
const int PROCNAMEFILEPATH_BUFLEN = 1024;
const int PROCNAMEFILEPATH_EXTENDED_EXTLEN = 64;
const int PROCNAMEFILEPATH_EXTENDED_BUFFER_BUFLEN = PROCNAMEFILEPATH_BUFLEN + PROCNAMEFILEPATH_EXTENDED_EXTLEN;

struct Direct3D9Data {
	/// Filepath of the module the offsets are for.
	wchar_t wcFileName[MODULEFILEPATH_BUFLEN];
	int iOffsetCreate;
	int iOffsetCreateEx;
};

struct DXGIData {
	/// Filepath of the module the offsets are for.
	wchar_t wcFileName[MODULEFILEPATH_BUFLEN];
	int iOffsetPresent;
	int iOffsetResize;
};

struct D3D10Data {
	/// Filepath of the module the offsets are for.
	wchar_t wcFileName[MODULEFILEPATH_BUFLEN];
	int iOffsetAddRef;
	int iOffsetRelease;
};

struct D3D11Data {
	/// Filepath of the module the offsets are for.
	wchar_t wcFileName[MODULEFILEPATH_BUFLEN];
	int iOffsetAddRef;
	int iOffsetRelease;
};

struct SharedData {
	bool bHooked;
};

struct FakeInterface {
	typedef ULONG(FakeInterface::* voidMemberFunc)();
	void **vtbl;
	IUnknown *pOriginal;
	IUnknown *pNew;
	void *pAssembly;
	FakeInterface(IUnknown *orig, int entries);
	~FakeInterface();
	void replace(LONG funcoffset, voidMemberFunc replacement);
};

class Mutex {
	protected:
		static CRITICAL_SECTION cs;
	public:
		static void init();
		Mutex();
		~Mutex();
};

class Pipe {
	private:
		HANDLE hSocket;
		HANDLE hMemory;

		void release();
	protected:
		unsigned int uiWidth, uiHeight;
		unsigned int uiLeft, uiTop, uiRight, uiBottom;
		unsigned char *a_ucTexture;
		DWORD dwAlreadyRead;
		OverlayMsg omMsg;

		void checkMessage(unsigned int w, unsigned int h);
		bool sendMessage(const OverlayMsg &m);
		virtual void blit(unsigned int x, unsigned int y, unsigned int w, unsigned int h) = 0;
		virtual void setRect() = 0;
		virtual void newTexture(unsigned int w, unsigned int h) = 0;
		Pipe();
		virtual ~Pipe();
	public:
		void disconnect();
};

// From lib.cpp
extern void checkHooks(bool preonly = false);
// From dxgi.cpp
extern void checkDXGIHook(bool preonly = false);
// From d3d10.cpp
extern void checkDXGI10Hook(bool preonly = false);
// From d3d11.cpp
extern void checkDXGI11Hook(bool preonly = false);
// From d3d9.cpp
extern void checkD3D9Hook(bool preonly = false);
// From opengl.cpp
extern void checkOpenGLHook();
// From d3d9.cpp
extern void freeD3D9Hook(HMODULE hModule);

// From d3d9.cpp
extern Direct3D9Data *d3dd;
// From dxgi.cpp
extern DXGIData *dxgi;
// From d3d10.cpp
extern D3D10Data *d3d10;
// From d3d11.cpp
extern D3D11Data *d3d11;
// From lib.cpp
extern BOOL bIsWin8;

// From lib.cpp
/// Checks if the module of the function pointer fnptr equals the module filepath
/// of refmodulepath.
///
/// @param fnptr
/// @param refmodulepath the module path to compare against
/// @param logPrefix Used for debug logging.
/// @param fnName name of the method fnptr points to. used for debug logging
/// @return true if the module filepath of the function pointer matches the reference one
extern bool IsFnInModule(voidFunc fnptr, wchar_t *refmodulepath, const std::string &logPrefix, const std::string &fnName);

// From lib.cpp
/// Checks fnptr is in a loaded module with module path refmodulepath.
///
/// @return Offset as int or < 0 on failure.
extern int GetFnOffsetInModule(voidFunc fnptr, wchar_t *refmodulepath, unsigned int refmodulepathLen, const std::string &logPrefix, const std::string &fnName);

#endif
