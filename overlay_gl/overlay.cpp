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

#include <GL/glx.h>
#include <GL/glu.h>
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
#include <map>
#include "../overlay/overlay.h"

using namespace std;

// Prototypes
static void resolveOpenGL(void *);
static void resolveSM();
static void ods(const char *format, ...);

static SharedMem *sm;

#define FDEF(return,name,args) typedef return(*f##name)args; static f##name o##name

FDEF(void,glXSwapBuffers,(Display *,GLXDrawable));
FDEF(GLXContext, glXGetCurrentContext,(void));
FDEF(void, glGenTextures,(GLsizei,GLuint *));
FDEF(void, glXQueryDrawable, (Display *,GLXDrawable, int, unsigned int *));
FDEF(void, glBindTexture, (GLenum, GLuint));
FDEF(void, glBegin, (GLenum));
FDEF(void, glVertex2f, (GLfloat, GLfloat));
FDEF(void, glEnd, (void));
FDEF(void, glPushAttrib, (GLbitfield));
FDEF(void, glPopAttrib, (void));
FDEF(void, glViewport, (GLint, GLint, GLsizei, GLsizei));
FDEF(void, glMatrixMode, (GLenum));
FDEF(void, glPushMatrix, (void));
FDEF(void, glPopMatrix, (void));
FDEF(void, glLoadIdentity, (void));
FDEF(void, glOrtho, (GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble));
FDEF(void, glActiveTextureARB, (GLenum));
FDEF(void, glEnable, (GLenum));
FDEF(void, glDisable, (GLenum));
FDEF(void, glBlendFunc, (GLenum, GLenum));
FDEF(void, glColorMaterial, (GLenum, GLenum));
FDEF(void, glTexParameteri, (GLenum, GLenum, GLint));
FDEF(void, glTexImage2D, (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *));
FDEF(void, glColor4ub, (GLubyte, GLubyte, GLubyte, GLubyte));
FDEF(void, glTranslatef, (GLfloat, GLfloat, GLfloat));
FDEF(void, glTexCoord2f, (GLfloat, GLfloat));

// Typdefs for functions we're overriding
typedef void *(*fdlsym)(void *, const char *);

// Original and replacement swapbuffers
void glXSwapBuffers(Display *, GLXDrawable);

// Define for the dlsym we're chaining to. Ugly hack.
extern "C" void * __libc_dlsym(void *, const char *);

// if true, we'll always return the "true" symbol.
static bool bTrueSymbol = false;

/*
 * This is a truly ugly hack, but as many games dlopen the GL
 * libraries and then dlsym the functions directly, we have
 * to override dlsym. And we also have to call the original dlsym,
 * which leaves us a problem, as we can't very well dlsym("dlsym").
 *
 * This works, but requires glibc2.
 */

void *dlsym(void *ctx, const char *fname) {
  static fdlsym gdlsym;
  if (gdlsym == NULL) {
    void *dl = dlopen("libdl.so.2", RTLD_LAZY);
    if (! dl) {
      fprintf(stderr, "Failed to open libdl.so.2\n");
    } else {
      gdlsym=(fdlsym) __libc_dlsym(dl, "dlsym");
    }
  }
  if (! bTrueSymbol) {
    if (strcmp(fname, "glXSwapBuffers")==0) {
      resolveOpenGL(ctx);
      if (oglXSwapBuffers)
        return (void *) glXSwapBuffers;
      else
        return (void *) oglXSwapBuffers;
    }
  }
  return gdlsym(ctx, fname);
}

#define RESOLVE(x) o##x = (f##x) dlsym(ctx, #x)

static void resolveOpenGL(void *ctx) {
  bTrueSymbol = true;
  if (ctx == RTLD_DEFAULT)
    ctx = RTLD_NEXT;

RESOLVE(glXSwapBuffers);
RESOLVE(glXGetCurrentContext);
RESOLVE(glGenTextures);
RESOLVE(glXQueryDrawable);
RESOLVE(glBindTexture);
RESOLVE(glBegin);
RESOLVE(glVertex2f);
RESOLVE(glEnd);
RESOLVE(glPushAttrib);
RESOLVE(glPopAttrib);
RESOLVE(glViewport);
RESOLVE(glMatrixMode);
RESOLVE(glPushMatrix);
RESOLVE(glPopMatrix);
RESOLVE(glLoadIdentity);
RESOLVE(glOrtho);
RESOLVE(glActiveTextureARB);
RESOLVE(glEnable);
RESOLVE(glDisable);
RESOLVE(glBlendFunc);
RESOLVE(glColorMaterial);
RESOLVE(glTexParameteri);
RESOLVE(glTexImage2D);
RESOLVE(glColor4ub);
RESOLVE(glTranslatef);
RESOLVE(glTexCoord2f);

  RESOLVE(glXGetCurrentContext);
  RESOLVE(glXSwapBuffers);
  RESOLVE(glGenTextures);
  RESOLVE(glXQueryDrawable);
  RESOLVE(glBindTexture);
  RESOLVE(glBegin);
  RESOLVE(glVertex2f);
  RESOLVE(glEnd);

  bTrueSymbol = false;
}

