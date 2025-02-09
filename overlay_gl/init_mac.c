// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// This file is included by overlay.c for
// Mac-specific overlay initialization.

@implementation NSOpenGLContext (MumbleOverlay)
- (void)overlayFlushBuffer {
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
		c->next   = contexts;
		c->nsctx  = (NSOpenGLContext *) self;
		c->cglctx = (CGLContextObj)[self CGLContextObj];
		contexts  = c;
		newContext(c);
	}

	NSView *v = [c->nsctx view];
	int width = 0, height = 0;
	if (v) {
		NSRect r = [v bounds];
		width    = (int) r.size.width;
		height   = (int) r.size.height;
	} else {
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		width  = viewport[2];
		height = viewport[3];
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
		c->next   = contexts;
		c->cglctx = ctx;
		c->nsctx  = NULL;
		contexts  = c;
		newContext(c);
	}

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	int width  = viewport[2];
	int height = viewport[3];
	/* Are the viewport values crazy? Skip them in that case. */
	if (height < 0 || width < 0 || height > 5000 || width > 5000) {
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

__attribute__((constructor)) static void initializeLibrary() {
	struct stat buf;

	bDebug = (stat("/Library/Application Support/.mumble_overlay_debug", &buf) == 0);

	ods("!");

	void *nsgl = NULL;
	nsgl       = dlsym(RTLD_DEFAULT, "NSClassFromString");

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
			Method new  = class_getInstanceMethod(c, @selector(overlayFlushBuffer));
			if (class_addMethod(c, @selector(flushBuffer), method_getImplementation(new), method_getTypeEncoding(new)))
				class_replaceMethod(c, @selector(overlayFlushBuffer), method_getImplementation(orig),
									method_getTypeEncoding(orig));
			else
				method_exchangeImplementations(orig, new);
		}
	}

	/* CGL */
	if (AVAIL(CGLFlushDrawable)) {
		ods("Attempting to hook CGL");
		if (mach_override_ptr(dlsym(RTLD_DEFAULT, "CGLFlushDrawable"), CGLFlushDrawableOverride,
							  (void **) &oCGLFlushDrawable)
			!= 0) {
			ods("CGLFlushDrawable override failed.");
		} else
			ods("Up running.");
	} else {
		ods("Required entry points not available in process. Not hooking up overlay.");
	}

	if (AVAIL(CGDisplayHideCursor) && AVAIL(CGDisplayShowCursor)) {
		if (mach_override_ptr(dlsym(RTLD_DEFAULT, "CGDisplayHideCursor"), CGDisplayHideCursorOverride,
							  (void **) &oCGDisplayHideCursor)
			!= 0) {
			ods("CGDisplayHideCursor override failed");
		}
		if (mach_override_ptr(dlsym(RTLD_DEFAULT, "CGDisplayShowCursor"), CGDisplayShowCursorOverride,
							  (void **) &oCGDisplayShowCursor)
			!= 0) {
			ods("CGDisplayShowCursor override failed");
		}
		ods("Hooked CGDisplayShowCursor and CGDisplayHideCursor");
		bCursorAvail = true;
	}
}
