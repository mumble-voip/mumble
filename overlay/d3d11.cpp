// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* Copyright (C) 2010-2013, Benjamin Jemlich <pcgod@users.sourceforge.net>
   Copyright (C) 2011-2013, Nye Liu <nyet@nyet.org>
   Copyright (C) 2011-2013, Kissaki <kissaki@gmx.de>

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

#include "D11StateBlock.h"
#include "lib.h"
#include "overlay11.ps.h"
#include "overlay11.vs.h"
#include <d3d11.h>
#include <time.h>

D3D11Data *d3d11 = nullptr;

static bool bHooked = false;
static HardHook hhAddRef;
static HardHook hhRelease;

typedef HRESULT(__stdcall *CreateDXGIFactory1Type)(REFIID, void **);
typedef HRESULT(__stdcall *D3D11CreateDeviceAndSwapChainType)(IDXGIAdapter *, D3D_DRIVER_TYPE, HMODULE Software,
															  UINT Flags, const D3D_FEATURE_LEVEL *, UINT, UINT,
															  const DXGI_SWAP_CHAIN_DESC *, IDXGISwapChain **,
															  ID3D11Device **, D3D_FEATURE_LEVEL *,
															  ID3D11DeviceContext **);

typedef ULONG(__stdcall *AddRefType)(ID3D11Device *);
typedef ULONG(__stdcall *ReleaseType)(ID3D11Device *);

struct SimpleVec3 {
	FLOAT x;
	FLOAT y;
	FLOAT z;
	SimpleVec3(FLOAT _x, FLOAT _y, FLOAT _z) : x(_x), y(_y), z(_z) {}
};

struct SimpleVec2 {
	FLOAT x;
	FLOAT y;
	SimpleVec2(FLOAT _x, FLOAT _y) : x(_x), y(_y) {}
};

struct SimpleVertex {
	SimpleVec3 Pos;
	SimpleVec2 Tex;
};

static const int VERTEXBUFFER_SIZE = 4 * sizeof(SimpleVertex);

class D11State : protected Pipe {
public:
	ULONG lHighMark;

	LONG initRefCount;
	LONG refCount;
	LONG myRefCount;

	D3D11_VIEWPORT vp;

	ID3D11Device *pDevice;
	ID3D11DeviceContext *pDeviceContext;
	bool bDeferredContext;
	IDXGISwapChain *pSwapChain;

	D11StateBlock *pOrigStateBlock;
	D11StateBlock *pMyStateBlock;
	ID3D11RenderTargetView *pRTV;
	ID3D11VertexShader *pVertexShader;
	ID3D11PixelShader *pPixelShader;
	ID3D11InputLayout *pVertexLayout;
	ID3D11Buffer *pVertexBuffer;
	ID3D11Buffer *pIndexBuffer;
	ID3D11BlendState *pBlendState;

	ID3D11Texture2D *pTexture;
	ID3D11ShaderResourceView *pSRView;

	clock_t timeT;
	unsigned int frameCount;

	D11State(IDXGISwapChain *, ID3D11Device *);
	virtual ~D11State();
	bool init();
	void draw();

