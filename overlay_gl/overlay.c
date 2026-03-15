// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <pwd.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdbool.h>
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
	GLuint vao;
	GLuint vbo;
	GLint locMvp;
	GLint locTex;

	clock_t timeT;
	unsigned int frameCount;
} Context;

static const char vshader[] =
	"#version 330\n"
	"layout(location = 0) in vec2 position;\n"
	"layout(location = 1) in vec2 texcoord;\n"
	"uniform mat4 mvp;\n"
	"out vec2 v_texcoord;\n"
	"void main() {\n"
	"    gl_Position = mvp * vec4(position, 0.0, 1.0);\n"
	"    v_texcoord = texcoord;\n"
	"}\n";

static const char fshader[] =
	"#version 330\n"
	"uniform sampler2D tex;\n"
	"in vec2 v_texcoord;\n"
	"layout(location = 0) out vec4 fragColor;\n"
	"void main() {\n"
	"    fragColor = texture(tex, v_texcoord);\n"
	"}\n";

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

// Function pointers for all GL functions used by the overlay.
// As an LD_PRELOAD library, we cannot rely on any GL symbols being
// available through normal dynamic linking. All must be resolved
// via glXGetProcAddress at runtime.
#if defined(TARGET_UNIX)
FDEF(glXGetCurrentContext);
FDEF(glXQueryDrawable);
FDEF(glXQueryVersion);

FDEF(glActiveTexture);
FDEF(glAttachShader);
FDEF(glBindBuffer);
FDEF(glBindTexture);
FDEF(glBindVertexArray);
FDEF(glBlendFunc);
FDEF(glBlendFuncSeparate);
FDEF(glBufferData);
FDEF(glBufferSubData);
FDEF(glCompileShader);
FDEF(glCreateProgram);
FDEF(glCreateShader);
FDEF(glDeleteProgram);
FDEF(glDeleteShader);
FDEF(glDeleteTextures);
FDEF(glDisable);
FDEF(glDrawArrays);
FDEF(glEnable);
FDEF(glEnableVertexAttribArray);
FDEF(glGenBuffers);
FDEF(glGenTextures);
FDEF(glGenVertexArrays);
FDEF(glGetError);
FDEF(glGetIntegerv);
FDEF(glGetProgramInfoLog);
FDEF(glGetProgramiv);
FDEF(glGetShaderInfoLog);
FDEF(glGetShaderiv);
FDEF(glGetString);
FDEF(glGetTexParameterfv);
FDEF(glGetUniformLocation);
FDEF(glIsEnabled);
FDEF(glIsTexture);
FDEF(glLinkProgram);
FDEF(glPixelStorei);
FDEF(glShaderSource);
FDEF(glTexImage2D);
FDEF(glTexParameterfv);
FDEF(glTexParameteri);
FDEF(glTexSubImage2D);
FDEF(glUniform1i);
FDEF(glUniformMatrix4fv);
FDEF(glUseProgram);
FDEF(glVertexAttribPointer);
FDEF(glViewport);

FDEF(glBindFramebuffer);
FDEF(glBlendEquation);
FDEF(glBlendEquationSeparate);
FDEF(glColorMask);
FDEF(glGetBooleanv);
FDEF(glPolygonMode);

static bool resolveFailed = false;

