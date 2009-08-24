/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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
#include <d3d10.h>

DXGIData *dxgi;

static bool bHooked = false;
static bool bChaining = false;
static HardHook hhPresent;

typedef HRESULT(__stdcall *CreateDXGIFactoryType)(REFIID, void **);
typedef HRESULT(__stdcall *D3D10CreateDeviceAndSwapChainType)(IDXGIAdapter *, D3D10_DRIVER_TYPE, HMODULE, UINT, UINT, DXGI_SWAP_CHAIN_DESC *, IDXGISwapChain **, ID3D10Device **);
typedef HRESULT(__stdcall *D3D10CreateDeviceType)(IDXGIAdapter *, D3D10_DRIVER_TYPE, HMODULE, UINT, UINT, ID3D10Device **);

typedef HRESULT(__stdcall *PresentType)(IDXGISwapChain *, UINT, UINT);
static HRESULT __stdcall myPresent(IDXGISwapChain *isc, UINT SyncInterval, UINT Flags) {
	ods("DXGI: Device Present");

//	doPresent(idd);

	PresentType oPresent = (PresentType) hhPresent.call;

	hhPresent.restore();
	HRESULT hr = oPresent(isc, SyncInterval, Flags);
	hhPresent.inject();
	return hr;
}

static void HookPresentRaw(voidFunc vfPresent) {
	ods("DXGI: Injecting Present Raw");
	hhPresent.setup(vfPresent, reinterpret_cast<voidFunc>(myPresent));
}


void checkDXGIHook(bool preonly) {
	if (bChaining) {
		return;
		ods("DXGI: Causing a chain");
	}

	bChaining = true;

	HMODULE hDXGI = GetModuleHandleW(L"DXGI.DLL");

	if (hDXGI != NULL) {
		if (! bHooked) {
			wchar_t procname[2048];
			GetModuleFileNameW(NULL, procname, 2048);
			fods("DXGI: Hookcheck '%ls'", procname);
			bHooked = true;

			// Add a ref to ourselves; we do NOT want to get unloaded directly from this process.
			GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<char *>(&checkDXGIHook), &hSelf);

			// Can we use the prepatch data?
			GetModuleFileNameW(hDXGI, procname, 2048);
			if (_wcsicmp(dxgi->wcFileName, procname) == 0) {
				unsigned char *raw = (unsigned char *) hDXGI;
				HookPresentRaw((voidFunc)(raw + dxgi->iOffsetPresent));
			} else if (! preonly) {
				fods("DXGI Interface changed, can't rawpatch");
			} else {
				bHooked = false;
			}
		}
	}

	bChaining = false;
}

extern "C" __declspec(dllexport) void __cdecl PrepareDXGI() {
	ods("Preparing static data for DXGI Injection");

	HMODULE hD3D10 = LoadLibrary("D3D10.DLL");
	HMODULE hDXGI = LoadLibrary("DXGI.DLL");
	HRESULT hr;

	dxgi->wcFileName[0] = 0;
	dxgi->iOffsetPresent = 0;

	if (hDXGI != NULL && hD3D10 != NULL) {
		CreateDXGIFactoryType pCreateDXGIFactory = reinterpret_cast<CreateDXGIFactoryType>(GetProcAddress(hDXGI, "CreateDXGIFactory"));
		ods("Got %p", pCreateDXGIFactory);
		if (pCreateDXGIFactory) {
			IDXGIFactory * pFactory;
			hr = pCreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory) );
			if (pFactory) {
				HWND hwnd = CreateWindowW( L"STATIC", L"Mumble DXGI Window", WS_OVERLAPPEDWINDOW,
										  CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, 0,
										  NULL, NULL, 0 );

				IDXGIAdapter *pAdapter = NULL;
				pFactory->EnumAdapters(0, &pAdapter);

				D3D10CreateDeviceAndSwapChainType pD3D10CreateDeviceAndSwapChain = reinterpret_cast<D3D10CreateDeviceAndSwapChainType>(GetProcAddress(hD3D10, "D3D10CreateDeviceAndSwapChain"));
				D3D10CreateDeviceType pD3D10CreateDevice = reinterpret_cast<D3D10CreateDeviceType>(GetProcAddress(hD3D10, "D3D10CreateDevice"));

				IDXGISwapChain *pSwapChain = NULL;
				ID3D10Device *pDevice = NULL;

				DXGI_SWAP_CHAIN_DESC desc;
				ZeroMemory(&desc, sizeof(desc));

		        RECT rcWnd;
				GetClientRect(hwnd, &rcWnd );
				desc.BufferDesc.Width = rcWnd.right - rcWnd.left;
				desc.BufferDesc.Height = rcWnd.bottom - rcWnd.top;

				ods("W %d H %d", desc.BufferDesc.Width, desc.BufferDesc.Height);

				desc.BufferDesc.RefreshRate.Numerator = 60;
				desc.BufferDesc.RefreshRate.Denominator = 1;
				desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
				desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
				desc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;

				desc.SampleDesc.Count = 1;
				desc.SampleDesc.Quality = 0;

				desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

				desc.BufferCount = 2;

				desc.OutputWindow = hwnd;

				desc.Windowed = true;

				desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

				hr = pD3D10CreateDeviceAndSwapChain(pAdapter, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, &desc, &pSwapChain, &pDevice);

				ods("Wohoo! %p %p %x", pSwapChain, pDevice, hr);
				if (pDevice && pSwapChain) {
						HMODULE hRef;
						void ***vtbl = (void ***) pSwapChain;
						void *pPresent = (*vtbl)[8];
						if (! GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (char *) pPresent, &hRef)) {
							ods("Failed to get module for Present");
						} else {
							GetModuleFileNameW(hRef, dxgi->wcFileName, 2048);
							unsigned char *b = (unsigned char *) pPresent;
							unsigned char *a = (unsigned char *) hRef;
							dxgi->iOffsetPresent = b-a;
							ods("Successfully found prepatch offset: %ls: %d", dxgi->wcFileName, dxgi->iOffsetPresent);
						}
				}
				if (pDevice)
					pDevice->Release();
				if (pSwapChain)
					pSwapChain->Release();
				DestroyWindow(hwnd);

				pFactory->Release();
			}
		}
	}
}
