#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11.h>

class D11StateBlock: protected IUnknown {
	private:
		ID3D11DeviceContext *pDeviceContext;
		ULONG refcnt;

		ID3D11RasterizerState *pRasterizerState;
		UINT NumViewports;
		D3D11_VIEWPORT pViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];

		ID3D11RenderTargetView *pRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		ID3D11DepthStencilView *pDepthStencilView;

		ID3D11BlendState *pBlendState;
		float BlendFactor[4];
		UINT32 SampleMask;

		ID3D11InputLayout *pInputLayout;

		ID3D11Buffer *pIndexBuffer;
		DXGI_FORMAT Format;
		UINT Offset;

		D3D11_PRIMITIVE_TOPOLOGY Topology;

		ID3D11Buffer *pVertexBuffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
		UINT Strides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
		UINT Offsets[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];

	public:
		STDMETHOD(QueryInterface)(REFIID, LPVOID *);
		STDMETHOD_(ULONG, AddRef)();
		STDMETHOD_(ULONG, Release)();

		void Capture();
		void Apply();
		void ReleaseObjects();
		void ReleaseAllDeviceObjects();
		void GetDeviceContext(ID3D11DeviceContext **);

		D11StateBlock(ID3D11DeviceContext *);
		~D11StateBlock();
};

void D11CreateStateBlock(ID3D11DeviceContext *, D11StateBlock **);
