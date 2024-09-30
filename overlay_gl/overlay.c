// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define GLX_GLXEXT_LEGACY
#define GL_GLEXT_PROTOTYPES
#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <pwd.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#if defined(TARGET_UNIX)
#	define GLX_GLXEXT_LEGACY
#	define GL_GLEXT_PROTOTYPES
#	define _GNU_SOURCE
#	include <GL/gl.h>
#	include <GL/glext.h>
#	include <GL/glx.h>

#	include <link.h>

typedef unsigned char bool;
#	define true 1
#	define false 0
#elif defined(TARGET_MAC)
#	include <AGL/agl.h>
#	include <Carbon/Carbon.h>
#	include <Cocoa/Cocoa.h>
#	include <OpenGL/OpenGL.h>
#
#	include <objc/objc-runtime.h>
#
#	include "mach_override.h"
#
#	include "avail_mac.h"
#endif

#include "../overlay/overlay.h"

static bool bDebug       = false;
static bool bCursorAvail = false;

typedef struct _Context {
	struct _Context *next;

#if defined(TARGET_UNIX)
	Display *dpy;
	GLXDrawable draw;
#elif defined(TARGET_MAC)
	CGLContextObj cglctx;
	NSOpenGLContext *nsctx;
#endif

	unsigned int uiWidth, uiHeight;
	unsigned int uiLeft, uiRight, uiTop, uiBottom;

	struct sockaddr_un saName;
	int iSocket;
	// overlay message, temporary variable for processing from socket
	struct OverlayMsg omMsg;
	// opengl overlay texture
	GLuint texture;

	// overlay texture in shared memory
	unsigned char *a_ucTexture;
	unsigned int uiMappedLength;

	bool bValid;
	bool bMesa;

	GLuint uiProgram;

	clock_t timeT;
	unsigned int frameCount;

	GLint maxVertexAttribs;
	GLboolean *vertexAttribStates;
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

const GLfloat fBorder[] = { 0.125f, 0.250f, 0.5f, 0.75f };

static Context *contexts = NULL;

#define AVAIL(name) dlsym(RTLD_DEFAULT, #name)
#define FDEF(name) static __typeof__(&name) o##name = NULL

#if defined(TARGET_UNIX)
FDEF(dlsym);
FDEF(glXSwapBuffers);
FDEF(glXGetProcAddressARB);
FDEF(glXGetProcAddress);
#elif defined(TARGET_MAC)
FDEF(CGLFlushDrawable);
FDEF(CGDisplayHideCursor);
FDEF(CGDisplayShowCursor);
#endif

__attribute__((format(printf, 1, 2))) static void ods(const char *format, ...) {
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

static void newContext(Context *ctx) {
	ctx->iSocket           = -1;
	ctx->omMsg.omh.iLength = -1;
	ctx->texture           = ~0U;
	ctx->timeT             = clock();
	ctx->frameCount        = 0;

	char *home = getenv("HOME");
	if (home == NULL) {
		struct passwd *pwent = getpwuid(getuid());
		if (pwent && pwent->pw_dir && pwent->pw_dir[0]) {
			home = pwent->pw_dir;
		}
	}

	char *xdgRuntimeDir = getenv("XDG_RUNTIME_DIR");

	if (xdgRuntimeDir != NULL) {
		ctx->saName.sun_family = PF_UNIX;
		strcpy(ctx->saName.sun_path, xdgRuntimeDir);
		strcat(ctx->saName.sun_path, "/MumbleOverlayPipe");
	} else if (home) {
		ctx->saName.sun_family = PF_UNIX;
		strcpy(ctx->saName.sun_path, home);
		strcat(ctx->saName.sun_path, "/.MumbleOverlayPipe");
	}

	ods("OpenGL Version %s, Vendor %s, Renderer %s, Shader %s", glGetString(GL_VERSION), glGetString(GL_VENDOR),
		glGetString(GL_RENDERER), glGetString(GL_SHADING_LANGUAGE_VERSION));

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

	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &ctx->maxVertexAttribs);
	ctx->vertexAttribStates = calloc((size_t) ctx->maxVertexAttribs, sizeof(GLboolean));
}

static void releaseMem(Context *ctx) {
	if (ctx->a_ucTexture) {
		munmap(ctx->a_ucTexture, ctx->uiMappedLength);
		ctx->a_ucTexture    = NULL;
		ctx->uiMappedLength = 0;
	}
	if (ctx->texture != ~0U) {
		glDeleteTextures(1, &ctx->texture);
		ctx->texture = ~0U;
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
		size_t wantsend = sizeof(struct OverlayMsgHeader) + (size_t) om->omh.iLength;
		ssize_t sent    = send(ctx->iSocket, om, wantsend, MSG_DONTWAIT);
		if (sent != -1 && wantsend == (size_t) sent) {
			return true;
		}
		ods("Short write. Disconnecting pipe.");
	}
	disconnect(ctx);
	return false;
}

static void regenTexture(Context *ctx) {
	if (ctx->texture != ~0U) {
		glDeleteTextures(1, &ctx->texture);
	}
	glGenTextures(1, &ctx->texture);

	glBindTexture(GL_TEXTURE_2D, ctx->texture);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, fBorder);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) ctx->uiWidth, (GLsizei) ctx->uiHeight, 0, GL_BGRA,
				 GL_UNSIGNED_BYTE, ctx->a_ucTexture);
}

