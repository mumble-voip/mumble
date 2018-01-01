// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

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
		int width, height;
		if (c->bMesa) {
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			width = viewport[2];
			height = viewport[3];
		} else {
			glXQueryDrawable(dpy, draw, GLX_WIDTH, (GLuint*)&width);
			glXQueryDrawable(dpy, draw, GLX_HEIGHT, (GLuint*)&height);
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

static int find_odlsym() {
#if defined(__linux__)
	void *dl = dlopen("libdl.so.2", RTLD_LAZY);
	if (!dl) {
		ods("Failed to open libdl.so.2");
	} else {
		int i = 0;
		struct link_map *lm = (struct link_map *) dl;
		int nchains = 0;
		ElfW(Sym) *symtab = NULL;
		const char *strtab = NULL;
#if defined(__GLIBC__)
		const ElfW(Addr) base = 0;
#else
		const ElfW(Addr) base = lm->l_addr;
#endif

		ElfW(Dyn) *dyn = lm->l_ld;

		while (dyn->d_tag) {
			switch (dyn->d_tag) {
				case DT_HASH:
					nchains = *(int *)(base + dyn->d_un.d_ptr + 4);
					break;
				case DT_STRTAB:
					strtab = (const char *)(base + dyn->d_un.d_ptr);
					break;
				case DT_SYMTAB:
					symtab = (ElfW(Sym) *)(base + dyn->d_un.d_ptr);
					break;
			}
			dyn ++;
		}
		ods("Iterating dlsym table %p %p %d", symtab, strtab, nchains);
		for (i = 0; i < nchains; i++) {
			// ELF32_ST_TYPE and ELF64_ST_TYPE are the same
			if (ELF32_ST_TYPE(symtab[i].st_info) != STT_FUNC) {
				continue;
			}
			if (strcmp(strtab+symtab[i].st_name, "dlsym") == 0) {
				odlsym = (void*)lm->l_addr + symtab[i].st_value;
			}
		}
		if (odlsym == NULL) {
			goto err;
		}
		ods("Original dlsym at %p", odlsym);
	}

	return 0;
#elif defined(__FreeBSD__)
	int i = 0;
	struct link_map *lm = NULL;
	int nchains = 0;
	Elf_Sym * symtab = NULL;
	const char *strtab = NULL;

	if (dlinfo(RTLD_SELF, RTLD_DI_LINKMAP, &lm) == -1) {
		ods("Unable to acquire link_map: %s", dlerror());
		goto err;
	}

	while (lm != NULL) {
		if (!strcmp(lm->l_name, "/libexec/ld-elf.so.1")) {
			break;
		}
		lm = lm->l_next;
	}
	if (lm == NULL) {
		goto err;
	}

	Elf_Dyn *dyn = (Elf_Dyn *) lm->l_ld;

	while (dyn->d_tag) {
		switch (dyn->d_tag) {
			case DT_HASH:
				nchains = *(int *)((uintptr_t)lm->l_addr + (uintptr_t)dyn->d_un.d_ptr + 4);
				break;
			case DT_STRTAB:
				strtab = (const char *)((uintptr_t)lm->l_addr + (uintptr_t)dyn->d_un.d_ptr);
				break;
			case DT_SYMTAB:
				symtab = (Elf_Sym *)((uintptr_t)lm->l_addr + (uintptr_t)dyn->d_un.d_ptr);
				break;
		}
		dyn++;
	}
	ods("Iterating dsym table %p %p %d", symtab, strtab, nchains);
	for (i = 0; i < nchains; i++) {
		if (ELF_ST_TYPE(symtab[i].st_info) != STT_FUNC) {
			continue;
		}
		if (strcmp(strtab + symtab[i].st_name, "dlsym") == 0) {
			odlsym = (void *) lm->l_addr + symtab[i].st_value;
		}
	}
	if (odlsym == NULL) {
		goto err;
	}
	ods("Original dlsym at %p", odlsym);

	return 0;
#endif
err:
	return -1;
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
	if (find_odlsym() == -1) {
		ods("Failed to find original address of dlsym().");
	}
}
