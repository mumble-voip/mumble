/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#include "openglcontext.h"
#include "openglfunctions.h"

static OpenGLFunctions gl;

GLDEF(BOOL, wglSwapBuffers, (HDC));
static HardHook hhwglSwapBuffers;

//GLDEF(BOOL, wglSwapLayerBuffers, (HDC, UINT));
//static HardHook hhwglSwapLayerBuffers;

//GLDEF(BOOL, SwapBuffers, (HDC));
//static HardHook hhSwapBuffers;

static bool bHooked = false;

static map<HDC, OpenGLOverlayContext *> contexts;

static void doSwap(HDC hdc) {
	OpenGLOverlayContext *c = contexts[hdc];
	if (!c) {
		ods("OpenGL: New context for device %p", hdc);
		c = new OpenGLOverlayContext(hdc, &gl);
		contexts[hdc] = c;
	} else {
		ods("OpenGL: Reusing old context");
	}

	c->draw();
}

static BOOL __stdcall mywglSwapBuffers(HDC hdc) {
	ods("OpenGL: wglSwapBuffers");
	doSwap(hdc);

	hhwglSwapBuffers.restore();
	BOOL ret = owglSwapBuffers(hdc);
	hhwglSwapBuffers.inject();

	return ret;
}

//static BOOL __stdcall mySwapBuffers(HDC hdc) {
//	ods("OpenGL: SwapBuffers");
//
//	hhSwapBuffers.restore();
//	BOOL ret=oSwapBuffers(hdc);
//	hhSwapBuffers.inject();
//
//	return ret;
//}

//static BOOL __stdcall mywglSwapLayerBuffers(HDC hdc, UINT fuPlanes) {
//	ods("OpenGL: SwapLayerBuffers %x",fuPlanes);
//
//	hhwglSwapLayerBuffers.restore();
//	BOOL ret=owglSwapLayerBuffers(hdc, fuPlanes);
//	hhwglSwapLayerBuffers.inject();
//
//	return ret;
//}

/// For assigning the function address to the function pointer variable
#define GLDEFASSIGN(name) gl.o##name = reinterpret_cast<OpenGLFunctions::t##name>(GetProcAddress(hGL, #name))

void checkOpenGLHook() {
	static bool bCheckHookActive = false;
	if (bCheckHookActive) {
		ods("OpenGL: Recursion in checkOpenGLHook");
		return;
	}

	bCheckHookActive = true;

	HMODULE hGL = GetModuleHandle("OpenGL32.DLL");

	if (hGL != NULL) {
		if (! bHooked) {
			char procname[1024];
			GetModuleFileName(NULL, procname, 1024);
			ods("OpenGL: Unhooked OpenGL App %s", procname);
			bHooked = true;

			// Add a ref to ourselves; we do NOT want to get unloaded directly from this process.
			HMODULE hTempSelf = NULL;
			GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<char *>(&checkOpenGLHook), &hTempSelf);

			INJECT(wglSwapBuffers);
			// INJECT(wglSwapLayerBuffers);

			GLDEFASSIGN(wglMakeCurrent);
			GLDEFASSIGN(wglGetCurrentContext);
			GLDEFASSIGN(wglGetCurrentDC);

			GLDEFASSIGN(glDisable);
			GLDEFASSIGN(glEnable);
			GLDEFASSIGN(glBlendFunc);
			GLDEFASSIGN(glBindTexture);
			GLDEFASSIGN(glTexImage2D);
			GLDEFASSIGN(glTexSubImage2D);

			GLDEFASSIGN(wglCreateContext);
			GLDEFASSIGN(glGenTextures);
			GLDEFASSIGN(glDeleteTextures);
			GLDEFASSIGN(glColorMaterial);
			GLDEFASSIGN(glViewport);
			GLDEFASSIGN(glMatrixMode);
			GLDEFASSIGN(glLoadIdentity);
			GLDEFASSIGN(glOrtho);
			GLDEFASSIGN(glPushMatrix);
			GLDEFASSIGN(glColor4ub);
			GLDEFASSIGN(glTranslatef);
			GLDEFASSIGN(glBegin);
			GLDEFASSIGN(glEnd);
			GLDEFASSIGN(glTexCoord2f);
			GLDEFASSIGN(glVertex2f);
			GLDEFASSIGN(glPopMatrix);
			GLDEFASSIGN(glTexParameteri);
			GLDEFASSIGN(glTexEnvi);
			GLDEFASSIGN(glPixelStorei);

			hGL = GetModuleHandle("GDI32.DLL");
			if (hGL) {
				// INJECT(SwapBuffers);
				GLDEFASSIGN(GetDeviceCaps);
			} else {
				ods("OpenGL: Failed to find GDI32");
			}
		} else {
			hhwglSwapBuffers.check();
		}
	}

	bCheckHookActive = false;
}