static void drawOverlay(Context *ctx, unsigned int width, unsigned int height) {
	// if no socket is active, initialize and connect to socket
	if (ctx->iSocket == -1) {
		releaseMem(ctx);
		if (!ctx->saName.sun_path[0])
			return;
		ctx->iSocket = socket(AF_UNIX, SOCK_STREAM, 0);
		if (ctx->iSocket == -1) {
			ods("socket() failure");
			return;
		}
		fcntl(ctx->iSocket, F_SETFL, O_NONBLOCK, 1);
		if (connect(ctx->iSocket, (struct sockaddr *) (&ctx->saName), sizeof(ctx->saName)) != 0) {
			close(ctx->iSocket);
			ctx->iSocket = -1;
			ods("connect() failure %s", ctx->saName.sun_path);
			return;
		}
		ods("Socket connected");

		struct OverlayMsg om;
		om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType  = OVERLAY_MSGTYPE_PID;
		om.omh.iLength = sizeof(struct OverlayMsgPid);
		om.omp.pid     = (unsigned int) getpid(); // getpid can't fail

		if (!sendMessage(ctx, &om))
			return;

		ods("SentPid");
	}

	// if overlay size (width or height) is not up-to-date create and send an overlay initialization message
	if ((ctx->uiWidth != width) || (ctx->uiHeight != height)) {
		ods("Sending init overlay msg with w h %i %i", width, height);
		releaseMem(ctx);

		ctx->uiWidth  = width;
		ctx->uiHeight = height;

		struct OverlayMsg om;
		om.omh.uiMagic  = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType   = OVERLAY_MSGTYPE_INIT;
		om.omh.iLength  = sizeof(struct OverlayMsgInit);
		om.omi.uiWidth  = ctx->uiWidth;
		om.omi.uiHeight = ctx->uiHeight;

		if (!sendMessage(ctx, &om))
			return;
	}

	// receive and process overlay messages
	while (1) {
		if (ctx->omMsg.omh.iLength < 0) {
			// receive the overlay message header
			ssize_t length = recv(ctx->iSocket, ctx->omMsg.headerbuffer, sizeof(struct OverlayMsgHeader), 0);
			if (length < 0) {
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
					break;
				disconnect(ctx);
				return;
			} else if (length != sizeof(struct OverlayMsgHeader)) {
				ods("Short header read on overlay message");
				disconnect(ctx);
				return;
			}
		} else {
			// receive the overlay message body
			ssize_t length = recv(ctx->iSocket, ctx->omMsg.msgbuffer, (size_t) ctx->omMsg.omh.iLength, 0);
			if (length < 0) {
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
					break;
				disconnect(ctx);
				return;
			} else if (length != ctx->omMsg.omh.iLength) {
				ods("Short overlay message read %x %zd/%d", ctx->omMsg.omh.uiType, length, ctx->omMsg.omh.iLength);
				disconnect(ctx);
				return;
			}
			// set len to -1 again for a clean state on next receive
			ctx->omMsg.omh.iLength = -1;

			switch (ctx->omMsg.omh.uiType) {
				// shared memory overlay message:
				case OVERLAY_MSGTYPE_SHMEM: {
					struct OverlayMsgShmem *oms = (struct OverlayMsgShmem *) &ctx->omMsg.omi;
					ods("SHMEM %s", oms->a_cName);
					releaseMem(ctx);
					int fd = shm_open(oms->a_cName, O_RDONLY, 0600);
					if (fd != -1) {
						struct stat buf;

						if (fstat(fd, &buf) != -1) {
							unsigned int buflen = buf.st_size;
							if (buflen >= ctx->uiWidth * ctx->uiHeight * 4 && buflen < 512 * 1024 * 1024) {
								ctx->uiMappedLength = buflen;
								ctx->a_ucTexture    = mmap(NULL, (size_t) buflen, PROT_READ, MAP_SHARED, fd, 0);
								if (ctx->a_ucTexture != MAP_FAILED) {
									// mmap successfull; send a new bodyless sharedmemory overlay message and regenerate
									// the overlay texture
									struct OverlayMsg om;
									om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
									om.omh.uiType  = OVERLAY_MSGTYPE_SHMEM;
									om.omh.iLength = 0;

									if (!sendMessage(ctx, &om))
										return;

									regenTexture(ctx);
									continue;
								}
								ctx->a_ucTexture = NULL;
							}
							ctx->uiMappedLength = 0;
						} else {
							ods("Failed to fstat memory map");
						}
						close(fd);
					}
					ods("Failed to map memory");
				} break;
				// blit overlay message: blit overlay texture from shared memory to gl-texture var
				case OVERLAY_MSGTYPE_BLIT: {
					struct OverlayMsgBlit *omb = &ctx->omMsg.omb;
					ods("BLIT %d %d %d %d", omb->x, omb->y, omb->w, omb->h);
					if ((ctx->a_ucTexture != NULL) && (ctx->texture != ~0U)) {
						glBindTexture(GL_TEXTURE_2D, ctx->texture);

						if ((omb->x == 0) && (omb->y == 0) && (omb->w == ctx->uiWidth) && (omb->h == ctx->uiHeight)) {
							ods("Optimzied fullscreen blit");
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) ctx->uiWidth, (GLsizei) ctx->uiHeight, 0,
										 GL_BGRA, GL_UNSIGNED_BYTE, ctx->a_ucTexture);
						} else {
							// allocate temporary memory
							unsigned int x     = omb->x;
							unsigned int y     = omb->y;
							unsigned int w     = omb->w;
							unsigned int h     = omb->h;
							unsigned char *ptr = (unsigned char *) malloc(w * h * 4);
							unsigned int row;
							memset(ptr, 0, w * h * 4);

							// copy overlay texture to temporary memory to adapt to full opengl ui size (overlay at
							// correct place)
							for (row = 0; row < h; ++row) {
								const unsigned char *sptr = ctx->a_ucTexture + 4 * ((y + row) * ctx->uiWidth + x);
								unsigned char *dptr       = ptr + 4 * w * row;
								memcpy(dptr, sptr, w * 4);
							}

							// copy temporary texture to opengl
							glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint) x, (GLint) y, (GLint) w, (GLint) h, GL_BGRA,
											GL_UNSIGNED_BYTE, ptr);
							free(ptr);
						}
					}
				} break;
				case OVERLAY_MSGTYPE_ACTIVE: {
					struct OverlayMsgActive *oma = &ctx->omMsg.oma;
					ods("ACTIVE %d %d %d %d", oma->x, oma->y, oma->w, oma->h);
					ctx->uiLeft   = oma->x;
					ctx->uiTop    = oma->y;
					ctx->uiRight  = oma->x + oma->w;
					ctx->uiBottom = oma->y + oma->h;
				} break;
				case OVERLAY_MSGTYPE_INTERACTIVE: {
#if defined(TARGET_MAC)
					struct OverlayMsgInteractive *omin = &ctx->omMsg.omin;
					ods("Interactive %d", omin->state);
					if (bCursorAvail) {
						if (omin->state) {
							oCGDisplayHideCursor(kCGNullDirectDisplay);
						} else {
							oCGDisplayShowCursor(kCGNullDirectDisplay);
						}
					}
#endif
				} break;
				default:
					break;
			}
		}
	}

	if ((ctx->a_ucTexture == NULL) || (ctx->texture == ~0U))
		return;

	// texture checks, that our gltexture is still valid and sane
	if (!glIsTexture(ctx->texture)) {
		ctx->texture = ~0U;
		ods("Lost texture");
		regenTexture(ctx);
	} else {
		glBindTexture(GL_TEXTURE_2D, ctx->texture);
		GLfloat bordercolor[4];
		glGetTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);
		if (bordercolor[0] != fBorder[0] || bordercolor[1] != fBorder[1] || bordercolor[2] != fBorder[2]
			|| bordercolor[3] != fBorder[3]) {
			ods("Texture was hijacked! Texture will be regenerated.");
			regenTexture(ctx);
		}
	}

	glBindTexture(GL_TEXTURE_2D, ctx->texture);
	glPushMatrix();

	float w = (float) (ctx->uiWidth);
	float h = (float) (ctx->uiHeight);

	float left   = (float) (ctx->uiLeft);
	float top    = (float) (ctx->uiTop);
	float right  = (float) (ctx->uiRight);
	float bottom = (float) (ctx->uiBottom);

	float xm  = left / w;
	float ym  = top / h;
	float xmx = right / w;
	float ymx = bottom / h;

	GLfloat vertex[] = { left, bottom, left,  top, right, top,

						 left, bottom, right, top, right, bottom };
	glVertexPointer(2, GL_FLOAT, 0, vertex);

	GLfloat tex[] = { xm, ymx, xm,  ym, xmx, ym,

					  xm, ymx, xmx, ym, xmx, ymx };
	glTexCoordPointer(2, GL_FLOAT, 0, tex);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glPopMatrix();
}

