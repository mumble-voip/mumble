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
#include <d3d9.h>

Direct3D9Data *d3dd;

typedef IDirect3D9*(WINAPI *pDirect3DCreate9)(UINT SDKVersion) ;
typedef HRESULT(WINAPI *pDirect3DCreate9Ex)(UINT SDKVersion, IDirect3D9Ex **ppD3D) ;

struct D3DTLVERTEX {
	float    x, y, z, rhw; // Position
	D3DCOLOR color;  // Vertex colour
	float    tu, tv;  // Texture coordinates
};
const DWORD D3DFVF_TLVERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

class DevState {
	public:
		IDirect3DDevice9 *dev;
		IDirect3DStateBlock9 *pSB;

		LONG initRefCount;
		LONG refCount;
		LONG myRefCount;
		DWORD dwMyThread;

		LPDIRECT3DTEXTURE9 tex[NUM_TEXTS];
		unsigned int uiCounter[NUM_TEXTS];

		DevState();

		void createCleanState();
		void releaseData();
		void releaseAll();
		void draw();
		void postDraw();
};

static map<IDirect3DDevice9 *, DevState *> devMap;
static bool bHooked = false;
static bool bChaining = false;
static bool bPresenting = false;

DevState::DevState() {
	dev = NULL;
	pSB = NULL;
	dwMyThread = 0;
	refCount = 0;
	myRefCount = 0;
	for (int i = 0;i < NUM_TEXTS;i++)
		tex[i] = NULL;
}

void DevState::releaseData() {
	ods("D3D9: Release Data");

	for (int i=0;i<NUM_TEXTS;i++)
		if (tex[i]) {
			tex[i]->Release();
			tex[i] = NULL;
		}
}

void DevState::releaseAll() {
	ods("D3D9: Release All");
	releaseData();
	if (pSB)
		pSB->Release();
	pSB = NULL;
}

void DevState::draw() {
	D3DVIEWPORT9 vp;
	dev->GetViewport(&vp);

	int idx = 0;

	vector<LPDIRECT3DTEXTURE9> texs;
	vector<unsigned int> widths;
	vector<unsigned int> yofs;
	vector<DWORD> colors;

	unsigned int y = 0;

	if (sm->fFontSize < 0.01f)
		sm->fFontSize = 0.01f;
	else if (sm->fFontSize > 1.0f)
		sm->fFontSize = 1.0f;

	int iHeight = lround(vp.Height * sm->fFontSize);

	if (iHeight > TEXT_HEIGHT)
		iHeight = TEXT_HEIGHT;

	float s = iHeight / 60.0f;

	ods("D3D9: Init: Scale %f. iH %d. Final scale %f", sm->fFontSize, iHeight, s);

	DWORD dwWaitResult = WaitForSingleObject(hSharedMutex, 50L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		for (int i=0;i<NUM_TEXTS;i++) {
			if (sm->texts[i].width == 0) {
				y += iHeight / 4;
			} else if (sm->texts[i].width > 0) {
				if (!tex[i] || (sm->texts[i].uiCounter != uiCounter[i])) {
					if (tex[i])
						tex[i]->Release();

					dev->CreateTexture(sm->texts[i].width, TEXT_HEIGHT, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tex[i], NULL);

					D3DLOCKED_RECT lr;
					tex[i]->LockRect(0, &lr, NULL, D3DLOCK_DISCARD);

					for (int r=0;r<TEXT_HEIGHT;r++) {
						unsigned char *dptr = reinterpret_cast<unsigned char *>(lr.pBits) + r * lr.Pitch;
						memcpy(dptr, sm->texts[i].texture + r * TEXT_WIDTH * 4, sm->texts[i].width * 4);
					}

					tex[i]->UnlockRect(0);
					uiCounter[i] = sm->texts[i].uiCounter;
				}
				unsigned int w = lround(sm->texts[i].width * s);
				texs.push_back(tex[i]);
				colors.push_back(sm->texts[i].color);
				widths.push_back(w);
				yofs.push_back(y);
				idx++;
				y += iHeight;
			}
		}
		ReleaseMutex(hSharedMutex);
	}

	if (idx == 0)
		return;

	int height = y;
	y = lround(vp.Height * sm->fY);

	if (sm->bTop) {
		y -= height;
	} else if (sm->bBottom) {
	} else {
		y -= height / 2;
	}


	if (y < 1)
		y = 1;
	if ((y + height + 1) > vp.Height)
		y = vp.Height - height - 1;

	for (int i=0;i<idx;i++) {
		unsigned int width = widths[i];

		int x = lround(vp.Width * sm->fX);

		if (sm->bLeft) {
			x -= width;
		} else if (sm->bRight) {
		} else {
			x -= width / 2;
		}

		if (x < 1)
			x = 1;
		if ((x + width + 1) > vp.Width)
			x = vp.Width - width - 1;

		D3DCOLOR color = colors[i];

		float left   = static_cast<float>(x);
		float top    = static_cast<float>(y + yofs[i]);
		float right  = left + width;
		float bottom = top + iHeight;

		const float z = 1.0f;
		D3DTLVERTEX vertices[4] = {
			// x, y, z, color, tu, tv
			{ left,  top,    z, 1, color, 0, 0 },
			{ right, top,    z, 1, color, 1, 0 },
			{ right, bottom, z, 1, color, 1, 1 },
			{ left,  bottom, z, 1, color, 0, 1 }
		};

		dev->SetTexture(0, texs[i]);
		dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(D3DTLVERTEX));
	}
}

