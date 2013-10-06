/* Copyright (C) 2011 Nye Liu <nyet@nyet.org> */

#include "d3dx11effect.h"

static HRESULT(__stdcall *pD3DReflect)(LPCVOID, SIZE_T, REFIID, void **) = NULL;
static HRESULT(__stdcall *pD3DGetInputSignatureBlob)(LPCVOID, SIZE_T, ID3DBlob **) = NULL;

extern "C" __declspec(dllexport) HRESULT __cdecl
CreateEffectFromMemory(CONST void *p, SIZE_T size, UINT flags, ID3D11Device *pD, ID3DX11Effect **ppE)
{
	return D3DX11CreateEffectFromMemory(p, size, flags, pD, ppE);
}

static HRESULT LoadD3DCompiler()
{
	LPCSTR libraries[] = {"d3dcompiler_43.dll", "d3dcompiler_42.dll", "d3dcompiler_41.dll", "d3dcompiler_40.dll", NULL};

	int i = 0;
	while (LPCSTR l = libraries[i++]) {
		HMODULE d3dcompiler = LoadLibrary(l);
		if (d3dcompiler) {
			pD3DReflect = (HRESULT(__stdcall *)(LPCVOID, SIZE_T, REFIID, void**)) GetProcAddress(d3dcompiler, "D3DReflect");
			pD3DGetInputSignatureBlob = (HRESULT(__stdcall *)(LPCVOID, SIZE_T, ID3DBlob **))GetProcAddress(d3dcompiler, "D3DGetInputSignatureBlob");
			if (pD3DReflect && pD3DGetInputSignatureBlob)
				break;
		}
	}
	if (!pD3DReflect || !pD3DGetInputSignatureBlob)
		return -1;
	return 0;
}

extern "C" HRESULT __stdcall D3DReflect(LPCVOID p, SIZE_T s, REFIID r, void **pp)
{
	if (!pD3DReflect) {
		HRESULT hr = LoadD3DCompiler();
		if (hr) return -1;
	}
	return pD3DReflect(p, s, r, pp);
}

extern "C" HRESULT __stdcall D3DGetInputSignatureBlob(LPCVOID p, SIZE_T s, ID3DBlob **pp)
{
	if (!pD3DGetInputSignatureBlob) {
		HRESULT hr = LoadD3DCompiler();
		if (hr) return -1;
	}
	return pD3DGetInputSignatureBlob(p, s, pp);
}