static void resolveGLFunctions(void) {
	static bool resolved = false;
	if (resolved) {
		return;
	}
	resolved = true;

#	define GLRESOLVE(name)                                                                    \
		do {                                                                                  \
			if (oglXGetProcAddressARB) {                                                      \
				o##name = (__typeof__(o##name)) oglXGetProcAddressARB((const GLubyte *) #name); \
			}                                                                                 \
			if (!o##name && oglXGetProcAddress) {                                              \
				o##name = (__typeof__(o##name)) oglXGetProcAddress((const GLubyte *) #name);   \
			}                                                                                 \
			if (!o##name && odlsym) {                                                         \
				o##name = (__typeof__(o##name)) odlsym(RTLD_DEFAULT, #name);                  \
			}                                                                                 \
		} while (0)

	GLRESOLVE(glXGetCurrentContext);
	GLRESOLVE(glXQueryDrawable);
	GLRESOLVE(glXQueryVersion);

	GLRESOLVE(glActiveTexture);
	GLRESOLVE(glAttachShader);
	GLRESOLVE(glBindBuffer);
	GLRESOLVE(glBindTexture);
	GLRESOLVE(glBindVertexArray);
	GLRESOLVE(glBlendFunc);
	GLRESOLVE(glBlendFuncSeparate);
	GLRESOLVE(glBufferData);
	GLRESOLVE(glBufferSubData);
	GLRESOLVE(glCompileShader);
	GLRESOLVE(glCreateProgram);
	GLRESOLVE(glCreateShader);
	GLRESOLVE(glDeleteProgram);
	GLRESOLVE(glDeleteShader);
	GLRESOLVE(glDeleteTextures);
	GLRESOLVE(glDisable);
	GLRESOLVE(glDrawArrays);
	GLRESOLVE(glEnable);
	GLRESOLVE(glEnableVertexAttribArray);
	GLRESOLVE(glGenBuffers);
	GLRESOLVE(glGenTextures);
	GLRESOLVE(glGenVertexArrays);
	GLRESOLVE(glGetError);
	GLRESOLVE(glGetIntegerv);
	GLRESOLVE(glGetProgramInfoLog);
	GLRESOLVE(glGetProgramiv);
	GLRESOLVE(glGetShaderInfoLog);
	GLRESOLVE(glGetShaderiv);
	GLRESOLVE(glGetString);
	GLRESOLVE(glGetTexParameterfv);
	GLRESOLVE(glGetUniformLocation);
	GLRESOLVE(glIsEnabled);
	GLRESOLVE(glIsTexture);
	GLRESOLVE(glLinkProgram);
	GLRESOLVE(glPixelStorei);
	GLRESOLVE(glShaderSource);
	GLRESOLVE(glTexImage2D);
	GLRESOLVE(glTexParameterfv);
	GLRESOLVE(glTexParameteri);
	GLRESOLVE(glTexSubImage2D);
	GLRESOLVE(glUniform1i);
	GLRESOLVE(glUniformMatrix4fv);
	GLRESOLVE(glUseProgram);
	GLRESOLVE(glVertexAttribPointer);
	GLRESOLVE(glViewport);

	GLRESOLVE(glBindFramebuffer);
	GLRESOLVE(glBlendEquation);
	GLRESOLVE(glBlendEquationSeparate);
	GLRESOLVE(glColorMask);
	GLRESOLVE(glGetBooleanv);
	GLRESOLVE(glPolygonMode);

#	undef GLRESOLVE

	if (!oglBindBuffer || !oglBindTexture || !oglBindVertexArray || !oglUseProgram || !oglDrawArrays
		|| !oglEnable || !oglDisable || !oglViewport || !oglGetIntegerv || !oglGetError
		|| !oglGenBuffers || !oglGenTextures || !oglGenVertexArrays
		|| !oglCreateProgram || !oglCreateShader || !oglCompileShader || !oglLinkProgram || !oglAttachShader
		|| !oglBlendEquation || !oglTexImage2D || !oglTexSubImage2D || !oglTexParameteri
		|| !oglGetTexParameterfv || !oglGetUniformLocation || !oglUniform1i || !oglUniformMatrix4fv
		|| !oglGetShaderiv || !oglGetProgramiv) {
		resolveFailed = true;
	}
}

// Redirect direct GL calls through the resolved function pointers.
// This must come after resolveGLFunctions so the resolve code can
// reference the original GL declarations from the headers.
#	define glXGetCurrentContext oglXGetCurrentContext
#	define glXQueryDrawable oglXQueryDrawable
#	define glXQueryVersion oglXQueryVersion