static void drawContext(Context *ctx, int width, int height) {
	// calculate FPS and send it as an overlay message
	clock_t t     = clock();
	float elapsed = (float) (t - ctx->timeT) / CLOCKS_PER_SEC;
	++(ctx->frameCount);
	if (elapsed > OVERLAY_FPS_INTERVAL) {
		struct OverlayMsg om;
		om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType  = OVERLAY_MSGTYPE_FPS;
		om.omh.iLength = sizeof(struct OverlayMsgFps);
		om.omf.fps     = (float) ctx->frameCount / elapsed;

		sendMessage(ctx, &om);

		ctx->frameCount = 0;
		ctx->timeT      = t;
	}

	GLuint program;
	GLint viewport[4];
	int i;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_ALL_ATTRIB_BITS);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *) &program);

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

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

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

	for (i = texunits - 1; i >= 0; --i) {
		glActiveTexture(GL_TEXTURE0 + (GLenum) i);
		glDisable(GL_TEXTURE_1D);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_3D);
	}

	glDisable(GL_TEXTURE_CUBE_MAP);
	glDisable(GL_VERTEX_PROGRAM_ARB);
	glDisable(GL_FRAGMENT_PROGRAM_ARB);

	GLint enabled;
	for (i = 0; i < ctx->maxVertexAttribs; ++i) {
		enabled = GL_FALSE;
		glGetVertexAttribiv((GLuint) i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
		if (enabled == GL_TRUE) {
			glDisableVertexAttribArray((GLuint) i);
			ctx->vertexAttribStates[i] = GL_TRUE;
		}
	}

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

	GLuint bound = 0, vbobound = 0;
	glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, (GLint *) &bound);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint *) &vbobound);

	if (bound != 0) {
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	}
	if (vbobound != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	drawOverlay(ctx, (unsigned int) width, (unsigned int) height);

	if (bound != 0) {
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, bound);
	}
	if (vbobound != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, vbobound);
	}

	for (i = 0; i < ctx->maxVertexAttribs; ++i) {
		if (ctx->vertexAttribStates[i] == GL_TRUE) {
			glEnableVertexAttribArray((GLuint) i);
			ctx->vertexAttribStates[i] = GL_FALSE;
		}
	}

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

	// drain opengl error queue
	while (glGetError() != GL_NO_ERROR)
		;
}

#if defined(TARGET_UNIX)
#	include "init_unix.c"
#elif defined(TARGET_MAC)
#	include "init_mac.c"
#endif
