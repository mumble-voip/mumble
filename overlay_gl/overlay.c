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

#define GLX_GLXEXT_LEGACY
#define GL_GLEXT_PROTOTYPES
#define _GNU_SOURCE
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <dlfcn.h>
#include <link.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdarg.h>
#include <math.h>

typedef unsigned char bool;
#define true 1
#define false 0

#include "../overlay/overlay.h"

// Prototypes
static void resolveSM();
static void ods(const char *format, ...);

static struct SharedMem *sm = NULL;
static sem_t *sem = NULL;

static bool bDebug;

typedef struct _Context {
	struct _Context *next;
	Display *dpy;
	GLXDrawable draw;

	bool bValid;
	bool bGlx;

	GLuint textures[NUM_TEXTS];
	unsigned int uiCounter[NUM_TEXTS];
	GLuint uiProgram;
} Context;

static const char vshader[] = ""
                              "void main() {"
                              "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
                              "gl_TexCoord[0] = gl_MultiTexCoord0;"
                              "gl_FrontColor = gl_Color;"
                              "}";

static const char fshader[] = ""
                              "uniform sampler2D tex;"
                              "void main() {"
                              "gl_FragColor = gl_Color * texture2D(tex, gl_TexCoord[0].st);"
                              "}";

const GLfloat fBorder[] = {0.125f, 0.250f, 0.5f, 0.75f};

static Context *contexts = NULL;

#define FDEF(name) static __typeof__(&name) o##name = NULL

FDEF(dlsym);

FDEF(glXSwapBuffers);
FDEF(glXGetProcAddressARB);
FDEF(glXGetProcAddress);

#define RESOLVE(x) if (! o##x) o##x = (__typeof__(&x)) odlsym(RTLD_NEXT, #x)

static void resolveOpenGL() {
	RESOLVE(glXSwapBuffers);
	RESOLVE(glXGetProcAddressARB);
	RESOLVE(glXGetProcAddress);
}

static void resolveSM() {
	static bool warned_sm = false;
	static bool warned_ver = false;

	char memname[256];
	char semname[256];
	snprintf(memname, 256, "/MumbleOverlayMem.%d", getuid());
	snprintf(semname, 256, "/MumbleOverlaySem.%d", getuid());

	int fd = shm_open(memname, O_RDWR, 0600);
	if (fd >= 0) {
		sm = (struct SharedMem *)(mmap(NULL, sizeof(struct SharedMem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
		if (sm == (void *)(-1)) {
			sm = NULL;
			close(fd);
		} else {
			if ((sm->version[0] != OVERLAY_VERSION_MAJ) ||
			        (sm->version[1] != OVERLAY_VERSION_MIN) ||
			        (sm->version[2] != OVERLAY_VERSION_PATCH)) {
				if (! warned_ver) {
					fflush(stderr);
					fprintf(stderr, "MUMBLE OVERLAY:: Version mismatch. Library is %u.%u.%u.%u, application is %u.%u.%u.%u\n",
					        OVERLAY_VERSION_MAJ, OVERLAY_VERSION_MIN, OVERLAY_VERSION_PATCH, OVERLAY_VERSION_SUB,
					        sm->version[0], sm->version[1], sm->version[2], sm->version[3]
					       );
					fflush(stderr);
					warned_ver = true;
				}
				munmap(sm, sizeof(struct SharedMem));
				sm = NULL;
				close(fd);
			} else {
				sem = sem_open(semname, 0);
				if (sem == SEM_FAILED) {
					munmap(sm, sizeof(struct SharedMem));
					sm = NULL;
					close(fd);
				}
			}
		}
	}

	if (sm == NULL) {
		if (! warned_sm && ! warned_ver) {
			fflush(stderr);
			fprintf(stderr, "MUMBLE OVERLAY:: NO CONTACT WITH MUMBLE\n");
			fflush(stderr);
			warned_sm = true;
		}
	}
}

__attribute__((format(printf, 1, 2)))
static void ods(const char *format, ...) {
	if (!bDebug) {
		if (! sm || !sm->bDebug)
			return;
	}

	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fprintf(stderr, "\n");
	fflush(stderr);
}

static void newContext(Context * ctx) {
	int i;

	if (sm) {
		sm->bHooked = true;
	}
	for (i = 0; i < NUM_TEXTS; i++) {
		ctx->uiCounter[i] = 0;
		ctx->textures[i] = -1;
	}

	ods("OpenGL Version %s, Vendor %s, Renderer %s, Shader %s", glGetString(GL_VERSION), glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_SHADING_LANGUAGE_VERSION));

	const char *vsource = vshader;
	const char *fsource = fshader;
	char buffer[8192];
	GLint l;
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vs, 1, &vsource, NULL);
	glShaderSource(fs, 1, &fsource, NULL);
	glCompileShader(vs);
	glCompileShader(fs);
	glGetShaderInfoLog(vs, 8192, &l, buffer);
	ods("VERTEX: %s", buffer);
	glGetShaderInfoLog(fs, 8192, &l, buffer);
	ods("FRAGMENT: %s", buffer);
	ctx->uiProgram = glCreateProgram();
	glAttachShader(ctx->uiProgram, vs);
	glAttachShader(ctx->uiProgram, fs);
	glLinkProgram(ctx->uiProgram);
}

static void drawOverlay(Context *ctx, int width, int height) {
	// DEBUG
	// sm->bDebug = true;

	int i;
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

	int indexes[NUM_TEXTS];
	int yofs[NUM_TEXTS];

	if (sem_trywait(sem) != 0) {
		ods("Fail lock");
		return;
	}

	for (i = 0; i < NUM_TEXTS; i++) {
		if (sm->texts[i].width == 0) {
			y += iHeight / 4;
		} else if (sm->texts[i].width > 0) {
			indexes[idx] = i;
			yofs[idx] = y;
			y += iHeight;
			idx++;
		}
	}

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
		int index = indexes[i];
		int w = (int)(sm->texts[index].width * s);
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

		bool regen = false;

		if ((ctx->textures[index] == -1) || (! glIsTexture(ctx->textures[index]))) {
			if (ctx->textures[index] != -1)
				ods("Lost texture");
			regen = true;
		} else {
			glBindTexture(GL_TEXTURE_2D, ctx->textures[index]);
			GLfloat bordercolor[4];
			glGetTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);
			if (bordercolor[0] != fBorder[0] || bordercolor[1] != fBorder[1] || bordercolor[2] != fBorder[2] || bordercolor[3] != fBorder[3]) {
				ods("Texture hijacked");
				regen = true;
			}
		}
		if (regen) {
			ctx->uiCounter[index] = 0;
			glGenTextures(1, &ctx->textures[index]);
			glBindTexture(GL_TEXTURE_2D, ctx->textures[index]);
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, fBorder);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		if (sm->texts[index].uiCounter != ctx->uiCounter[index]) {
			ods("Updating %d %d texture", sm->texts[index].width, TEXT_HEIGHT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXT_WIDTH, TEXT_HEIGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, sm->texts[index].texture);
			ctx->uiCounter[index] = sm->texts[index].uiCounter;
		}

		ods("Drawing text at %d %d  %d %d", x, y + yofs[i], w, iHeight);

		glPushMatrix();

		double xm = 0.0;
		double ym = 0.0;
		double xmx = (1.0 * sm->texts[index].width) / TEXT_WIDTH;
		double ymx = 1.0;

		unsigned int c = sm->texts[index].color;

		glColor4ub((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF, (c >> 24) & 0xFF);

		glTranslatef(x, y + yofs[i], 0.0);

		GLint vertex[] = {0, iHeight, 0, 0, w, 0, w, iHeight };
		GLfloat tex[] = {xm, ymx, xm, ym, xmx, ym, xmx, ymx};
		glVertexPointer(2, GL_INT, 0, vertex);
		glTexCoordPointer(2, GL_FLOAT, 0, tex);
		glDrawArrays(GL_QUADS, 0, 4);

		glPopMatrix();
	}
	sem_post(sem);
}

