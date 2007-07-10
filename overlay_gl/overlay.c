/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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
#define _GNU_SOURCE
#include <GL/glx.h>
#include <GL/gl.h>
#include <dlfcn.h>
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

void *__libc_dlsym(void *, const char *);

// Prototypes
static void resolveSM();
static void ods(const char *format, ...);

static struct SharedMem *sm = NULL;
static sem_t *sem = NULL;

typedef struct _Context {
    struct _Context *next;
    Display *dpy;
    GLXDrawable draw;
    GLXContext glctx;
    GLuint textures[NUM_TEXTS];
} Context;

static Context *contexts = NULL;

#define FDEF(name) static __typeof__(&name) o##name = NULL

#ifdef PRELOAD
FDEF(dlsym);
#endif

FDEF(glXSwapBuffers);
FDEF(glXGetProcAddressARB);
FDEF(glXGetProcAddress);

#define RESOLVE(x) if (! o##x) o##x = (__typeof__(&x)) dlsym(RTLD_NEXT, #x)

static void resolveOpenGL()
{
    RESOLVE(glXSwapBuffers);
    RESOLVE(glXGetProcAddressARB);
    RESOLVE(glXGetProcAddress);
}

static void resolveSM()
{
    int fd = shm_open("/MumbleOverlayMem", O_RDWR, 0600);
    if (fd >= 0) {
	sm = (struct SharedMem *) (mmap(NULL, sizeof(struct SharedMem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
	if (sm == (void *) (-1)) {
	    sm = NULL;
	    close(fd);
	} else {
	    sem = sem_open("/MumbleOverlaySem", 0);
	    if (sem == SEM_FAILED) {
		munmap(sm, sizeof(struct SharedMem));
		sm = NULL;
		close(fd);
	    }
	}
    }
}

__attribute__ ((format(printf, 1, 2)))
void ods(const char *format, ...)
{
    if (sm && !sm->bDebug)
	return;

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
    fflush(stderr);
}

static void newContext(Context * ctx)
{
    int i;

    if (sm) {
	sm->bHooked = true;
	for (i = 0; i < NUM_TEXTS; i++)
	    sm->texts[i].bUpdated = true;
    }

    glGenTextures(NUM_TEXTS, ctx->textures);

    // Here we go. From the top. Where is glResetState?
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_AUTO_NORMAL);
    glEnable(GL_BLEND);
    // Skip clip planes, there are thousands of them.
    glDisable(GL_COLOR_LOGIC_OP);
    glEnable(GL_COLOR_MATERIAL);
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
    // Skip line smmooth 
    // Skip map
    glDisable(GL_MINMAX);
    // Skip polygon offset
    glDisable(GL_SEPARABLE_2D);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_GEN_Q);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);


}

static void drawContext(Context * ctx, Display * dpy, GLXDrawable draw)
{
    sm->bHooked = true;

    // DEBUG
    // sm->bDebug = true;

    unsigned int width, height;
    int i;

    glXQueryDrawable(dpy, draw, GLX_WIDTH, &width);
    glXQueryDrawable(dpy, draw, GLX_HEIGHT, &height);

    ods("DrawStart: Screen is %d x %d", width, height);

    if (sm->fFontSize < 0.01)
	sm->fFontSize = 0.01;
    else if (sm->fFontSize > 1.0)
	sm->fFontSize = 1.0;

    int iHeight = (int) ((height * 1.0) * sm->fFontSize);
    if (iHeight > TEXT_HEIGHT)
	iHeight = TEXT_HEIGHT;

    float s = iHeight / 60.0;
    int y = 0;
    int idx = 0;

    int texs[NUM_TEXTS];
    int widths[NUM_TEXTS];
    int yofs[NUM_TEXTS];
    unsigned int color[NUM_TEXTS];

    if (sem_trywait(sem) != 0)
	return;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -100.0, 100.0);

    glMatrixMode(GL_MODELVIEW);

    for (i = 0; i < NUM_TEXTS; i++) {
	if (sm->texts[i].width == 0) {
	    y += iHeight / 4;
	} else if (sm->texts[i].width > 0) {
	    if (sm->texts[i].bUpdated) {
		ods("Updating %d %d texture", sm->texts[i].width, TEXT_HEIGHT);
		glBindTexture(GL_TEXTURE_2D, ctx->textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXT_WIDTH, TEXT_HEIGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, sm->texts[i].texture);
		sm->texts[i].bUpdated = false;
	    }
	    texs[idx] = ctx->textures[i];
	    widths[idx] = sm->texts[i].width;
	    color[idx] = sm->texts[i].color;
	    yofs[idx] = y;
	    y += iHeight;
	    idx++;
	}
    }
    sem_post(sem);

    int h = y;
    y = (int) (height * sm->fY);

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
	int w = (int) (widths[i] * s);
	int x = (int) (width * sm->fX);
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

	ods("Drawing text at %d %d  %d %d", x, y + yofs[i], w, iHeight);
	glBindTexture(GL_TEXTURE_2D, texs[i]);
	glPushMatrix();
	glLoadIdentity();

	double xm = 0.0;
	double ym = 0.0;
	double xmx = (1.0 * widths[i]) / TEXT_WIDTH;
	double ymx = 1.0;

	unsigned int c = color[i];

	glColor4ub((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF, (c >> 24) & 0xFF);


	glTranslatef(x, y + yofs[i], 0.0);
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
	glPopMatrix();
    }
}

