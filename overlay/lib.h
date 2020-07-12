// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_LIB_H_
#define MUMBLE_LIB_H_

#define _UNICODE
#ifdef _WIN32_WINNT
# undef _WIN32_WINNT
#endif
#define  _WIN32_WINNT 0x0501
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <windows.h>
#include <cmath>
#include <map>
#include <vector>
#include <string>
#include <boost/optional.hpp>
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
	size_t offsetCreate;
	size_t offsetCreateEx;
};

struct DXGIData {
	/// Filepath of the module the offsets are for.
	wchar_t wcFileName[MODULEFILEPATH_BUFLEN];
	size_t offsetPresent;
	size_t offsetResize;
};

struct D3D10Data {
	/// Filepath of the module the offsets are for.
	wchar_t wcFileName[MODULEFILEPATH_BUFLEN];
	size_t offsetAddRef;
	size_t offsetRelease;
};

struct D3D11Data {
	/// Filepath of the module the offsets are for.
	wchar_t wcFileName[MODULEFILEPATH_BUFLEN];
	size_t offsetAddRef;
	size_t offsetRelease;
};

struct SharedData {
	bool bHooked;
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
extern boost::optional<size_t> GetFnOffsetInModule(voidFunc fnptr, wchar_t *refmodulepath, unsigned int refmodulepathLen, const std::string &logPrefix, const std::string &fnName);

#endif
