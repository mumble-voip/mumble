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
#include <map>
#include <vector>
#include <string>
#include "overlay.h"

using namespace std;

static HANDLE hMapObject = NULL;
static SharedMem *sm;
static HANDLE hSharedMutex = NULL;
static HANDLE hHookMutex = NULL;

static HHOOK hhookWnd = 0;
static bool bChaining = false;

typedef IDirect3D9* (WINAPI *pDirect3DCreate9) (UINT SDKVersion) ;

void checkUnhook(IDirect3DDevice9 *idd = NULL);
void __cdecl ods(const char *format, ...);

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

map<IDirect3DDevice9 *, DevState *> devMap;
bool bHooked = false;
HMODULE hSelf = NULL;

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
	ods("Release Data");

	for(int i=0;i<NUM_TEXTS;i++)
		if (tex[i]) {
		    tex[i]->Release();
		    tex[i] = NULL;
	       }
}

void DevState::releaseAll() {
	ods("Release All");
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
	vector<int> widths;
	vector<int> yofs;
	vector<DWORD> colors;

	int y = 0;

	if (sm->fFontSize < 0.01)
		sm->fFontSize = 0.01;
	else if (sm->fFontSize > 1.0)
		sm->fFontSize = 1.0;

	int iHeight = vp.Height * sm->fFontSize;

	if (iHeight > TEXT_HEIGHT)
		iHeight = TEXT_HEIGHT;

	float s = iHeight / 60.0;

	ods("Init: Scale %f. iH %d. Final scale %f", sm->fFontSize, iHeight, s);

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
					DWORD res=tex[i]->LockRect(0, &lr, NULL, D3DLOCK_DISCARD);

					for(int r=0;r<TEXT_HEIGHT;r++) {
					    unsigned char *dptr = (unsigned char *) lr.pBits + r * lr.Pitch;
						memcpy(dptr, sm->texts[i].texture + r * TEXT_WIDTH * 4, sm->texts[i].width * 4);
					}

					tex[i]->UnlockRect(0);
					sm->texts[i].bUpdated = false;
				}
				int w = sm->texts[i].width * s;
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
	y = vp.Height * sm->fY;

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
		int width = widths[i];

		int x = vp.Width * sm->fX;

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

		float left   = (float)x;
		float top    = (float)y;
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

static void __cdecl ods(const char *format, ...) {

	if (!sm || ! sm->bDebug)
		return;

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

static HardHook hhCreateDevice;
static HardHook hhReset;
static HardHook hhAddRef;
static HardHook hhRelease;
static HardHook hhPresent;

static HRESULT __stdcall myPresent(IDirect3DDevice9 * idd, CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion) {
	DevState *ds = devMap[idd];
	if (ds && sm->bShow) {
		IDirect3DSurface9 *pTarget = NULL;
		IDirect3DSurface9 *pRenderTarget = NULL;
		idd->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pTarget);
		idd->GetRenderTarget(0, &pRenderTarget);

		ods("myPresent Back %p RenderT %p",pTarget,pRenderTarget);

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
	}
	hhPresent.restore();
	HRESULT hr=idd->Present(pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion);
	hhPresent.inject();
	checkUnhook(idd);
	return hr;
}

static HRESULT __stdcall myReset(IDirect3DDevice9 * idd, D3DPRESENT_PARAMETERS *param) {
	ods("Chaining Reset");

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

	checkUnhook(idd);

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
	ods("Unknown device, chaining AddRef");
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

		ods("Final release");

		bool b = ds->bMyRefs;
		ds->bMyRefs = true;
		ds->releaseAll();
		ds->bMyRefs = b;

		devMap.erase(idd);
		delete ds;
	}
	ods("Chaining Release");
	hhRelease.restore();
	LONG res = idd->Release();
	hhRelease.inject();
	return res;
}

static HRESULT __stdcall myCreateDevice(IDirect3D9 * id3d, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface) {
	ods("Chaining CreateDevice");

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

	hhAddRef.setupInterface(idd, 1, (voidFunc) myAddRef);
	hhAddRef.inject();

	hhRelease.setupInterface(idd, 2, (voidFunc) myRelease);
	hhRelease.inject();

	hhReset.setupInterface(idd, 16, (voidFunc) myReset);
	hhReset.inject();

	hhPresent.setupInterface(idd, 17, (voidFunc) myPresent);
	hhPresent.inject();

	ds->createCleanState();

	checkUnhook(idd);

	return hr;
}

static void HookCreate(IDirect3D9 *pD3D) {
	ods("Injecting CreateDevice");

	// Add a ref to ourselves; we do NOT want to get unloaded directly from this process.
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (wchar_t *) &HookCreate, &hSelf);

	hhCreateDevice.setupInterface(pD3D, 16, (voidFunc) myCreateDevice);
	hhCreateDevice.inject();
}

