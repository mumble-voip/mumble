/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2008-2010, Mikkel Krautz <mikkel@krautz.dk>

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
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <mach/mach_init.h>
#include <mach/task.h>
#include <dlfcn.h>
#include <pwd.h>
#include <unistd.h>
#include <objc/objc-runtime.h>

#include <OpenGL/OpenGL.h>
#include <Carbon/Carbon.h>
#include <Cocoa/Cocoa.h>
#include <AGL/AGL.h>

#include "mach_override.h"

#include "../../overlay/overlay.h"
#include "avail.h"

static bool bDebug = false;
static bool bCursorAvail = false;

typedef struct _Context {
	struct _Context *next;
	GLuint uiProgram;

	CGLContextObj cglctx;
	NSOpenGLContext *nsctx;

	unsigned int uiWidth, uiHeight;
	unsigned int uiLeft, uiRight, uiTop, uiBottom;

	struct sockaddr_un saName;
	int iSocket;
	struct OverlayMsg omMsg;
	GLuint texture;

	unsigned char *a_ucTexture;
	unsigned int uiMappedLength;
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

#define AVAIL(name) dlsym(RTLD_DEFAULT,#name)
#define FDEF(name) static __typeof__(&name) o##name = NULL
FDEF(CGLFlushDrawable);
FDEF(CGDisplayHideCursor);
FDEF(CGDisplayShowCursor);

__attribute__((format(printf, 1, 2)))
static void ods(const char *format, ...) {
	if (!bDebug) {
		return;
	}

	char fmt[1024] = { 0, };
	strlcat(fmt, "MumbleOverlay: ", 1024);
	strlcat(fmt, format, 1024);
	strlcat(fmt, "\n", 1024);

	va_list args;
	va_start(args, format);
	vfprintf(stderr, fmt, args);
	va_end(args);
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

		struct OverlayMsg om;
		om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType = OVERLAY_MSGTYPE_PID;
		om.omh.iLength = sizeof(struct OverlayMsgPid);
		om.omp.pid = getpid();

		if (!sendMessage(ctx, &om))
			return;

		ods("SentPid");
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
				case OVERLAY_MSGTYPE_INTERACTIVE: {
						struct OverlayMsgInteractive *omin = & ctx->omMsg.omin;
						ods("Interactive %d", omin->state);
						if (bCursorAvail) {
							if (omin->state) {
								oCGDisplayHideCursor(kCGNullDirectDisplay);
							} else {
								oCGDisplayShowCursor(kCGNullDirectDisplay);
							}
						}
				}
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

	glBegin(GL_QUADS);
	glTexCoord2f(tex[0], tex[1]);
	glVertex2f(vertex[0], vertex[1]);
	glTexCoord2f(tex[2], tex[3]);
	glVertex2f(vertex[2], vertex[3]);
	glTexCoord2f(tex[4], tex[5]);
	glVertex2f(vertex[4], vertex[5]);
	glTexCoord2f(tex[6], tex[7]);
	glVertex2f(vertex[6], vertex[7]);
	glEnd();

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

	int bound = 0;
	glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &bound);

	if (bound != 0)
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

	drawOverlay(ctx, width, height);

	if (bound != 0)
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, bound);

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

@implementation NSOpenGLContext (MumbleOverlay)
- (void) overlayFlushBuffer {
	ods("[NSOpenGLContext flushBuffer] %p %p", self, [self CGLContextObj]);

	Context *c = contexts;
	while (c) {
		if (c->nsctx == self && c->cglctx == [self CGLContextObj])
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
		c->nsctx = (NSOpenGLContext *)self;
		c->cglctx = (CGLContextObj)[self CGLContextObj];
		contexts = c;
		newContext(c);
	}

	NSView *v = [c->nsctx view];
	int width = 0, height = 0;
	if (v) {
		NSRect r = [v bounds];
		width = (int)r.size.width;
		height = (int)r.size.height;
	} else {
		if (AVAIL(CGMainDisplayID)) {
			CGDirectDisplayID md = CGMainDisplayID();
			if (CGDisplayIsCaptured(md)) {
				width = CGDisplayPixelsWide(md);
				height = CGDisplayPixelsHigh(md);
			}
		}
		if (!width && !height) {
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			width = viewport[2];
			height = viewport[3];
		}
	}

	drawContext(c, width, height);
	[self overlayFlushBuffer];
}
@end