static sem_t *sem;

static void resolveSM() {
  int fd = shm_open("/MumbleOverlayMem", O_RDWR, 0600);
  if (fd >= 0) {
    sm=static_cast<SharedMem *>(mmap(NULL, sizeof(SharedMem), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0));
    if (sm == reinterpret_cast<SharedMem *>(-1)) {
      sm = NULL;
      close(fd);
    } else {
      sem = sem_open("/MumbleOverlaySem", 0);
      if (sem == SEM_FAILED) {
        munmap(sm, sizeof(SharedMem));
        sm = NULL;
        close(fd);
      }
    }
  }
}

void ods(const char *format, ...) {
  if (!sm || ! sm->bDebug)
    return;
    
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");
  fflush(stderr);
}

class GLContext {
  public:
    GLXContext ctx;
    GLuint textures[NUM_TEXTS];
    GLContext(GLXContext context);
    void draw(Display *, GLXDrawable);
    ~GLContext();
};

GLContext::GLContext(GLXContext context) { 
  ctx = context;
  oglGenTextures(NUM_TEXTS, textures);
}

GLContext::~GLContext() {
}

void GLContext::draw(Display *dpy, GLXDrawable draw) {
  sm->bHooked = true;
  
  // DEBUG
  // sm->bDebug = true;
  
  unsigned int width, height;
  oglXQueryDrawable(dpy, draw, GLX_WIDTH, &width);
  oglXQueryDrawable(dpy, draw, GLX_HEIGHT, &height);
  
  ods("DrawStart: Screen is %d x %d", width, height);

  if (sm->fFontSize < 0.01)
    sm->fFontSize = 0.01;
  else if (sm->fFontSize > 1.0)
    sm->fFontSize = 1.0;
    
  int iHeight = (height *1.0) * sm->fFontSize;
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
    
  // Save all state. Please?
  oglPushAttrib(-1);

  oglViewport(0,0,width,height);

  oglMatrixMode(GL_PROJECTION);
  oglPushMatrix();
  oglLoadIdentity();
  oglOrtho(0, width, height, 0, -100.0, 100.0);
  oglMatrixMode(GL_TEXTURE);
  oglPushMatrix();
  oglLoadIdentity();
  oglMatrixMode(GL_COLOR);
  oglPushMatrix();
  oglLoadIdentity();

  oglMatrixMode(GL_MODELVIEW);
  oglPushMatrix();
  oglLoadIdentity();

  oglActiveTextureARB(GL_TEXTURE0_ARB);

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
  
  for(int i=0;i<NUM_TEXTS;i++) {
    if (sm->texts[i].width == 0) {
      y+= iHeight / 4;
    } else if (sm->texts[i].width > 0) {
      if (sm->texts[i].bUpdated) {
        ods("Updating %d %d texture", sm->texts[i].width, TEXT_HEIGHT);      
        oglBindTexture(GL_TEXTURE_2D, textures[i]);
        oglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        oglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//        oglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
//        oglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        oglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        oglTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
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
  sem_post(sem);
  
  int h = y;
  y = height * sm->fY;

  if (sm->bTop) {
    y -= h;
  } else if (sm->bBottom) {
  } else {
      y -= h / 2;
  }
  
  if (y < 1)
    y = 1;
  if ((y+h+1) > height)
    y = height - h - 1;


  for(int i=0;i<idx;i++) {
    int w = widths[i] * s;
    int x = width * sm->fX;
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
    
    ods("Drawing text at %d %d  %d %d", x, y+yofs[i], w, iHeight);
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
  
  oglBindTexture(GL_TEXTURE_2D, textures[NUM_TEXTS-1]);
  
  oglPopMatrix();
  oglMatrixMode(GL_COLOR);
  oglPopMatrix();
  oglMatrixMode(GL_TEXTURE);
  oglPopMatrix();
  oglMatrixMode(GL_PROJECTION);
  oglPopMatrix();
  
  oglPopAttrib();
}

static map<GLXContext, GLContext *> contexts;

void glXSwapBuffers(Display *dpy, GLXDrawable draw) {
  if (! oglXSwapBuffers)
    resolveOpenGL(RTLD_NEXT);
  
  if (! sm) {
    resolveSM();
    if (sm) {
      sm->bHooked = true;
      for(int i=0;i<NUM_TEXTS;i++)
        sm->texts[i].bUpdated = true;
    }
  }
    
  if (sm) {
      GLXContext ctx = oglXGetCurrentContext();
  
    GLContext *c = contexts[ctx];
    if (!c) {
      c = new GLContext(ctx);
      contexts[ctx] = c;
    }
    c->draw(dpy,draw);
  } else {
    static bool warned = false;
    if (! warned) {
      fprintf(stderr, "MUMBLE OVERLAY:: NO CONTACT WITH MUMBLE\n");
      warned = true;
    }
  }
  oglXSwapBuffers(dpy, draw);
}