static void checkHook() {
	if (bChaining) {
		return;
		ods("Causing a chain");
	}

	bChaining = true;

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

	bChaining = false;
}

static LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam) {
	CWPSTRUCT *s = (CWPSTRUCT *) lParam;
	if (s) {
		switch (s->message) {
			case WM_CREATE:
			case WM_DESTROY:
			case WM_SETFOCUS:
			case WM_GETMINMAXINFO:	// For things that link directly
			case WM_GETICON:		// Worked for BF2
			case WM_NCCREATE:		// Lots of games
				checkHook();
				break;
			default:
				break;
		}
	}
	return CallNextHookEx(hhookWnd, nCode, wParam, lParam);
}

extern "C" __declspec(dllexport) void __cdecl RemoveHooks() {
	DWORD dwWaitResult = WaitForSingleObject(hHookMutex, 1000L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		if (sm->bHooked) {
			if (hhookWnd) {
				UnhookWindowsHookEx(hhookWnd);
				hhookWnd = NULL;
			}
			sm->bHooked = false;
		}
		ReleaseMutex(hHookMutex);
	}
}

extern "C" __declspec(dllexport) void __cdecl InstallHooks() {
	sm->lastAppAlive = GetTickCount();

	DWORD dwWaitResult = WaitForSingleObject(hHookMutex, 1000L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		if (! sm->bHooked) {
			GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (wchar_t *) &InstallHooks, &hSelf);
			if (hSelf == NULL) {
				ods("Failed to find myself");
			} else {
				hhookWnd = SetWindowsHookEx(WH_CALLWNDPROC,CallWndProc,hSelf,0);
				if (hhookWnd == NULL)
					ods("Failed to insert WNDProc hook");
			}
			sm->bHooked = true;
		}
		ReleaseMutex(hHookMutex);
	}
}

extern "C" __declspec(dllexport) SharedMem * __cdecl GetSharedMemory() {
	return sm;
}

static void checkUnhook(IDirect3DDevice9 *idd) {
	DWORD now = GetTickCount();
	if ((now - sm->lastAppAlive) > 30000) {
		ods("Application is dead.");
		RemoveHooks();
		DevState *ds = devMap[idd];
		if (ds) {
			bool b = ds->bMyRefs;
			ds->bMyRefs = true;
			ds->releaseAll();
			ds->bMyRefs = b;
		} else {
			ods("Terminating with leakage.");
		}
		hhCreateDevice.restore();
		hhPresent.restore();
		hhReset.restore();
		hhAddRef.restore();
		hhRelease.restore();
		if (ds) {
			for(int i=0;i < (ds->refCount + ds->myRefCount); i++)
				ds->dev->AddRef();
		}
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			{
				hSharedMutex = CreateMutex(NULL, false, L"MumbleSharedMutex");
				hHookMutex = CreateMutex(NULL, false, L"MumbleHookMutex");
				if ((hSharedMutex == NULL) || (hHookMutex == NULL)) {
					ods("CreateMutex failed");
					return FALSE;
				}

				DWORD dwWaitResult = WaitForSingleObject(hSharedMutex, 1000L);
				if (dwWaitResult != WAIT_OBJECT_0) {
					ods("WaitForMutex failed");
					return FALSE;
				}

				hMapObject = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SharedMem), L"MumbleSharedMemory");
				if (hMapObject == NULL) {
					ods("CreateFileMapping failed");
					ReleaseMutex(hSharedMutex);
					return FALSE;
				}

				bool bInit = (GetLastError() != ERROR_ALREADY_EXISTS);

				sm = (SharedMem *) MapViewOfFile(hMapObject, FILE_MAP_WRITE, 0, 0, 0);
				if (sm == NULL) {
					ods("MapViewOfFile Failed");
					ReleaseMutex(hSharedMutex);
					return FALSE;
				}
				if (bInit) {
					memset(sm, 0, sizeof(SharedMem));
					sm->lastAppAlive = 0;
					sm->bHooked = false;
					sm->bDebug = false;
					sm->bShow = true;
					sm->bReset = false;
					sm->fX = sm->fY = 1.0;
					sm->bTop = false;
					sm->bBottom = true;
					sm->bLeft = true;
					sm->bRight = false;
					sm->fFontSize = 72;
				}
				ReleaseMutex(hSharedMutex);
			}
			break;
		case DLL_PROCESS_DETACH:
			{
				UnmapViewOfFile(sm);
				CloseHandle(hMapObject);
				CloseHandle(hSharedMutex);
				CloseHandle(hHookMutex);
			}
			break;
		default:
			break;
	}
	return TRUE;
}
