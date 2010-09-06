/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdarg.h>
#include <pwd.h>
#include <math.h>
#include <errno.h>
#include <time.h>

typedef unsigned char bool;
#define true 1
#define false 0

#include "../overlay/overlay.h"

// Prototypes
static void ods(const char *format, ...);

static bool bDebug;

typedef struct _Context {
	struct _Context *next;
	Display *dpy;
	GLXDrawable draw;

	unsigned int uiWidth, uiHeight;
	unsigned int uiLeft, uiRight, uiTop, uiBottom;

	struct sockaddr_un saName;
	int iSocket;
	struct OverlayMsg omMsg;
	GLuint texture;

	unsigned char *a_ucTexture;
	unsigned int uiMappedLength;

	bool bValid;
	bool bGlx;

	GLuint uiProgram;

	clock_t timeT;
	unsigned int frameCount;
} Context;

static const char vshader[] = ""
                              "void main() {"
                              "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
                              "gl_TexCoord[0] = gl_MultiTexCoord0;"
                              "}";

static const char fshader[] = ""
                              "uniform sampler2D tex;"
                              "void main() {"
			      "gl_FragColor = texture2D(tex, gl_TexCoord[0].st);"
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

	if (! oglXSwapBuffers) {
		void *lib = dlopen("libGL.so.1", RTLD_GLOBAL | RTLD_NOLOAD);
		if (! lib)
			return;
		RESOLVE(glXSwapBuffers);
		if (! oglXSwapBuffers)
			dlclose(lib);
	}

	RESOLVE(glXGetProcAddressARB);
	RESOLVE(glXGetProcAddress);
}

__attribute__((format(printf, 1, 2)))
static void ods(const char *format, ...) {
	if (!bDebug) {
		return;
	}

	fprintf(stderr, "MumbleOverlay: ");

	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fprintf(stderr, "\n");
	fflush(stderr);
}

static void newContext(Context * ctx) {
	ctx->iSocket = -1;
	ctx->omMsg.omh.iLength = -1;
	ctx->texture = ~0;
	ctx->timeT = clock();
	ctx->frameCount = 0;

	char *home = getenv("HOME");
	if (home == NULL) {
		struct passwd *pwent= getpwuid(getuid());
		if (pwent && pwent->pw_dir && pwent->pw_dir[0])
			home = pwent->pw_dir;
	}

	if (home) {
		ctx->saName.sun_family = PF_UNIX;
		strcpy(ctx->saName.sun_path, home);
		strcat(ctx->saName.sun_path, "/.MumbleOverlayPipe");
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

static void releaseMem(Context *ctx) {
	if (ctx->a_ucTexture) {
		munmap(ctx->a_ucTexture, ctx->uiMappedLength);
		ctx->a_ucTexture = NULL;
		ctx->uiMappedLength = 0;
	}
	if (ctx->texture != ~0) {
		glDeleteTextures(1, &ctx->texture);
		ctx->texture = ~0;
	}
	ctx->uiLeft = ctx->uiTop = ctx->uiRight = ctx->uiBottom = 0;
}

static void disconnect(Context *ctx) {
	releaseMem(ctx);
	ctx->uiWidth = ctx->uiHeight = 0;
	if (ctx->iSocket != -1) {
		close(ctx->iSocket);
		ctx->iSocket = -1;
	}
	ods("Disconnected");
}

static bool sendMessage(Context *ctx, struct OverlayMsg *om) {
	if (ctx->iSocket != -1) {
		ssize_t wantsend = sizeof(struct OverlayMsgHeader) + om->omh.iLength;
		ssize_t sent = send(ctx->iSocket, om, wantsend, MSG_DONTWAIT);
		if (wantsend == sent)
			return true;
		ods("Short write");
	}
	disconnect(ctx);
	return false;
}

static void regenTexture(Context *ctx) {
	if (ctx->texture != ~0)
		glDeleteTextures(1, & ctx->texture);
	glGenTextures(1, &ctx->texture);

	glBindTexture(GL_TEXTURE_2D, ctx->texture);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, fBorder);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ctx->uiWidth, ctx->uiHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, ctx->a_ucTexture);
}

