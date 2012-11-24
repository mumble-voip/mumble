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

#ifndef _LIB_H
#define _LIB_H

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

using namespace std;

void __cdecl ods(const char *format, ...);

struct Direct3D9Data {
	int iOffsetCreate;
	int iOffsetCreateEx;
	char cFileName[2048];
};

struct DXGIData {
	int iOffsetPresent;
	int iOffsetResize;
	int iOffsetAddRef;
	int iOffsetRelease;
	wchar_t wcDXGIFileName[2048];
	wchar_t wcD3D10FileName[2048];
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
		~Pipe();
	public:
		void disconnect();
};

extern void checkDXGIHook(bool preonly = false);
extern void checkD3D9Hook(bool preonly = false);
extern void checkOpenGLHook();

extern Direct3D9Data *d3dd;
extern DXGIData *dxgi;
extern HMODULE hSelf;
extern BOOL bIsWin8;
extern unsigned int uiAudioCount;
extern bool bVideoHooked;

#endif
