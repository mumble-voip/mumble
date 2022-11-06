// Copyright 2015-2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// This file is included by overlay.c for
// Unix/X11/GLX-specific overlay initialization.

#include <inttypes.h>

#if defined(__linux__)
// ELF32_ST_TYPE and ELF64_ST_TYPE are the same.
#	define ELF_ST_TYPE ELF32_ST_TYPE

typedef ElfW(Dyn) Elf_Dyn;
typedef ElfW(Sym) Elf_Sym;
#endif

static void initializeLibrary();

#define RESOLVE(x) \
	if (!o##x)     \
	o##x = (__typeof__(&x)) odlsym(RTLD_NEXT, #x)
static void resolveOpenGL() {
	RESOLVE(glXSwapBuffers);

	if (!oglXSwapBuffers) {
		void *lib = dlopen("libGL.so.1", RTLD_GLOBAL | RTLD_NOLOAD);
		if (!lib)
			return;
		RESOLVE(glXSwapBuffers);
		if (!oglXSwapBuffers) {
			dlclose(lib);
		}
	}

	RESOLVE(glXGetProcAddressARB);
	RESOLVE(glXGetProcAddress);
}

__attribute__((visibility("default"))) void glXSwapBuffers(Display *dpy, GLXDrawable draw) {
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
		c->dpy  = dpy;
		c->draw = draw;

		c->bMesa  = false;
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
			width  = viewport[2];
			height = viewport[3];
		} else {
			glXQueryDrawable(dpy, draw, GLX_WIDTH, (GLuint *) &width);
			glXQueryDrawable(dpy, draw, GLX_HEIGHT, (GLuint *) &height);
		}

		drawContext(c, width, height);
	}
	oglXSwapBuffers(dpy, draw);
}