#	define glActiveTexture oglActiveTexture
#	define glAttachShader oglAttachShader
#	define glBindBuffer oglBindBuffer
#	define glBindTexture oglBindTexture
#	define glBindVertexArray oglBindVertexArray
#	define glBlendFunc oglBlendFunc
#	define glBlendFuncSeparate oglBlendFuncSeparate
#	define glBufferData oglBufferData
#	define glBufferSubData oglBufferSubData
#	define glCompileShader oglCompileShader
#	define glCreateProgram oglCreateProgram
#	define glCreateShader oglCreateShader
#	define glDeleteProgram oglDeleteProgram
#	define glDeleteShader oglDeleteShader
#	define glDeleteTextures oglDeleteTextures
#	define glDisable oglDisable
#	define glDrawArrays oglDrawArrays
#	define glEnable oglEnable
#	define glEnableVertexAttribArray oglEnableVertexAttribArray
#	define glGenBuffers oglGenBuffers
#	define glGenTextures oglGenTextures
#	define glGenVertexArrays oglGenVertexArrays
#	define glGetError oglGetError
#	define glGetIntegerv oglGetIntegerv
#	define glGetProgramInfoLog oglGetProgramInfoLog
#	define glGetProgramiv oglGetProgramiv
#	define glGetShaderInfoLog oglGetShaderInfoLog
#	define glGetShaderiv oglGetShaderiv
#	define glGetString oglGetString
#	define glGetTexParameterfv oglGetTexParameterfv
#	define glGetUniformLocation oglGetUniformLocation
#	define glIsEnabled oglIsEnabled
#	define glIsTexture oglIsTexture
#	define glLinkProgram oglLinkProgram
#	define glPixelStorei oglPixelStorei
#	define glShaderSource oglShaderSource
#	define glTexImage2D oglTexImage2D
#	define glTexParameterfv oglTexParameterfv
#	define glTexParameteri oglTexParameteri
#	define glTexSubImage2D oglTexSubImage2D
#	define glUniform1i oglUniform1i
#	define glUniformMatrix4fv oglUniformMatrix4fv
#	define glUseProgram oglUseProgram
#	define glVertexAttribPointer oglVertexAttribPointer
#	define glViewport oglViewport

#	define glBindFramebuffer oglBindFramebuffer
#	define glBlendEquation oglBlendEquation
#	define glBlendEquationSeparate oglBlendEquationSeparate
#	define glColorMask oglColorMask
#	define glGetBooleanv oglGetBooleanv
#	define glPolygonMode oglPolygonMode
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
#if defined(TARGET_UNIX)
	resolveGLFunctions();
#endif
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

	char *xdgRuntimeDir            = getenv("XDG_RUNTIME_DIR");
	const char *overlayPipeXdgDir  = "/MumbleOverlayPipe";
	const char *overlayPipeHomeDir = "/.MumbleOverlayPipe";
	// ctx->saName.sun_path is a statically sized char array, therefore sizeof is correct here
	size_t sunPathBufLen = sizeof(ctx->saName.sun_path) / sizeof(ctx->saName.sun_path[0]);

	if (xdgRuntimeDir != NULL && strlen(xdgRuntimeDir) + strlen(overlayPipeXdgDir) < sunPathBufLen) {
		ctx->saName.sun_family = PF_UNIX;
		strcpy(ctx->saName.sun_path, xdgRuntimeDir);
		strcat(ctx->saName.sun_path, overlayPipeXdgDir);
	} else if (home && strlen(home) + strlen(overlayPipeHomeDir) < sunPathBufLen) {
		ctx->saName.sun_family = PF_UNIX;
		strcpy(ctx->saName.sun_path, home);
		strcat(ctx->saName.sun_path, overlayPipeHomeDir);
	}

	ods("OpenGL Version %s, Vendor %s, Renderer %s, Shader %s", glGetString(GL_VERSION), glGetString(GL_VENDOR),
		glGetString(GL_RENDERER), glGetString(GL_SHADING_LANGUAGE_VERSION));

	const char *vsource = vshader;
	const char *fsource = fshader;
	char buffer[8192];
	GLint status;
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vs, 1, &vsource, NULL);
	glShaderSource(fs, 1, &fsource, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if (!status) {
		glGetShaderInfoLog(vs, 8192, NULL, buffer);
		ods("Vertex shader compile failed: %s", buffer);
		glDeleteShader(vs);
		glDeleteShader(fs);
		return;
	}
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if (!status) {
		glGetShaderInfoLog(fs, 8192, NULL, buffer);
		ods("Fragment shader compile failed: %s", buffer);
		glDeleteShader(vs);
		glDeleteShader(fs);
		return;
	}
	ctx->uiProgram = glCreateProgram();
	glAttachShader(ctx->uiProgram, vs);
	glAttachShader(ctx->uiProgram, fs);
	glLinkProgram(ctx->uiProgram);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glGetProgramiv(ctx->uiProgram, GL_LINK_STATUS, &status);
	if (!status) {
		glGetProgramInfoLog(ctx->uiProgram, 8192, NULL, buffer);
		glDeleteProgram(ctx->uiProgram);
		ctx->uiProgram = 0;
		ods("Shader program link failed: %s", buffer);
		return;
	}

	ctx->locMvp = glGetUniformLocation(ctx->uiProgram, "mvp");
	ctx->locTex = glGetUniformLocation(ctx->uiProgram, "tex");

	glGenVertexArrays(1, &ctx->vao);
	glGenBuffers(1, &ctx->vbo);

	GLint savedVao, savedVbo;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &savedVao);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &savedVbo);

	glBindVertexArray(ctx->vao);
	glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) (2 * sizeof(GLfloat)));

	glBindVertexArray((GLuint) savedVao);
	glBindBuffer(GL_ARRAY_BUFFER, (GLuint) savedVbo);
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
		ctx->iSocket = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
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
					int fd = shm_open(oms->a_cName, O_RDONLY | O_CLOEXEC, 0600);
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
							if (!ptr) {
								ods("malloc failure for blit buffer");
								break;
							}
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

	GLfloat data[] = {
		left,  bottom, xm,  ymx,
		left,  top,    xm,  ym,
		right, top,    xmx, ym,
		left,  bottom, xm,  ymx,
		right, top,    xmx, ym,
		right, bottom, xmx, ymx
	};

	glBindVertexArray(ctx->vao);
	glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data), data);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

