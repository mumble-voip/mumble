// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "lib.h"
#include "overlay.hex"
#include <d3d10.h>
#include <time.h>

D3D10Data *d3d10 = nullptr;

static bool bHooked = false;
static HardHook hhAddRef;
static HardHook hhRelease;

typedef HRESULT(__stdcall *CreateDXGIFactoryType)(REFIID, void **);
typedef HRESULT(__stdcall *D3D10CreateDeviceAndSwapChainType)(IDXGIAdapter *, D3D10_DRIVER_TYPE, HMODULE, UINT, UINT,
															  DXGI_SWAP_CHAIN_DESC *, IDXGISwapChain **,
															  ID3D10Device **);

typedef HRESULT(__stdcall *D3D10CreateStateBlockType)(ID3D10Device *, D3D10_STATE_BLOCK_MASK *, ID3D10StateBlock **);
typedef HRESULT(__stdcall *D3D10StateBlockMaskEnableAllType)(D3D10_STATE_BLOCK_MASK *);
typedef HRESULT(__stdcall *D3D10CreateEffectFromMemoryType)(void *, SIZE_T, UINT, ID3D10Device *, ID3D10EffectPool *,
															ID3D10Effect **);

typedef ULONG(__stdcall *AddRefType)(ID3D10Device *);
typedef ULONG(__stdcall *ReleaseType)(ID3D10Device *);

#define HMODREF(mod, func) func##Type p##func = (func##Type) GetProcAddress(mod, #func)

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

class D10State : protected Pipe {
public:
	ULONG lHighMark;

	LONG initRefCount;
	LONG refCount;
	LONG myRefCount;

	D3D10_VIEWPORT vp;

	ID3D10Device *pDevice;
	IDXGISwapChain *pSwapChain;

	ID3D10StateBlock *pOrigStateBlock;
	ID3D10StateBlock *pMyStateBlock;
	ID3D10RenderTargetView *pRTV;
	ID3D10Effect *pEffect;
	ID3D10EffectTechnique *pTechnique;
	ID3D10EffectShaderResourceVariable *pDiffuseTexture;
	ID3D10InputLayout *pVertexLayout;
	ID3D10Buffer *pVertexBuffer;
	ID3D10Buffer *pIndexBuffer;
	ID3D10BlendState *pBlendState;

	ID3D10Texture2D *pTexture;
	ID3D10ShaderResourceView *pSRView;

	clock_t timeT;
	unsigned int frameCount;

	D10State(IDXGISwapChain *, ID3D10Device *);
	virtual ~D10State();
	bool init();
	void draw();

	virtual void blit(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
	virtual void setRect();
	virtual void newTexture(unsigned int w, unsigned int h);
};

typedef map< IDXGISwapChain *, D10State * > SwapchainMap;
SwapchainMap chains;
typedef map< ID3D10Device *, D10State * > DeviceMap;
DeviceMap devices;

D10State::D10State(IDXGISwapChain *pSwapChain, ID3D10Device *pDevice) {
	this->pSwapChain = pSwapChain;
	this->pDevice    = pDevice;

	lHighMark = initRefCount = refCount = myRefCount = 0;
	ZeroMemory(&vp, sizeof(vp));

	pOrigStateBlock = nullptr;
	pMyStateBlock   = nullptr;
	pRTV            = nullptr;
	pEffect         = nullptr;
	pTechnique      = nullptr;
	pDiffuseTexture = nullptr;
	pVertexLayout   = nullptr;
	pVertexBuffer   = nullptr;
	pIndexBuffer    = nullptr;
	pBlendState     = nullptr;
	pTexture        = nullptr;
	pSRView         = nullptr;

	timeT      = clock();
	frameCount = 0;

	pDevice->AddRef();
	initRefCount = pDevice->Release();
}

void D10State::blit(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	HRESULT hr;

	ods("D3D10: Blit %d %d %d %d", x, y, w, h);

	if (!pTexture || !pSRView || uiLeft == uiRight)
		return;

	D3D10_MAPPED_TEXTURE2D mappedTex;
	hr = pTexture->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE_DISCARD, 0, &mappedTex);
	if (FAILED(hr)) {
		ods("D3D10: Failed map");
	}

