/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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
#include <GL/gl.h>
#include <GL/glext.h>

#define INJDEF(name) static __typeof__(&name) o##name = NULL; static HardHook hh##name
#define INJECT(name) ({ o##name = reinterpret_cast<__typeof__(&name)>(GetProcAddress(hGL, #name)); if (o##name) { hh##name.setup(reinterpret_cast<voidFunc>(o##name), reinterpret_cast<voidFunc>(my##name)); hh##name.inject(); } else { ods("OpenAL: No GetProc for %s", #name);} })

#define GLDEF(name) static __typeof__(&name) o##name = NULL;

// #define RESOLVE(name) o##name = reinterpret_cast<__typeof__(&name)>(GetProcAddress(hGL, #name))

extern "C" WINGDIAPI BOOL APIENTRY wglSwapBuffers(HDC);

INJDEF(wglSwapBuffers);
INJDEF(wglSwapLayerBuffers);
INJDEF(SwapBuffers);

GLDEF(wglCreateContext);
GLDEF(glGenTextures);
GLDEF(glEnable);
GLDEF(glDisable);
GLDEF(glBlendFunc);
GLDEF(glColorMaterial);
GLDEF(glViewport);
GLDEF(glMatrixMode);
GLDEF(glLoadIdentity);
GLDEF(glOrtho);
GLDEF(glBindTexture);
GLDEF(glPushMatrix);
GLDEF(glColor4ub);
GLDEF(glTranslatef);
GLDEF(glBegin);
GLDEF(glEnd);
GLDEF(glTexCoord2f);
GLDEF(glVertex2f);
GLDEF(glPopMatrix);
GLDEF(glTexParameteri);
GLDEF(glTexImage2D);
GLDEF(wglMakeCurrent);
GLDEF(wglGetCurrentContext);
GLDEF(wglGetCurrentDC);
GLDEF(GetDeviceCaps);

static bool bHooked = false;
static bool bChaining = false;

struct Context {
	HGLRC ctx;
	GLuint textures[NUM_TEXTS];
	Context(HDC hdc);
	void draw(HDC hdc);
};

Context::Context(HDC hdc) {
	ctx = owglCreateContext(hdc);
	owglMakeCurrent(hdc, ctx);

	oglGenTextures(NUM_TEXTS, textures);

	// Here we go. From the top. Where is glResetState?
	oglDisable(GL_ALPHA_TEST);
	oglDisable(GL_AUTO_NORMAL);
	oglEnable(GL_BLEND);
	// Skip clip planes, there are thousands of them.
	oglDisable(GL_COLOR_LOGIC_OP);
	oglEnable(GL_COLOR_MATERIAL);
	oglDisable(GL_COLOR_TABLE);
	oglDisable(GL_CONVOLUTION_1D);
	oglDisable(GL_CONVOLUTION_2D);
	oglDisable(GL_CULL_FACE);
	oglDisable(GL_DEPTH_TEST);
	oglDisable(GL_DITHER);
	oglDisable(GL_FOG);
	oglDisable(GL_HISTOGRAM);
	oglDisable(GL_INDEX_LOGIC_OP);
	oglDisable(GL_LIGHTING);
	// Skip line smmooth
	// Skip map
	oglDisable(GL_MINMAX);
	// Skip polygon offset
	oglDisable(GL_SEPARABLE_2D);
	oglDisable(GL_SCISSOR_TEST);
	oglDisable(GL_STENCIL_TEST);
	oglEnable(GL_TEXTURE_2D);
	oglDisable(GL_TEXTURE_GEN_Q);
	oglDisable(GL_TEXTURE_GEN_R);
	oglDisable(GL_TEXTURE_GEN_S);
	oglDisable(GL_TEXTURE_GEN_T);

	oglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	oglColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	DWORD dwWaitResult = WaitForSingleObject(hSharedMutex, 50L);
	if (dwWaitResult != WAIT_OBJECT_0)
		return;

	for (int i = 0; i < NUM_TEXTS; i++) {
		sm->texts[i].bUpdated = true;
	}

	ReleaseMutex(hSharedMutex);
}

