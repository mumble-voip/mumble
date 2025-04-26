// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2011, Kissaki <kissaki@gmx.de>
   Copyright (C) 2011, Nye Liu <nyet@nyet.org>

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

#include "lib.h"
#include <dxgi.h>

DXGIData *dxgi = nullptr;

static bool bHooked = false;
static HardHook hhPresent;
static HardHook hhResize;

typedef HRESULT(__stdcall *CreateDXGIFactory1Type)(REFIID, void **);

typedef HRESULT(__stdcall *PresentType)(IDXGISwapChain *, UINT, UINT);
typedef HRESULT(__stdcall *ResizeBuffersType)(IDXGISwapChain *, UINT, UINT, UINT, DXGI_FORMAT, UINT);

#define HMODREF(mod, func) func##Type p##func = (func##Type) GetProcAddress(mod, #func)

// From d3d10.cpp
extern void presentD3D10(IDXGISwapChain *pSwapChain);

// From d3d11.cpp
extern void presentD3D11(IDXGISwapChain *pSwapChain);

static HRESULT __stdcall myPresent(IDXGISwapChain *pSwapChain, UINT SyncInterval, UINT Flags) {
// Present is called for each frame. Thus, we do not want to always log here.
#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
	ods("DXGI: Call to Present; Drawing and then chaining the call to the original logic");
#endif

	// From this D3D-version-independent point, call the present logic for both
	// D3D 10 and 11. Those that apply will be executed adequately, those that
	// do not will (silently) fail.
	presentD3D10(pSwapChain);
	presentD3D11(pSwapChain);

	// TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	PresentType oPresent = (PresentType) hhPresent.call;
	hhPresent.restore();
	HRESULT hr = oPresent(pSwapChain, SyncInterval, Flags);
	hhPresent.inject();

	return hr;
}

// From d3d10.cpp
extern void resizeD3D10(IDXGISwapChain *pSwapChain);
// From d3d11.cpp
extern void resizeD3D11(IDXGISwapChain *pSwapChain);

static HRESULT __stdcall myResize(IDXGISwapChain *pSwapChain, UINT BufferCount, UINT Width, UINT Height,
								  DXGI_FORMAT NewFormat, UINT SwapChainFlags) {
#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
	ods("DXGI: Call to Resize. Forwarding to D3D10 and D3D11 custom implementations before calling original logic ...");
#endif

	resizeD3D10(pSwapChain);
	resizeD3D11(pSwapChain);

	// TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	ResizeBuffersType oResize = (ResizeBuffersType) hhResize.call;
	hhResize.restore();
	HRESULT hr = oResize(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
	hhResize.inject();
	return hr;
}

static void HookPresentRaw(voidFunc vfPresent) {
	ods("DXGI: Injecting Present");
	hhPresent.setup(vfPresent, reinterpret_cast< voidFunc >(myPresent));
}

static void HookResizeRaw(voidFunc vfResize) {
	ods("DXGI: Injecting ResizeBuffers Raw");
	hhResize.setup(vfResize, reinterpret_cast< voidFunc >(myResize));
}

void hookDXGI(HMODULE hDXGI, bool preonly);

void checkDXGIHook(bool preonly) {
	static bool bCheckHookActive = false;
	if (bCheckHookActive) {
		ods("DXGI: Recursion in checkDXGIHook");
		return;
	}

	if (dxgi->offsetPresent == 0 || dxgi->offsetResize == 0)
		return;

	bCheckHookActive = true;

	HMODULE hDXGI = GetModuleHandleW(L"DXGI.DLL");

	if (hDXGI) {
		if (!bHooked) {
			hookDXGI(hDXGI, preonly);
		}
#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
	} else {
		ods("DXGI: No DXGI.DLL found as loaded. No hooking at this point.");
#endif
	}

	bCheckHookActive = false;
}

/// @param hDXGI must be a valid module handle.
void hookDXGI(HMODULE hDXGI, bool preonly) {
	wchar_t modulename[MODULEFILEPATH_BUFLEN];
	GetModuleFileNameW(nullptr, modulename, ARRAY_NUM_ELEMENTS(modulename));
	ods("DXGI: hookDXGI in App '%ls'", modulename);

	// Add a ref to ourselves; we do NOT want to get unloaded directly from this process.
	HMODULE hTempSelf = nullptr;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast< LPCTSTR >(&hookDXGI), &hTempSelf);

	bHooked = true;

	// Can we use the prepatch data?
	GetModuleFileNameW(hDXGI, modulename, ARRAY_NUM_ELEMENTS(modulename));
	if (_wcsicmp(dxgi->wcFileName, modulename) == 0) {
		// The module seems to match the one we prepared d3dd for.

		unsigned char *raw = (unsigned char *) hDXGI;
		HookPresentRaw((voidFunc)(raw + dxgi->offsetPresent));
		HookResizeRaw((voidFunc)(raw + dxgi->offsetResize));

	} else if (!preonly) {
		ods("DXGI: Interface changed, can't rawpatch. Current: %ls ; Previously: %ls", modulename, dxgi->wcFileName);
	} else {
		bHooked = false;
	}
}

