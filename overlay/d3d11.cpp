/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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
#include "D11StateBlock.h"
#include "overlay11.hex"
#include "Effects11/Inc/d3dx11effect.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <time.h>

static bool bHooked = false;
static bool bChaining = false;
static HardHook hhPresent;
static HardHook hhResize;
static HardHook hhAddRef;
static HardHook hhRelease;

typedef HRESULT(__stdcall *CreateDXGIFactory1Type)(REFIID, void **);
typedef HRESULT(__stdcall *D3D11CreateDeviceAndSwapChainType)(IDXGIAdapter *, D3D_DRIVER_TYPE, HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL *, UINT, UINT, const DXGI_SWAP_CHAIN_DESC *, IDXGISwapChain **, ID3D11Device **, D3D_FEATURE_LEVEL *, ID3D11DeviceContext **);
typedef HRESULT(__stdcall *PresentType)(IDXGISwapChain *, UINT, UINT);
typedef HRESULT(__stdcall *ResizeBuffersType)(IDXGISwapChain *, UINT, UINT, UINT, DXGI_FORMAT, UINT);
typedef ULONG(__stdcall *AddRefType)(ID3D11Device *);
typedef ULONG(__stdcall *ReleaseType)(ID3D11Device *);

#define HMODREF(mod, func) func##Type p##func = (func##Type) GetProcAddress(mod, #func)

struct SimpleVertex {
	D3DXVECTOR3 Pos;
	D3DXVECTOR2 Tex;
};

class D11State: protected Pipe {
	public:
		LONG lHighMark;

		LONG initRefCount;
		LONG refCount;
		LONG myRefCount;
		DWORD dwMyThread;

		D3D11_VIEWPORT vp;

		ID3D11Device *pDevice;
		ID3D11DeviceContext *pDeviceContext;
		bool bDeferredContext;
		IDXGISwapChain *pSwapChain;

		D11StateBlock *pOrigStateBlock;
		D11StateBlock *pMyStateBlock;
		ID3D11RenderTargetView *pRTV;
		ID3DX11Effect *pEffect;
		ID3DX11EffectTechnique *pTechnique;
		ID3DX11EffectShaderResourceVariable * pDiffuseTexture;
		ID3D11InputLayout *pVertexLayout;
		ID3D11Buffer *pVertexBuffer;
		ID3D11Buffer *pIndexBuffer;
		ID3D11BlendState *pBlendState;

		ID3D11Texture2D *pTexture;
		ID3D11ShaderResourceView *pSRView;

		clock_t timeT;
		unsigned int frameCount;

		D11State(IDXGISwapChain *, ID3D11Device *);
		~D11State();
		void init();
		void draw();

