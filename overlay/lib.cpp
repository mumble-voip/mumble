/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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

#define UNICODE
#define _UNICODE
#define  _WIN32_WINNT 0x0501
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9core.h>
#include <map>
#include <string>
#include "overlay.h"

using namespace std;

#pragma data_seg(".SHARED")

extern "C" __declspec(dllexport) SharedMem sm = {0, false, 100, 20};

HHOOK hhookCBT = 0, hhookWnd = 0;

#pragma data_seg()
#pragma comment(linker, "/section:.SHARED,RWS")

typedef IDirect3D9* (WINAPI *pDirect3DCreate9) (UINT SDKVersion) ;

void checkUnhook(IDirect3DDevice9 *idd = NULL);
void __cdecl ods(const char *format, ...);

class DevState {
public:
	IDirect3DDevice9 *dev;
	LONG initRefCount;
	LONG refCount;
	LONG myRefCount;
	LONG triggerCount;
	bool bInitialized;
	bool bNeedPrep;
	bool bMyRefs;
	DevState();

	ID3DXSprite*        pTextSprite;
	ID3DXFont*          pFont;

	void cleanState();
	void initData();
	void releaseData();
	void prep();
	void draw();
	void postDraw();
	void mkString(const wchar_t *string);
};

map<IDirect3DDevice9 *, DevState *> devMap;
map<wstring, LPDIRECT3DTEXTURE9> texMap;
bool bHooked = false;
HMODULE hSelf = NULL;
HANDLE hSharedMutex = NULL;
bool bPinned = false;

DevState::DevState() {
	dev = NULL;
	bInitialized = false;
	bMyRefs = false;
	bNeedPrep = false;
	refCount = 0;
	myRefCount = 0;
	triggerCount = 0;
}

void DevState::initData() {
	ods("Init Data of %p", dev);

	D3DXCreateSprite(dev, &pTextSprite );
	D3DXCreateFont(dev, sm.iHeight, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
					L"Arial", &pFont);

	bNeedPrep = true;

	return;
}

void DevState::prep() {
	ods("Preparing....");

	DWORD dwWaitResult = WaitForSingleObject(hSharedMutex, 50L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		for(int i=0;i<16;i++) {
			if (sm.players[i].name[0]) {
				wstring str(sm.players[i].name);
				if (texMap[str] == NULL) {
					mkString(sm.players[i].name);
				}
			}
		}
		ReleaseMutex(hSharedMutex);
	}
}

void DevState::mkString(const wchar_t *str) {
	ods("Making string %ls", str);
	IDirect3DSurface9 *pSurf, *pPrevSurf = NULL, *pDS = NULL;
	LPDIRECT3DTEXTURE9 pTex;

	dev->CreateTexture(sm.iWidth, sm.iHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pTex, NULL);

	dev->GetRenderTarget(0, &pPrevSurf);

	pTex->GetSurfaceLevel(0, &pSurf);
	dev->SetRenderTarget(0, pSurf);
	pSurf->Release();

	dev->GetDepthStencilSurface(&pDS);
	dev->SetDepthStencilSurface(NULL);

	D3DVIEWPORT9 vp, oldVP;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = sm.iWidth;
	vp.Height = sm.iHeight;
	vp.MinZ = 0.0;
	vp.MaxZ = 1.0;


	dev->GetViewport(&oldVP);
	dev->SetViewport(&vp);

	dev->BeginScene();
	dev->Clear( 0L, NULL, D3DCLEAR_TARGET, 0x000000ff, 1.0f, 0L );

	pTextSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );

	RECT rc;
	SetRect( &rc, 0, 0, 0, 0 );
	pFont->DrawText( pTextSprite, str, -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ));
	SetRect( &rc, 2, 0, 0, 0 );
	pFont->DrawText( pTextSprite, str, -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ));
	SetRect( &rc, 2, 2, 0, 0 );
	pFont->DrawText( pTextSprite, str, -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ));
	SetRect( &rc, 0, 2, 0, 0 );
	pFont->DrawText( pTextSprite, str, -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ));
	SetRect( &rc, 1, 1, 0, 0 );
	pFont->DrawText( pTextSprite, str, -1, &rc, DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
	pTextSprite->End();

	dev->EndScene();
	dev->SetViewport(&oldVP);

	dev->SetDepthStencilSurface(pDS);
	if (pDS)
		pDS->Release();

	dev->SetRenderTarget(0, pPrevSurf);
	pPrevSurf->Release();

	wstring s(str);
	texMap[str]=pTex;
}