	virtual void blit(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
	virtual void setRect();
	virtual void newTexture(unsigned int w, unsigned int h);
};

typedef map< IDXGISwapChain *, D11State * > SwapchainMap;
SwapchainMap chains;
typedef map< ID3D11Device *, D11State * > DeviceMap;
DeviceMap devices;

D11State::D11State(IDXGISwapChain *pSwapChain, ID3D11Device *pDevice) : bDeferredContext(false) {
	this->pSwapChain = pSwapChain;
	this->pDevice    = pDevice;

	lHighMark = initRefCount = refCount = myRefCount = 0;

	ZeroMemory(&vp, sizeof(vp));

	pOrigStateBlock = nullptr;
	pMyStateBlock   = nullptr;
	pRTV            = nullptr;
	pVertexShader   = nullptr;
	pPixelShader    = nullptr;
	pVertexLayout   = nullptr;
	pVertexBuffer   = nullptr;
	pIndexBuffer    = nullptr;
	pBlendState     = nullptr;
	pTexture        = nullptr;
	pSRView         = nullptr;
	pDeviceContext  = nullptr;

	timeT      = clock();
	frameCount = 0;

	pDevice->AddRef();
	initRefCount = pDevice->Release();
}

void D11State::blit(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	HRESULT hr;

	ods("D3D11: Blit %d %d %d %d", x, y, w, h);

	if (!pTexture || !pSRView || uiLeft == uiRight)
		return;

	D3D11_MAPPED_SUBRESOURCE mappedTex;
	hr = pDeviceContext->Map(pTexture, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE_DISCARD, 0, &mappedTex);
	if (FAILED(hr)) {
		ods("D3D11: Failed map");
	}

	UCHAR *pTexels = (UCHAR *) mappedTex.pData;

	for (unsigned int r = 0; r < uiHeight; ++r) {
		unsigned char *sptr = a_ucTexture + r * uiWidth * 4;
		unsigned char *dptr = reinterpret_cast< unsigned char * >(pTexels) + r * mappedTex.RowPitch;
		memcpy(dptr, sptr, uiWidth * 4);
	}

	pDeviceContext->Unmap(pTexture, D3D11CalcSubresource(0, 0, 1));
}

void D11State::setRect() {
	HRESULT hr;

	ods("D3D11: SetRect");

	float w = static_cast< float >(uiWidth);
	float h = static_cast< float >(uiHeight);

	float left   = static_cast< float >(uiLeft) - 0.5f;
	float top    = static_cast< float >(uiTop) - 0.5f;
	float right  = static_cast< float >(uiRight) + 0.5f;
	float bottom = static_cast< float >(uiBottom) + 0.5f;

	float texl = (left) / w;
	float text = (top) / h;
	float texr = (right + 1.0f) / w;
	float texb = (bottom + 1.0f) / h;

	left   = 2.0f * (left / vp.Width) - 1.0f;
	right  = 2.0f * (right / vp.Width) - 1.0f;
	top    = -2.0f * (top / vp.Height) + 1.0f;
	bottom = -2.0f * (bottom / vp.Height) + 1.0f;

	ods("D3D11: Vertex (%f %f) (%f %f)", left, top, right, bottom);

	// Create vertex buffer
	SimpleVertex vertices[] = {
		{ SimpleVec3(left, top, 0.5f), SimpleVec2(texl, text) },
		{ SimpleVec3(right, top, 0.5f), SimpleVec2(texr, text) },
		{ SimpleVec3(right, bottom, 0.5f), SimpleVec2(texr, texb) },
		{ SimpleVec3(left, bottom, 0.5f), SimpleVec2(texl, texb) },
	};

	// map/unmap to temporarily deny GPU access to the resource pVertexBuffer
	D3D11_MAPPED_SUBRESOURCE res;
	hr = pDeviceContext->Map(pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);

	const int verticesSize = sizeof(vertices);
	static_assert(verticesSize == VERTEXBUFFER_SIZE,
				  "The vertex buffer size differs from the locally created vertex buffer.");
	memcpy(res.pData, vertices, verticesSize);
	ods("D3D11: Map: %lx %d", hr, sizeof(vertices));
	pDeviceContext->Unmap(pVertexBuffer, 0);
}

void D11State::newTexture(unsigned int w, unsigned int h) {
	HRESULT hr;

	ods("D3D11: newTex %d %d", w, h);

	if (pTexture) {
		pTexture->Release();
		pTexture = nullptr;
	}
	if (pSRView) {
		pSRView->Release();
		pSRView = nullptr;
	}

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width     = w;
	desc.Height    = h;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format                     = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count           = 1;
	desc.Usage                      = D3D11_USAGE_DYNAMIC;
	desc.BindFlags                  = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags             = D3D11_CPU_ACCESS_WRITE;
	hr                              = pDevice->CreateTexture2D(&desc, nullptr, &pTexture);

	if (FAILED(hr)) {
		pTexture = nullptr;
		ods("D3D11: Failed to create texture.");
		return;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format                    = desc.Format;
	srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels       = desc.MipLevels;

	hr = pDevice->CreateShaderResourceView(pTexture, &srvDesc, &pSRView);
	if (FAILED(hr)) {
		pSRView = nullptr;
		pTexture->Release();
		pTexture = nullptr;
		ods("D3D11: Failed to create resource view.");
		return;
	}
}

bool D11State::init() {
	HRESULT hr;

	ID3D11Texture2D *pBackBuffer = nullptr;
	hr                           = pSwapChain->GetBuffer(0, __uuidof(*pBackBuffer), (LPVOID *) &pBackBuffer);
	if (FAILED(hr)) {
		ods("D3D11: pSwapChain->GetBuffer failure!");
		return false;
	}

	hr = pDevice->CreateDeferredContext(0, &pDeviceContext);
	// Depending on the device settings a deferred context may not be createable
	// for example if it is a SINGLETHREADED device.
	// (See http://msdn.microsoft.com/en-us/library/windows/desktop/ff476505%28v=vs.85%29.aspx)
	// We handle the expected failure and failure fallback in the same way -
	// by trying to use an immediate context.
	if (FAILED(hr) || !pDeviceContext) {
		ods("D3D11: Failed to create DeferredContext (0x%x). Getting ImmediateContext", hr);
		pDevice->GetImmediateContext(&pDeviceContext);
		D11CreateStateBlock(pDeviceContext, &pOrigStateBlock);
		D11CreateStateBlock(pDeviceContext, &pMyStateBlock);

		pOrigStateBlock->Capture();
		bDeferredContext = false;
	} else {
		bDeferredContext = true;
	}

	D3D11_TEXTURE2D_DESC backBufferSurfaceDesc;
	pBackBuffer->GetDesc(&backBufferSurfaceDesc);

	ZeroMemory(&vp, sizeof(vp));
	vp.Width    = static_cast< float >(backBufferSurfaceDesc.Width);
	vp.Height   = static_cast< float >(backBufferSurfaceDesc.Height);
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDeviceContext->RSSetViewports(1, &vp);

	hr = pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRTV);
	if (FAILED(hr)) {
		ods("D3D11: pDevice->CreateRenderTargetView failed!");
		return false;
	}

	pDeviceContext->OMSetRenderTargets(1, &pRTV, nullptr);

	// Settings for an "over" operation.
	// https://en.wikipedia.org/w/index.php?title=Alpha_compositing&oldid=580659153#Description
	D3D11_BLEND_DESC blend;
	ZeroMemory(&blend, sizeof(blend));
	blend.RenderTarget[0].BlendEnable           = TRUE;
	blend.RenderTarget[0].SrcBlend              = D3D11_BLEND_ONE;
	blend.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
	blend.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
	blend.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_INV_SRC_ALPHA;
	blend.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	pDevice->CreateBlendState(&blend, &pBlendState);
	if (FAILED(hr)) {
		ods("D3D11: pDevice->CreateBlendState failed!");
		return false;
	}

	pDeviceContext->OMSetBlendState(pBlendState, nullptr, 0xffffffff);

	hr = pDevice->CreateVertexShader(g_vertex_shader, sizeof(g_vertex_shader), nullptr, &pVertexShader);
	if (FAILED(hr)) {
		ods("D3D11: Failed to create vertex shader.");
		return false;
	}

	hr = pDevice->CreatePixelShader(g_pixel_shader, sizeof(g_pixel_shader), nullptr, &pPixelShader);
	if (FAILED(hr)) {
		ods("D3D11: Failed to create pixel shader.");
		return false;
	}

	pTexture = nullptr;
	pSRView  = nullptr;

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = pDevice->CreateInputLayout(layout, ARRAY_NUM_ELEMENTS(layout), g_vertex_shader, sizeof(g_vertex_shader),
									&pVertexLayout);
	if (FAILED(hr)) {
		ods("D3D11: pDevice->CreateInputLayout failure!");
		return false;
	}

	pDeviceContext->IASetInputLayout(pVertexLayout);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage          = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth      = VERTEXBUFFER_SIZE;
	bd.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags      = 0;

	hr = pDevice->CreateBuffer(&bd, nullptr, &pVertexBuffer);
	if (FAILED(hr)) {
		ods("D3D11: pDevice->CreateBuffer failure!");
		return false;
	}

	DWORD indices[] = {
		0, 1, 3, 1, 2, 3,
	};

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage          = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth      = sizeof(DWORD) * 6;
	bd.BindFlags      = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags      = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;

	hr = pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer);
	if (FAILED(hr)) {
		ods("D3D11: pDevice->CreateBuffer failure!");
		return false;
	}