static void drawOverlay(Context *ctx, unsigned int width, unsigned int height) {
	if (ctx->iSocket == -1) {
		releaseMem(ctx);
		if (! ctx->saName.sun_path[0])
			return;
		ctx->iSocket = socket(AF_UNIX, SOCK_STREAM, 0);
		if (ctx->iSocket == -1) {
			ods("socket() failure");
			return;
		}
		fcntl(ctx->iSocket, F_SETFL, O_NONBLOCK, 1);
		if (connect(ctx->iSocket, (struct sockaddr *)(& ctx->saName), sizeof(ctx->saName)) != 0) {
			close(ctx->iSocket);
			ctx->iSocket = -1;
			ods("connect() failure %s", ctx->saName.sun_path);
			return;
		}
		ods("Connected");
	}

	if ((ctx->uiWidth != width) || (ctx->uiHeight != height)) {
		ods("Sent init %i %i", width, height);
		releaseMem(ctx);

		ctx->uiWidth = width;
		ctx->uiHeight = height;

		struct OverlayMsg om;
		om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType = OVERLAY_MSGTYPE_INIT;
		om.omh.iLength = sizeof(struct OverlayMsgInit);
		om.omi.uiWidth = ctx->uiWidth;
		om.omi.uiHeight = ctx->uiHeight;

		if (! sendMessage(ctx, &om))
			return;
	}

	while (1) {
		if (ctx->omMsg.omh.iLength == -1) {
			ssize_t length = recv(ctx->iSocket, ctx->omMsg.headerbuffer, sizeof(struct OverlayMsgHeader), 0);
			if (length < 0) {
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
					break;
				disconnect(ctx);
				return;
			} else if (length != sizeof(struct OverlayMsgHeader)) {
				ods("Short header read");
				disconnect(ctx);
				return;
			}
		} else {
			ssize_t  length = recv(ctx->iSocket, ctx->omMsg.msgbuffer, ctx->omMsg.omh.iLength, 0);
			if (length < 0) {
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
					break;
				disconnect(ctx);
				return;
			} else if (length != ctx->omMsg.omh.iLength) {
				ods("Short message read %x %d/%d", ctx->omMsg.omh.uiType, length, ctx->omMsg.omh.iLength);
				disconnect(ctx);
				return;
			}
			ctx->omMsg.omh.iLength = -1;

			switch (ctx->omMsg.omh.uiType) {
				case OVERLAY_MSGTYPE_SHMEM: {
						struct OverlayMsgShmem *oms = & ctx->omMsg.omi;
						ods("SHMEM %s", oms->a_cName);
						releaseMem(ctx);
						int fd = shm_open(oms->a_cName, O_RDONLY, 0600);
						if (fd != -1) {
							struct stat buf;
							fstat(fd, &buf);
							if (buf.st_size >= ctx->uiWidth * ctx->uiHeight * 4) {
								ctx->uiMappedLength = buf.st_size;
								ctx->a_ucTexture = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
								if (ctx->a_ucTexture != (unsigned char *) -1) {
									struct OverlayMsg om;
									om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
									om.omh.uiType = OVERLAY_MSGTYPE_SHMEM;
									om.omh.iLength = 0;

									if (! sendMessage(ctx, &om))
										return;

									regenTexture(ctx);
									continue;
								}
								ctx->a_ucTexture = NULL;
							}
							ctx->uiMappedLength = 0;
							close(fd);
						}
						ods("Failed to map memory");
					}
					break;
				case OVERLAY_MSGTYPE_BLIT: {
						struct OverlayMsgBlit *omb = & ctx->omMsg.omb;
						ods("BLIT %d %d %d %d", omb->x, omb->y, omb->w, omb->h);
						if ((ctx->a_ucTexture != NULL) && (ctx->texture != ~0)) {
							glBindTexture(GL_TEXTURE_2D, ctx->texture);

							if ((omb->x == 0) && (omb->y == 0) && (omb->w == ctx->uiWidth) && (omb->h == ctx->uiHeight)) {
								ods("Optimzied fullscreen blit");
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ctx->uiWidth, ctx->uiHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, ctx->a_ucTexture);
							} else {
								unsigned int x = omb->x;
								unsigned int y = omb->y;
								unsigned int w = omb->w;
								unsigned int h = omb->h;
								unsigned char *ptr = (unsigned char *) malloc(w*h*4);
								int r;
								memset(ptr, 0, w * h * 4);

								for (r = 0; r < h; ++r) {
									const unsigned char *sptr = ctx->a_ucTexture + 4 * ((y+r) * ctx->uiWidth + x);
									unsigned char *dptr = ptr + 4 * w * r;
									memcpy(dptr, sptr, w * 4);
								}

								glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_BGRA, GL_UNSIGNED_BYTE, ptr);
								free(ptr);
							}
						}
					}
					break;
				case OVERLAY_MSGTYPE_ACTIVE: {
						struct OverlayMsgActive *oma = & ctx->omMsg.oma;
						ods("ACTIVE %d %d %d %d", oma->x, oma->y, oma->w, oma->h);
						ctx->uiLeft = oma->x;
						ctx->uiTop = oma->y;
						ctx->uiRight = oma->x + oma->w;
						ctx->uiBottom = oma->y + oma->h;
					}
					break;
				default:
					break;
			}
		}
	}

	if ((ctx->a_ucTexture == NULL) || (ctx->texture == ~0))
		return;

	if (! glIsTexture(ctx->texture)) {
		ctx->texture = ~0;
		ods("Lost texture");
		regenTexture(ctx);
	} else {
		glBindTexture(GL_TEXTURE_2D, ctx->texture);
		GLfloat bordercolor[4];
		glGetTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);
		if (bordercolor[0] != fBorder[0] || bordercolor[1] != fBorder[1] || bordercolor[2] != fBorder[2] || bordercolor[3] != fBorder[3]) {
			ods("Texture hijacked");
			regenTexture(ctx);
		}
	}

	glBindTexture(GL_TEXTURE_2D, ctx->texture);
	glPushMatrix();

	float w = (float)(ctx->uiWidth);
	float h = (float)(ctx->uiHeight);

	float left   = (float)(ctx->uiLeft);
	float top    = (float)(ctx->uiTop);
	float right  = (float)(ctx->uiRight);
	float bottom = (float)(ctx->uiBottom);

	float xm = (left) / w;
	float ym = (top) / h;
	float xmx = (right) / w;
	float ymx = (bottom) / h;


	GLfloat vertex[] = {left, bottom,
			    left, top,
			    right, top,
			    right, bottom};
	GLfloat tex[] = {xm, ymx, xm, ym, xmx, ym, xmx, ymx};
	glVertexPointer(2, GL_FLOAT, 0, vertex);
	glTexCoordPointer(2, GL_FLOAT, 0, tex);
	glDrawArrays(GL_QUADS, 0, 4);

	glPopMatrix();
}

