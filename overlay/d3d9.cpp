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

#include "lib.h"
#include <d3d9.h>
#include <time.h>

Direct3D9Data *d3dd = NULL;

typedef IDirect3D9* (WINAPI *pDirect3DCreate9)(UINT SDKVersion) ;
typedef HRESULT (WINAPI *pDirect3DCreate9Ex)(UINT SDKVersion, IDirect3D9Ex **ppD3D) ;

template<class T>
class Stash {
public:
	Stash(T *variable, T newValue)
		: var(variable)
		, value(*var) {
		*variable = newValue;
	}
	~Stash() {
		*var = value;
	}

private:
	T *var;
	T value;
};

struct D3DTLVERTEX {
	float    x, y, z, rhw; // Position
	float    tu, tv;  // Texture coordinates
};
static const DWORD D3DFVF_TLVERTEX = D3DFVF_XYZRHW | D3DFVF_TEX1;

class DevState : public Pipe {
	public:
		IDirect3DDevice9 *dev;
		IDirect3DStateBlock9 *pSB;

		/// Non-Win8: Initial ref count, directly after device creation.
		/// Win8: Unused
		LONG initRefCount;
		LONG refCount;
		/// Refcount of self (library). Used to make the overlay transparent
		/// (invisible) to the outside.
		LONG myRefCount;
		DWORD dwMyThread;

		D3DTLVERTEX vertices[4];
		LPDIRECT3DTEXTURE9 texTexture;

		clock_t timeT;
		unsigned int frameCount;

		DevState();

		void createCleanState();
		void releaseData();
		void releaseAll();
		void draw();