	// Set index buffer
	pDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set primitive topology
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (!bDeferredContext) {
		pMyStateBlock->Capture();
		pOrigStateBlock->Apply();
	}
	pBackBuffer->Release();

	return true;
}

D11State::~D11State() {
	if (pBlendState)
		pBlendState->Release();
	if (pVertexBuffer)
		pVertexBuffer->Release();
	if (pIndexBuffer)
		pIndexBuffer->Release();
	if (pVertexLayout)
		pVertexLayout->Release();
	if (pVertexShader)
		pVertexShader->Release();
	if (pPixelShader)
		pPixelShader->Release();
	if (pRTV)
		pRTV->Release();

	if (pTexture)
		pTexture->Release();
	if (pSRView)
		pSRView->Release();

	delete pMyStateBlock;
	pMyStateBlock = nullptr;
	delete pOrigStateBlock;
	pOrigStateBlock = nullptr;

	if (pDeviceContext)
		pDeviceContext->Release();
}

void D11State::draw() {
	clock_t t     = clock();
	float elapsed = static_cast< float >(t - timeT) / CLOCKS_PER_SEC;
	++frameCount;
	if (elapsed > OVERLAY_FPS_INTERVAL) {
		OverlayMsg om;
		om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType  = OVERLAY_MSGTYPE_FPS;
		om.omh.iLength = sizeof(OverlayMsgFps);
		om.omf.fps     = frameCount / elapsed;

		sendMessage(om);

		frameCount = 0;
		timeT      = t;
	}

	checkMessage(static_cast< unsigned int >(vp.Width), static_cast< unsigned int >(vp.Height));

	if (a_ucTexture && pSRView && (uiLeft != uiRight)) {
		if (!bDeferredContext) {
			pOrigStateBlock->Capture();
			pMyStateBlock->Apply();
		}

		// Set vertex buffer
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

		pDeviceContext->VSSetShader(pVertexShader, nullptr, 0);
		pDeviceContext->GSSetShader(nullptr, nullptr, 0);
		pDeviceContext->PSSetShaderResources(0, 1, &pSRView);
		pDeviceContext->PSSetShader(pPixelShader, nullptr, 0);
		pDeviceContext->DrawIndexed(6, 0, 0);

		if (bDeferredContext) {
			ID3D11CommandList *pCommandList;
			pDeviceContext->FinishCommandList(TRUE, &pCommandList);
			ID3D11DeviceContext *ctx = nullptr;
			pDevice->GetImmediateContext(&ctx);
			ctx->ExecuteCommandList(pCommandList, TRUE);
			ctx->Release();
			pCommandList->Release();
		} else {
			pDeviceContext->Flush();
			pMyStateBlock->Capture();
			pOrigStateBlock->Apply();
		}
	}
}