void DevState::releaseData() {
	ods("Release Data");
	pFont->Release();
	pTextSprite->Release();

	map<wstring, LPDIRECT3DTEXTURE9>::const_iterator texIter;
	for(texIter=texMap.begin();texIter != texMap.end(); ++texIter) {
		if ((*texIter).second != NULL) {
			(*texIter).second->Release();
		}
	}
	texMap.clear();
}

void DevState::draw() {
	D3DVIEWPORT9 vp;
	dev->GetViewport(&vp);

	pTextSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
	int idx = 0;

	DWORD dwWaitResult = WaitForSingleObject(hSharedMutex, 50L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		for(int i=0;i<16;i++) {
			if (sm.players[i].name[0]) {
				wstring str(sm.players[i].name);
				IDirect3DTexture9 *tex = texMap[str];
				if (tex) {
					D3DXVECTOR3 p(vp.Width-sm.iWidth, idx*sm.iHeight, 1.0);
					pTextSprite->Draw(tex, NULL, NULL, &p, sm.players[i].bTalking ? 0xFFFFFFC0 : 0x80FFFFFF);
					idx++;
				} else {
					bNeedPrep = true;
				}
			}
		}
		ReleaseMutex(hSharedMutex);
	}

	pTextSprite->End();

}

void DevState::cleanState() {
		dev->SetVertexShader(NULL);
		dev->SetPixelShader(NULL);

		for(int i=0;i<10;i++)
			dev->SetTexture(i, NULL);

		dev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		dev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE );
		dev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD );
		dev->SetRenderState(D3DRS_CLIPPING, FALSE);
		dev->SetRenderState(D3DRS_CLIPPLANEENABLE, 0);
		dev->SetRenderState(D3DRS_COLORVERTEX, FALSE);
		dev->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA|D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_RED );
		dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO );
		dev->SetRenderState(D3DRS_DITHERENABLE, FALSE);
		dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		dev->SetRenderState(D3DRS_FOGENABLE, FALSE);
		dev->SetRenderState(D3DRS_LIGHTING, FALSE);
		dev->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
		dev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		dev->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE );
		dev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
		dev->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
		dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE );
		dev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		dev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		dev->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		dev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

void __cdecl ods(const char *format, ...) {
	char	buf[4096], *p = buf;
	va_list	args;

	va_start(args, format);
	p += _vsnprintf(p, sizeof buf - 1, format, args);
	va_end(args);

	while ( p > buf  &&  isspace(p[-1]) )
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p   = '\0';

	OutputDebugStringA(buf);
	FILE *f = fopen("c:\\overlay.log", "a");
	fprintf(f, "%s", buf);
	fclose(f);
}

typedef void *(*voidFunc)();

struct HardHook {
	unsigned char *baseptr;
	unsigned char orig[5];
	unsigned char replace[5];
	HardHook();
	void setup(voidFunc func, voidFunc replacement);
	void setupInterface(IUnknown *intf, LONG funcoffset, voidFunc replacement);
	void inject();
	void restore();
};

HardHook::HardHook() {
	int i;
	baseptr = NULL;
	for(i=0;i<5;i++)
		orig[i]=replace[i]=0;
}

void HardHook::setup(voidFunc func, voidFunc replacement) {
	int i;
	DWORD oldProtect, restoreProtect;

	if (baseptr)
		return;

	unsigned char *fptr = (unsigned char *) func;
	unsigned char *nptr = (unsigned char *) replacement;

	ods("Asked to replace %p with %p", func, replacement);

	int offs = nptr - fptr - 5;
	int *iptr = (int *) &replace[1];
	*iptr = offs;
	replace[0] = 0xe9;

	if (VirtualProtect(fptr, 5, PAGE_EXECUTE_READ, &oldProtect)) {
		for(i=0;i<5;i++)
			orig[i]=fptr[i];
		VirtualProtect(fptr, 5, oldProtect, &restoreProtect);
		baseptr = fptr;
	}
}

void HardHook::setupInterface(IUnknown *unkn, LONG funcoffset, voidFunc replacement) {
	ods("Replacing %p function #%ld", unkn, funcoffset);
	void **ptr = (void **) unkn;
	ptr = (void **) ptr[0];
	setup((voidFunc) ptr[funcoffset], replacement);
}

