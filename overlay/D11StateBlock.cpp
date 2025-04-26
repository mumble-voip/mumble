// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* Copyright (C) 2011-2013, Nye Liu <nyet@nyet.org>
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

void D11CreateStateBlock(ID3D11DeviceContext *pDeviceContext, D11StateBlock **ppStateBlock) {
	*ppStateBlock = new D11StateBlock(pDeviceContext);
}

D11StateBlock::D11StateBlock(ID3D11DeviceContext *pDC)
	: pRasterizerState(0), uiSampleMask(0), Format(DXGI_FORMAT_UNKNOWN), Topology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED),
	  pBlendState(0), pIndexBuffer(0), pInputLayout(0), uiOffset(0), pDepthStencilView(0) {
	uiNumViewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	ZeroMemory(pViewports, sizeof(pViewports));
	ZeroMemory(pRenderTargetViews, sizeof(pRenderTargetViews));
	ZeroMemory(fBlendFactor, sizeof(fBlendFactor));
	ZeroMemory(pVertexBuffers, sizeof(pVertexBuffers));
	ZeroMemory(uiStrides, sizeof(uiStrides));
	ZeroMemory(uiOffsets, sizeof(uiOffsets));

	pDeviceContext = pDC;
	pDeviceContext->AddRef();
}

D11StateBlock::~D11StateBlock() {
	ReleaseObjects();
	pDeviceContext->Release();
}

void D11StateBlock::Capture() {
	// Release potential references to make sure we have no leaks.
	ReleaseObjects();

	pDeviceContext->RSGetState(&pRasterizerState);
	uiNumViewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	pDeviceContext->RSGetViewports(&uiNumViewports, pViewports);
	pDeviceContext->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, pRenderTargetViews, &pDepthStencilView);
	pDeviceContext->OMGetBlendState(&pBlendState, fBlendFactor, &uiSampleMask);
	pDeviceContext->IAGetInputLayout(&pInputLayout);
	pDeviceContext->IAGetIndexBuffer(&pIndexBuffer, &Format, &uiOffset);
	pDeviceContext->IAGetPrimitiveTopology(&Topology);
	pDeviceContext->IAGetVertexBuffers(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, pVertexBuffers, uiStrides,
									   uiOffsets);
}

void D11StateBlock::Apply() {
	pDeviceContext->RSSetState(pRasterizerState);
	pDeviceContext->RSSetViewports(uiNumViewports, pViewports);
	pDeviceContext->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, pRenderTargetViews, pDepthStencilView);
	pDeviceContext->OMSetBlendState(pBlendState, fBlendFactor, uiSampleMask);
	pDeviceContext->IASetInputLayout(pInputLayout);
	pDeviceContext->IASetIndexBuffer(pIndexBuffer, Format, uiOffset);
	pDeviceContext->IASetPrimitiveTopology(Topology);
	pDeviceContext->IASetVertexBuffers(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, pVertexBuffers, uiStrides,
									   uiOffsets);
	ReleaseObjects();
}

/// Release state information
void D11StateBlock::ReleaseObjects() {
	if (pRasterizerState) {
		pRasterizerState->Release();
		pRasterizerState = nullptr;
	}

	for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		if (pRenderTargetViews[i]) {
			pRenderTargetViews[i]->Release();
			pRenderTargetViews[i] = nullptr;
		}


	if (pDepthStencilView) {
		pDepthStencilView->Release();
		pDepthStencilView = nullptr;
	}

	if (pBlendState) {
		pBlendState->Release();
		pBlendState = nullptr;
	}

	if (pInputLayout) {
		pInputLayout->Release();
		pInputLayout = nullptr;
	}

	if (pIndexBuffer) {
		pIndexBuffer->Release();
		pIndexBuffer = nullptr;
	}

	for (int i = 0; i < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT; i++)
		if (pVertexBuffers[i]) {
			pVertexBuffers[i]->Release();
			pVertexBuffers[i] = nullptr;
		}
}
