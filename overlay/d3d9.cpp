/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>

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

typedef IDirect3D9* (WINAPI *pDirect3DCreate9) (UINT SDKVersion) ;
typedef HRESULT (WINAPI *pDirect3DCreate9Ex) (UINT SDKVersion, IDirect3D9Ex **ppD3D) ;

struct D3DTLVERTEX
{
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
	bool bMyRefs;

	LPDIRECT3DTEXTURE9 tex[NUM_TEXTS];

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
	bMyRefs = false;
	refCount = 0;
	myRefCount = 0;
	for(int i = 0;i < NUM_TEXTS;i++)
		tex[i] = NULL;
}

void DevState::releaseData() {
	ods("D3D9: Release Data");

	for(int i=0;i<NUM_TEXTS;i++)
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
		for(int i=0;i<NUM_TEXTS;i++) {
			if (sm->texts[i].width == 0) {
				y += iHeight / 4;
			} else if (sm->texts[i].width > 0) {
				if (!tex[i] || sm->texts[i].bUpdated) {
					if (tex[i])
						tex[i]->Release();

					dev->CreateTexture(sm->texts[i].width, TEXT_HEIGHT, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tex[i], NULL);

					D3DLOCKED_RECT lr;
					tex[i]->LockRect(0, &lr, NULL, D3DLOCK_DISCARD);

					for(int r=0;r<TEXT_HEIGHT;r++) {
					    unsigned char *dptr = reinterpret_cast<unsigned char *>(lr.pBits) + r * lr.Pitch;
						memcpy(dptr, sm->texts[i].texture + r * TEXT_WIDTH * 4, sm->texts[i].width * 4);
					}

					tex[i]->UnlockRect(0);
					sm->texts[i].bUpdated = false;
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

	for(int i=0;i<idx;i++) {
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
		D3DTLVERTEX vertices[4] =
		{
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
	bool b = bMyRefs;
	bMyRefs = true;

	if (pSB)
		pSB->Release();
	pSB = NULL;

	IDirect3DStateBlock9* pStateBlock = NULL;
  	dev->CreateStateBlock( D3DSBT_ALL, &pStateBlock );
  	pStateBlock->Capture();

  	dev->CreateStateBlock( D3DSBT_ALL, &pSB );

	D3DVIEWPORT9 vp;
	dev->GetViewport(&vp);

	dev->SetVertexShader(NULL);
	dev->SetPixelShader(NULL);
	dev->SetFVF(D3DFVF_TLVERTEX);

	dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	dev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // 0x16
	dev->SetRenderState(D3DRS_WRAP0, FALSE); // 0x80

	dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	dev->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
	dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	dev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE );
	dev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER );

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

	bMyRefs = b;
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
		IDirect3DSurface9 *pTarget = NULL;
		IDirect3DSurface9 *pRenderTarget = NULL;
		idd->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pTarget);
		idd->GetRenderTarget(0, &pRenderTarget);

		ods("D3D9: doPresent Back %p RenderT %p",pTarget,pRenderTarget);

		bool b = ds->bMyRefs;
		ds->bMyRefs = true;

		IDirect3DStateBlock9* pStateBlock = NULL;
	  	idd->CreateStateBlock( D3DSBT_ALL, &pStateBlock );
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

		ds->bMyRefs = b;

		pRenderTarget->Release();
		pTarget->Release();
		bVideoHooked = true;
	}
}

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

	hhSwapPresent.restore();
	HRESULT hr = ids->Present(pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion,dwFlags);
	hhSwapPresent.inject();
	return hr;
}

static HRESULT __stdcall myPresent(IDirect3DDevice9 * idd, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion) {
	ods("D3D9: Device Present");

	doPresent(idd);

	hhPresent.restore();
	HRESULT hr = idd->Present(pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion);
	hhPresent.inject();
	return hr;
}

static HRESULT __stdcall myReset(IDirect3DDevice9 * idd, D3DPRESENT_PARAMETERS *param) {
	ods("D3D9: Chaining Reset");

	DevState *ds = devMap[idd];
	if (ds) {
		bool b = ds->bMyRefs;
		ds->bMyRefs = true;
		ds->releaseAll();
		ds->bMyRefs = b;
	}
	hhReset.restore();
	HRESULT hr=idd->Reset(param);
	hhReset.inject();

	ds->createCleanState();
	return hr;
}

static ULONG __stdcall myAddRef(IDirect3DDevice9 *idd) {
	DevState *ds = devMap[idd];
	if (ds) {
		if (ds->bMyRefs)
			ds->myRefCount++;
		else
			ds->refCount++;
		return ds->refCount + ds->initRefCount;
	}
	ods("D3D9: Unknown device, chaining AddRef");
	hhAddRef.restore();
	LONG res = idd->AddRef();
	hhAddRef.inject();
	return res;
}

static ULONG __stdcall myRelease(IDirect3DDevice9 *idd) {
	DevState *ds = devMap[idd];
	if (ds) {
		if (ds->bMyRefs) {
			ds->myRefCount--;
			return ds->refCount + ds->initRefCount;
		} else {
			ds->refCount--;
		}

		if (ds->refCount >= 0)
			return ds->refCount + ds->initRefCount;

		ods("D3D9: Final release");

		bool b = ds->bMyRefs;
		ds->bMyRefs = true;
		ds->releaseAll();
		ds->bMyRefs = b;

		devMap.erase(idd);
		delete ds;
	}
	ods("D3D9: Chaining Release");
	hhRelease.restore();
	LONG res = idd->Release();
	hhRelease.inject();
	return res;
}