void DevState::createCleanState() {
	DWORD dwOldThread = dwMyThread;
	if (dwOldThread) {
		ods("CreateCleanState from other thread.");
	}
	dwMyThread = GetCurrentThreadId();

	if (pSB)
		pSB->Release();
	pSB = NULL;

	IDirect3DStateBlock9* pStateBlock = NULL;
	dev->CreateStateBlock(D3DSBT_ALL, &pStateBlock);
	pStateBlock->Capture();

	dev->CreateStateBlock(D3DSBT_ALL, &pSB);

	D3DVIEWPORT9 vp;
	dev->GetViewport(&vp);

	dev->SetVertexShader(NULL);
	dev->SetPixelShader(NULL);
	dev->SetFVF(D3DFVF_TLVERTEX);

	dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	dev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // 0x16
	dev->SetRenderState(D3DRS_WRAP0, FALSE); // 0x80

	dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	dev->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
	dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	dev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	dev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	dev->SetRenderState(D3DRS_ZENABLE, FALSE);
	dev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	dev->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	dev->SetRenderState(D3DRS_COLORVERTEX, FALSE);

	dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	dev->SetRenderState(D3DRS_LIGHTING, FALSE);

	pSB->Capture();

	pStateBlock->Apply();
	pStateBlock->Release();

	dwMyThread = dwOldThread;
}

static HardHook hhCreateDevice;
static HardHook hhCreateDeviceEx;
static HardHook hhReset;
static HardHook hhAddRef;
static HardHook hhRelease;
static HardHook hhPresent;
static HardHook hhPresentEx;
static HardHook hhSwapPresent;

static void doPresent(IDirect3DDevice9 *idd) {
	DevState *ds = devMap[idd];

	if (ds && sm->bShow) {
		DWORD dwOldThread = ds->dwMyThread;
		if (dwOldThread)
			ods("doPresent from other thread");
		ds->dwMyThread = GetCurrentThreadId();

		IDirect3DSurface9 *pTarget = NULL;
		IDirect3DSurface9 *pRenderTarget = NULL;
		idd->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pTarget);
		idd->GetRenderTarget(0, &pRenderTarget);

		ods("D3D9: doPresent Back %p RenderT %p",pTarget,pRenderTarget);


		IDirect3DStateBlock9* pStateBlock = NULL;
		idd->CreateStateBlock(D3DSBT_ALL, &pStateBlock);
		pStateBlock->Capture();

		ds->pSB->Apply();

		if (pTarget != pRenderTarget)
			idd->SetRenderTarget(0, pTarget);

		if (sm->bReset) {
			sm->bReset = false;
			ds->releaseData();
		}

		idd->BeginScene();
		ds->draw();
		idd->EndScene();

		pStateBlock->Apply();
		pStateBlock->Release();

		pRenderTarget->Release();
		pTarget->Release();

		ods("Finished ref is %d %d", ds->myRefCount, ds->refCount);
		ds->dwMyThread = dwOldThread;
	}
}