static void drawContext(Context * ctx, int width, int height) {
	GLint program;
	GLint viewport[4];
	int i;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_ALL_ATTRIB_BITS);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, height, 0, -100.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();


	glDisable(GL_ALPHA_TEST);
	glDisable(GL_AUTO_NORMAL);
	// Skip clip planes, there are thousands of them.
	glDisable(GL_COLOR_LOGIC_OP);
	glDisable(GL_COLOR_TABLE);
	glDisable(GL_CONVOLUTION_1D);
	glDisable(GL_CONVOLUTION_2D);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_DITHER);
	glDisable(GL_FOG);
	glDisable(GL_HISTOGRAM);
	glDisable(GL_INDEX_LOGIC_OP);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
	// Skip line smmooth
	// Skip map
	glDisable(GL_MINMAX);
	// Skip polygon offset
	glDisable(GL_SEPARABLE_2D);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_TEXTURE_GEN_Q);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glRenderMode(GL_RENDER);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_EDGE_FLAG_ARRAY);

	glPixelStorei(GL_UNPACK_SWAP_BYTES, 0);
	glPixelStorei(GL_UNPACK_LSB_FIRST, 0);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GLint texunits = 1;

	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &texunits);

	for (i=texunits-1;i>=0;--i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glDisable(GL_TEXTURE_1D);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_3D);
	}

	glDisable(GL_TEXTURE_CUBE_MAP);
	glDisable(GL_VERTEX_PROGRAM_ARB);
	glDisable(GL_FRAGMENT_PROGRAM_ARB);

	glUseProgram(ctx->uiProgram);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glMatrixMode(GL_MODELVIEW);

	GLint uni = glGetUniformLocation(ctx->uiProgram, "tex");
	glUniform1i(uni, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	drawOverlay(ctx, width, height);

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopClientAttrib();
	glPopAttrib();
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glUseProgram(program);
}