	UCHAR *pTexels = (UCHAR *) mappedTex.pData;

	for (unsigned int r = 0; r < uiHeight; ++r) {
		unsigned char *sptr = a_ucTexture + r * uiWidth * 4;
		unsigned char *dptr = reinterpret_cast< unsigned char * >(pTexels) + r * mappedTex.RowPitch;
		memcpy(dptr, sptr, uiWidth * 4);
	}

	pTexture->Unmap(D3D10CalcSubresource(0, 0, 1));
}

void D10State::setRect() {
	HRESULT hr;

	ods("D3D10: SetRect");

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

	ods("D3D10: Vertex (%f %f) (%f %f)", left, top, right, bottom);

	// Create vertex buffer
	SimpleVertex vertices[] = {
		{ SimpleVec3(left, top, 0.5f), SimpleVec2(texl, text) },
		{ SimpleVec3(right, top, 0.5f), SimpleVec2(texr, text) },
		{ SimpleVec3(right, bottom, 0.5f), SimpleVec2(texr, texb) },
		{ SimpleVec3(left, bottom, 0.5f), SimpleVec2(texl, texb) },
	};

	void *pData = nullptr;

	hr = pVertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &pData);
	memcpy(pData, vertices, sizeof(vertices));
	ods("D3D10: Map %lx %d", hr, sizeof(vertices));
	pVertexBuffer->Unmap();
}

void D10State::newTexture(unsigned int w, unsigned int h) {
	HRESULT hr;

	ods("D3D10: newTexture %d %d", w, h);

	if (pTexture) {
		pTexture->Release();
		pTexture = nullptr;
	}
	if (pSRView) {
		pSRView->Release();
		pSRView = nullptr;
	}

	D3D10_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width     = w;
	desc.Height    = h;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format                     = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count           = 1;
	desc.Usage                      = D3D10_USAGE_DYNAMIC;
	desc.BindFlags                  = D3D10_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags             = D3D10_CPU_ACCESS_WRITE;
	hr                              = pDevice->CreateTexture2D(&desc, nullptr, &pTexture);

	if (FAILED(hr)) {
		pTexture = nullptr;
		ods("D3D10: Failed to create texture.");
		return;
	}

	D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format                    = desc.Format;
	srvDesc.ViewDimension             = D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels       = desc.MipLevels;

	hr = pDevice->CreateShaderResourceView(pTexture, &srvDesc, &pSRView);
	if (FAILED(hr)) {
		pSRView = nullptr;
		pTexture->Release();
		pTexture = nullptr;
		ods("D3D10: Failed to create resource view.");
		return;
	}
}