typedef HRESULT(__stdcall *SwapPresentType)(IDirect3DSwapChain9 *, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *, DWORD);
static HRESULT __stdcall mySwapPresent(IDirect3DSwapChain9 * ids, CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion, DWORD dwFlags) {
	ods("D3D9: SwapChain Present");

	if (bPresenting) {
		ods("D3D9: Not doublepresenting in chain!");
	}  else {
		IDirect3DDevice9 *idd = NULL;
		ids->GetDevice(&idd);
		if (idd) {
			doPresent(idd);
			idd->Release();
		}
	}

	SwapPresentType oSwapPresent;
	oSwapPresent = (SwapPresentType) hhSwapPresent.call;

	hhSwapPresent.restore();
	HRESULT hr = oSwapPresent(ids, pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion,dwFlags);
	hhSwapPresent.inject();
	return hr;
}

typedef HRESULT(__stdcall *PresentType)(IDirect3DDevice9 *, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *);
static HRESULT __stdcall myPresent(IDirect3DDevice9 * idd, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion) {
	ods("D3D9: Device Present");

	doPresent(idd);

	PresentType oPresent = (PresentType) hhPresent.call;

	hhPresent.restore();
	HRESULT hr = oPresent(idd,pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion);
	hhPresent.inject();
	return hr;
}

typedef HRESULT(__stdcall *ResetType)(IDirect3DDevice9 *, D3DPRESENT_PARAMETERS *);
static HRESULT __stdcall myReset(IDirect3DDevice9 * idd, D3DPRESENT_PARAMETERS *param) {
	ods("D3D9: Chaining Reset");

	DevState *ds = devMap[idd];
	if (ds) {
		DWORD dwOldThread = ds->dwMyThread;
		if (dwOldThread)
			ods("myReset from other thread");
		ds->dwMyThread = GetCurrentThreadId();

		ds->releaseAll();
		ds->dwMyThread = dwOldThread;
	}


	ResetType oReset = (ResetType) hhReset.call;

	hhReset.restore();
	HRESULT hr=oReset(idd, param);
	hhReset.inject();

	ds->createCleanState();
	return hr;
}

typedef ULONG(__stdcall *AddRefType)(IDirect3DDevice9 *);

static ULONG __stdcall myAddRef(IDirect3DDevice9 *idd) {
	Mutex m;
	DevState *ds = devMap[idd];
	if (ds) {
		if (ds->dwMyThread == GetCurrentThreadId()) {
			ds->myRefCount++;
		} else
			ds->refCount++;
		return ds->refCount + ds->initRefCount;
	}
	AddRefType oAddRef = (AddRefType) hhAddRef.call;

	hhAddRef.restore();
	LONG res = oAddRef(idd);
	hhAddRef.inject();
	ods("D3D9: Chaining AddRef: %d", res);
	return res;
}

typedef ULONG(__stdcall *ReleaseType)(IDirect3DDevice9 *);
static ULONG __stdcall myRelease(IDirect3DDevice9 *idd) {
	Mutex m;
	DevState *ds = devMap[idd];

	if (ds) {
		if (ds->dwMyThread == GetCurrentThreadId()) {
			ds->myRefCount--;
			return ds->refCount + ds->initRefCount;
		} else {
			ds->refCount--;
		}

		if (ds->refCount >= 0)
			return ds->refCount + ds->initRefCount;

		ods("D3D9: Final release. MyRefs = %d, Tot = %d", ds->myRefCount, ds->refCount);

		DWORD dwOldThread = ds->dwMyThread;
		if (dwOldThread)
			ods("finalRelease from other thread");
		ds->dwMyThread = GetCurrentThreadId();

		ds->releaseAll();

		ds->dwMyThread = dwOldThread;

		ods("D3D9: Final release, MyRefs = %d Tot = %d", ds->myRefCount, ds->refCount);

		devMap.erase(idd);
		delete ds;
	}
	ReleaseType oRelease = (ReleaseType) hhRelease.call;
	hhRelease.restore();
	LONG res = oRelease(idd);
	hhRelease.inject();
	ods("D3D9: Chaining Release: %d", res);
	return res;
}