void Context::draw(HDC hdc) {
	bVideoHooked = true;
	sm->bHooked = true;

	// DEBUG
	// sm->bDebug = true;

	unsigned int width, height;
	int i;

	width = oGetDeviceCaps(hdc, HORZRES);
	height = oGetDeviceCaps(hdc, VERTRES);

	ods("DrawStart: Screen is %d x %d", width, height);

	if (sm->fFontSize < 0.01)
		sm->fFontSize = 0.01;
	else if (sm->fFontSize > 1.0)
		sm->fFontSize = 1.0;

	int iHeight = (int)((height * 1.0) * sm->fFontSize);
	if (iHeight > TEXT_HEIGHT)
		iHeight = TEXT_HEIGHT;

	float s = iHeight / 60.0;
	int y = 0;
	int idx = 0;

	int texs[NUM_TEXTS];
	int widths[NUM_TEXTS];
	int yofs[NUM_TEXTS];
	unsigned int color[NUM_TEXTS];


	DWORD dwWaitResult = WaitForSingleObject(hSharedMutex, 50L);
	if (dwWaitResult != WAIT_OBJECT_0)
		return;

	oglViewport(0, 0, width, height);

	oglMatrixMode(GL_PROJECTION);
	oglLoadIdentity();
	oglOrtho(0, width, height, 0, -100.0, 100.0);

	oglMatrixMode(GL_MODELVIEW);

	for (i = 0; i < NUM_TEXTS; i++) {
		if (sm->texts[i].width == 0) {
			y += iHeight / 4;
		} else if (sm->texts[i].width > 0) {
			if (sm->texts[i].bUpdated) {
				ods("OpenGL: Updating %d %d texture", sm->texts[i].width, TEXT_HEIGHT);
				oglBindTexture(GL_TEXTURE_2D, textures[i]);
				oglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				oglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				oglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				oglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				oglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXT_WIDTH, TEXT_HEIGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, sm->texts[i].texture);
				sm->texts[i].bUpdated = false;
			}
			texs[idx] = textures[i];
			widths[idx] = sm->texts[i].width;
			color[idx] = sm->texts[i].color;
			yofs[idx] = y;
			y += iHeight;
			idx++;
		}
	}
	ReleaseMutex(hSharedMutex);

	int h = y;
	y = (int)(height * sm->fY);

	if (sm->bTop) {
		y -= h;
	} else if (sm->bBottom) {
	} else {
		y -= h / 2;
	}

	if (y < 1)
		y = 1;
	if ((y + h + 1) > height)
		y = height - h - 1;


	for (i = 0; i < idx; i++) {
		int w = (int)(widths[i] * s);
		int x = (int)(width * sm->fX);
		if (sm->bLeft) {
			x -= w;
		} else if (sm->bRight) {
		} else {
			x -= w / 2;
		}

		if (x < 1)
			x = 1;
		if ((x + w + 1) > width)
			x = width - w - 1;

		ods("OpenGL: Drawing text at %d %d  %d %d", x, y + yofs[i], w, iHeight);
		oglBindTexture(GL_TEXTURE_2D, texs[i]);
		oglPushMatrix();
		oglLoadIdentity();

		double xm = 0.0;
		double ym = 0.0;
		double xmx = (1.0 * widths[i]) / TEXT_WIDTH;
		double ymx = 1.0;

		unsigned int c = color[i];

		oglColor4ub((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF, (c >> 24) & 0xFF);


		oglTranslatef(x, y + yofs[i], 0.0);
		oglBegin(GL_QUADS);
		oglTexCoord2f(xm, ymx);
		oglVertex2f(0, iHeight);
		oglTexCoord2f(xm, ym);
		oglVertex2f(0, 0);
		oglTexCoord2f(xmx, ym);
		oglVertex2f(w, 0);
		oglTexCoord2f(xmx, ymx);
		oglVertex2f(w, iHeight);
		oglEnd();
		oglPopMatrix();
	}
}