void HardHook::inject() {
	DWORD oldProtect, restoreProtect;
	int i;

	if (! baseptr)
		return;
	if (VirtualProtect(baseptr, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		for(i=0;i<5;i++)
			baseptr[i] = replace[i];
		VirtualProtect(baseptr, 5, oldProtect, &restoreProtect);
		FlushInstructionCache(GetCurrentProcess(),baseptr, 5);
	}
}

void HardHook::restore() {
	DWORD oldProtect, restoreProtect;
	int i;

	if (! baseptr)
		return;
	if (VirtualProtect(baseptr, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		for(i=0;i<5;i++)
			baseptr[i] = orig[i];
		VirtualProtect(baseptr, 5, oldProtect, &restoreProtect);
		FlushInstructionCache(GetCurrentProcess(),baseptr, 5);
	}
}

HardHook hhCreateDevice;
HardHook hhBeginScene;
HardHook hhEndScene;
HardHook hhReset;
HardHook hhAddRef;
HardHook hhRelease;

HRESULT __stdcall myBeginScene(IDirect3DDevice9 * idd) {
	hhBeginScene.restore();

	DevState *ds = devMap[idd];
	if (ds && (ds->bNeedPrep || ! ds->bInitialized)) {
		hhEndScene.restore();

		bool b = ds->bMyRefs;
		ds->bMyRefs = true;

		IDirect3DStateBlock9* pStateBlock = NULL;
	  	idd->CreateStateBlock( D3DSBT_ALL, &pStateBlock );
	  	pStateBlock->Capture();

		if (! ds->bInitialized) {
			ds->initData();
			ds->bInitialized = true;
		}
		if (ds->bNeedPrep) {
			ds->prep();
			ds->bNeedPrep = false;
		}

		pStateBlock->Apply();
		pStateBlock->Release();

		ds->bMyRefs = b;

		hhEndScene.inject();
	}
	HRESULT hr=idd->BeginScene();
	hhBeginScene.inject();
	checkUnhook(idd);
	return hr;
}

HRESULT __stdcall myEndScene(IDirect3DDevice9 * idd) {
	DevState *ds = devMap[idd];
	if (ds) {
		bool b = ds->bMyRefs;
		ds->bMyRefs = true;
		ds->triggerCount = ds->refCount / 4;

		IDirect3DStateBlock9* pStateBlock = NULL;
	  	idd->CreateStateBlock( D3DSBT_ALL, &pStateBlock );
	  	pStateBlock->Capture();

		ds->cleanState();

		ds->draw();

		pStateBlock->Apply();
		pStateBlock->Release();

		ds->bMyRefs = b;
	}

	hhEndScene.restore();
	HRESULT hr=idd->EndScene();
	hhEndScene.inject();
	checkUnhook(idd);
	return hr;
}

HRESULT __stdcall myReset(IDirect3DDevice9 * idd, D3DPRESENT_PARAMETERS *param) {
	ods("Chaining Reset");

	DevState *ds = devMap[idd];
	if (ds) {
		if (ds->bInitialized) {
			bool b = ds->bMyRefs;
			ds->bMyRefs = true;
			ds->releaseData();
			ds->bMyRefs = b;
		}
		ds->bInitialized = false;
	}
	hhReset.restore();
	HRESULT hr=idd->Reset(param);
	hhReset.inject();

	checkUnhook(idd);

	return hr;
}

ULONG __stdcall myAddRef(IDirect3DDevice9 *idd) {
	DevState *ds = devMap[idd];
	if (ds) {
		if (ds->bMyRefs)
			ds->myRefCount++;
		else
			ds->refCount++;
		return ds->refCount + ds->initRefCount;
	}
	ods("Unknown device, chaining AddRef");
	hhAddRef.restore();
	LONG res = idd->AddRef();
	hhAddRef.inject();
	return res;
}

// The font and such seems to be used through a callback or something, because
// it generates a reference outside the blocks here. Hence the trigger
// mechanism.

ULONG __stdcall myRelease(IDirect3DDevice9 *idd) {
	DevState *ds = devMap[idd];
	if (ds) {
		if (ds->bMyRefs) {
			ds->myRefCount--;
			return ds->refCount + ds->initRefCount;
		} else {
			ds->refCount--;
		}

		if (ds->triggerCount && (ds->refCount < ds->triggerCount)) {
			ods("Trigger release");
			ds->triggerCount = 0;
			if (ds->bInitialized) {
				bool b = ds->bMyRefs;
				ds->bMyRefs = true;
				ds->releaseData();
				ds->bMyRefs = b;
				ds->bInitialized = false;
			}
			ds->refCount += ds->myRefCount;
			ds->myRefCount = 0;
		}

		if (ds->refCount >= 0)
			return ds->refCount + ds->initRefCount;

		ods("Final release");
		devMap.erase(idd);
	}
	ods("Chaining Release");
	hhRelease.restore();
	LONG res = idd->Release();
	hhRelease.inject();
	return res;
}

HRESULT __stdcall myCreateDevice(IDirect3D9 * id3d, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface) {
	ods("Chaining CreateDevice");

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

	hhAddRef.setupInterface(idd, 1, (voidFunc) myAddRef);
	hhAddRef.inject();

	hhRelease.setupInterface(idd, 2, (voidFunc) myRelease);
	hhRelease.inject();

	hhBeginScene.setupInterface(idd, 41, (voidFunc) myBeginScene);
	hhBeginScene.inject();

	hhEndScene.setupInterface(idd, 42, (voidFunc) myEndScene);
	hhEndScene.inject();

	hhReset.setupInterface(idd, 16, (voidFunc) myReset);
	hhReset.inject();

	checkUnhook(idd);

	return hr;
}

void HookCreate(IDirect3D9 *pD3D) {
	ods("Injecting CreateDevice");

	bPinned = true;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (wchar_t *) &HookCreate, &hSelf);

	hSharedMutex = CreateMutex(NULL, false, L"MumbleSharedMutex");

	hhCreateDevice.setupInterface(pD3D, 16, (voidFunc) myCreateDevice);
	hhCreateDevice.inject();
}

LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam) {
	wchar_t procname[1024];
	GetModuleFileName(NULL, procname, 1024);
	HMODULE hD3D = GetModuleHandle(L"D3D9.DLL");
	if (hD3D != NULL) {
		if (! bHooked) {
			wchar_t procname[1024];
			GetModuleFileName(NULL, procname, 1024);
			ods("CreateWnd in unhooked D3D App %ls", procname);
			bHooked = true;
			pDirect3DCreate9 d3dc9 = reinterpret_cast<pDirect3DCreate9>(GetProcAddress(hD3D, "Direct3DCreate9"));
			if (d3dc9) {
				IDirect3D9 *id3d9 = d3dc9(D3D_SDK_VERSION);
				if (id3d9) {
					HookCreate(id3d9);
					IDirect3D9_Release(id3d9);
				}
			}
		}
	}

	checkUnhook();

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

extern "C" __declspec(dllexport) void __cdecl RemoveHooks() {
	HANDLE hMutex = NULL;
	hMutex = CreateMutex(NULL, false, L"MumbleHookMutex");

	DWORD dwWaitResult = WaitForSingleObject(hMutex, 1000L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		if (sm.bHooked) {
			if (hhookCBT) {
				UnhookWindowsHookEx(hhookCBT);
				hhookCBT = NULL;
			}
			if (hhookWnd) {
				UnhookWindowsHookEx(hhookWnd);
				hhookWnd = NULL;
			}
			sm.bHooked = false;
		}
		ReleaseMutex(hMutex);
	}
}


extern "C" __declspec(dllexport) void __cdecl InstallHooks() {
	sm.lastAppAlive = GetTickCount();

	HANDLE hMutex = NULL;
	hMutex = CreateMutex(NULL, false, L"MumbleHookMutex");

	DWORD dwWaitResult = WaitForSingleObject(hMutex, 1000L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		if (! sm.bHooked) {
			GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (wchar_t *) &InstallHooks, &hSelf);
			if (hSelf == NULL) {
				ods("Failed to find myself");
			} else {
				hhookCBT = SetWindowsHookEx(WH_CBT,CBTProc,hSelf,0);
				if (hhookCBT == NULL)
					ods("Failed to insert CBT hook");
				hhookWnd = SetWindowsHookEx(WH_CALLWNDPROC,CBTProc,hSelf,0);
				if (hhookWnd == NULL)
					ods("Failed to insert WNDProc hook");
			}
			sm.bHooked = true;
		}
	}
	ReleaseMutex(hMutex);
}

void checkUnhook(IDirect3DDevice9 *idd) {
	DWORD now = GetTickCount();
	if ((now - sm.lastAppAlive) > 5000) {
		ods("Application is dead.");
		if (bPinned) {
			DevState *ds = devMap[idd];
			if (ds && ds->bInitialized) {
				bool b = ds->bMyRefs;
				ds->bMyRefs = true;
				ds->releaseData();
				ds->bMyRefs = b;
				ds->bInitialized = false;
			} else {
				ods("Terminating with leakage.");
			}
			hhCreateDevice.restore();
			hhEndScene.restore();
			hhReset.restore();
			hhAddRef.restore();
			hhRelease.restore();
			if (ds) {
				for(int i=0;i < (ds->refCount + ds->myRefCount); i++)
					ds->dev->AddRef();
			}
			FreeLibrary(hSelf);
			bPinned = false;
		}
		RemoveHooks();
	}
}