		void blit(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
		void setRect();
		void newTexture(unsigned int w, unsigned int h);
};

map<IDXGISwapChain *, D11State *> chains;
map<ID3D11Device *, D11State *> devices;

D11State::D11State(IDXGISwapChain *pSwapChain, ID3D11Device *pDevice) {
	this->pSwapChain = pSwapChain;
	this->pDevice = pDevice;

	lHighMark = initRefCount  = refCount = myRefCount = 0;
	dwMyThread = 0;

	ZeroMemory(&vp, sizeof(vp));

	pOrigStateBlock = NULL;
	pMyStateBlock = NULL;
	pRTV = NULL;
	pEffect = NULL;
	pTechnique = NULL;
	pDiffuseTexture = NULL;
	pVertexLayout = NULL;
	pVertexBuffer = NULL;
	pIndexBuffer = NULL;
	pBlendState = NULL;
	pTexture = NULL;
	pSRView = NULL;
	pDeviceContext = NULL;

	timeT = clock();
	frameCount = 0;

	pDevice->AddRef();
	initRefCount = pDevice->Release();
}

void D11State::blit(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	HRESULT hr;

	ods("D3D11: Blit %d %d %d %d", x, y, w, h);

	if (! pTexture || ! pSRView)
		return;

	D3D11_MAPPED_SUBRESOURCE mappedTex;
	hr = pDeviceContext->Map(pTexture, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE_DISCARD, 0, &mappedTex);
	if (FAILED(hr)) {
		ods("D3D11: Failed map");
	}

	UCHAR* pTexels = (UCHAR*)mappedTex.pData;

	for (unsigned int r=0;r< uiHeight; ++r) {
		unsigned char *sptr = a_ucTexture + r * uiWidth * 4;
		unsigned char *dptr = reinterpret_cast<unsigned char *>(pTexels) + r * mappedTex.RowPitch;
		memcpy(dptr, sptr, uiWidth * 4);
	}

	pDeviceContext->Unmap(pTexture, D3D11CalcSubresource(0, 0, 1));
}

void D11State::setRect() {
	HRESULT hr;

	ods("D3D11: SetRect");

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

	left = 2.0f * (left / vp.Width) - 1.0f;
	right = 2.0f * (right / vp.Width) - 1.0f;
	top = -2.0f * (top / vp.Height) + 1.0f;
	bottom = -2.0f * (bottom / vp.Height) + 1.0f;

	ods("D3D11-setRect: Vertex (%f %f) (%f %f)", left, top, right, bottom);

	// Create vertex buffer
	SimpleVertex vertices[] = {
		{ D3DXVECTOR3(left, top, 0.5f), D3DXVECTOR2(texl, text) },
		{ D3DXVECTOR3(right, top, 0.5f), D3DXVECTOR2(texr, text) },
		{ D3DXVECTOR3(right, bottom, 0.5f), D3DXVECTOR2(texr, texb) },
		{ D3DXVECTOR3(left, bottom, 0.5f), D3DXVECTOR2(texl, texb) },
	};


	// map/unmap to temporarily deny GPU access to the resource pVertexBuffer
	D3D11_MAPPED_SUBRESOURCE res;
	hr = pDeviceContext->Map(pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
	//TODO no size-checks? Is that correct and safe?
	memcpy(res.pData, vertices, sizeof(vertices));
	ods("Map: %lx %d", hr, sizeof(vertices));
	pDeviceContext->Unmap(pVertexBuffer, 0);
}

void D11State::newTexture(unsigned int w, unsigned int h) {
	HRESULT hr;

	ods("D3D11: newTex %d %d", w, h);

	if (pTexture) {
		pTexture->Release();
		pTexture = NULL;
	}
	if (pSRView) {
		pSRView->Release();
		pSRView = NULL;
	}

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width = w;
	desc.Height = h;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = pDevice->CreateTexture2D(&desc, NULL, &pTexture);

	if (! SUCCEEDED(hr)) {
		pTexture = NULL;
		ods("D3D11: Failed to create texture.");
		return;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;

	hr = pDevice->CreateShaderResourceView(pTexture, &srvDesc, &pSRView);
	if (! SUCCEEDED(hr)) {
		pSRView = NULL;
		pTexture->Release();
		pTexture = NULL;
		ods("D3D11: Failed to create resource view.");
		return;
	}
}

void D11State::init() {
	HRESULT hr;

	dwMyThread = GetCurrentThreadId();


	ID3D11Texture2D* pBackBuffer = NULL;
	hr = pSwapChain->GetBuffer(0, __uuidof(*pBackBuffer), (LPVOID*)&pBackBuffer);

	hr = pDevice->CreateDeferredContext(0, &pDeviceContext);

	if (! SUCCEEDED(hr) || !pDeviceContext) {
		ods("D3D11: Failed to create DeferredContext (0x%x). Getting ImmediateContext", hr);
		pDevice->GetImmediateContext(&pDeviceContext);
		D11CreateStateBlock(pDeviceContext, &pOrigStateBlock);
		D11CreateStateBlock(pDeviceContext, &pMyStateBlock);

		pOrigStateBlock->Capture();
		bDeferredContext=false;
	} else {
		bDeferredContext=true;
	}

	D3D11_TEXTURE2D_DESC backBufferSurfaceDesc;
	pBackBuffer->GetDesc(&backBufferSurfaceDesc);

	ZeroMemory(&vp, sizeof(vp));
	vp.Width = (float) backBufferSurfaceDesc.Width;
	vp.Height = (float) backBufferSurfaceDesc.Height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDeviceContext->RSSetViewports(1, &vp);

	hr = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRTV);

	pDeviceContext->OMSetRenderTargets(1, &pRTV, NULL);

	D3D11_BLEND_DESC blend;
	ZeroMemory(&blend, sizeof(blend));
	blend.RenderTarget[0].BlendEnable = TRUE;
	blend.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	pDevice->CreateBlendState(&blend, &pBlendState);
	float bf[4];
	pDeviceContext->OMSetBlendState(pBlendState, bf, 0xffffffff);

	hr = D3DX11CreateEffectFromMemory((void *) g_main11, sizeof(g_main11), 0, pDevice, &pEffect);

	pTechnique = pEffect->GetTechniqueByName("Render");
	pDiffuseTexture = pEffect->GetVariableByName("txDiffuse")->AsShaderResource();

	pTexture = NULL;
	pSRView = NULL;

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// Create the input layout
	D3DX11_PASS_DESC PassDesc;
	pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
	hr = pDevice->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pVertexLayout);
	pDeviceContext->IASetInputLayout(pVertexLayout);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(SimpleVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	hr = pDevice->CreateBuffer(&bd, NULL, &pVertexBuffer);

	DWORD indices[] = {
		0,1,3,
		1,2,3,
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DWORD) * 6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;
	hr = pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer);

	// Set index buffer
	pDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set primitive topology
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (!bDeferredContext) {
		pMyStateBlock->Capture();
		pOrigStateBlock->Apply();
	}
	pBackBuffer->Release();

	dwMyThread = 0;
}

D11State::~D11State() {
	pBlendState->Release();
	pVertexBuffer->Release();
	pIndexBuffer->Release();
	pVertexLayout->Release();
	pEffect->Release();
	pRTV->Release();
	if (pTexture)
		pTexture->Release();
	if (pSRView)
		pSRView->Release();

	if (pMyStateBlock) {
		pMyStateBlock->ReleaseAllDeviceObjects();
		pMyStateBlock->ReleaseObjects();
	}

	if (pOrigStateBlock) {
		pOrigStateBlock->ReleaseAllDeviceObjects();
		pOrigStateBlock->ReleaseObjects();
	}

	if (pDeviceContext)
		pDeviceContext->Release();
}

void D11State::draw() {
	ods("D3D11-draw: DrawBegin");
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

	dwMyThread = GetCurrentThreadId();

	checkMessage((unsigned int)vp.Width, (unsigned int)vp.Height);

	if (a_ucTexture && pSRView && (uiLeft != uiRight)) {
		if (!bDeferredContext) {
			pOrigStateBlock->Capture();
			pMyStateBlock->Apply();
		}
		HRESULT hr;

		D3DX11_TECHNIQUE_DESC techDesc;
		pTechnique->GetDesc(&techDesc);

		// Set vertex buffer
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

		hr = pDiffuseTexture->SetResource(pSRView);
		if (! SUCCEEDED(hr))
			ods("D3D11: Failed to set resource");

		for (UINT p = 0; p < techDesc.Passes; ++p) {
			pTechnique->GetPassByIndex(p)->Apply(0, pDeviceContext);
			pDeviceContext->DrawIndexed(6, 0, 0);
		}

		if (bDeferredContext) {
			ID3D11CommandList *pCommandList;
			pDeviceContext->FinishCommandList(TRUE, &pCommandList);
			ID3D11DeviceContext *ctx = NULL;
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

	dwMyThread = 0;
}



static HRESULT __stdcall myPresent(IDXGISwapChain *pSwapChain, UINT SyncInterval, UINT Flags) {
	HRESULT hr;

	ID3D11Device *pDevice = NULL;

	ods("DXGI11: myPresent");

	hr = pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **) &pDevice);
	if (pDevice) {
		D11State *ds = chains[pSwapChain];
		if (ds && ds->pDevice != pDevice) {
			ods("DXGI11: SwapChain device changed");
			devices.erase(ds->pDevice);
			delete ds;
			ds = NULL;
		}
		if (! ds) {
			ods("DXGI11: New state");
			ds = new D11State(pSwapChain, pDevice);
			chains[pSwapChain] = ds;
			devices[pDevice] = ds;
			ds->init();
		}

		ds->draw();
		pDevice->Release();
	}

	PresentType oPresent = (PresentType) hhPresent.call;
	hhPresent.restore();
	hr = oPresent(pSwapChain, SyncInterval, Flags);
	hhPresent.inject();
	return hr;
}

static HRESULT __stdcall myResize(IDXGISwapChain *pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {
	HRESULT hr;

	D11State *ds = chains[pSwapChain];
	if (ds) {
		devices.erase(ds->pDevice);
		chains.erase(pSwapChain);
		delete ds;
	}

	ResizeBuffersType oResize = (ResizeBuffersType) hhResize.call;
	hhResize.restore();
	hr = oResize(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
	hhResize.inject();
	return hr;
}

static ULONG __stdcall myAddRef(ID3D11Device *pDevice) {
	AddRefType oAddRef = (AddRefType) hhAddRef.call;

	hhAddRef.restore();
	LONG res = oAddRef(pDevice);
	hhAddRef.inject();

	Mutex m;
	D11State *ds = devices[pDevice];
	if (ds)
		ds->lHighMark = res;

	return res;
}

static ULONG __stdcall myRelease(ID3D11Device *pDevice) {
	ReleaseType oRelease = (ReleaseType) hhRelease.call;

	hhRelease.restore();
	LONG res = oRelease(pDevice);
	hhRelease.inject();

	Mutex m;
	D11State *ds = devices[pDevice];
	if (ds)
		if (res < (ds->lHighMark / 2)) {
			ods("D3D11: Deleting resources %d < .5 %d", res, ds->lHighMark);
			devices.erase(ds->pDevice);
			chains.erase(ds->pSwapChain);
			delete ds;
			ods("D3D11: Deleted");
			ds = NULL;
		}

	return res;
}

static void HookAddRelease(voidFunc vfAdd, voidFunc vfRelease) {
	ods("D3D11: Injecting device add/remove");
	hhAddRef.setup(vfAdd, reinterpret_cast<voidFunc>(myAddRef));
	hhRelease.setup(vfRelease, reinterpret_cast<voidFunc>(myRelease));
}

static void HookPresentRaw(voidFunc vfPresent) {
	hhPresent.setup(vfPresent, reinterpret_cast<voidFunc>(myPresent));
}

static void HookResizeRaw(voidFunc vfResize) {
	ods("DXGI11: Injecting ResizeBuffers Raw");
	hhResize.setup(vfResize, reinterpret_cast<voidFunc>(myResize));
}

void checkDXGI11Hook(bool preonly) {
	if (bChaining) {
		return;
		ods("DXGI11: Causing a chain");
	}

	if (! dxgi->iOffsetPresent || ! dxgi->iOffsetResize)
		return;

	bChaining = true;

	HMODULE hDXGI = GetModuleHandleW(L"DXGI.DLL");
	HMODULE hD3D11 = GetModuleHandleW(L"D3D11.DLL");

	if (hDXGI && hD3D11) {
		if (! bHooked) {
			wchar_t procname[2048];
			GetModuleFileNameW(NULL, procname, 2048);
			fods("DXGI11: Hookcheck '%ls'", procname);
			bHooked = true;

			// Add a ref to ourselves; we do NOT want to get unloaded directly from this process.
			GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<char *>(&checkDXGI11Hook), &hSelf);

			// Can we use the prepatch data?
			GetModuleFileNameW(hDXGI, procname, 2048);
			if (_wcsicmp(dxgi->wcDXGIFileName, procname) == 0) {
				unsigned char *raw = (unsigned char *) hDXGI;
				HookPresentRaw((voidFunc)(raw + dxgi->iOffsetPresent));
				HookResizeRaw((voidFunc)(raw + dxgi->iOffsetResize));

				GetModuleFileNameW(hD3D11, procname, 2048);
				if (_wcsicmp(dxgi->wcD3D11FileName, procname) == 0) {
					unsigned char *raw = (unsigned char *) hD3D11;
					HookAddRelease((voidFunc)(raw + dxgi->iOffsetAddRef), (voidFunc)(raw + dxgi->iOffsetRelease));
				}
			} else if (! preonly) {
				fods("DXGI11: DXGI Interface changed, can't rawpatch");
			} else {
				bHooked = false;
			}
		}
	}

	bChaining = false;
}

extern "C" __declspec(dllexport) void __cdecl PrepareDXGI11() {
	if (! dxgi)
		return;

	ods("DXGI11: Preparing static data for Injection");

	dxgi->wcDXGIFileName[0] = 0;
	dxgi->wcD3D11FileName[0] = 0;
	dxgi->iOffsetPresent = 0;
	dxgi->iOffsetResize = 0;
	dxgi->iOffsetAddRef = 0;
	dxgi->iOffsetRelease = 0;

	OSVERSIONINFOEXW ovi;
	memset(&ovi, 0, sizeof(ovi));
	ovi.dwOSVersionInfoSize = sizeof(ovi);
	GetVersionExW(reinterpret_cast<OSVERSIONINFOW *>(&ovi));
	if (ovi.dwMajorVersion >= 7 || ((ovi.dwMajorVersion == 6) && (ovi.dwBuildNumber >= 7600))) {
		HMODULE hD3D11 = LoadLibrary("D3D11.DLL");
		HMODULE hDXGI = LoadLibrary("DXGI.DLL");

		if (hDXGI != NULL && hD3D11 != NULL) {
			CreateDXGIFactory1Type pCreateDXGIFactory1 = reinterpret_cast<CreateDXGIFactory1Type>(GetProcAddress(hDXGI, "CreateDXGIFactory1"));
			ods("DXGI11: Got %p", pCreateDXGIFactory1);
			if (pCreateDXGIFactory1) {
				HRESULT hr;
				IDXGIFactory1 * pFactory;
				hr = pCreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&pFactory));
				if (pFactory) {
					HWND hwnd = CreateWindowW(L"STATIC", L"Mumble DXGI1 Window", WS_OVERLAPPEDWINDOW,
								  CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, 0,
								  NULL, NULL, 0);

					IDXGIAdapter1 *pAdapter = NULL;
					pFactory->EnumAdapters1(0, &pAdapter);

					D3D11CreateDeviceAndSwapChainType pD3D11CreateDeviceAndSwapChain = reinterpret_cast<D3D11CreateDeviceAndSwapChainType>(GetProcAddress(hD3D11, "D3D11CreateDeviceAndSwapChain"));

					IDXGISwapChain *pSwapChain = NULL;
					ID3D11Device *pDevice = NULL;
					ID3D11DeviceContext *pDeviceContext = NULL;

					DXGI_SWAP_CHAIN_DESC desc;
					ZeroMemory(&desc, sizeof(desc));

					RECT rcWnd;
					GetClientRect(hwnd, &rcWnd);
					desc.BufferDesc.Width = rcWnd.right - rcWnd.left;
					desc.BufferDesc.Height = rcWnd.bottom - rcWnd.top;

					ods("DXGI11: W %d H %d", desc.BufferDesc.Width, desc.BufferDesc.Height);

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

					D3D_FEATURE_LEVEL featureLevel;
					hr = pD3D11CreateDeviceAndSwapChain(pAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &desc, &pSwapChain, &pDevice, &featureLevel, &pDeviceContext);

					if (pDevice && pDeviceContext && pSwapChain) {
						HMODULE hRef;
						void ***vtbl = (void ***) pSwapChain;
						void *pPresent = (*vtbl)[8];
						if (! GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (char *) pPresent, &hRef)) {
							ods("DXGI11: Failed to get module for Present");
						} else {
							GetModuleFileNameW(hRef, dxgi->wcDXGIFileName, 2048);
							unsigned char *b = (unsigned char *) pPresent;
							unsigned char *a = (unsigned char *) hRef;
							dxgi->iOffsetPresent = b-a;
							ods("DXGI11: Successfully found Present offset: %ls: %d", dxgi->wcDXGIFileName, dxgi->iOffsetPresent);
						}

						void *pResize = (*vtbl)[13];
						if (! GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (char *) pResize, &hRef)) {
							ods("DXGI11: Failed to get module for ResizeBuffers");
						} else {
							wchar_t buff[2048];
							GetModuleFileNameW(hRef, buff, 2048);
							if (wcscmp(buff, dxgi->wcDXGIFileName) == 0) {
								unsigned char *b = (unsigned char *) pResize;
								unsigned char *a = (unsigned char *) hRef;
								dxgi->iOffsetResize = b-a;
								ods("DXGI11: Successfully found ResizeBuffers offset: %ls: %d", dxgi->wcDXGIFileName, dxgi->iOffsetResize);
							}
						}

						vtbl = (void ***) pDevice;

						void *pAddRef = (*vtbl)[1];
						if (! GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (char *) pAddRef, &hRef)) {
							ods("D3D11: Failed to get module for AddRef");
						} else {
							GetModuleFileNameW(hRef, dxgi->wcD3D11FileName, 2048);
							unsigned char *b = (unsigned char *) pAddRef;
							unsigned char *a = (unsigned char *) hRef;
							dxgi->iOffsetAddRef = b-a;
							ods("D3D11: Successfully found AddRef offset: %ls: %d", dxgi->wcD3D11FileName, dxgi->iOffsetAddRef);
						}

						void *pRelease = (*vtbl)[2];
						if (! GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (char *) pRelease, &hRef)) {
							ods("D3D11: Failed to get module for Release");
						} else {
							wchar_t buff[2048];
							GetModuleFileNameW(hRef, buff, 2048);
							if (wcscmp(buff, dxgi->wcD3D11FileName) == 0) {
								unsigned char *b = (unsigned char *) pRelease;
								unsigned char *a = (unsigned char *) hRef;
								dxgi->iOffsetRelease = b-a;
								ods("D3D11: Successfully found Release offset: %ls: %d", dxgi->wcD3D11FileName, dxgi->iOffsetRelease);
							}
						}
					} else {
						ods("D3D11CreateDeviceAndSwapChain failed");
					}

					if (pDevice)
						pDevice->Release();
					if (pDeviceContext)
						pDeviceContext->Release();
					if (pSwapChain)
						pSwapChain->Release();
					DestroyWindow(hwnd);

					pFactory->Release();
				}
			}
		}
	} else {
		ods("No DXGI1 pre-Win7");
	}
}
