#include "lib.h"
#include "D11StateBlock.h"

HRESULT __stdcall D11StateBlock::QueryInterface(REFIID, LPVOID *)
{
	return 0;
}

ULONG __stdcall D11StateBlock::AddRef()
{
	this->refcnt++;
	return 0;
}

ULONG __stdcall D11StateBlock::Release()
{
	this->refcnt--;
	return 0;
}

void D11StateBlock::Capture()
{
	this->pDeviceContext->RSGetState(&this->pRasterizerState);
	this->NumViewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	this->pDeviceContext->RSGetViewports(&this->NumViewports, this->pViewports);
	this->pDeviceContext->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, this->pRenderTargetViews, &this->pDepthStencilView);
	this->pDeviceContext->OMGetBlendState(&this->pBlendState, this->BlendFactor, &this->SampleMask);
	this->pDeviceContext->IAGetInputLayout(&this->pInputLayout);
	this->pDeviceContext->IAGetIndexBuffer(&this->pIndexBuffer, &this->Format, &this->Offset);
	this->pDeviceContext->IAGetPrimitiveTopology(&this->Topology);
	this->pDeviceContext->IAGetVertexBuffers(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, this->pVertexBuffers, Strides, Offsets);
}

void D11StateBlock::Apply()
{
	this->pDeviceContext->RSSetState(this->pRasterizerState);
	this->pDeviceContext->RSSetViewports(this->NumViewports, this->pViewports);
	this->pDeviceContext->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, this->pRenderTargetViews, this->pDepthStencilView);
	this->pDeviceContext->OMSetBlendState(this->pBlendState, this->BlendFactor, this->SampleMask);
	this->pDeviceContext->IASetInputLayout(this->pInputLayout);
	this->pDeviceContext->IASetIndexBuffer(this->pIndexBuffer, this->Format, this->Offset);
	this->pDeviceContext->IASetPrimitiveTopology(this->Topology);
	this->pDeviceContext->IASetVertexBuffers(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, this->pVertexBuffers, this->Strides, this->Offsets);
	this->ReleaseObjects();
}

void D11StateBlock::ReleaseObjects()
{
	if (this->pRasterizerState) {
		this->pRasterizerState->Release();
		this->pRasterizerState = NULL;
	}

	for (int i=0; i<this->NumViewports; i++)
		if (this->pRenderTargetViews[i]) {
			this->pRenderTargetViews[i]->Release();
			this->pRenderTargetViews[i] = NULL;
		}


	if (this->pDepthStencilView) {
		this->pDepthStencilView->Release();
		this->pDepthStencilView = NULL;
	}

	for (int i=0; i<D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT; i++)
		if (this->pVertexBuffers[i]) {
			this->pVertexBuffers[i]->Release();
			this->pVertexBuffers[i]=NULL;
		}
}

void D11StateBlock::ReleaseAllDeviceObjects()
{
	this->ReleaseObjects();
}

void D11StateBlock::GetDeviceContext(ID3D11DeviceContext **ppDC)
{
	*ppDC = this->pDeviceContext;
}

D11StateBlock::D11StateBlock(ID3D11DeviceContext *pDC) {
	pDeviceContext = pDC;
	NumViewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	ZeroMemory(pViewports, sizeof(pViewports));
	ZeroMemory(pRenderTargetViews, sizeof (pRenderTargetViews));
	ZeroMemory(BlendFactor, sizeof (BlendFactor));
	ZeroMemory(pVertexBuffers, sizeof (pVertexBuffers));
	ZeroMemory(Strides, sizeof (Strides));
	ZeroMemory(Offsets, sizeof (Offsets));
}

D11StateBlock::~D11StateBlock() {
	this->ReleaseObjects();
}

void D11CreateStateBlock(ID3D11DeviceContext *pDeviceContext, D11StateBlock **ppStateBlock)
{
	*ppStateBlock = new D11StateBlock(pDeviceContext);
}