// static map<GLXContext, GLContext *> contexts;

__attribute__ ((visibility("default")))
void glXSwapBuffers(Display * dpy, GLXDrawable draw)
{
    if (!oglXSwapBuffers)
	resolveOpenGL();

    if (!sm) {
	resolveSM();
    }

    if (sm) {
	GLXContext ctx = glXGetCurrentContext();

	Context *c = contexts;
	while (c) {
	    if ((c->dpy == dpy) && (c->draw == draw))
		break;
	    c = c->next;
	}

	if (!c) {

	    int attrib[4] = { GLX_FBCONFIG_ID, -1, 0, 0 };
	    glXQueryContext(dpy, ctx, GLX_FBCONFIG_ID, &attrib[1]);

	    int screen = -1;
	    glXQueryContext(dpy, ctx, GLX_SCREEN, &screen);

	    ods("Query Context: xid %d screen %d\n", attrib[1], screen);

	    int nelem = -1;
	    GLXFBConfig *fb = glXChooseFBConfig(dpy, screen, attrib, &nelem);
	    ods("ChooseFB returned %d elems: %p\n", nelem, fb);

	    GLXContext myctx = glXCreateNewContext(dpy, *fb, GLX_RGBA_TYPE, NULL, 1);
	    ods("Got Context %p\n", myctx);

	    if ((nelem == 1) && (myctx)) {
		c = (Context *) malloc(sizeof(Context));
		c->next = contexts;

		c->glctx = myctx;
		c->dpy = dpy;
		c->draw = draw;

		contexts = c;

		glXMakeCurrent(dpy, draw, myctx);

		newContext(c);
	    }
	}

	if (c) {
	    glXMakeCurrent(dpy, draw, c->glctx);
	    drawContext(c, dpy, draw);
	    glXMakeCurrent(dpy, draw, ctx);
	}
    } else {
	static bool warned = false;
	if (!warned) {
	    ods("MUMBLE OVERLAY:: NO CONTACT WITH MUMBLE\n");
	    warned = true;
	}
    }
    oglXSwapBuffers(dpy, draw);
}

#define FGRAB(x) if (strcmp((const char *)(func), #x)==0) return (__GLXextFuncPtr)(x);

__attribute__ ((visibility("default")))
void (*glXGetProcAddress(const GLubyte * func)) (void)
{
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
	return (__GLXextFuncPtr) (dlsym(RTLD_NEXT, (const char *) (func)));
}

__attribute__ ((visibility("default")))
__GLXextFuncPtr glXGetProcAddressARB(const GLubyte * func)
{
    return (void (*)(void)) glXGetProcAddress(func);
}

#ifdef PRELOAD

#define OGRAB(name) if (handle == RTLD_DEFAULT) handle = RTLD_NEXT; __typeof__(&name) t = (__typeof__(&name)) (__typeof__(&name))(odlsym(handle, #name)); if (t) { o##name = t; return (void *)(name); } else { return NULL;}

__attribute__ ((visibility("default")))
void *dlsym(void *handle, const char *name)
{
    if (!odlsym) {
	void *dl = dlopen("libdl.so.2", RTLD_LAZY);
	if (!dl) {
	    ods("Failed to open libdl.so.2\n");
	} else {
	    odlsym = (__typeof__(&dlsym)) __libc_dlsym(dl, "dlsym");
	}
    }
    if (strcmp(name, "glXSwapBuffers") == 0) {
	OGRAB(glXSwapBuffers);
    } else if (strcmp(name, "glXGetProcAddress") == 0) {
	OGRAB(glXGetProcAddress);
    } else if (strcmp(name, "glXGetProcAddressARB") == 0) {
	OGRAB(glXGetProcAddressARB);
    }
    return odlsym(handle, name);
}

#endif
