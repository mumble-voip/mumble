/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2008-2009, Mikkel Krautz <mikkel@krautz.dk>

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

#include <wchar.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ipc.h>   
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <mach/mach_init.h>
#include <mach/task.h>
#include <semaphore.h>
#include <dlfcn.h>

#include <OpenGL/OpenGL.h>
#include <Carbon/Carbon.h>
#include <Cocoa/Cocoa.h>
#include <AGL/AGL.h>

#include "mach_override.h"
#include <objc/objc-runtime.h>

#include "../../overlay/overlay.h"

static struct SharedMem *sm = NULL;
static sem_t *sem = NULL;
static bool bDebug = false;

enum {
	OVERLAY_TYPE_AGL = 0,
	OVERLAY_TYPE_COCOA,
	OVERLAY_TYPE_CGL,
};

typedef struct _Context {
	struct _Context *next;
	union {
		AGLContext       aglctx;
		NSOpenGLContext  *nsctx;
		CGLContextObj    cglctx;
	};
	unsigned int type;
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
FDEF(aglSwapBuffers);
FDEF(CGLFlushDrawable);

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

	if (bDebug)
		fprintf(stderr, "Successfully hooked GL!\n");
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

#if 0
		GLint vertex[] = {0, iHeight, 0, 0, w, 0, w, iHeight };
		GLfloat tex[] = {xm, ymx, xm, ym, xmx, ym, xmx, ymx};
		glVertexPointer(2, GL_INT, 0, vertex);
		glTexCoordPointer(2, GL_FLOAT, 0, tex);
		glDrawArrays(GL_QUADS, 0, 4);
#else
		glBegin(GL_QUADS);
		glTexCoord2f(xm, ymx);
		glVertex2f(0, iHeight);
		glTexCoord2f(xm, ym);
		glVertex2f(0, 0);
		glTexCoord2f(xmx, ym);
		glVertex2f(w, 0);
		glTexCoord2f(xmx, ymx);
		glVertex2f(w, iHeight);
		glEnd();
#endif 

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
	
	for(i=texunits-1;i>=0;--i) {
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

void aglSwapBuffersOverride(AGLContext ctx) {
	ods("aglSwapBuffers()");

	if (sm) {
		Context *c = contexts;
	
		while (c) {
			if (c->aglctx == ctx)
				break;
			c = c->next;
		}

		if (!c) {
			ods("Current context is: %p", ctx);

			c = malloc(sizeof(Context));
			if (!c) {
				ods("malloc failure");
				return;
			}
			c->next = contexts;
			c->aglctx = ctx;
			c->type = OVERLAY_TYPE_AGL;
			contexts = c;
			newContext(c);
		}

		Rect rect;
		AGLDrawable ptr = aglGetDrawable(c->aglctx);
		WindowRef win = GetWindowFromPort(ptr);
		GetWindowPortBounds(win, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		if (!(width > 0 && height > 0)) {
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			width = viewport[2];
			height = viewport[3];
		}
		drawContext(c, width, height);
	}

	oaglSwapBuffers(ctx);
}

@implementation NSObject (NSOpenGLContextOverride)
- (void) overlayFlushBuffer {
	ods("[NSOpenGLContext flushBuffer]");

	if (sm) {
		Context *c = contexts;

		while (c) {
			if (c->nsctx == self)
				break;
			c = c->next;
		}

		if (!c) {
			ods("Current context is: %p", self);

			c = malloc(sizeof(Context));
			if (!c) {
				ods("malloc failure");
				return;
			}
			c->next = contexts;
			c->nsctx = self;
			c->type = OVERLAY_TYPE_COCOA;
			contexts = c;
			newContext(c);
		}

		NSView *v = [c->nsctx view];
		NSRect r = [v bounds];
		int width = (int)r.size.width;
		int height = (int)r.size.height;
		if (!(width > 0 && height > 0)) {
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			width = viewport[2];
			height = viewport[3];
		}

		drawContext(c, width, height);
	}

	[self overlayFlushBuffer];
}
@end

void CGLFlushDrawableOverride(CGLContextObj ctx) {
	ods("CGLFlushDrawable()");

	if (sm) {
		Context *c = contexts;
	
		while (c) {
			if (c->cglctx == ctx)
				break;
			c = c->next;
		}

		if (!c) {
			ods("Current context is: %p", ctx);

			c = malloc(sizeof(Context));
			if (!c) {
				ods("malloc failure");
				return;
			}
			c->next = contexts;
			c->cglctx = ctx;
			c->type = OVERLAY_TYPE_CGL;
			contexts = c;
			newContext(c);
		}

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		int width = viewport[2];
		int height = viewport[3];
		drawContext(c, width, height);
	}

	oCGLFlushDrawable(ctx);
}


__attribute__((constructor))
static void entryPoint() {
	bool b;

	if (getenv("MUMBLE_OVERLAY_DEBUG"))
		bDebug = true;
	else            
		bDebug = false;

	resolveSM();

	void *agl = NULL, *nsgl = NULL, *cgl = NULL;
	agl = dlsym(RTLD_DEFAULT, "aglSwapBuffers");
	nsgl = dlsym(RTLD_DEFAULT, "NSClassFromString");
	cgl = dlsym(RTLD_DEFAULT, "CGLFlushDrawable");

	ods("agl: %s, nsgl: %s, cgl: %s",
		agl ? "true" : "false",
	   nsgl ? "true" : "false",
	    cgl ? "true" : "false");

#if 0
	/* AGL */
	if (agl) {
		ods("Attempting to hook AGL");
		if (mach_override("_aglSwapBuffers", NULL, aglSwapBuffersOverride, (void **) &oaglSwapBuffers) != 0) {
			ods("aglSwapBuffers override failed.");
		}
	}

	/* NSOpenGL */
	if (nsgl) {
		ods("Attempting to hook NSOpenGL");
		Class c = NSClassFromString(@"NSOpenGLContext");
		if (c) {
			Method orig = class_getInstanceMethod(c, @selector(flushBuffer));
			Method new = class_getInstanceMethod(c, @selector(overlayFlushBuffer));
			if (orig && new) {
				IMP tmp = orig->method_imp;
				orig->method_imp = new->method_imp;
				new->method_imp = tmp;
			} else
				ods("Unable to retrieve method pointers.");
		} else
			ods("Unable to look up class NSOpenGLContext.");
	}
#endif

	/* CGL */
	if (cgl) {
		ods("Attempting to hook CGL");
		if (mach_override("_CGLFlushDrawable", NULL, CGLFlushDrawableOverride, (void **) &oCGLFlushDrawable) != 0) {
			ods("CGLFlushDrawable override failed.");
		}
	}

	ods("Up running.");
}