// From d3d10.cpp
extern void PrepareDXGI10(IDXGIAdapter1 *pAdapter, bool initializeDXGIData);
// From d3d11.cpp
extern void PrepareDXGI11(IDXGIAdapter1 *pAdapter, bool initializeDXGIData);

/// This function is called by the Mumble client in Mumble's scope
/// mainly to extract the offsets of various functions in the IDXGISwapChain
/// and IDXGIObject interfaces that need to be hooked in target
/// applications. The data is stored in the dxgi shared memory structure.
extern "C" __declspec(dllexport) void __cdecl PrepareDXGI() {
	if (!dxgi)
		return;

	ods("DXGI: Preparing static data for DXGI Injection");

	dxgi->wcFileName[0] = 0;
	dxgi->offsetPresent = 0;
	dxgi->offsetResize  = 0;

	// Make sure this is Vista or greater as quite a number of <=WinXP users have fake DX10 libs installed
	OSVERSIONINFOEXW ovi;
	memset(&ovi, 0, sizeof(ovi));
	ovi.dwOSVersionInfoSize = sizeof(ovi);
	GetVersionExW(reinterpret_cast< OSVERSIONINFOW * >(&ovi));
	if (ovi.dwMajorVersion < 6 || (ovi.dwMajorVersion == 6 && ovi.dwBuildNumber < 6001)) {
		ods("DXGI: No DXGI pre-Vista - skipping prepare");
		return;
	}

	HMODULE hDXGI = LoadLibrary("DXGI.DLL");

	if (hDXGI) {
		GetModuleFileNameW(hDXGI, dxgi->wcFileName, ARRAY_NUM_ELEMENTS(dxgi->wcFileName));

		CreateDXGIFactory1Type pCreateDXGIFactory1 =
			reinterpret_cast< CreateDXGIFactory1Type >(GetProcAddress(hDXGI, "CreateDXGIFactory1"));
		ods("DXGI: Got CreateDXGIFactory1 at %p", pCreateDXGIFactory1);
		if (pCreateDXGIFactory1) {
			IDXGIFactory1 *pFactory;
			HRESULT hr = pCreateDXGIFactory1(__uuidof(IDXGIFactory1), (void **) (&pFactory));
			if (FAILED(hr))
				ods("DXGI: Call to pCreateDXGIFactory1 failed!");
			if (pFactory) {
				IDXGIAdapter1 *pAdapter = nullptr;
				pFactory->EnumAdapters1(0, &pAdapter);

				/// Offsets have to be identified and initialized only once.
				bool initializeDXGIData = !dxgi->offsetPresent && !dxgi->offsetResize;
				PrepareDXGI10(pAdapter, initializeDXGIData);
				initializeDXGIData = !dxgi->offsetPresent && !dxgi->offsetResize;
				PrepareDXGI11(pAdapter, initializeDXGIData);

				pFactory->Release();
			} else {
				FreeLibrary(hDXGI);
			}
		} else {
			FreeLibrary(hDXGI);
		}
	} else {
		FreeLibrary(hDXGI);
	}
}