bool D10State::init() {
	static HMODREF(GetModuleHandleW(L"D3D10.DLL"), D3D10CreateEffectFromMemory);
	static HMODREF(GetModuleHandleW(L"D3D10.DLL"), D3D10CreateStateBlock);
	static HMODREF(GetModuleHandleW(L"D3D10.DLL"), D3D10StateBlockMaskEnableAll);

	if (!pD3D10CreateEffectFromMemory || !pD3D10CreateStateBlock || !pD3D10StateBlockMaskEnableAll) {
		ods("D3D10: Could not get handles for all required D3D10 state initialization functions");
		return false;
	}

	HRESULT hr;

	D3D10_STATE_BLOCK_MASK StateBlockMask;
	ZeroMemory(&StateBlockMask, sizeof(StateBlockMask));
	hr = pD3D10StateBlockMaskEnableAll(&StateBlockMask);
	if (FAILED(hr)) {
		ods("D3D10: D3D10StateBlockMaskEnableAll failed");
		return false;
	}

	hr = pD3D10CreateStateBlock(pDevice, &StateBlockMask, &pOrigStateBlock);
	if (FAILED(hr)) {
		ods("D3D10: D3D10CreateStateBlock for pOrigStateBlock failed");
		return false;
	}

	hr = pD3D10CreateStateBlock(pDevice, &StateBlockMask, &pMyStateBlock);
	if (FAILED(hr)) {
		ods("D3D10: D3D10CreateStateBlock for pMyStateBlock failed");
		return false;
	}

	hr = pOrigStateBlock->Capture();
	if (FAILED(hr)) {
		ods("D3D10: Failed to store original state block during init");
		return false;
	}

	ID3D10Texture2D *pBackBuffer = nullptr;
	hr                           = pSwapChain->GetBuffer(0, __uuidof(*pBackBuffer), (LPVOID *) &pBackBuffer);
	if (FAILED(hr)) {
		ods("D3D10: pSwapChain->GetBuffer failure!");
		return false;
	}

	pDevice->ClearState();

	D3D10_TEXTURE2D_DESC backBufferSurfaceDesc;
	pBackBuffer->GetDesc(&backBufferSurfaceDesc);

	ZeroMemory(&vp, sizeof(vp));
	vp.Width    = backBufferSurfaceDesc.Width;
	vp.Height   = backBufferSurfaceDesc.Height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDevice->RSSetViewports(1, &vp);

	hr = pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRTV);
	if (FAILED(hr)) {
		ods("D3D10: pDevice->CreateRenderTargetView failed!");
		return false;
	}

	pDevice->OMSetRenderTargets(1, &pRTV, nullptr);

	// Settings for an "over" operation.
	// https://en.wikipedia.org/w/index.php?title=Alpha_compositing&oldid=580659153#Description
	D3D10_BLEND_DESC blend;
	ZeroMemory(&blend, sizeof(blend));
	blend.BlendEnable[0]           = TRUE;
	blend.SrcBlend                 = D3D10_BLEND_ONE;
	blend.DestBlend                = D3D10_BLEND_INV_SRC_ALPHA;
	blend.BlendOp                  = D3D10_BLEND_OP_ADD;
	blend.SrcBlendAlpha            = D3D10_BLEND_ONE;
	blend.DestBlendAlpha           = D3D10_BLEND_INV_SRC_ALPHA;
	blend.BlendOpAlpha             = D3D10_BLEND_OP_ADD;
	blend.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	hr = pDevice->CreateBlendState(&blend, &pBlendState);
	if (FAILED(hr)) {
		ods("D3D10: pDevice->CreateBlendState failed!");
		return false;
	}

	pDevice->OMSetBlendState(pBlendState, nullptr, 0xffffffff);

	hr = pD3D10CreateEffectFromMemory((void *) g_main, sizeof(g_main), 0, pDevice, nullptr, &pEffect);
	if (FAILED(hr)) {
		ods("D3D10: D3D10CreateEffectFromMemory failed!");
		return false;
	}

	pTechnique = pEffect->GetTechniqueByName("Render");
	if (!pTechnique) {
		ods("D3D10: Could not get technique for name 'Render'");
		return false;
	}

	pDiffuseTexture = pEffect->GetVariableByName("txDiffuse")->AsShaderResource();
	if (!pDiffuseTexture) {
		ods("D3D10: Could not get variable by name 'txDiffuse'");
		return false;
	}

	pTexture = nullptr;
	pSRView  = nullptr;

	// Define the input layout
	D3D10_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// Create the input layout
	D3D10_PASS_DESC PassDesc;
	hr = pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
	if (FAILED(hr)) {
		ods("D3D10: Couldn't get pass description for technique");
		return false;
	}

	hr = pDevice->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize,
									&pVertexLayout);
	if (FAILED(hr)) {
		ods("D3D10: pDevice->CreateInputLayout failure!");
		return false;
	}

	pDevice->IASetInputLayout(pVertexLayout);

	D3D10_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage          = D3D10_USAGE_DYNAMIC;
	bd.ByteWidth      = sizeof(SimpleVertex) * 4;
	bd.BindFlags      = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	bd.MiscFlags      = 0;

	hr = pDevice->CreateBuffer(&bd, nullptr, &pVertexBuffer);
	if (FAILED(hr)) {
		ods("D3D10: pDevice->CreateBuffer failure!");
		return false;
	}

	DWORD indices[] = {
		0, 1, 3, 1, 2, 3,
	};

	bd.Usage          = D3D10_USAGE_DEFAULT;
	bd.ByteWidth      = sizeof(DWORD) * 6;
	bd.BindFlags      = D3D10_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags      = 0;
	D3D10_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;

	hr = pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer);
	if (FAILED(hr)) {
		ods("D3D10: pDevice->CreateBuffer failure!");
		return false;
	}

	// Set index buffer
	pDevice->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set primitive topology
	pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	hr = pMyStateBlock->Capture();
	if (FAILED(hr)) {
		ods("D3D10: Failed to capture newly created state block");
		return false;
	}

	hr = pOrigStateBlock->Apply();
	if (FAILED(hr)) {
		ods("D3D10: Failed to restore original state block during init");
		return false;
	}

	pBackBuffer->Release();

	return true;
}