#define FGRAB(x)                                \
	if (strcmp((const char *) (func), #x) == 0) \
		return (__GLXextFuncPtr)(x);

__attribute__((visibility("default"))) void (*glXGetProcAddress(const GLubyte *func))(void) {
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
		return (__GLXextFuncPtr)(odlsym(RTLD_NEXT, (const char *) (func)));
}

__attribute__((visibility("default"))) __GLXextFuncPtr glXGetProcAddressARB(const GLubyte *func) {
	return (void (*)(void)) glXGetProcAddress(func);
}

#define OGRAB(name)                           \
	if (handle == RTLD_DEFAULT)               \
		handle = RTLD_NEXT;                   \
	symbol = odlsym(handle, #name);           \
	if (symbol) {                             \
		o##name = (__typeof__(&name)) symbol; \
		symbol  = (void *) name;              \
	}
__attribute__((visibility("default"))) void *dlsym(void *handle, const char *name) {
	if (!odlsym) {
		initializeLibrary();
	}

	void *symbol;

	ods("Request for symbol; name: %s, handle: %p, odlsym: %p", name, handle, odlsym);

	if (strcmp(name, "glXSwapBuffers") == 0) {
		OGRAB(glXSwapBuffers);
	} else if (strcmp(name, "glXGetProcAddress") == 0) {
		OGRAB(glXGetProcAddress);
	} else if (strcmp(name, "glXGetProcAddressARB") == 0) {
		OGRAB(glXGetProcAddressARB);
	} else if (strcmp(name, "dlsym") == 0) {
		return (void *) dlsym;
	} else if (odlsym) {
		symbol = odlsym(handle, name);
	} else {
		return NULL;
	}

	return symbol;
}

static int find_odlsym() {
	// clang-format off
	const char *libs[] = {
		"libc.so.6",
		"libdl.so.2",
		"/libexec/ld-elf.so.1"
	};
	// clang-format on

	for (uint8_t libIdx = 0; libIdx < sizeof(libs) / sizeof(libs[0]); ++libIdx) {
		const char *lib = libs[libIdx];
		ods("Searching for dlsym() in \"%s\"...", lib);
#ifdef RTLD_SELF
		struct link_map *lm = NULL;
		if (dlinfo(RTLD_SELF, RTLD_DI_LINKMAP, &lm) == -1) {
			ods("Unable to acquire link_map: %s", dlerror());
			return -1;
		}

		while (lm) {
			if (strcmp(lm->l_name, lib) == 0) {
				break;
			}

			lm = lm->l_next;
		}

		if (!lm) {
			ods("Failed to find \"%s\"!", lib);
			continue;
		}
#else
		void *dl = dlopen(lib, RTLD_LAZY);
		if (!dl) {
			ods("dlopen() failed: %s", dlerror());
			continue;
		}

		const struct link_map *lm = dl;
#endif
		bool hashTableGNU    = false;
		uintptr_t hashTable  = 0;
		const char *strTable = NULL;
		Elf_Sym *symTable    = NULL;
#if defined(__GLIBC__)
		const uintptr_t base = 0;
#else
		const uintptr_t base      = (uintptr_t) lm->l_addr;
#endif
		for (const Elf_Dyn *dyn = lm->l_ld; dyn; ++dyn) {
			switch (dyn->d_tag) {
				case DT_GNU_HASH:
					if (!hashTable) {
						hashTable    = base + dyn->d_un.d_ptr;
						hashTableGNU = true;
					}
					break;
				case DT_HASH:
					if (!hashTable) {
						hashTable = base + dyn->d_un.d_ptr;
					}
					break;
				case DT_STRTAB:
					strTable = (const char *) (base + dyn->d_un.d_ptr);
					break;
				case DT_SYMTAB:
					symTable = (Elf_Sym *) (base + dyn->d_un.d_ptr);
					break;
			}

			if (hashTable && strTable && symTable) {
				break;
			}
		}

		ods("hashTable: 0x%" PRIxPTR ", strTable: %p, symTable: %p", hashTable, strTable, symTable);

		if (!hashTable || !strTable || !symTable) {
			continue;
		}

		if (!hashTableGNU) {
			ods("Using DT_HASH");
			// Hash table pseudo-struct:
			// uint32_t nBucket;
			// uint32_t nChain;
			// uint32_t bucket[nBucket];
			// uint32_t chain[nChain];
			const uint32_t nChain = ((uint32_t *) hashTable)[1];

			for (uint32_t i = 0; i < nChain; ++i) {
				if (ELF_ST_TYPE(symTable[i].st_info) != STT_FUNC) {
					continue;
				}

				if (strcmp(strTable + symTable[i].st_name, "dlsym") == 0) {
					odlsym = (void *) lm->l_addr + symTable[i].st_value;
					break;
				}
			}
		} else {
			ods("Using DT_GNU_HASH");
			// Hash table pseudo-struct:
			// uint32_t  nBucket;
			// uint32_t  symOffset;
			// uint32_t  nBloom;
			// uint32_t  bloomShift;
			// uintptr_t blooms[nBloom];
			// uint32_t  buckets[nBucket];
			// uint32_t  chain[];
			uint32_t *hashStruct = (uint32_t *) hashTable;

			const uint32_t nBucket   = hashStruct[0];
			const uint32_t symOffset = hashStruct[1];
			const uint32_t nBloom    = hashStruct[2];
			const uintptr_t *bloom   = (uintptr_t *) &hashStruct[4];
			const uint32_t *buckets  = (uint32_t *) &bloom[nBloom];
			const uint32_t *chain    = &buckets[nBucket];

			for (uint32_t i = 0; i < nBucket; ++i) {
				uint32_t symIndex = buckets[i];
				if (symIndex < symOffset) {
					continue;
				}

				do {
					if (strcmp(strTable + symTable[symIndex].st_name, "dlsym") == 0) {
						odlsym = (void *) lm->l_addr + symTable[symIndex].st_value;
					}
				} while (!odlsym && !(chain[symIndex++ - symOffset] & 1));
			}
		}
	}

	if (!odlsym) {
		return -1;
	}

	ods("Original dlsym at %p", odlsym);
	return 0;
}

__attribute__((constructor)) static void initializeLibrary() {
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