// D3D11 specific logic for the Present function.
extern void presentD3D11(IDXGISwapChain *pSwapChain) {
	ID3D11Device *pDevice = nullptr;
	HRESULT hr            = pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **) &pDevice);
	if (SUCCEEDED(hr) && pDevice) {
		SwapchainMap::iterator it = chains.find(pSwapChain);
		D11State *ds              = it != chains.end() ? it->second : nullptr;
		if (ds && ds->pDevice != pDevice) {
			ods("D3D11: SwapChain device changed");
			devices.erase(ds->pDevice);
			delete ds;
			ds = nullptr;
		}
		if (!ds) {
			ods("D3D11: New state");
			ds = new D11State(pSwapChain, pDevice);
			if (!ds->init()) {
				pDevice->Release();
				delete ds;
				return;
			}

			chains[pSwapChain] = ds;
			devices[pDevice]   = ds;
		}

		ds->draw();
		pDevice->Release();
	} else {
#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
		// DXGI is used for multiple D3D versions. Thus, this is expected if
		// another version is used (like D3D10).
		ods("D3D11: Could not draw because ID3D11Device could not be retrieved.");
#endif
	}
}

extern void resizeD3D11(IDXGISwapChain *pSwapChain) {
	// Remove the D11State from our "cache" (= Invalidate)
	SwapchainMap::iterator it = chains.find(pSwapChain);
	if (it != chains.end()) {
		D11State *ds = it->second;
		devices.erase(ds->pDevice);
		chains.erase(pSwapChain);
		delete ds;
	}
}

static ULONG __stdcall myAddRef(ID3D11Device *pDevice) {
	// TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	AddRefType oAddRef = (AddRefType) hhAddRef.call;
	hhAddRef.restore();
	ULONG res = oAddRef(pDevice);
	hhAddRef.inject();

	Mutex m;
	DeviceMap::iterator it = devices.find(pDevice);
	if (it != devices.end()) {
		it->second->lHighMark = res;
	}

	return res;
}

static ULONG __stdcall myRelease(ID3D11Device *pDevice) {
	// TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	ReleaseType oRelease = (ReleaseType) hhRelease.call;
	hhRelease.restore();
	ULONG res = oRelease(pDevice);
	hhRelease.inject();

	Mutex m;
	DeviceMap::iterator it = devices.find(pDevice);
	if (it != devices.end()) {
		D11State *ds = it->second;
		// If we are receiving a lot of subsequent releases lets eagerly drop
		// our state object. If the device presents something again a state
		// object is created again just fine anyway.
		if (res < ds->lHighMark / 2) {
			ods("D3D11: Deleting resources %u < 0.5 * %u", res, ds->lHighMark);
			devices.erase(it);
			chains.erase(ds->pSwapChain);
			delete ds;
			ods("D3D11: Deleted");
			ds = nullptr;
		}
	}

	return res;
}