static void drawContext(Context *ctx, int width, int height) {
	if (resolveFailed) {
		static bool logged = false;
		if (!logged) {
			ods("OpenGL overlay: failed to resolve required GL functions");
			logged = true;
		}
		return;
	}

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

	GLint savedViewport[4];
	GLint savedProgram, savedVao, savedVbo, savedPbo, savedTex, savedActiveTexture;
	GLint savedUnpackAlignment, savedUnpackRowLength, savedUnpackSkipPixels, savedUnpackSkipRows;
	GLboolean savedBlend, savedDepthTest, savedScissorTest, savedCullFace, savedStencilTest;
	GLboolean savedFramebufferSrgb;
	GLint savedBlendSrcRGB, savedBlendDstRGB, savedBlendSrcAlpha, savedBlendDstAlpha;
	GLint savedBlendEqRGB, savedBlendEqAlpha;
	GLint savedDrawFbo, savedReadFbo;
	GLboolean savedColorMask[4];
	GLint savedPolygonMode[2];

	glGetIntegerv(GL_VIEWPORT, savedViewport);
	glGetIntegerv(GL_CURRENT_PROGRAM, &savedProgram);
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &savedVao);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &savedVbo);
	glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &savedPbo);
	glGetIntegerv(GL_ACTIVE_TEXTURE, &savedActiveTexture);
	glActiveTexture(GL_TEXTURE0);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &savedTex);
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &savedUnpackAlignment);
	glGetIntegerv(GL_UNPACK_ROW_LENGTH, &savedUnpackRowLength);
	glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &savedUnpackSkipPixels);
	glGetIntegerv(GL_UNPACK_SKIP_ROWS, &savedUnpackSkipRows);
	savedBlend          = glIsEnabled(GL_BLEND);
	savedDepthTest      = glIsEnabled(GL_DEPTH_TEST);
	savedScissorTest    = glIsEnabled(GL_SCISSOR_TEST);
	savedCullFace       = glIsEnabled(GL_CULL_FACE);
	savedStencilTest    = glIsEnabled(GL_STENCIL_TEST);
	savedFramebufferSrgb = glIsEnabled(GL_FRAMEBUFFER_SRGB);
	glGetIntegerv(GL_BLEND_SRC_RGB, &savedBlendSrcRGB);
	glGetIntegerv(GL_BLEND_DST_RGB, &savedBlendDstRGB);
	glGetIntegerv(GL_BLEND_SRC_ALPHA, &savedBlendSrcAlpha);
	glGetIntegerv(GL_BLEND_DST_ALPHA, &savedBlendDstAlpha);
	glGetIntegerv(GL_BLEND_EQUATION_RGB, &savedBlendEqRGB);
	glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &savedBlendEqAlpha);
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &savedDrawFbo);
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &savedReadFbo);
	glGetBooleanv(GL_COLOR_WRITEMASK, savedColorMask);
	glGetIntegerv(GL_POLYGON_MODE, savedPolygonMode);

	if (savedDrawFbo != 0) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
	if (savedReadFbo != 0) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}
	glViewport(0, 0, width, height);
	glUseProgram(ctx->uiProgram);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_FRAMEBUFFER_SRGB);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	if (savedPbo != 0) {
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	}

	GLfloat mvp[16];
	memset(mvp, 0, sizeof(mvp));
	mvp[0]  = 2.0f / (float) width;
	mvp[5]  = -2.0f / (float) height;
	mvp[10] = -0.01f;
	mvp[12] = -1.0f;
	mvp[13] = 1.0f;
	mvp[15] = 1.0f;
	glUniformMatrix4fv(ctx->locMvp, 1, GL_FALSE, mvp);
	glUniform1i(ctx->locTex, 0);

	drawOverlay(ctx, (unsigned int) width, (unsigned int) height);

	{
		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			ods("GL error after drawOverlay: 0x%x", err);
		}
	}

	glBindVertexArray((GLuint) savedVao);
	glBindBuffer(GL_ARRAY_BUFFER, (GLuint) savedVbo);
	if (savedPbo != 0) {
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, (GLuint) savedPbo);
	}
	glBindTexture(GL_TEXTURE_2D, (GLuint) savedTex);
	glActiveTexture((GLenum) savedActiveTexture);
	glViewport(savedViewport[0], savedViewport[1], savedViewport[2], savedViewport[3]);
	glUseProgram((GLuint) savedProgram);
	if (savedBlend) { glEnable(GL_BLEND); } else { glDisable(GL_BLEND); }
	if (savedDepthTest) { glEnable(GL_DEPTH_TEST); } else { glDisable(GL_DEPTH_TEST); }
	if (savedScissorTest) { glEnable(GL_SCISSOR_TEST); } else { glDisable(GL_SCISSOR_TEST); }
	if (savedCullFace) { glEnable(GL_CULL_FACE); } else { glDisable(GL_CULL_FACE); }
	if (savedStencilTest) { glEnable(GL_STENCIL_TEST); } else { glDisable(GL_STENCIL_TEST); }
	if (savedFramebufferSrgb) { glEnable(GL_FRAMEBUFFER_SRGB); } else { glDisable(GL_FRAMEBUFFER_SRGB); }
	glBlendFuncSeparate((GLenum) savedBlendSrcRGB, (GLenum) savedBlendDstRGB,
						(GLenum) savedBlendSrcAlpha, (GLenum) savedBlendDstAlpha);
	glBlendEquationSeparate((GLenum) savedBlendEqRGB, (GLenum) savedBlendEqAlpha);
	glColorMask(savedColorMask[0], savedColorMask[1], savedColorMask[2], savedColorMask[3]);
	if (savedPolygonMode[0] != GL_FILL || savedPolygonMode[1] != GL_FILL) {
		if (savedPolygonMode[0] == savedPolygonMode[1]) {
			glPolygonMode(GL_FRONT_AND_BACK, (GLenum) savedPolygonMode[0]);
		} else {
			glPolygonMode(GL_FRONT, (GLenum) savedPolygonMode[0]);
			glPolygonMode(GL_BACK, (GLenum) savedPolygonMode[1]);
		}
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, savedUnpackAlignment);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, savedUnpackRowLength);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, savedUnpackSkipPixels);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, savedUnpackSkipRows);
	if (savedDrawFbo != 0) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, (GLuint) savedDrawFbo);
	}
	if (savedReadFbo != 0) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, (GLuint) savedReadFbo);
	}

	// drain opengl error queue
	while (glGetError() != GL_NO_ERROR)
		;
}

#if defined(TARGET_UNIX)
#	include "init_unix.c"
#elif defined(TARGET_MAC)
#	include "init_mac.c"
#endif