__attribute__((visibility("default")))
void glXSwapBuffers(Display * dpy, GLXDrawable draw) {
	if (!oglXSwapBuffers)
		resolveOpenGL();

	if (!sm) {
		resolveSM();
	}

	if (sm) {
		sm->bHooked = true;
		GLXContext ctx = glXGetCurrentContext();

		Context *c = contexts;
		while (c) {
			if ((c->dpy == dpy) && (c->draw == draw))
				break;
			c = c->next;
		}

		if (!c) {
			ods("Current context is: %p", ctx);

			c = (Context *) malloc(sizeof(Context));
			if (!c) {
				ods("malloc failure");
				return;
			}
			c->next = contexts;
			c->dpy = dpy;
			c->draw = draw;

			c->bGlx = false;
			c->bValid = false;

			int major, minor;
			if (glXQueryVersion(dpy, &major, &minor)) {
				ods("GLX version %d.%d", major, minor);
				c->bValid = true;
				if ((major > 1) || (major==1 && minor >= 3))
					c->bGlx = true;
			}
			contexts = c;
			newContext(c);
		}

		if (c->bValid) {
			GLuint width, height;
			if (c->bGlx) {
				glXQueryDrawable(dpy, draw, GLX_WIDTH, (unsigned int *) &width);
				glXQueryDrawable(dpy, draw, GLX_HEIGHT, (unsigned int *) &height);
			} else {
				GLint viewport[4];
				glGetIntegerv(GL_VIEWPORT, viewport);
				width = viewport[2];
				height = viewport[3];
			}

			drawContext(c, width, height);
		}
	}
	oglXSwapBuffers(dpy, draw);
}

#define FGRAB(x) if (strcmp((const char *)(func), #x)==0) return (__GLXextFuncPtr)(x);

__attribute__((visibility("default")))
void (*glXGetProcAddress(const GLubyte * func))(void) {
	FGRAB(glXSwapBuffers);
	FGRAB(glXGetProcAddressARB);
	FGRAB(glXGetProcAddress);

	if (!oglXGetProcAddressARB && !oglXGetProcAddress)
		resolveOpenGL();
	if (oglXGetProcAddress)
		return oglXGetProcAddress(func);
	else if (oglXGetProcAddressARB)
		return oglXGetProcAddressARB(func);
	else
		return (__GLXextFuncPtr)(odlsym(RTLD_NEXT, (const char *)(func)));
}

__attribute__((visibility("default")))
__GLXextFuncPtr glXGetProcAddressARB(const GLubyte * func) {
	return (void (*)(void)) glXGetProcAddress(func);
}

__attribute__((constructor))
static void initializeLibrary() {
	if (odlsym)
		return;

	if (getenv("MUMBLE_OVERLAY_DEBUG"))
		bDebug = true;
	else
		bDebug = false;

	ods("Mumble overlay library loaded\n");
	void *dl = dlopen("libdl.so.2", RTLD_LAZY);
	if (!dl) {
		ods("Failed to open libdl.so.2\n");
	} else {
		int i;
		struct link_map *lm = (struct link_map *) dl;
		int nchains = 0;
		ElfW(Sym) *symtab = NULL;
		const char *strtab = NULL;

		ElfW(Dyn) *dyn = lm->l_ld;

		while (dyn->d_tag) {
			switch (dyn->d_tag) {
				case DT_HASH:
					nchains = *(int *)(dyn->d_un.d_ptr + 4);
					break;
				case DT_STRTAB:
					strtab = (const char *) dyn->d_un.d_ptr;
					break;
				case DT_SYMTAB:
					symtab = (ElfW(Sym) *) dyn->d_un.d_ptr;
					break;
			}
			dyn ++;
		}
		ods("Iterating dlsym table %p %p %d", symtab, strtab, nchains);
		for (i=0;i<nchains;++i) {
			if (ELF32_ST_TYPE(symtab[i].st_info) != STT_FUNC)
				continue;
			if (strcmp(strtab+symtab[i].st_name, "dlsym") == 0)
				odlsym = lm->l_addr + symtab[i].st_value;
		}
		ods("Original dlsym at %p", odlsym);
	}
}

#define OGRAB(name) if (handle == RTLD_DEFAULT) handle = RTLD_NEXT; symbol = odlsym(handle, #name); if (symbol) { o##name = (__typeof__(&name)) symbol; symbol = (void *) name;}
__attribute__((visibility("default")))
void *dlsym(void *handle, const char *name) {
	if (!odlsym)
		initializeLibrary();

	void *symbol;

	ods("Request for symbol %s (%p:%p)\n", name, handle, odlsym);

	if (strcmp(name, "glXSwapBuffers") == 0) {
		OGRAB(glXSwapBuffers);
	} else if (strcmp(name, "glXGetProcAddress") == 0) {
		OGRAB(glXGetProcAddress);
	} else if (strcmp(name, "glXGetProcAddressARB") == 0) {
		OGRAB(glXGetProcAddressARB);
	} else if (strcmp(name, "dlsym") == 0) {
		return (void *) dlsym;
	} else {
		symbol = odlsym(handle, name);
	}
	return symbol;
}
