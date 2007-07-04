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
#include <GL/gl.h>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Typdefs for functions we're overriding
typedef void *(*fdlsym)(void *, const char *);
typedef void(*fglXSwapBuffers)(Display *,GLXDrawable);

// Original and replacement swapbuffers
static fglXSwapBuffers oglXSwapBuffers;
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
      oglXSwapBuffers = (fglXSwapBuffers) gdlsym(ctx, fname);
      if (oglXSwapBuffers)
        return (void *) glXSwapBuffers;
      else
        return (void *) oglXSwapBuffers;
    }
  }
  return gdlsym(ctx, fname);
}

static void *glsym(const char *fname) {
  bTrueSymbol = true;
  void *r=dlsym(RTLD_NEXT, fname);
  bTrueSymbol = false;
  return r;
}

void glXSwapBuffers(Display *dpy, GLXDrawable draw) {
  if (! oglXSwapBuffers)
    oglXSwapBuffers = (fglXSwapBuffers) glsym("glXSwapBuffers");
  fprintf(stderr, "Swapping\n");
  oglXSwapBuffers(dpy, draw);
}