typedef HRESULT(__stdcall *CreateDeviceType)(IDirect3D9 *, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS *, IDirect3DDevice9 **);
static HRESULT __stdcall myCreateDevice(IDirect3D9 * id3d, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface) {
	ods("D3D9: Chaining CreateDevice");
	Mutex m;

//	BehaviorFlags &= ~D3DCREATE_PUREDEVICE;

	CreateDeviceType oCreateDevice = (CreateDeviceType) hhCreateDevice.call;

	hhCreateDevice.restore();
	HRESULT hr=oCreateDevice(id3d, Adapter,DeviceType,hFocusWindow,BehaviorFlags,pPresentationParameters,ppReturnedDeviceInterface);
	hhCreateDevice.inject();

	if (FAILED(hr))
		return hr;

	IDirect3DDevice9 *idd = *ppReturnedDeviceInterface;
	IDirect3DSwapChain9 *pSwap = NULL;

	// Get real interface, please.
	bool bfound;
	do {
		bfound = false;
		idd->GetSwapChain(0, &pSwap);
		if (pSwap) {
			IDirect3DDevice9 *idorig = NULL;
			if (SUCCEEDED(pSwap->GetDevice(&idorig))) {
				if (idorig != idd) {
					ods("Prepatched device, using original. %p => %p", idorig, idd);
					if (idd != *ppReturnedDeviceInterface)
						idd->Release();
					idd = idorig;
					bfound = true;
				} else {
					idorig->Release();
				}
			}
			pSwap->Release();
		}
	} while (bfound);

	DevState *ds = new DevState;
	ds->dev = idd;

	idd->AddRef();
	ds->initRefCount = idd->Release();

	devMap[idd] = ds;

	hhAddRef.setupInterface(idd, 1, reinterpret_cast<voidFunc>(myAddRef));
	hhRelease.setupInterface(idd, 2, reinterpret_cast<voidFunc>(myRelease));
	hhReset.setupInterface(idd, 16, reinterpret_cast<voidFunc>(myReset));
	hhPresent.setupInterface(idd, 17, reinterpret_cast<voidFunc>(myPresent));

	pSwap = NULL;
	idd->GetSwapChain(0, &pSwap);
	if (pSwap) {
		hhSwapPresent.setupInterface(pSwap, 3, reinterpret_cast<voidFunc>(mySwapPresent));
		pSwap->Release();
	} else {
		ods("D3D9: Failed to get swapchain");
	}

	ds->createCleanState();
	return hr;
}

typedef HRESULT(__stdcall *CreateDeviceExType)(IDirect3D9Ex *, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS *, D3DDISPLAYMODEEX *, IDirect3DDevice9Ex **);
static HRESULT __stdcall myCreateDeviceEx(IDirect3D9Ex * id3d, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, IDirect3DDevice9Ex** ppReturnedDeviceInterface) {
	Mutex m;
	ods("D3D9: Chaining CreateDeviceEx");

//	BehaviorFlags &= ~D3DCREATE_PUREDEVICE;

	CreateDeviceExType oCreateDeviceEx = (CreateDeviceExType) hhCreateDeviceEx.call;

	hhCreateDeviceEx.restore();
	HRESULT hr=oCreateDeviceEx(id3d, Adapter,DeviceType,hFocusWindow,BehaviorFlags,pPresentationParameters,pFullscreenDisplayMode,ppReturnedDeviceInterface);
	hhCreateDeviceEx.inject();

	if (FAILED(hr))
		return hr;

	IDirect3DDevice9Ex *idd = *ppReturnedDeviceInterface;

	DevState *ds = new DevState;
	ds->dev = idd;

	idd->AddRef();
	ds->initRefCount = idd->Release();

	devMap[idd] = ds;

	hhAddRef.setupInterface(idd, 1, reinterpret_cast<voidFunc>(myAddRef));
	hhRelease.setupInterface(idd, 2, reinterpret_cast<voidFunc>(myRelease));
	hhReset.setupInterface(idd, 16, reinterpret_cast<voidFunc>(myReset));
	hhPresent.setupInterface(idd, 17, reinterpret_cast<voidFunc>(myPresent));

	IDirect3DSwapChain9 *pSwap = NULL;
	idd->GetSwapChain(0, &pSwap);
	if (pSwap) {
		hhSwapPresent.setupInterface(pSwap, 3, reinterpret_cast<voidFunc>(mySwapPresent));
		pSwap->Release();
	} else {
		ods("D3D9: Failed to get swapchain for DevEx");
	}

	ds->createCleanState();
	return hr;
}

static void HookCreateRaw(voidFunc vfCreate) {
	ods("D3D9: Injecting CreateDevice Raw");
	hhCreateDevice.setup(vfCreate, reinterpret_cast<voidFunc>(myCreateDevice));
}