static HRESULT __stdcall myCreateDevice(IDirect3D9 * id3d, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface) {
	ods("D3D9: Chaining CreateDevice");

//	BehaviorFlags &= ~D3DCREATE_PUREDEVICE;

	hhCreateDevice.restore();
	HRESULT hr=id3d->CreateDevice(Adapter,DeviceType,hFocusWindow,BehaviorFlags,pPresentationParameters,ppReturnedDeviceInterface);
	hhCreateDevice.inject();

	if (FAILED(hr))
		return hr;

	IDirect3DDevice9 *idd = *ppReturnedDeviceInterface;

	DevState *ds = new DevState;
	ds->dev = idd;

	idd->AddRef();
	ds->initRefCount = idd->Release();

	devMap[idd] = ds;

	hhAddRef.setupInterface(idd, 1, reinterpret_cast<voidFunc>(myAddRef));
	hhAddRef.inject();

	hhRelease.setupInterface(idd, 2, reinterpret_cast<voidFunc>(myRelease));
	hhRelease.inject();

	hhReset.setupInterface(idd, 16, reinterpret_cast<voidFunc>(myReset));
	hhReset.inject();

	hhPresent.setupInterface(idd, 17, reinterpret_cast<voidFunc>(myPresent));
	hhPresent.inject();

	IDirect3DSwapChain9 *pSwap = NULL;
	idd->GetSwapChain(0, &pSwap);
	if (pSwap) {
		hhSwapPresent.setupInterface(pSwap, 3, reinterpret_cast<voidFunc>(mySwapPresent));
		hhSwapPresent.inject();
		pSwap->Release();
	} else {
		ods("D3D9: Failed to get swapchain");
	}

	ds->createCleanState();
	return hr;
}

static HRESULT __stdcall myCreateDeviceEx(IDirect3D9Ex * id3d, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, IDirect3DDevice9Ex** ppReturnedDeviceInterface) {
	ods("D3D9: Chaining CreateDeviceEx");

//	BehaviorFlags &= ~D3DCREATE_PUREDEVICE;

	hhCreateDeviceEx.restore();
	HRESULT hr=id3d->CreateDeviceEx(Adapter,DeviceType,hFocusWindow,BehaviorFlags,pPresentationParameters,pFullscreenDisplayMode,ppReturnedDeviceInterface);
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
	hhAddRef.inject();

	hhRelease.setupInterface(idd, 2, reinterpret_cast<voidFunc>(myRelease));
	hhRelease.inject();

	hhReset.setupInterface(idd, 16, reinterpret_cast<voidFunc>(myReset));
	hhReset.inject();

	hhPresent.setupInterface(idd, 17, reinterpret_cast<voidFunc>(myPresent));
	hhPresent.inject();

	IDirect3DSwapChain9 *pSwap = NULL;
	idd->GetSwapChain(0, &pSwap);
	if (pSwap) {
		hhSwapPresent.setupInterface(pSwap, 3, reinterpret_cast<voidFunc>(mySwapPresent));
		hhSwapPresent.inject();
		pSwap->Release();
	} else {
		ods("D3D9: Failed to get swapchain for DevEx");
	}

	ds->createCleanState();
	return hr;
}

static void HookCreate(IDirect3D9 *pD3D) {
	ods("D3D9: Injecting CreateDevice");

	// Add a ref to ourselves; we do NOT want to get unloaded directly from this process.
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<char *>(&HookCreate), &hSelf);

	hhCreateDevice.setupInterface(pD3D, 16, reinterpret_cast<voidFunc>(myCreateDevice));
	hhCreateDevice.inject();
}

static void HookCreateEx(IDirect3D9Ex *pD3D) {
	ods("D3D9Ex: Injecting CreateDevice / CreateDeviceEx");

	// Add a ref to ourselves; we do NOT want to get unloaded directly from this process.
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<char *>(&HookCreate), &hSelf);

	hhCreateDevice.setupInterface(pD3D, 16, reinterpret_cast<voidFunc>(myCreateDevice));
	hhCreateDevice.inject();

	hhCreateDeviceEx.setupInterface(pD3D, 20, reinterpret_cast<voidFunc>(myCreateDeviceEx));
	hhCreateDeviceEx.inject();
}


void checkD3D9Hook() {
	if (bChaining) {
		return;
		ods("D3D9: Causing a chain");
	}

	bChaining = true;

	HMODULE hD3D = GetModuleHandle("D3D9.DLL");

	if (hD3D != NULL) {
		if (! bHooked) {
			char procname[1024];
			GetModuleFileName(NULL, procname, 1024);
			ods("D3D9: CreateWnd in unhooked D3D App %s", procname);
			bHooked = true;

			pDirect3DCreate9 d3dc9 = reinterpret_cast<pDirect3DCreate9>(GetProcAddress(hD3D, "Direct3DCreate9"));
			if (d3dc9) {
				IDirect3D9 *id3d9 = d3dc9(D3D_SDK_VERSION);
				if (id3d9) {
					HookCreate(id3d9);
					IDirect3D9_Release(id3d9);
				}
			}

			pDirect3DCreate9Ex d3dc9ex = reinterpret_cast<pDirect3DCreate9Ex>(GetProcAddress(hD3D, "Direct3DCreate9Ex"));
			if (d3dc9ex) {
				IDirect3D9Ex *id3d9ex = NULL;
				d3dc9ex(D3D_SDK_VERSION, &id3d9ex);
				if (id3d9ex) {
					HookCreateEx(id3d9ex);
					IDirect3D9Ex_Release(id3d9ex);
				}
			}
		}
	}

	bChaining = false;
}