		virtual void blit(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
		virtual void setRect();
		virtual void newTexture(unsigned int width, unsigned int height);
};

/// Vtable offset; see d3d9.h of win-/D3D-API.
/// 3 from IUnknown + 14 in IDirect3D9, 0-based => 16
static const int VTABLE_OFFSET_ID3D_CREATEDEVICE = 16;
/// Vtable offset; see d3d9.h of win-/D3D-API.
/// Offset: 3 from IUnknown + 14 from IDirect3D9 + 4 in IDirect3D9Ex, 0-based => 20
static const int VTABLE_OFFSET_ID3D_CREATEDEVICE_EX = 20;

typedef map<IDirect3DDevice9 *, DevState *> DevMapType;
static DevMapType devMap;
static bool bHooked = false;

DevState::DevState() {
	dev = NULL;
	pSB = NULL;
	dwMyThread = 0;
	initRefCount = 0;
	refCount = 0;
	myRefCount = 0;
	texTexture = NULL;

	timeT = clock();
	frameCount = 0;

	for (int i = 0; i < 4; ++i) {
		vertices[i].x = vertices[i].y = 0.0f;
		vertices[i].tu = vertices[i].tv = 0.0f;
		vertices[i].z = vertices[i].rhw = 1.0f;
	}
}

void DevState::releaseData() {
	ods("D3D9: Release Data");

	if (texTexture) {
		texTexture->Release();
		texTexture = NULL;
	}
}

void DevState::blit(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	// Blit is called often. Thus, we do not want to always log here.
	#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
	ods("D3D9: Blit %d %d %d %d", x, y, w, h);
	#endif

	if (! texTexture || !a_ucTexture)
		return;

	D3DLOCKED_RECT lr;

	if ((x == 0) && (y == 0) && (w == uiWidth) && (h == uiHeight)) {
		if (texTexture->LockRect(0, &lr, NULL, D3DLOCK_DISCARD) != D3D_OK)
			return;
	} else {
		RECT r;

		r.left = x;
		r.top = y;
		r.right = x + w;
		r.bottom = y + h;

		if (texTexture->LockRect(0, &lr, &r, 0) != D3D_OK)
			return;
	}

	for (unsigned int r=0;r < h;++r) {
		unsigned char *dptr = reinterpret_cast<unsigned char *>(lr.pBits) + r * lr.Pitch;
		unsigned char *sptr = a_ucTexture + 4 * ((y + r) * uiWidth + x);
		memcpy(dptr, sptr, w * 4);
	}

	texTexture->UnlockRect(0);
}

void DevState::setRect() {
	ods("D3D9: New subrect");

	float w = static_cast<float>(uiWidth);
	float h = static_cast<float>(uiHeight);

	float left   = static_cast<float>(uiLeft) - 0.5f;
	float top    = static_cast<float>(uiTop) - 0.5f;
	float right  = static_cast<float>(uiRight) + 0.5f;
	float bottom = static_cast<float>(uiBottom) + 0.5f;

	float texl = (left) / w;
	float text = (top) / h;
	float texr = (right + 1.0f) / w;
	float texb = (bottom + 1.0f) / h;

	vertices[0].x = left;
	vertices[0].y = top;
	vertices[0].tu = texl;
	vertices[0].tv = text;

	vertices[1].x = right;
	vertices[1].y = top;
	vertices[1].tu = texr;
	vertices[1].tv = text;

	vertices[2].x = right;
	vertices[2].y = bottom;
	vertices[2].tu = texr;
	vertices[2].tv = texb;

	vertices[3].x = left;
	vertices[3].y = bottom;
	vertices[3].tu = texl;
	vertices[3].tv = texb;
}

void DevState::newTexture(unsigned int width, unsigned int height) {
	ods("D3D9: New texture %d x %d", width, height);

	if (texTexture) {
		texTexture->Release();
		texTexture = NULL;
	}

	dev->CreateTexture(uiWidth, uiHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texTexture, NULL);

	for (int i = 0; i < 4; ++i) {
		vertices[i].x = vertices[i].y = vertices[i].z = 0.0f;
		vertices[i].tu = vertices[i].tv = 0.0f;
		vertices[i].rhw = 1.0f;
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
	clock_t t = clock();
	float elapsed = static_cast<float>(t - timeT) / CLOCKS_PER_SEC;
	++frameCount;
	if (elapsed > OVERLAY_FPS_INTERVAL) {
		OverlayMsg om;
		om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType = OVERLAY_MSGTYPE_FPS;
		om.omh.iLength = sizeof(OverlayMsgFps);
		om.omf.fps = frameCount / elapsed;

		sendMessage(om);

		frameCount = 0;
		timeT = t;
	}

	D3DVIEWPORT9 vp;
	dev->GetViewport(&vp);

	checkMessage(vp.Width, vp.Height);

	if (! a_ucTexture || (uiLeft == uiRight))
		return;

	if (! texTexture) {
		unsigned int l, r, t, b;
		l = uiLeft;
		r = uiRight;
		t = uiTop;
		b = uiBottom;
		newTexture(uiWidth, uiHeight);
		blit(0, 0, uiWidth, uiHeight);

		uiLeft = l;
		uiRight = r;
		uiTop = t;
		uiBottom = b;
		setRect();
	}

	dev->SetTexture(0, texTexture);
	dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(D3DTLVERTEX));
}

void DevState::createCleanState() {
	if (dwMyThread != 0) {
		ods("D3D9: CreateCleanState from other thread.");
	}
	Stash<DWORD> stashThread(&dwMyThread, GetCurrentThreadId());

	if (pSB)
		pSB->Release();
	pSB = NULL;

	IDirect3DStateBlock9* pStateBlock = NULL;
	dev->CreateStateBlock(D3DSBT_ALL, &pStateBlock);
	if (! pStateBlock)
		return;

	pStateBlock->Capture();

	dev->CreateStateBlock(D3DSBT_ALL, &pSB);
	if (! pSB) {
		pStateBlock->Release();
		return;
	}

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
	dev->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ONE);
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
}

static HardHook hhCreateDevice;
static HardHook hhCreateDeviceEx;
static HardHook hhReset;
static HardHook hhResetEx;
static HardHook hhAddRef;
static HardHook hhRelease;
static HardHook hhPresent;
static HardHook hhPresentEx;
static HardHook hhSwapPresent;

static void doPresent(IDirect3DDevice9 *idd) {
	DevMapType::iterator it = devMap.find(idd);
	DevState *ds = it != devMap.end() ? it->second : NULL;

	if (ds && ds->pSB) {
		if (ds->dwMyThread != 0) {
			ods("D3D9: doPresent from other thread");
		}
		Stash<DWORD> stashThread(&(ds->dwMyThread), GetCurrentThreadId());

		IDirect3DSurface9 *pTarget = NULL;
		IDirect3DSurface9 *pRenderTarget = NULL;
		idd->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pTarget);
		idd->GetRenderTarget(0, &pRenderTarget);

		// Present is called for each frame. Thus, we do not want to always log here.
		#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
		ods("D3D9: doPresent BackB %p RenderT %p", pTarget, pRenderTarget);
		#endif

		IDirect3DStateBlock9* pStateBlock = NULL;
		idd->CreateStateBlock(D3DSBT_ALL, &pStateBlock);
		pStateBlock->Capture();

		ds->pSB->Apply();

		if (pTarget != pRenderTarget)
			idd->SetRenderTarget(0, pTarget);

		idd->BeginScene();
		ds->draw();
		idd->EndScene();

		pStateBlock->Apply();
		pStateBlock->Release();

		pRenderTarget->Release();
		pTarget->Release();

//		ods("D3D9: Finished ref is %d %d", ds->myRefCount, ds->refCount);
	}
}