static void HookAddRelease(voidFunc vfAdd, voidFunc vfRelease) {
	ods("D3D11: Injecting device add/remove");
	hhAddRef.setup(vfAdd, reinterpret_cast< voidFunc >(myAddRef));
	hhRelease.setup(vfRelease, reinterpret_cast< voidFunc >(myRelease));
}

void hookD3D11(HMODULE hD3D11, bool preonly);

void checkDXGI11Hook(bool preonly) {
	static bool bCheckHookActive = false;
	if (bCheckHookActive) {
		ods("D3D11: Recursion in checkDXGI11Hook");
		return;
	}

	if (d3d11->offsetAddRef == 0 || d3d11->offsetRelease == 0) {
		return;
	}

	bCheckHookActive = true;

	HMODULE hDXGI  = GetModuleHandleW(L"DXGI.DLL");
	HMODULE hD3D11 = GetModuleHandleW(L"D3D11.DLL");

	if (hDXGI && hD3D11) {
		if (!bHooked) {
			hookD3D11(hD3D11, preonly);
		}
	} else {
#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
		if (hDXGI) {
			ods("D3D11: No DXGI.DLL found as loaded. No hooking at this point.");
		} else {
			ods("D3D11: No D3D11.DLL found as loaded. No hooking at this point.");
		}
#endif
	}

	bCheckHookActive = false;
}

/// @param hD3D11 must be a valid module handle
void hookD3D11(HMODULE hD3D11, bool preonly) {
	// Add a ref to ourselves; we do NOT want to get unloaded directly from this process.
	HMODULE hTempSelf = nullptr;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast< LPCTSTR >(&hookD3D11), &hTempSelf);

	bHooked = true;

	const int modulenamesize = MODULEFILEPATH_BUFLEN;
	wchar_t modulename[modulenamesize];
	GetModuleFileNameW(hD3D11, modulename, modulenamesize);

	if (_wcsicmp(d3d11->wcFileName, modulename) == 0) {
		unsigned char *raw = (unsigned char *) hD3D11;
		HookAddRelease((voidFunc)(raw + d3d11->offsetAddRef), (voidFunc)(raw + d3d11->offsetRelease));
	} else if (!preonly) {
		ods("D3D11: Interface changed, can't rawpatch. Current: %ls ; Previously: %ls", modulename, d3d11->wcFileName);
	} else {
		bHooked = false;
	}
}