D10State::~D10State() {
	if (pBlendState)
		pBlendState->Release();
	if (pVertexBuffer)
		pVertexBuffer->Release();
	if (pIndexBuffer)
		pIndexBuffer->Release();
	if (pVertexLayout)
		pVertexLayout->Release();
	if (pEffect)
		pEffect->Release();
	if (pRTV)
		pRTV->Release();
	if (pTexture)
		pTexture->Release();
	if (pSRView)
		pSRView->Release();

	if (pMyStateBlock) {
		pMyStateBlock->ReleaseAllDeviceObjects();
		pMyStateBlock->Release();
	}

	if (pOrigStateBlock) {
		pOrigStateBlock->ReleaseAllDeviceObjects();
		pOrigStateBlock->Release();
	}
}

void D10State::draw() {
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

	checkMessage(vp.Width, vp.Height);

	if (a_ucTexture && pSRView && (uiLeft != uiRight)) {
		HRESULT hr;
		pOrigStateBlock->Capture();
		pMyStateBlock->Apply();

		D3D10_TECHNIQUE_DESC techDesc;
		pTechnique->GetDesc(&techDesc);

		// Set vertex buffer
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		pDevice->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

		hr = pDiffuseTexture->SetResource(pSRView);
		if (FAILED(hr))
			ods("D3D10: Failed to set resource");

		for (UINT p = 0; p < techDesc.Passes; ++p) {
			pTechnique->GetPassByIndex(p)->Apply(0);
			pDevice->DrawIndexed(6, 0, 0);
		}
		pOrigStateBlock->Apply();
	}
}