static void HookCreate(IDirect3D9 *pD3D) {
	ods("D3D9: Injecting CreateDevice");

	hhCreateDevice.setupInterface(pD3D, 16, reinterpret_cast<voidFunc>(myCreateDevice));
}

static void HookCreateEx(IDirect3D9Ex *pD3D) {
	ods("D3D9Ex: Injecting CreateDevice / CreateDeviceEx");

	hhCreateDevice.setupInterface(pD3D, 16, reinterpret_cast<voidFunc>(myCreateDevice));
	hhCreateDeviceEx.setupInterface(pD3D, 20, reinterpret_cast<voidFunc>(myCreateDeviceEx));
}

void checkD3D9Hook(bool preonly) {
	if (bChaining) {
		return;
		ods("D3D9: Causing a chain");
	}

	bChaining = true;

	HMODULE hD3D = GetModuleHandle("D3D9.DLL");

	if (hD3D != NULL) {
		if (! bHooked) {
			char procname[2048];
			GetModuleFileName(NULL, procname, 2048);
			fods("D3D9: CreateWnd in unhooked D3D App %s", procname);
			bHooked = true;

			// Add a ref to ourselves; we do NOT want to get unloaded directly from this process.
			GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<char *>(&HookCreate), &hSelf);

			// Can we use the prepatch data?
			GetModuleFileName(hD3D, procname, 2048);
			if (_stricmp(d3dd->cFileName, procname) == 0) {
				unsigned char *raw = (unsigned char *) hD3D;
				HookCreateRaw((voidFunc)(raw + d3dd->iOffsetCreate));
			} else if (! preonly) {
				fods("D3D9 Interface changed, can't rawpatch");
				pDirect3DCreate9 d3dc9 = reinterpret_cast<pDirect3DCreate9>(GetProcAddress(hD3D, "Direct3DCreate9"));
				ods("Got %p", d3dc9);
				if (d3dc9) {
					IDirect3D9 *id3d9 = d3dc9(D3D_SDK_VERSION);
					if (id3d9) {
						HookCreate(id3d9);
						id3d9->Release();
					} else {
						ods("Failed Direct3DCreate9");
					}
				} else {
					ods("D3D Library without Direct3DCreate9?");
				}
			} else {
				bHooked = false;
			}
		}
	}

	bChaining = false;
}

extern "C" __declspec(dllexport) void __cdecl PrepareD3D9() {
	ods("Preparing static data for D3D9 Injection");

	char buffb[2048];

	HMODULE hD3D = LoadLibrary("D3D9.DLL");
	HMODULE hRef;

	if (hD3D != NULL) {
		GetModuleFileName(hD3D, d3dd->cFileName, 2048);
		pDirect3DCreate9 d3dc9 = reinterpret_cast<pDirect3DCreate9>(GetProcAddress(hD3D, "Direct3DCreate9"));
		if (! d3dc9) {
			ods("D3D9 Library without Direct3DCreate9");
		} else {
			if (! GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (const char *) d3dc9, &hRef)) {
				ods("Failed to get module for D3D9");
			} else {
				GetModuleFileName(hRef, buffb, 2048);
				if (_stricmp(d3dd->cFileName, buffb) != 0) {
					ods("Direct3DCreate9 is not in D3D9 library");
				} else {
					IDirect3D9 *id3d9 = d3dc9(D3D_SDK_VERSION);
					if (id3d9) {
						void ***vtbl = (void ***) id3d9;
						void *pCreate = (*vtbl)[16];

						if (! GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (char *) pCreate, &hRef)) {
							ods("Failed to get module for CreateDevice");
						} else {
							GetModuleFileName(hRef, buffb, 2048);
							if (_stricmp(d3dd->cFileName, buffb) != 0) {
								ods("CreateDevice is not in D3D9 library");
							} else {
								unsigned char *b = (unsigned char *) pCreate;
								unsigned char *a = (unsigned char *) hD3D;
								d3dd->iOffsetCreate = b-a;
								ods("Successfully found prepatch offset: %p %p %p: %d", hD3D, d3dc9, pCreate, d3dd->iOffsetCreate);
							}
						}
						id3d9->Release();
					}
				}
			}
		}
		FreeLibrary(hD3D);
	}
}