/// Prepares DXGI and D3D11 data by trying to determine the module filepath
/// and function offsets in memory.
/// (This data can later be used for hooking / code injection.)
///
/// Adjusts the data behind the global variables dxgi and d3d11.
void PrepareDXGI11(IDXGIAdapter1 *pAdapter, bool initializeDXGIData) {
	if (!dxgi || !d3d11 || !pAdapter)
		return;

	ods("D3D11: Preparing static data for DXGI and D3D11 Injection");

	d3d11->wcFileName[0] = 0;
	d3d11->offsetAddRef  = 0;
	d3d11->offsetRelease = 0;

	HMODULE hD3D11 = LoadLibrary("D3D11.DLL");

	if (hD3D11) {
		HWND hwnd = CreateWindowW(L"STATIC", L"Mumble DXGI Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
								  640, 480, 0, nullptr, nullptr, 0);

		D3D11CreateDeviceAndSwapChainType pD3D11CreateDeviceAndSwapChain =
			reinterpret_cast< D3D11CreateDeviceAndSwapChainType >(
				GetProcAddress(hD3D11, "D3D11CreateDeviceAndSwapChain"));

		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		RECT rcWnd;
		BOOL success = GetClientRect(hwnd, &rcWnd);
		if (success) {
			desc.BufferDesc.Width  = rcWnd.right - rcWnd.left;
			desc.BufferDesc.Height = rcWnd.bottom - rcWnd.top;

			ods("D3D11: Got ClientRect W %d H %d", desc.BufferDesc.Width, desc.BufferDesc.Height);

			desc.BufferDesc.RefreshRate.Numerator   = 60;
			desc.BufferDesc.RefreshRate.Denominator = 1;
			desc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			desc.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			desc.BufferDesc.Scaling                 = DXGI_MODE_SCALING_CENTERED;

			desc.SampleDesc.Count   = 1;
			desc.SampleDesc.Quality = 0;

			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

			desc.BufferCount = 2;

			desc.OutputWindow = hwnd;

			desc.Windowed = true;

			desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

			IDXGISwapChain *pSwapChain = nullptr;
			ID3D11Device *pDevice      = nullptr;
			D3D_FEATURE_LEVEL featureLevel;
			ID3D11DeviceContext *pDeviceContext = nullptr;
			HRESULT hr = pD3D11CreateDeviceAndSwapChain(pAdapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, 0, nullptr, 0,
														D3D11_SDK_VERSION, &desc, &pSwapChain, &pDevice, &featureLevel,
														&pDeviceContext);
			if (FAILED(hr))
				ods("D3D11: pD3D11CreateDeviceAndSwapChain failure!");

			if (pDevice && pDeviceContext && pSwapChain) {
				// For VC++ the vtable is located at the base addr. of the object and each function entry is a single
				// pointer. Since p.e. the base classes of IDXGISwapChain have a total of 8 functions the 8+Xth entry
				// points to the Xth added function in the derived interface.

				void ***vtbl = (void ***) pSwapChain;

				void *pPresent = (*vtbl)[8];
				boost::optional< size_t > offset =
					GetFnOffsetInModule(reinterpret_cast< voidFunc >(pPresent), dxgi->wcFileName,
										ARRAY_NUM_ELEMENTS(dxgi->wcFileName), "D3D11", "Present");
				if (offset) {
					if (initializeDXGIData) {
						dxgi->offsetPresent = *offset;
						ods("D3D11: Successfully found Present offset: %ls: %d", dxgi->wcFileName, dxgi->offsetPresent);
					} else {
						if (dxgi->offsetPresent == *offset) {
							ods("D3D11: Successfully verified Present offset: %ls: %d", dxgi->wcFileName,
								dxgi->offsetPresent);
						} else {
							ods("D3D11: Failed to verify Present offset for %ls. Found %d, but previously found %d.",
								dxgi->wcFileName, offset, dxgi->offsetPresent);
						}
					}
				}

				void *pResize = (*vtbl)[13];
				offset        = GetFnOffsetInModule(reinterpret_cast< voidFunc >(pResize), dxgi->wcFileName,
                                             ARRAY_NUM_ELEMENTS(dxgi->wcFileName), "D3D11", "ResizeBuffers");
				if (offset) {
					if (initializeDXGIData) {
						dxgi->offsetResize = *offset;
						ods("D3D11: Successfully found ResizeBuffers offset: %ls: %d", dxgi->wcFileName,
							dxgi->offsetResize);
					} else {
						if (dxgi->offsetResize == *offset) {
							ods("D3D11: Successfully verified ResizeBuffers offset: %ls: %d", dxgi->wcFileName,
								dxgi->offsetResize);
						} else {
							ods("D3D11: Failed to verify ResizeBuffers offset for %ls. Found %d, but previously found "
								"%d.",
								dxgi->wcFileName, offset, dxgi->offsetResize);
						}
					}
				}

				vtbl = (void ***) pDevice;

				void *pAddRef = (*vtbl)[1];
				offset        = GetFnOffsetInModule(reinterpret_cast< voidFunc >(pAddRef), d3d11->wcFileName,
                                             ARRAY_NUM_ELEMENTS(d3d11->wcFileName), "D3D11", "AddRef");
				if (offset) {
					d3d11->offsetAddRef = *offset;
					ods("D3D11: Successfully found AddRef offset: %ls: %d", d3d11->wcFileName, d3d11->offsetAddRef);
				}

				void *pRelease = (*vtbl)[2];
				offset         = GetFnOffsetInModule(reinterpret_cast< voidFunc >(pRelease), d3d11->wcFileName,
                                             ARRAY_NUM_ELEMENTS(d3d11->wcFileName), "D3D11", "Release");
				if (offset) {
					d3d11->offsetRelease = *offset;
					ods("D3D11: Successfully found Release offset: %ls: %d", d3d11->wcFileName, d3d11->offsetRelease);
				}
			}

			if (pDevice)
				pDevice->Release();
			if (pDeviceContext)
				pDeviceContext->Release();
			if (pSwapChain)
				pSwapChain->Release();

		} else {
			FreeLibrary(hD3D11);
		}

		DestroyWindow(hwnd);
	} else {
		FreeLibrary(hD3D11);
	}
}
