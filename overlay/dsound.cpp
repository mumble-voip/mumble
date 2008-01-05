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
#include <dsound.h>

typedef HRESULT (*pDirectSoundCreate8)(LPCGUID lpcGuidDevice, LPDIRECTSOUND8 * ppDS8, LPUNKNOWN pUnkOuter);

static bool bHooked = false;
static bool bChaining = false;

static HardHook hhSetAllParameters;
static HardHook hhSetPosition;
static HardHook hhSetOrientation;

static HRESULT __stdcall mySetAllParameters(IDirectSound3DListener8 *idsl, LPCDS3DLISTENER pcListener, DWORD dwApply) {
	ods("DS8: SetAllParameters");
	hhSetAllParameters.restore();
	HRESULT hr=idsl->SetAllParameters(pcListener, dwApply);
	hhSetAllParameters.inject();
	if (FAILED(hr))
		return hr;

	ods("DS8: Position %f %f %f", pcListener->vPosition.x, pcListener->vPosition.y, pcListener->vPosition.z);

	if (bVideoHooked) {
		// DSound is lowest priority audio updater.
		DWORD t = GetTickCount();
		if ((sm->uiAudioTick - t) > 1) {
			uiAudioCount++;
			sm->uiAudioTick = t;
			sm->fPos[0] = pcListener->vPosition.x;
			sm->fPos[1] = pcListener->vPosition.y;
			sm->fPos[2] = pcListener->vPosition.z;
			sm->fFront[0] = pcListener->vOrientFront.x;
			sm->fFront[1] = pcListener->vOrientFront.y;
			sm->fFront[2] = pcListener->vOrientFront.z;
			sm->fTop[0] = pcListener->vOrientTop.x;
			sm->fTop[1] = pcListener->vOrientTop.y;
			sm->fTop[2] = pcListener->vOrientTop.z;
		}
	}

	return hr;
}

static HRESULT __stdcall mySetPosition(IDirectSound3DListener8 *idsl, D3DVALUE x, D3DVALUE y, D3DVALUE z, DWORD dwApply) {
	ods("DS8: SetPosition");
	hhSetPosition.restore();
	HRESULT hr=idsl->SetPosition(x,y,z, dwApply);
	hhSetPosition.inject();
	if (FAILED(hr))
		return hr;

	ods("DS8: Position %f %f %f", x, y, z);

	if (bVideoHooked) {
		DWORD t = GetTickCount();
		uiAudioCount++;
		sm->uiAudioTick = t;
		sm->fPos[0] = x;
		sm->fPos[1] = y;
		sm->fPos[2] = z;
	}

	return hr;
}

static HRESULT __stdcall mySetOrientation(IDirectSound3DListener8 *idsl,  D3DVALUE xFront, D3DVALUE yFront, D3DVALUE zFront, D3DVALUE xTop, D3DVALUE yTop, D3DVALUE zTop, DWORD dwApply) {
	ods("DS8: SetOrientation");
	hhSetOrientation.restore();
	HRESULT hr=idsl->SetOrientation(xFront,yFront,zFront,xTop,yTop,zTop, dwApply);
	hhSetOrientation.inject();
	if (FAILED(hr))
		return hr;

	if (bVideoHooked) {
		DWORD t = GetTickCount();
		uiAudioCount++;
		sm->uiAudioTick = t;
		sm->fFront[0] = xFront;
		sm->fFront[1] = yFront;
		sm->fFront[2] = zFront;
		sm->fTop[0] = xTop;
		sm->fTop[1] = yTop;
		sm->fTop[2] = zTop;
	}

	return hr;
}

void checkDSHook(HWND hwnd) {
	if (bChaining) {
		return;
		ods("DS8: Causing a chain");
	}

	bChaining = true;

	HRESULT hr;
	HMODULE hDS = GetModuleHandle(L"DSOUND.DLL");

	if (hDS != NULL) {
		if (! bHooked) {
			wchar_t procname[1024];
			GetModuleFileName(NULL, procname, 1024);
			ods("DS8: CreateWnd in unhooked DSound App %ls", procname);
			bHooked = true;
			pDirectSoundCreate8 ds8c = reinterpret_cast<pDirectSoundCreate8>(GetProcAddress(hDS, "DirectSoundCreate8"));
			if (ds8c) {
				IDirectSound8 *ids8 = NULL;
				hr = ds8c(NULL, &ids8, NULL);
				if (ids8) {
					// HookCreate(ids8);
					if (FAILED(ids8->SetCooperativeLevel(hwnd, DSSCL_PRIORITY))) {
						ods("DS8: Failed SetCooperativeLevel for %lx",hwnd);
					}

					WAVEFORMATEX wf;
					ZeroMemory(&wf, sizeof(WAVEFORMATEX));

					wf.wFormatTag = WAVE_FORMAT_PCM;
					wf.nChannels = 1;
					wf.nSamplesPerSec = 16000;
					wf.nBlockAlign = 2;
					wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
					wf.wBitsPerSample = 16;

					DSBUFFERDESC dsc;
					ZeroMemory(&dsc, sizeof(DSBUFFERDESC));

					dsc.dwSize = sizeof(dsc);
					dsc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;

					IDirectSoundBuffer *dsb = NULL;
					hr = ids8->CreateSoundBuffer(&dsc, &dsb, NULL);

					if (dsb) {
						IDirectSound3DListener8 *dsl = NULL;
						hr = dsb->QueryInterface(IID_IDirectSound3DListener8, reinterpret_cast<void **>(&dsl));
						if (dsl) {
							// Add a ref to ourselves; we do NOT want to get unloaded directly from this process.
							GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<wchar_t *>(&checkDSHook), &hSelf);

							hhSetAllParameters.setupInterface(dsl, 10, reinterpret_cast<voidFunc>(mySetAllParameters));
							hhSetAllParameters.inject();

							hhSetPosition.setupInterface(dsl, 14, reinterpret_cast<voidFunc>(mySetPosition));
							hhSetPosition.inject();

							hhSetOrientation.setupInterface(dsl, 13, reinterpret_cast<voidFunc>(mySetOrientation));
							hhSetOrientation.inject();

							ods("DS8: Patched DirectSound3DListener8");

							dsl->Release();
						} else {
							ods("DS8: Failed QueryInterface for 3DListener8 %lx",hr);
						}
						dsb->Release();
					} else {
						ods("DS8: Failed CreateSoundBuffer %lx",hr);
					}

					ids8->Release();
				} else {
					ods("DS8: Failed DirectSoundCreate8 %lx",hr);
				}
			}
		}
	}

	bChaining = false;
}
