/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

typedef unsigned int    GLenum;
typedef unsigned char   GLboolean;
typedef void            GLvoid;
typedef int             GLint;
typedef unsigned char   GLubyte;
typedef unsigned int    GLuint;
typedef int             GLsizei;
typedef float           GLfloat;
typedef double          GLdouble;

#define GL_TEXTURE_2D                           0x0DE1
#define GL_UNSIGNED_BYTE                        0x1401
#define GL_QUADS                                0x0007
#define GL_RGBA                                 0x1908
#define GL_BGRA                                 0x80E1
#define GL_TEXTURE_WRAP_S                       0x2802
#define GL_TEXTURE_WRAP_T                       0x2803
#define GL_CLAMP_TO_EDGE                        0x812F
#define GL_TEXTURE_MAG_FILTER                   0x2800
#define GL_TEXTURE_MIN_FILTER                   0x2801
#define GL_LINEAR                               0x2601
#define GL_FRONT_AND_BACK                       0x0408
#define GL_AMBIENT_AND_DIFFUSE                  0x1602
#define GL_SRC_ALPHA                            0x0302
#define GL_ONE_MINUS_SRC_ALPHA                  0x0303
#define GL_TEXTURE_GEN_S                        0x0C60
#define GL_TEXTURE_GEN_T                        0x0C61
#define GL_TEXTURE_GEN_R                        0x0C62
#define GL_TEXTURE_GEN_Q                        0x0C63
#define GL_STENCIL_TEST                         0x0B90
#define GL_SCISSOR_TEST                         0x0C11
#define GL_SEPARABLE_2D                         0x8012
#define GL_MINMAX                               0x802E
#define GL_LIGHTING                             0x0B50
#define GL_INDEX_LOGIC_OP                       0x0BF1
#define GL_HISTOGRAM                            0x8024
#define GL_FOG                                  0x0B60
#define GL_DITHER                               0x0BD0
#define GL_DEPTH_TEST                           0x0B71
#define GL_CULL_FACE                            0x0B44
#define GL_CONVOLUTION_1D                       0x8010
#define GL_CONVOLUTION_2D                       0x8011
#define GL_COLOR_TABLE                          0x80D0
#define GL_COLOR_MATERIAL                       0x0B57
#define GL_AUTO_NORMAL                          0x0D80
#define GL_ALPHA_TEST                           0x0BC0
#define GL_COLOR_LOGIC_OP                       0x0BF2
#define GL_MATRIX_MODE                          0x0BA0
#define GL_MODELVIEW                            0x1700
#define GL_PROJECTION                           0x1701
#define GL_BLEND                                0x0BE2

#define TDEF(ret, name, arg) typedef ret (__stdcall * t##name) arg
#define GLDEF(ret, name, arg) TDEF(ret, name, arg); t##name o##name = NULL

GLDEF(HGLRC, wglCreateContext, (HDC));
GLDEF(void, glGenTextures, (GLsizei, GLuint *));
GLDEF(void, glEnable, (GLenum));
GLDEF(void, glDisable, (GLenum));
GLDEF(void, glBlendFunc, (GLenum, GLenum));
GLDEF(void, glColorMaterial, (GLenum, GLenum));
GLDEF(void, glViewport, (GLint, GLint, GLsizei, GLsizei));
GLDEF(void, glMatrixMode, (GLenum));
GLDEF(void, glLoadIdentity, (void));
GLDEF(void, glOrtho, (GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble));
GLDEF(void, glBindTexture, (GLenum, GLuint));
GLDEF(void, glPushMatrix, (void));
GLDEF(void, glColor4ub, (GLubyte, GLubyte, GLubyte, GLubyte));
GLDEF(void, glTranslatef, (GLfloat, GLfloat, GLfloat));
GLDEF(void, glBegin, (GLenum));
GLDEF(void, glEnd, (void));
GLDEF(void, glTexCoord2f, (GLfloat, GLfloat));
GLDEF(void, glVertex2f, (GLfloat, GLfloat));
GLDEF(void, glPopMatrix, (void));
GLDEF(void, glTexParameteri, (GLenum, GLenum, GLint));
GLDEF(void, glTexImage2D, (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *));
GLDEF(void, wglMakeCurrent, (HDC, HGLRC));
GLDEF(HGLRC, wglGetCurrentContext, (void));
GLDEF(HDC, wglGetCurrentDC, (void));
GLDEF(int, GetDeviceCaps, (HDC, int));

#define INJDEF(ret, name, arg) GLDEF(ret, name, arg); static HardHook hh##name
#define INJECT(name) { o##name = reinterpret_cast<t##name>(GetProcAddress(hGL, #name)); if (o##name) { hh##name.setup(reinterpret_cast<voidFunc>(o##name), reinterpret_cast<voidFunc>(my##name)); o##name = (t##name) hh##name.call; } else { ods("OpenGL: No GetProc for %s", #name);} }