// D3D10 specific logic for the Present function.
void presentD3D10(IDXGISwapChain *pSwapChain) {
	ID3D10Device *pDevice = nullptr;
	HRESULT hr            = pSwapChain->GetDevice(__uuidof(ID3D10Device), (void **) &pDevice);
	if (SUCCEEDED(hr) && pDevice) {
		SwapchainMap::iterator it = chains.find(pSwapChain);
		D10State *ds              = it != chains.end() ? it->second : nullptr;

		if (ds && ds->pDevice != pDevice) {
			ods("D3D10: SwapChain device changed");
			devices.erase(ds->pDevice);
			delete ds;
			ds = nullptr;
		}
		if (!ds) {
			ods("D3D10: New state");
			ds = new D10State(pSwapChain, pDevice);
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
		// DXGI is used for multiple D3D versions. Thus, it is possible a device
		// associated with the DXGISwapChain may very well not be a D3D10 one,
		// in which case we can safely ignore it.
		ods("D3D10: Could not draw because ID3D10Device could not be retrieved.");
#endif
	}
}

void resizeD3D10(IDXGISwapChain *pSwapChain) {
	// Remove the D10State from our "cache" (= Invalidate)
	SwapchainMap::iterator it = chains.find(pSwapChain);
	if (it != chains.end()) {
		D10State *ds = it->second;
		devices.erase(ds->pDevice);
		chains.erase(it);
		delete ds;
	}
}

static ULONG __stdcall myAddRef(ID3D10Device *pDevice) {
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

static ULONG __stdcall myRelease(ID3D10Device *pDevice) {
	// TODO: Move logic to HardHook.
	// Call base without active hook in case of no trampoline.
	ReleaseType oRelease = (ReleaseType) hhRelease.call;
	hhRelease.restore();
	ULONG res = oRelease(pDevice);
	hhRelease.inject();

	Mutex m;
	DeviceMap::iterator it = devices.find(pDevice);
	if (it != devices.end()) {
		D10State *ds = it->second;
		// If we are receiving a lot of subsequent releases lets eagerly drop
		// our state object. If the device presents something again a state
		// object is created again just fine anyway.
		if (res < ds->lHighMark / 2) {
			ods("D3D10: Deleting resources %u < 0.5 * %u", res, ds->lHighMark);
			devices.erase(it);
			chains.erase(ds->pSwapChain);
			delete ds;
			ods("D3D10: Deleted");
			ds = nullptr;
		}
	}

	return res;
}

static void HookAddRelease(voidFunc vfAdd, voidFunc vfRelease) {
	ods("D3D10: Injecting device add/remove");
	hhAddRef.setup(vfAdd, reinterpret_cast< voidFunc >(myAddRef));
	hhRelease.setup(vfRelease, reinterpret_cast< voidFunc >(myRelease));
}

static void hookD3D10(HMODULE hD3D10, bool preonly);

void checkDXGI10Hook(bool preonly) {
	static bool bCheckHookActive = false;
	if (bCheckHookActive) {
		ods("D3D10: Recursion in checkDXGI10Hook");
		return;
	}

	if (d3d10->offsetAddRef == 0 || d3d10->offsetRelease == 0) {
		return;
	}

	bCheckHookActive = true;

	HMODULE hD3D10 = GetModuleHandleW(L"D3D10CORE.DLL");

	if (hD3D10) {
		if (!bHooked) {
			hookD3D10(hD3D10, preonly);
		}
	} else {
#ifdef EXTENDED_OVERLAY_DEBUGOUTPUT
		if (hDXGI) {
			ods("D3D10: No DXGI.DLL found as loaded. No hooking at this point.");
		} else {
			ods("D3D10: No D3D10CORE.DLL found as loaded. No hooking at this point.");
		}
#endif
	}

	bCheckHookActive = false;
}

/// @param hD3D10 must be a valid module handle
void hookD3D10(HMODULE hD3D10, bool preonly) {
	// Add a ref to ourselves; we do NOT want to get unloaded directly from this process.
	HMODULE hTempSelf = nullptr;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast< LPCTSTR >(&hookD3D10), &hTempSelf);

	bHooked = true;

	wchar_t modulename[MODULEFILEPATH_BUFLEN];
	GetModuleFileNameW(hD3D10, modulename, ARRAY_NUM_ELEMENTS(modulename));

	if (_wcsicmp(d3d10->wcFileName, modulename) == 0) {
		unsigned char *raw = (unsigned char *) hD3D10;
		HookAddRelease((voidFunc)(raw + d3d10->offsetAddRef), (voidFunc)(raw + d3d10->offsetRelease));
	} else if (!preonly) {
		ods("D3D10: Interface changed, can't rawpatch. Current: %ls ; Previously: %ls", modulename, d3d10->wcFileName);
	} else {
		bHooked = false;
	}
}

/// Prepares DXGI and D3D10 data by trying to determining the module filepath
/// and function offsets in memory.
/// (This data can later be used for hooking / code injection.
///
/// Adjusts the data behind the global variables dxgi and d3d10.
void PrepareDXGI10(IDXGIAdapter1 *pAdapter, bool initializeDXGIData) {
	if (!dxgi || !d3d10 || !pAdapter)
		return;

	ods("D3D10: Preparing static data for DXGI and D3D10 Injection");

	d3d10->wcFileName[0] = 0;
	d3d10->offsetAddRef  = 0;
	d3d10->offsetRelease = 0;

	HMODULE hD3D10 = LoadLibrary("D3D10.DLL");

	if (hD3D10) {
		HWND hwnd = CreateWindowW(L"STATIC", L"Mumble DXGI Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
								  640, 480, 0, nullptr, nullptr, 0);

		D3D10CreateDeviceAndSwapChainType pD3D10CreateDeviceAndSwapChain =
			reinterpret_cast< D3D10CreateDeviceAndSwapChainType >(
				GetProcAddress(hD3D10, "D3D10CreateDeviceAndSwapChain"));

		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		RECT rcWnd;
		GetClientRect(hwnd, &rcWnd);
		desc.BufferDesc.Width  = rcWnd.right - rcWnd.left;
		desc.BufferDesc.Height = rcWnd.bottom - rcWnd.top;

		ods("D3D10: Got ClientRect W %d H %d", desc.BufferDesc.Width, desc.BufferDesc.Height);

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
		ID3D10Device *pDevice      = nullptr;
		HRESULT hr = pD3D10CreateDeviceAndSwapChain(pAdapter, D3D10_DRIVER_TYPE_HARDWARE, nullptr, 0, D3D10_SDK_VERSION,
													&desc, &pSwapChain, &pDevice);
		if (FAILED(hr))
			ods("D3D10: pD3D10CreateDeviceAndSwapChain failure!");

		if (pDevice && pSwapChain) {
			// For VC++ the vtable is located at the base addr. of the object and each function entry is a single
			// pointer. Since p.e. the base classes of IDXGISwapChain have a total of 8 functions the 8+Xth entry points
			// to the Xth added function in the derived interface.

			void ***vtbl = (void ***) pSwapChain;

			void *pPresent = (*vtbl)[8];
			boost::optional< size_t > offset =
				GetFnOffsetInModule(reinterpret_cast< voidFunc >(pPresent), dxgi->wcFileName,
									ARRAY_NUM_ELEMENTS(dxgi->wcFileName), "D3D10", "Present");
			if (offset) {
				if (initializeDXGIData) {
					dxgi->offsetPresent = *offset;
					ods("D3D10: Successfully found Present offset: %ls: %d", dxgi->wcFileName, dxgi->offsetPresent);
				} else {
					if (dxgi->offsetPresent == *offset) {
						ods("D3D10: Successfully verified Present offset: %ls: %d", dxgi->wcFileName,
							dxgi->offsetPresent);
					} else {
						ods("D3D10: Failed to verify Present offset for %ls. Found %d, but previously found %d.",
							dxgi->wcFileName, offset, dxgi->offsetPresent);
					}
				}
			}

			void *pResize = (*vtbl)[13];
			offset        = GetFnOffsetInModule(reinterpret_cast< voidFunc >(pResize), dxgi->wcFileName,
                                         ARRAY_NUM_ELEMENTS(dxgi->wcFileName), "D3D10", "ResizeBuffers");
			if (offset) {
				if (initializeDXGIData) {
					dxgi->offsetResize = *offset;
					ods("D3D10: Successfully found ResizeBuffers offset: %ls: %d", dxgi->wcFileName,
						dxgi->offsetResize);
				} else {
					if (dxgi->offsetResize == *offset) {
						ods("D3D10: Successfully verified ResizeBuffers offset: %ls: %d", dxgi->wcFileName,
							dxgi->offsetResize);
					} else {
						ods("D3D10: Failed to verify ResizeBuffers offset for %ls. Found %d, but previously found %d.",
							dxgi->wcFileName, offset, dxgi->offsetResize);
					}
				}
			}

			vtbl = (void ***) pDevice;

			void *pAddRef = (*vtbl)[1];
			offset        = GetFnOffsetInModule(reinterpret_cast< voidFunc >(pAddRef), d3d10->wcFileName,
                                         ARRAY_NUM_ELEMENTS(d3d10->wcFileName), "D3D10", "AddRef");
			if (offset) {
				d3d10->offsetAddRef = *offset;
				ods("D3D10: Successfully found AddRef offset: %ls: %d", d3d10->wcFileName, d3d10->offsetAddRef);
			}

			void *pRelease = (*vtbl)[2];
			offset         = GetFnOffsetInModule(reinterpret_cast< voidFunc >(pRelease), d3d10->wcFileName,
                                         ARRAY_NUM_ELEMENTS(d3d10->wcFileName), "D3D10", "Release");
			if (offset) {
				d3d10->offsetRelease = *offset;
				ods("D3D10: Successfully found Release offset: %ls: %d", d3d10->wcFileName, d3d10->offsetRelease);
			}
		}

		if (pDevice)
			pDevice->Release();
		if (pSwapChain)
			pSwapChain->Release();
		DestroyWindow(hwnd);
	} else {
		FreeLibrary(hD3D10);
	}
}