void CGLFlushDrawableOverride(CGLContextObj ctx) {
	ods("CGLFlushDrawableOverride %p", ctx);
	Context *c = contexts;

	/* Sometimes, we can get a FlushDrawable where the current context is NULL.
	 * Also, check that the context CGLFlushDrawable was called on is the active context.
	 * If it isn't, who knows what context we're drawing on?
	 *
	 * Maybe we should even use CGLSetCurrentContext() to switch to the context that's being
	 * flushed?
	 */
	CGLContextObj current = CGLGetCurrentContext();
	if (current == NULL || ctx != current)
		goto skip;

	while (c) {
		if (c->cglctx == ctx) {
			/* There is no NSOpenGLContext for this CGLContext, so we should draw. */
			if (c->nsctx == NULL)
				break;
			/* This context is coupled with a NSOpenGLContext, so skip. */
			else
				goto skip;
		}
		c = c->next;
	}

	if (!c) {
		ods("Current context is: %p", ctx);

		c = malloc(sizeof(Context));
		if (!c) {
			ods("malloc failure");
			return;
		}
		memset(c, 0, sizeof(Context));
		c->next = contexts;
		c->cglctx = ctx;
		c->nsctx = NULL;
		contexts = c;
		newContext(c);
	}

	int width = 0, height = 0;
	if (AVAIL(CGMainDisplayID)) {
		CGDirectDisplayID md = CGMainDisplayID();
		if (CGDisplayIsCaptured(md)) {
			width = CGDisplayPixelsWide(md);
			height = CGDisplayPixelsHigh(md);
		}
	}
	if (!width && !height) {
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		width = viewport[2];
		height = viewport[3];
		/* Are the viewport values crazy? Skip them in that case. */
		if (height < 0 || width < 0 || height > 5000 || width > 5000)
			goto skip;
	}

	drawContext(c, width, height);

skip:
	oCGLFlushDrawable(ctx);
}

CGError CGDisplayShowCursorOverride(CGDirectDisplayID display) {
	ods("CGDisplayShowCursor()");
	return oCGDisplayShowCursor(display);
}

CGError CGDisplayHideCursorOverride(CGDirectDisplayID display) {
	ods("CGDisplayHideCursor()");
	return oCGDisplayHideCursor(display);
}

__attribute__ ((visibility("default")))
__attribute__((constructor))
void MumbleOverlayEntryPoint() {
	struct stat buf;
	bDebug = (stat("/Library/Application Support/.mumble_overlay_debug", &buf) == 0);

	ods("!");

	void *nsgl = NULL, *cgl = NULL;
	nsgl = dlsym(RTLD_DEFAULT, "NSClassFromString");
	cgl = dlsym(RTLD_DEFAULT, "CGLFlushDrawable");

	/* Check for GL symbol availability */
	if (!(AVAIL_ALL_GLSYM)) {
		ods("Missing GL symbols. Disabling overlay.");
		return;
	}

	/* NSOpenGL */
	if (nsgl) {
		ods("Attempting to hook NSOpenGL");
		Class c = NSClassFromString(@"NSOpenGLContext");
		if (c) {
			Method orig = class_getInstanceMethod(c, @selector(flushBuffer));
			Method new = class_getInstanceMethod(c, @selector(overlayFlushBuffer));
			if (class_addMethod(c, @selector(flushBuffer), method_getImplementation(new), method_getTypeEncoding(new)))
				class_replaceMethod(c, @selector(overlayFlushBuffer), method_getImplementation(orig), method_getTypeEncoding(orig));
			else
				method_exchangeImplementations(orig, new);
		}
	}

	/* CGL */
	if (AVAIL(CGLFlushDrawable)) {
		ods("Attempting to hook CGL");
		if (mach_override("_CGLFlushDrawable", NULL, CGLFlushDrawableOverride, (void **) &oCGLFlushDrawable) != 0) {
			ods("CGLFlushDrawable override failed.");
		} else
			ods("Up running.");
	} else {
		ods("Required entry points not available in process. Not hooking up overlay.");
	}

	if (AVAIL(CGDisplayHideCursor) && AVAIL(CGDisplayShowCursor)) {
		if (mach_override("_CGDisplayHideCursor", NULL, CGDisplayHideCursorOverride, (void **) &oCGDisplayHideCursor) != 0) {
			ods("CGDisplayHideCursor override failed");
		}
		if (mach_override("_CGDisplayShowCursor", NULL, CGDisplayShowCursorOverride, (void **) &oCGDisplayShowCursor) != 0) {
			ods("CGDisplayShowCursor override failed");
		}
		ods("Hooked CGDisplayShowCursor and CGDisplayHideCursor");
		bCursorAvail = true;
	}
}