static void drawContext(Context * ctx, int width, int height) {
	clock_t t = clock();
	float elapsed = (float)(t - ctx->timeT) / CLOCKS_PER_SEC;
	++(ctx->frameCount);
	if (elapsed > OVERLAY_FPS_INTERVAL) {
		struct OverlayMsg om;
		om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType = OVERLAY_MSGTYPE_FPS;
		om.omh.iLength = sizeof(struct OverlayMsgFps);
		om.omf.fps = ctx->frameCount / elapsed;

		sendMessage(ctx, &om);

		ctx->frameCount = 0;
		ctx->timeT = t;
	}

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

	GLboolean b = 0;
	glGetBooleanv(GL_TEXTURE_GEN_Q, &b);
	if (b)
		glDisable(GL_TEXTURE_GEN_Q);
	glGetBooleanv(GL_TEXTURE_GEN_R, &b);
	if (b)
		glDisable(GL_TEXTURE_GEN_R);
	glGetBooleanv(GL_TEXTURE_GEN_S, &b);
	if (b)
		glDisable(GL_TEXTURE_GEN_S);
	glGetBooleanv(GL_TEXTURE_GEN_T, &b);
	if (b)
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
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glMatrixMode(GL_MODELVIEW);

	GLint uni = glGetUniformLocation(ctx->uiProgram, "tex");
	glUniform1i(uni, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	GLint bound = 0, vbobound = 0;
	glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &bound);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbobound);

	if (bound != 0)
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	if (vbobound != 0)
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	drawOverlay(ctx, width, height);
	
	if (bound != 0)
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, bound);
	if (vbobound != 0)
		glBindBuffer(GL_ARRAY_BUFFER, vbobound);

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
	
	while (glGetError() != GL_NO_ERROR);
}

__attribute__((visibility("default")))
void glXSwapBuffers(Display * dpy, GLXDrawable draw) {
	if (!oglXSwapBuffers)
		resolveOpenGL();

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

#define OGRAB(name) if (handle == RTLD_DEFAULT) handle = RTLD_NEXT; symbol = odlsym(handle, #name); if (symbol) { o##name = (__typeof__(&name)) symbol; symbol = (void *) name;}
__attribute__((visibility("default")))
void *dlsym(void *handle, const char *name) {
	if (!odlsym)
		initializeLibrary();

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