INJDEF(BOOL, wglSwapLayerBuffers, (HDC, UINT));
INJDEF(BOOL, wglSwapBuffers, (HDC));
INJDEF(BOOL, SwapBuffers, (HDC));

static bool bHooked = false;
static bool bChaining = false;

struct Context {
	HGLRC ctx;
	GLuint textures[NUM_TEXTS];
	unsigned int uiCounter[NUM_TEXTS];
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

	for (int i=0;i<NUM_TEXTS;++i)
		uiCounter[i] = 0;

	ReleaseMutex(hSharedMutex);
}

void Context::draw(HDC hdc) {
	sm->bHooked = true;

	// DEBUG
	// sm->bDebug = true;

	unsigned int width, height;
	int i;

	width = oGetDeviceCaps(hdc, HORZRES);
	height = oGetDeviceCaps(hdc, VERTRES);

	ods("DrawStart: Screen is %d x %d", width, height);

	if (sm->fFontSize < 0.01f)
		sm->fFontSize = 0.01f;
	else if (sm->fFontSize > 1.0f)
		sm->fFontSize = 1.0f;

	int iHeight = (int)((height * 1.0) * sm->fFontSize);
	if (iHeight > TEXT_HEIGHT)
		iHeight = TEXT_HEIGHT;

	float s = iHeight / 60.0f;
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
			if (sm->texts[i].uiCounter != uiCounter[i]) {
				ods("OpenGL: Updating %d %d texture", sm->texts[i].width, TEXT_HEIGHT);
				oglBindTexture(GL_TEXTURE_2D, textures[i]);
				oglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				oglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				oglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				oglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				oglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXT_WIDTH, TEXT_HEIGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, sm->texts[i].texture);
				uiCounter[i] = sm->texts[i].uiCounter;
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
	if ((y + h + 1) > (int)height)
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
		if ((x + w + 1) > (int)width)
			x = width - w - 1;

		ods("OpenGL: Drawing text at %d %d  %d %d", x, y + yofs[i], w, iHeight);
		oglBindTexture(GL_TEXTURE_2D, texs[i]);
		oglPushMatrix();
		oglLoadIdentity();

		float xm = 0.0;
		float ym = 0.0;
		float xmx = (1.0f * widths[i]) / TEXT_WIDTH;
		float ymx = 1.0f;

		unsigned int c = color[i];

		oglColor4ub((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF, (c >> 24) & 0xFF);


		oglTranslatef(static_cast<float>(x), static_cast<float>(y + yofs[i]), 0.0f);
		oglBegin(GL_QUADS);
		oglTexCoord2f(xm, ymx);
		oglVertex2f(0.0f, static_cast<float>(iHeight));
		oglTexCoord2f(xm, ym);
		oglVertex2f(0.0f, 0.0f);
		oglTexCoord2f(xmx, ym);
		oglVertex2f(static_cast<float>(w), 0.0f);
		oglTexCoord2f(xmx, ymx);
		oglVertex2f(static_cast<float>(w), static_cast<float>(iHeight));
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

static BOOL __stdcall mywglSwapBuffers(HDC hdc) {
	ods("OpenGL: wglSwapBuffers");
	doSwap(hdc);
	hhwglSwapBuffers.restore();
	BOOL ret=owglSwapBuffers(hdc);
	hhwglSwapBuffers.inject();
	return ret;
}

static BOOL __stdcall mySwapBuffers(HDC hdc) {
	ods("OpenGL: SwapBuffers");
	hhSwapBuffers.restore();
	BOOL ret=oSwapBuffers(hdc);
	hhSwapBuffers.inject();
	return ret;
}

static BOOL __stdcall mywglSwapLayerBuffers(HDC hdc, UINT fuPlanes) {
	ods("OpenGL: SwapLayerBuffers %x",fuPlanes);
	hhwglSwapLayerBuffers.restore();
	BOOL ret=owglSwapLayerBuffers(hdc, fuPlanes);
	hhwglSwapLayerBuffers.inject();
	return ret;
}


#undef GLDEF
#define GLDEF(name) o##name = reinterpret_cast<t##name>(GetProcAddress(hGL, #name))

void checkOpenGLHook() {
	if (bChaining) {
		return;
		ods("Causing a chain");
	}

	bChaining = true;

	HMODULE hGL = GetModuleHandle("OpenGL32.DLL");

	if (hGL != NULL) {
		if (! bHooked) {
			char procname[1024];
			GetModuleFileName(NULL, procname, 1024);
			fods("OpenGL: Unhooked OpenGL App %s", procname);
			bHooked = true;

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

			hGL = GetModuleHandle("GDI32.DLL");
			if (hGL) {
				// INJECT(SwapBuffers);
				GLDEF(GetDeviceCaps);
			} else {
				ods("OpenGL: Failed to find GDI32");
			}
		}
	}

	bChaining = false;
}