static map<HDC, Context *> contexts;

static void doSwap(HDC hdc) {
	HGLRC oldctx = owglGetCurrentContext();
	HDC oldhdc = owglGetCurrentDC();
	Context *c = contexts[hdc];

	if (!c) {
		ods("OpenGL: New context for device %p", hdc);
		c = new Context(hdc);
		contexts[hdc] = c;
	} else {
		ods("OpenGL: Reusing old context");
		owglMakeCurrent(hdc, c->ctx);
	}
	c->draw(hdc);
	owglMakeCurrent(oldhdc, oldctx);
}

static WINGDIAPI BOOL APIENTRY mywglSwapBuffers(HDC hdc) {
	ods("OpenGL: wglSwapBuffers");
	doSwap(hdc);
	hhwglSwapBuffers.restore();
	BOOL ret=owglSwapBuffers(hdc);
	hhwglSwapBuffers.inject();
	return ret;
}

static WINGDIAPI BOOL WINAPI mySwapBuffers(HDC hdc) {
	ods("OpenGL: SwapBuffers");
	hhSwapBuffers.restore();
	BOOL ret=oSwapBuffers(hdc);
	hhSwapBuffers.inject();
	return ret;
}

static WINGDIAPI BOOL WINAPI mywglSwapLayerBuffers(HDC hdc, UINT fuPlanes) {
	ods("OpenGL: SwapLayerBuffers %x",fuPlanes);
	hhwglSwapLayerBuffers.restore();
	BOOL ret=owglSwapLayerBuffers(hdc, fuPlanes);
	hhwglSwapLayerBuffers.inject();
	return ret;
}


#undef GLDEF
#define GLDEF(name) o##name = reinterpret_cast<__typeof__(&name)>(GetProcAddress(hGL, #name))

void checkOpenGLHook() {
	if (bChaining) {
		return;
		ods("Causing a chain");
	}

	bChaining = true;

	HMODULE hGL = GetModuleHandle(L"OpenGL32.DLL");

	if (hGL != NULL) {
		if (! bHooked) {
			wchar_t procname[1024];
			GetModuleFileName(NULL, procname, 1024);
			ods("OpenGL: CreateWnd in unhooked OpenGL App %ls", procname);
			bHooked = true;

			if (wcsstr(procname, L"mumble.exe") != NULL) {
				ods("OpenGL: Ignoring mumble.exe");
			} else {
				INJECT(wglSwapBuffers);
				// INJECT(wglSwapLayerBuffers);

				GLDEF(wglCreateContext);
				GLDEF(glGenTextures);
				GLDEF(glEnable);
				GLDEF(glDisable);
				GLDEF(glBlendFunc);
				GLDEF(glColorMaterial);
				GLDEF(glViewport);
				GLDEF(glMatrixMode);
				GLDEF(glLoadIdentity);
				GLDEF(glOrtho);
				GLDEF(glBindTexture);
				GLDEF(glPushMatrix);
				GLDEF(glColor4ub);
				GLDEF(glTranslatef);
				GLDEF(glBegin);
				GLDEF(glEnd);
				GLDEF(glTexCoord2f);
				GLDEF(glVertex2f);
				GLDEF(glPopMatrix);
				GLDEF(glTexParameteri);
				GLDEF(glTexImage2D);
				GLDEF(wglMakeCurrent);
				GLDEF(wglGetCurrentContext);
				GLDEF(wglGetCurrentDC);

				hGL = GetModuleHandle(L"GDI32.DLL");
				if (hGL) {
					// INJECT(SwapBuffers);
					GLDEF(GetDeviceCaps);
				} else {
					ods("OpenGL: Failed to find GDI32");
				}
			}
		}
	}

	bChaining = false;
}
