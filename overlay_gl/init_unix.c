/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2008-2015, Mikkel Krautz <mikkel@krautz.dk>

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

// This file is included by overlay.c for
// Unix/X11/GLX-specific overlay initialization.

static void initializeLibrary();

#define RESOLVE(x) if (! o##x) o##x = (__typeof__(&x)) odlsym(RTLD_NEXT, #x)
static void resolveOpenGL() {
	RESOLVE(glXSwapBuffers);

	if (! oglXSwapBuffers) {
		void *lib = dlopen("libGL.so.1", RTLD_GLOBAL | RTLD_NOLOAD);
		if (! lib)
			return;
		RESOLVE(glXSwapBuffers);
		if (! oglXSwapBuffers) {
			dlclose(lib);
		}
	}

	RESOLVE(glXGetProcAddressARB);
	RESOLVE(glXGetProcAddress);
}

__attribute__((visibility("default")))
void glXSwapBuffers(Display * dpy, GLXDrawable draw) {
	if (!oglXSwapBuffers) {
		resolveOpenGL();
	}

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
		memset(c, 0, sizeof(Context));
		c->next = contexts;
		c->dpy = dpy;
		c->draw = draw;

		c->bMesa = false;
		c->bValid = false;

		int major, minor;
		if (glXQueryVersion(dpy, &major, &minor)) {
			ods("GLX version %d.%d", major, minor);
			c->bValid = true;
		}

		const char *version = (const char *) glGetString(GL_VERSION);
		if (version) {
			ods("GL version string: %s", version);
			if (strstr(version, "Mesa") != NULL) {
				c->bMesa = true;
			}
		}

		contexts = c;
		newContext(c);
	}

	if (c->bValid) {
		GLuint width, height;
		if (c->bMesa) {
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			width = viewport[2];
			height = viewport[3];
		} else {
			glXQueryDrawable(dpy, draw, GLX_WIDTH, (unsigned int *) &width);
			glXQueryDrawable(dpy, draw, GLX_HEIGHT, (unsigned int *) &height);
		}

		drawContext(c, width, height);
	}
	oglXSwapBuffers(dpy, draw);
}

#define FGRAB(x) if (strcmp((const char *)(func), #x)==0) return (__GLXextFuncPtr)(x);

__attribute__((visibility("default")))
void (*glXGetProcAddress(const GLubyte * func))(void) {
	FGRAB(glXSwapBuffers);
	FGRAB(glXGetProcAddressARB);
	FGRAB(glXGetProcAddress);

	if (!oglXGetProcAddressARB && !oglXGetProcAddress) {
		resolveOpenGL();
	}
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

#define OGRAB(name) if (handle == RTLD_DEFAULT) handle = RTLD_NEXT; symbol = odlsym(handle, #name); if (symbol) { o##name = (__typeof__(&name)) symbol; symbol = (void *) name;}
__attribute__((visibility("default")))
void *dlsym(void *handle, const char *name) {
	if (!odlsym) {
		initializeLibrary();
	}

	void *symbol;

	ods("Request for symbol %s (%p:%p)", name, handle, odlsym);

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

__attribute__((constructor))
static void initializeLibrary() {
	if (odlsym)
		return;

	if (getenv("MUMBLE_OVERLAY_DEBUG")) {
		bDebug = true;
	} else {
		bDebug = false;
	}

	bCursorAvail = false;

	ods("Mumble overlay library loaded");
	void *dl = dlopen("libdl.so.2", RTLD_LAZY);
	if (!dl) {
		ods("Failed to open libdl.so.2");
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
			// ELF32_ST_TYPE and ELF64_ST_TYPE are the same
			if (ELF32_ST_TYPE(symtab[i].st_info) != STT_FUNC)
				continue;
			if (strcmp(strtab+symtab[i].st_name, "dlsym") == 0)
				odlsym = (void*)lm->l_addr + symtab[i].st_value;
		}
		ods("Original dlsym at %p", odlsym);
	}
}
