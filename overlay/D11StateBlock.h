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

#ifndef MUMBLE_D11STATEBLOCK_H_
#define MUMBLE_D11STATEBLOCK_H_

// File contains the D11StateBlock class and a function D11CreateStateBlock.

#include <d3d11.h>

class D11StateBlock;

void D11CreateStateBlock(ID3D11DeviceContext *, D11StateBlock **);

class D11StateBlock {
public:
	D11StateBlock(ID3D11DeviceContext *);
	~D11StateBlock();

	void Capture();
	void Apply();
	void ReleaseObjects();
	void ReleaseAllDeviceObjects();

private:
	ID3D11DeviceContext *pDeviceContext;

	ID3D11RasterizerState *pRasterizerState;
	UINT uiNumViewports;
	D3D11_VIEWPORT pViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];

	ID3D11RenderTargetView *pRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	ID3D11DepthStencilView *pDepthStencilView;

	ID3D11BlendState *pBlendState;
	float fBlendFactor[4];
	UINT32 uiSampleMask;

	ID3D11InputLayout *pInputLayout;

	ID3D11Buffer *pIndexBuffer;
	DXGI_FORMAT Format;
	UINT uiOffset;

	D3D11_PRIMITIVE_TOPOLOGY Topology;

	ID3D11Buffer *pVertexBuffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
	UINT uiStrides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
	UINT uiOffsets[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
};

#endif /* !D11STATEBLOCK_H_ */