typedef HRESULT(__stdcall *SwapPresentType)(IDirect3DSwapChain9 *, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *, DWORD);
static HRESULT __stdcall mySwapPresent(IDirect3DSwapChain9 *ids, CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion, DWORD dwFlags) {
	// Present is called for each frame. Thus, we do not want to always log here.
	#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
	ods("D3D9: SwapChain Present");
	#endif

	IDirect3DDevice9 *idd = NULL;
	ids->GetDevice(&idd);
	if (idd) {
		doPresent(idd);
		idd->Release();
	}

	//TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	SwapPresentType oSwapPresent = (SwapPresentType) hhSwapPresent.call;
	hhSwapPresent.restore();
	HRESULT hr = oSwapPresent(ids, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
	hhSwapPresent.inject();

	return hr;
}

typedef HRESULT(__stdcall *PresentType)(IDirect3DDevice9 *, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *);
static HRESULT __stdcall myPresent(IDirect3DDevice9 *idd, CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride,CONST RGNDATA *pDirtyRegion) {
	// Present is called for each frame. Thus, we do not want to always log here.
	#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
	ods("D3D9: Device Present");
	#endif

	doPresent(idd);

	//TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	PresentType oPresent = (PresentType) hhPresent.call;
	hhPresent.restore();
	HRESULT hr = oPresent(idd, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	hhPresent.inject();

	return hr;
}

typedef HRESULT(__stdcall *PresentExType)(IDirect3DDevice9Ex *, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *, DWORD);
static HRESULT __stdcall myPresentEx(IDirect3DDevice9Ex *idd, CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion, DWORD dwFlags) {
	// Present is called for each frame. Thus, we do not want to always log here.
	#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
	ods("D3D9: Device Present Ex");
	#endif

	doPresent(idd);

	PresentExType oPresentEx = (PresentExType) hhPresentEx.call;
	hhPresentEx.restore();
	HRESULT hr = oPresentEx(idd, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
	hhPresentEx.inject();

	return hr;
}

typedef HRESULT(__stdcall *ResetType)(IDirect3DDevice9 *, D3DPRESENT_PARAMETERS *);
static HRESULT __stdcall myReset(IDirect3DDevice9 *idd, D3DPRESENT_PARAMETERS *param) {
	ods("D3D9: Chaining Reset");

	DevMapType::iterator it = devMap.find(idd);
	DevState *ds = it != devMap.end() ? it->second : NULL;
	if (ds) {
		if (ds->dwMyThread != 0) {
			ods("D3D9: myReset from other thread");
		}
		Stash<DWORD> stashThread(&(ds->dwMyThread), GetCurrentThreadId());

		ds->releaseAll();
	}

	//TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	ResetType oReset = (ResetType) hhReset.call;
	hhReset.restore();
	HRESULT hr = oReset(idd, param);
	hhReset.inject();

	if (ds)
		ds->createCleanState();

	return hr;
}

typedef HRESULT(__stdcall *ResetExType)(IDirect3DDevice9Ex *, D3DPRESENT_PARAMETERS *, D3DDISPLAYMODEEX *);
static HRESULT __stdcall myResetEx(IDirect3DDevice9Ex *idd, D3DPRESENT_PARAMETERS *param, D3DDISPLAYMODEEX *param2) {
	ods("D3D9: Chaining ResetEx");

	DevMapType::iterator it = devMap.find(idd);
	DevState *ds = it != devMap.end() ? it->second : NULL;
	if (ds) {
		if (ds->dwMyThread) {
			ods("D3D9: myResetEx from other thread");
		}
		Stash<DWORD> stashThread(&(ds->dwMyThread), GetCurrentThreadId());

		ds->releaseAll();
	}

	//TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	ResetExType oResetEx = (ResetExType) hhResetEx.call;
	hhResetEx.restore();
	HRESULT hr = oResetEx(idd, param, param2);
	hhResetEx.inject();

	if (ds)
		ds->createCleanState();

	return hr;
}

typedef ULONG(__stdcall *AddRefType)(IDirect3DDevice9 *);

static ULONG __stdcall myAddRef(IDirect3DDevice9 *idd) {
	Mutex m;

	DevMapType::iterator it = devMap.find(idd);
	DevState *ds = it != devMap.end() ? it->second : NULL;
	if (ds) {
		// AddRef is called very often. Thus, we do not want to always log here.
		#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
		ods("D3D9: Using own Refcount implementation for call to AddRef.");
		#endif

		if (ds->dwMyThread == GetCurrentThreadId()) {
			ds->myRefCount++;
		} else {
			ds->refCount++;
		}
		return ds->initRefCount + ds->refCount;
	}

	//TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	AddRefType oAddRef = (AddRefType) hhAddRef.call;
	hhAddRef.restore();
	ULONG res = oAddRef(idd);
	hhAddRef.inject();

	// AddRef is called very often. Thus, we do not want to always log here.
	#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
	ods("D3D9: Chained AddRef with result %d", res);
	#endif

	return res;
}

static ULONG __stdcall myWin8AddRef(IDirect3DDevice9 *idd) {
	Mutex m;

	DevMapType::iterator it = devMap.find(idd);
	DevState *ds = it != devMap.end() ? it->second : NULL;
	if (ds && ds->dwMyThread == GetCurrentThreadId()) {
		// AddRef is called very often. Thus, we do not want to always log here.
		#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
		ods("D3D9: Using own Refcount implementation for call to AddRef (Win8).");
		#endif

		ds->myRefCount++;
		return ds->refCount;
	}

	//TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	AddRefType oAddRef = (AddRefType) hhAddRef.call;
	hhAddRef.restore();
	ULONG res = oAddRef(idd);
	hhAddRef.inject();

	if (ds)
		ds->refCount = res;

	// AddRef is called very often. Thus, we do not want to always log here.
	#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
	ods("D3D9: Chained AddRef (Win8) with result %d", res);
	#endif

	return res;
}

typedef ULONG(__stdcall *ReleaseType)(IDirect3DDevice9 *);

static ULONG __stdcall myRelease(IDirect3DDevice9 *idd) {
	Mutex m;

	DevMapType::iterator it = devMap.find(idd);
	DevState *ds = it != devMap.end() ? it->second : NULL;
	if (ds) {
		// Release is called very often. Thus, we do not want to always log here.
		#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
		ods("D3D9: Using own Refcount implementation for call to Release.");
		#endif

		if (ds->dwMyThread == GetCurrentThreadId()) {
			ds->myRefCount--;
			return ds->initRefCount + ds->refCount;
		} else {
			ds->refCount--;
		}

		if (ds->refCount <= 1) {
			ds->disconnect();
		}

		if (ds->refCount >= 0) {
			return ds->initRefCount + ds->refCount;
		}

		ods("D3D9: Final release is following. MyRefs = %d, Tot = %d", ds->myRefCount, ds->refCount);

		if (ds->dwMyThread != 0) {
			ods("D3D9: finalRelease from other thread");
		}

		// Codeblock for stashing threadid
		{
			Stash<DWORD> stashThread(&(ds->dwMyThread), GetCurrentThreadId());

			ds->releaseAll();
		}

		ods("D3D9: Final release of %p. MyRefs = %d Tot = %d", idd, ds->myRefCount, ds->refCount);

		devMap.erase(it);
		delete ds;
		ds = NULL;
	}

	//TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	ReleaseType oRelease = (ReleaseType) hhRelease.call;
	hhRelease.restore();
	ULONG res = oRelease(idd);
	hhRelease.inject();

	// Release is called very often. Thus, we do not want to always log here.
	#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
	ods("D3D9: Chained Release with result %d", res);
	#endif

	return res;
}

static ULONG __stdcall myWin8Release(IDirect3DDevice9 *idd) {
	Mutex m;

	DevMapType::iterator it = devMap.find(idd);
	DevState *ds = it != devMap.end() ? it->second : NULL;
	if (ds) {
		// Release is called very often. Thus, we do not want to always log here.
		#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
		ods("D3D9: Using own Refcount implementation for call to Release.");
		#endif

		if (ds->dwMyThread == GetCurrentThreadId()) {
			ds->myRefCount--;
			return ds->refCount;
		}
		if (ds->refCount == 1) {
			ds->disconnect();

			ods("D3D9: Final release. MyRefs = %d, Tot = %d", ds->myRefCount, ds->refCount);

			if (ds->dwMyThread != 0) {
				ods("D3D9: finalRelease from other thread");
			}

			// Codeblock for stashing threadid
			{
				Stash<DWORD> stashThread(&(ds->dwMyThread), GetCurrentThreadId());

				ds->releaseAll();
			}

			ods("D3D9: Final release of %p. MyRefs = %d Tot = %d", idd, ds->myRefCount, ds->refCount);

			devMap.erase(it);
			delete ds;
			ds = NULL;
		}
	}

	//TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	ReleaseType oRelease = (ReleaseType) hhRelease.call;
	hhRelease.restore();
	ULONG res = oRelease(idd);
	hhRelease.inject();

	if (ds)
		ds->refCount = res;

	// Release is called very often. Thus, we do not want to always log here.
	#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
	ods("D3D9: Chained Release (Win8) with result: %d", res);
	#endif

	return res;
}

static IDirect3DDevice9* findOriginalDevice(IDirect3DDevice9 *device) {

	IDirect3DSwapChain9 *pSwap = NULL;
	device->GetSwapChain(0, &pSwap);
	if (pSwap) {

		IDirect3DDevice9 *originalDevice = NULL;
		if (SUCCEEDED(pSwap->GetDevice(&originalDevice))) {

			if (originalDevice == device) {
				// Found the original device. Release responsibility is passed
				// to the caller.
			} else {
				device = findOriginalDevice(originalDevice);
				originalDevice->Release();
			}

		} else {
			ods("D3D9: Failed to recurse to find original device. Could not get Device from Swapchain.");
		}

		pSwap->Release();
	} else {
		ods("D3D9: Failed to recurse to find original device. Could not get Swapchain.");
	}

	return device;
}

typedef HRESULT(__stdcall *CreateDeviceType)(IDirect3D9 *, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS *, IDirect3DDevice9 **);
static HRESULT __stdcall myCreateDevice(IDirect3D9 *id3d, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface) {
	Mutex m;

	ods("D3D9: Chaining CreateDevice");

//	BehaviorFlags &= ~D3DCREATE_PUREDEVICE;

	//TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	CreateDeviceType oCreateDevice = (CreateDeviceType) hhCreateDevice.call;
	hhCreateDevice.restore();
	HRESULT hr = oCreateDevice(id3d, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
	hhCreateDevice.inject();

	if (FAILED(hr))
		return hr;

	IDirect3DDevice9 *idd = *ppReturnedDeviceInterface;

	// Get real interface, please.
	IDirect3DDevice9 *originalDevice = findOriginalDevice(idd);
	if (idd != originalDevice) {
		ods("D3D9: Prepatched device, using original. %p => %p", idd, originalDevice);
		idd = originalDevice;
	}

	DevState *ds = new DevState;
	ds->dev = idd;

	idd->AddRef();
	ds->initRefCount = idd->Release();

	DevMapType::iterator it = devMap.find(idd);
	if (it != devMap.end()) {
		ods("Device exists in devMap already - canceling injection into device. Thread prev: %d ; new: %d", it->second->dwMyThread, GetCurrentThreadId());
		delete ds;

		return hr;
	}
	devMap[idd] = ds;

	// The offsets are dependent on the declaration order of the struct.
	// See IDirect3DDevice9 (2nd, 3rd, 17th, 18th functions)
	const unsigned int offsetAddref = 1;
	const unsigned int offsetRelease = 2;
	const unsigned int offsetReset = 16;
	const unsigned int offsetPresent = 17;
	if (bIsWin8) {
		hhAddRef.setupInterface(idd, offsetAddref, reinterpret_cast<voidFunc>(myWin8AddRef));
		hhRelease.setupInterface(idd, offsetRelease, reinterpret_cast<voidFunc>(myWin8Release));
	} else {
		hhAddRef.setupInterface(idd, offsetAddref, reinterpret_cast<voidFunc>(myAddRef));
		hhRelease.setupInterface(idd, offsetRelease, reinterpret_cast<voidFunc>(myRelease));
	}
	hhReset.setupInterface(idd, offsetReset, reinterpret_cast<voidFunc>(myReset));
	hhPresent.setupInterface(idd, offsetPresent, reinterpret_cast<voidFunc>(myPresent));

	IDirect3DSwapChain9 *pSwap = NULL;
	idd->GetSwapChain(0, &pSwap);
	if (pSwap) {
		// The offset is dependent on the declaration order of the struct.
		// See IDirect3DSwapChain9 (Present is the fourth function)
		const unsigned int offsetPresent = 3;
		hhSwapPresent.setupInterface(pSwap, offsetPresent, reinterpret_cast<voidFunc>(mySwapPresent));
		pSwap->Release();
	} else {
		ods("D3D9: Failed to get swapchain");
	}

	ds->createCleanState();

	return hr;
}

typedef HRESULT(__stdcall *CreateDeviceExType)(IDirect3D9Ex *, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS *, D3DDISPLAYMODEEX *, IDirect3DDevice9Ex **);
static HRESULT __stdcall myCreateDeviceEx(IDirect3D9Ex *id3d, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, D3DDISPLAYMODEEX *pFullscreenDisplayMode, IDirect3DDevice9Ex **ppReturnedDeviceInterface) {
	Mutex m;
	ods("D3D9: Chaining CreateDeviceEx");

//	BehaviorFlags &= ~D3DCREATE_PUREDEVICE;

	//TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	CreateDeviceExType oCreateDeviceEx = (CreateDeviceExType) hhCreateDeviceEx.call;
	hhCreateDeviceEx.restore();
	HRESULT hr = oCreateDeviceEx(id3d, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, pFullscreenDisplayMode, ppReturnedDeviceInterface);
	hhCreateDeviceEx.inject();

	if (FAILED(hr))
		return hr;

	IDirect3DDevice9Ex *idd = *ppReturnedDeviceInterface;

	DevState *ds = new DevState;
	ds->dev = idd;

	idd->AddRef();
	ds->initRefCount = idd->Release();

	DevMapType::iterator it = devMap.find(idd);
	if (it != devMap.end()) {
		ods("Device exists in devMap already - canceling injection into device. Thread prev: %d ; new: %d", it->second->dwMyThread, GetCurrentThreadId());
		delete ds;

		return hr;
	}
	devMap[idd] = ds;

	// The offsets are dependent on the declaration order of the struct.
	// See IDirect3DDevice9 (2nd, 3rd, 17th, 18th functions)
	const unsigned int offsetAddref = 1;
	const unsigned int offsetRelease = 2;
	const unsigned int offsetReset = 16;
	const unsigned int offsetPresent = 17;
	// On IDirect3DDevice9Ex
	const unsigned int offsetPresentEx = 121;
	const unsigned int offsetResetEx = 132;
	if (bIsWin8) {
		hhAddRef.setupInterface(idd, offsetAddref, reinterpret_cast<voidFunc>(myWin8AddRef));
		hhRelease.setupInterface(idd, offsetRelease, reinterpret_cast<voidFunc>(myWin8Release));
	} else {
		hhAddRef.setupInterface(idd, offsetAddref, reinterpret_cast<voidFunc>(myAddRef));
		hhRelease.setupInterface(idd, offsetRelease, reinterpret_cast<voidFunc>(myRelease));
	}
	hhReset.setupInterface(idd, offsetReset, reinterpret_cast<voidFunc>(myReset));
	hhResetEx.setupInterface(idd, offsetResetEx, reinterpret_cast<voidFunc>(myResetEx));
	hhPresent.setupInterface(idd, offsetPresent, reinterpret_cast<voidFunc>(myPresent));
	hhPresentEx.setupInterface(idd, offsetPresentEx, reinterpret_cast<voidFunc>(myPresentEx));

	IDirect3DSwapChain9 *pSwap = NULL;
	idd->GetSwapChain(0, &pSwap);
	if (pSwap) {
		// The offset is dependent on the declaration order of the struct.
		// See IDirect3DSwapChain9 (Present is the fourth function)
		const unsigned int offsetPresent = 3;
		hhSwapPresent.setupInterface(pSwap, offsetPresent, reinterpret_cast<voidFunc>(mySwapPresent));
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

static void HookCreateRawEx(voidFunc vfCreate) {
	ods("D3D9: Injecting CreateDeviceEx Raw");
	hhCreateDeviceEx.setup(vfCreate, reinterpret_cast<voidFunc>(myCreateDeviceEx));
}

static void HookCreate(IDirect3D9 *pD3D) {
	ods("D3D9: Injecting CreateDevice");

	hhCreateDevice.setupInterface(pD3D, VTABLE_OFFSET_ID3D_CREATEDEVICE, reinterpret_cast<voidFunc>(myCreateDevice));
}

static void HookCreateEx(IDirect3D9Ex *pD3D) {
	ods("D3D9Ex: Injecting CreateDevice / CreateDeviceEx");

	// Best effort hooking for the non-ex create method. If it was set up
	// previously this setup call is safely ignored by the hook.
	HookCreate(pD3D);

	hhCreateDeviceEx.setupInterface(pD3D, VTABLE_OFFSET_ID3D_CREATEDEVICE_EX, reinterpret_cast<voidFunc>(myCreateDeviceEx));
}

static void hookD3D9(HMODULE hD3D, bool preonly);

// @param preonly If rawpatching the createdevice-functions fails, don't try to
//                patch Direct3DCreate9.
//                Should be true on PROC_ATTACH, and false on THREAD_ATTACH. (?)
void checkD3D9Hook(bool preonly) {
	static bool bCheckHookActive = false;
	if (bCheckHookActive) {
		ods("D3D9: Recursion in checkD3D9Hook");
		return;
	}

	bCheckHookActive = true;

	HMODULE hD3D = GetModuleHandle("D3D9.DLL");

	if (hD3D != NULL) {
		if (! bHooked) {
			hookD3D9(hD3D, preonly);
		}
	} else {
		#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
		ods("D3D9: No D3D9.DLL found as loaded. No hooking at this point.");
		#endif
	}

	bCheckHookActive = false;
}

static void hookD3D9(HMODULE hD3D, bool preonly) {

	char procname[MODULEFILEPATH_BUFLEN];
	GetModuleFileName(NULL, procname, ARRAY_NUM_ELEMENTS(procname));
	ods("D3D9: hookD3D9 in App '%s'", procname);

	// Add a ref to ourselves; we do NOT want to get unloaded directly from this process.
	HMODULE hTempSelf = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<char *>(&hookD3D9), &hTempSelf);

	bHooked = true;

	// Can we use the prepatch data?
	wchar_t modulename[MODULEFILEPATH_BUFLEN];
	GetModuleFileNameW(hD3D, modulename, ARRAY_NUM_ELEMENTS(modulename));
	if (_wcsicmp(d3dd->wcFileName, modulename) == 0) {
		// The module seems to match the one we prepared d3dd for.

		unsigned char *raw = (unsigned char *) hD3D;
		HookCreateRaw((voidFunc)(raw + d3dd->iOffsetCreate));
		if (d3dd->iOffsetCreateEx) {
			HookCreateRawEx((voidFunc)(raw + d3dd->iOffsetCreateEx));
		}

	} else if (! preonly) {
		ods("D3D9: Interface changed, can't rawpatch. Current: %ls ; Previously: %ls", modulename, d3dd->wcFileName);

		pDirect3DCreate9 d3dcreate9 = reinterpret_cast<pDirect3DCreate9>(GetProcAddress(hD3D, "Direct3DCreate9"));
		if (d3dcreate9) {
			ods("D3D9: Got %p for Direct3DCreate9", d3dcreate9);

			IDirect3D9 *id3d9 = d3dcreate9(D3D_SDK_VERSION);
			if (id3d9) {
				HookCreate(id3d9);
				id3d9->Release();
			} else {
				ods("D3D9: Failed call to Direct3DCreate9");
			}
		} else {
			ods("D3D9: Library without Direct3DCreate9?");
		}

		pDirect3DCreate9Ex d3dcreate9ex = reinterpret_cast<pDirect3DCreate9Ex>(GetProcAddress(hD3D, "Direct3DCreate9Ex"));
		if (d3dcreate9ex) {
			ods("D3D9: Got %p for Direct3DCreate9Ex", d3dcreate9ex);
			IDirect3D9Ex** id3d9ex = 0;
			HRESULT hr = d3dcreate9ex(D3D_SDK_VERSION, id3d9ex);
			if (SUCCEEDED(hr)) {
				HookCreateEx(*id3d9ex);
				(*id3d9ex)->Release();
			} else {
				switch (hr) {
					case D3DERR_OUTOFVIDEOMEMORY:
						ods("D3D11: Direct3DCreate9Ex returned with out of memory error.");
						break;
					case D3DERR_NOTAVAILABLE:
						ods("D3D11: Direct3DCreate9Ex is not available.");
						break;
					default:
						ods("D3D11: Unexpected return result from Direct3DCreate9Ex");
						break;
				}
			}
		}
	} else {
		bHooked = false;
	}
}

void freeD3D9Hook(HMODULE hModule) {
	HMODULE hD3D = GetModuleHandle("D3D9.DLL");
	if (bHooked && !hD3D) {
		ods("D3D9: Freeing hooks for module %p", hModule);

		hhCreateDevice.reset();
		hhCreateDeviceEx.reset();
		hhReset.reset();
		hhResetEx.reset();
		hhAddRef.reset();
		hhRelease.reset();
		hhPresent.reset();
		hhPresentEx.reset();
		hhSwapPresent.reset();

		bHooked = false;
	}
}

extern "C" __declspec(dllexport) void __cdecl PrepareD3D9() {
	if (! d3dd)
		return;

	ods("D3D9: Preparing static data for D3D9 Injection");

	HMODULE hD3D = LoadLibrary("D3D9.DLL");

	if (hD3D != NULL) {

		GetModuleFileNameW(hD3D, d3dd->wcFileName, ARRAY_NUM_ELEMENTS(d3dd->wcFileName));

		std::string d3d9FnName("Direct3DCreate9");
		pDirect3DCreate9 d3dcreate9 = reinterpret_cast<pDirect3DCreate9>(GetProcAddress(hD3D, d3d9FnName.c_str()));
		if (! d3dcreate9) {
			ods(("D3D9: Library without " + d3d9FnName).c_str());
		} else {
			if (!IsFnInModule(reinterpret_cast<voidFunc>(d3dcreate9), d3dd->wcFileName, "D3D9", d3d9FnName)) {
				ods(("D3D9: " + d3d9FnName + " is not in D3D9 library").c_str());
			} else {
				IDirect3D9 *id3d9 = d3dcreate9(D3D_SDK_VERSION);
				if (id3d9) {
					void ***vtbl = (void ***) id3d9;

					void *pCreate = (*vtbl)[VTABLE_OFFSET_ID3D_CREATEDEVICE];

					if (!IsFnInModule(reinterpret_cast<voidFunc>(pCreate), d3dd->wcFileName, "D3D9", "CreateDevice")) {
						ods("D3D9: CreateDevice is not in D3D9 library");
					} else {
						unsigned char *fn = reinterpret_cast<unsigned char *>(pCreate);
						unsigned char *base = reinterpret_cast<unsigned char *>(hD3D);
						d3dd->iOffsetCreate = fn - base;
						ods("D3D9: Successfully found prepatch offset: %p %p %p: %d", hD3D, d3dcreate9, pCreate, d3dd->iOffsetCreate);
					}
					id3d9->Release();
				}
			}
		}

		std::string d3d9exFnName("Direct3DCreate9Ex");
		pDirect3DCreate9Ex d3dcreate9ex = reinterpret_cast<pDirect3DCreate9Ex>(GetProcAddress(hD3D, d3d9exFnName.c_str()));
		if (! d3dcreate9ex) {
			ods(("D3D9: Library without " + d3d9exFnName).c_str());
		} else {
			if (!IsFnInModule(reinterpret_cast<voidFunc>(d3dcreate9ex), d3dd->wcFileName, "D3D9", d3d9exFnName)) {
				ods(("D3D9: " + d3d9exFnName + " is not in D3D9 library").c_str());
			} else {
					IDirect3D9Ex *id3d9 = NULL;
					d3dcreate9ex(D3D_SDK_VERSION, &id3d9);
					if (id3d9) {
						void ***vtbl = (void ***) id3d9;
						void *pCreateEx = (*vtbl)[VTABLE_OFFSET_ID3D_CREATEDEVICE_EX];

						if (!IsFnInModule(reinterpret_cast<voidFunc>(pCreateEx), d3dd->wcFileName, "D3D9", "CreateDeviceEx")) {
							ods("D3D9: CreateDeviceEx is not in D3D9 library");
						} else {
							unsigned char *fn = reinterpret_cast<unsigned char *>(pCreateEx);
							unsigned char *base = reinterpret_cast<unsigned char *>(hD3D);
							d3dd->iOffsetCreateEx = fn - base;
							ods("D3D9: Successfully found prepatch ex offset: %p %p %p: %d", hD3D, d3dcreate9ex, pCreateEx, d3dd->iOffsetCreateEx);
						}

						id3d9->Release();
					}
			}
		}

		FreeLibrary(hD3D);
	}
}
