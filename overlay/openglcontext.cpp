#include "openglcontext.h"

#include "openglfunctions.h"

OpenGLOverlayContext::OpenGLOverlayContext(HDC hdc, OpenGLFunctions* glfunctions) {
	timeT = clock();
	frameCount = 0;

	ctx = 0;
	device = hdc;
	gl = glfunctions;
	texture = ~0;

	if (!gl->isValid()) {
		ods("OpenGL: Blit - skipping because not all required GL functions are determined.");
		return;
	}
	ctx = gl->owglCreateContext(device);
	gl->owglMakeCurrent(device, ctx);
	initContext();
}

void OpenGLOverlayContext::initContext() {
	// Here we go. From the top. Where is glResetState?
	gl->oglDisable(GL_ALPHA_TEST);
	gl->oglDisable(GL_AUTO_NORMAL);
	gl->oglEnable(GL_BLEND);
	// Skip clip planes, there are thousands of them.
	gl->oglDisable(GL_COLOR_LOGIC_OP);
	gl->oglDisable(GL_COLOR_MATERIAL);
	gl->oglDisable(GL_COLOR_TABLE);
	gl->oglDisable(GL_CONVOLUTION_1D);
	gl->oglDisable(GL_CONVOLUTION_2D);
	gl->oglDisable(GL_CULL_FACE);
	gl->oglDisable(GL_DEPTH_TEST);
	gl->oglDisable(GL_DITHER);
	gl->oglDisable(GL_FOG);
	gl->oglDisable(GL_HISTOGRAM);
	gl->oglDisable(GL_INDEX_LOGIC_OP);
	gl->oglDisable(GL_LIGHTING);
	// Skip line smmooth
	// Skip map
	gl->oglDisable(GL_MINMAX);
	// Skip polygon offset
	gl->oglDisable(GL_SEPARABLE_2D);
	gl->oglDisable(GL_SCISSOR_TEST);
	gl->oglDisable(GL_STENCIL_TEST);
	gl->oglEnable(GL_TEXTURE_2D);
	gl->oglDisable(GL_TEXTURE_GEN_Q);
	gl->oglDisable(GL_TEXTURE_GEN_R);
	gl->oglDisable(GL_TEXTURE_GEN_S);
	gl->oglDisable(GL_TEXTURE_GEN_T);

	gl->oglBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLOverlayContext::blit(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	if (!gl->isValid()) {
		ods("OpenGL: Blit - skipping because not all required GL functions are determined.");
		return;
	} else {
		ods("OpenGL: Blit %d %d %d %d -- %d %d : %d", x, y, w, h, uiWidth, uiHeight, texture);
	}

	if (texture == ~0)
		return;

	gl->oglBindTexture(GL_TEXTURE_2D, texture);

	if ((x == 0) && (y == 0) && (w == uiWidth) && (h == uiHeight)) {
		gl->oglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, uiWidth, uiHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, a_ucTexture);
	} else {
		if (w != uiWidth)
			gl->oglPixelStorei(GL_UNPACK_ROW_LENGTH, uiWidth);
		gl->oglTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_BGRA, GL_UNSIGNED_BYTE, a_ucTexture + 4 * (y * uiWidth + x));
		if (w != uiWidth)
			gl->oglPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	}
}

void OpenGLOverlayContext::setRect() {
	ods("OpenGL: setRect");
}

void OpenGLOverlayContext::newTexture(unsigned int width, unsigned int height) {
	if (!gl->isValid()) {
		ods("OpenGL: newTex - skipping because not all required GL functions are determined.");
		return;
	} else {
		ods("OpenGL: newTex");
	}

	if (texture == ~0) {
		gl->oglBindTexture(GL_TEXTURE_2D, 0);
		gl->oglDeleteTextures(1, &texture);
		texture = ~0;
	}
	gl->oglGenTextures(1, &texture);

	gl->oglBindTexture(GL_TEXTURE_2D, texture);
	gl->oglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gl->oglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gl->oglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	gl->oglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	unsigned char *ptr = new unsigned char[width*height*4];
	memset(ptr, 0, width*height*4);
	gl->oglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, ptr);
	delete [] ptr;
}

void OpenGLOverlayContext::draw() {
	if (!gl->isValid()) {
		return;
	}

	// Change drawing context, but remember the original for restoring it later.
	HGLRC oldctx = gl->owglGetCurrentContext();
	HDC oldhdc = gl->owglGetCurrentDC();
	gl->owglMakeCurrent(device, ctx);

	clock_t t = clock();
	float elapsed = static_cast<float>(t - timeT) / CLOCKS_PER_SEC;
	++frameCount;
	if (elapsed > OVERLAY_FPS_INTERVAL) {
		OverlayMsg om;
		om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType = OVERLAY_MSGTYPE_FPS;
		om.omh.iLength = sizeof(OverlayMsgFps);
		om.omf.fps = frameCount / elapsed;

		sendMessage(om);

		frameCount = 0;
		timeT = t;
	}

	unsigned int width, height;
	width = gl->oGetDeviceCaps(device, HORZRES);
	height = gl->oGetDeviceCaps(device, VERTRES);

	HWND hwnd = WindowFromDC(device);
	if (hwnd) {
		RECT r;
		if (GetClientRect(hwnd, &r)) {
			width = r.right - r.left;
			height = r.bottom - r.top;
		}
	}

	ods("OpenGL: DrawStart: Screen is %d x %d", width, height);

	checkMessage(width, height);

	gl->oglViewport(0, 0, width, height);

	gl->oglMatrixMode(GL_PROJECTION);
	gl->oglLoadIdentity();
	gl->oglOrtho(0, width, height, 0, -100.0, 100.0);

	gl->oglMatrixMode(GL_MODELVIEW);

	gl->oglBindTexture(GL_TEXTURE_2D, texture);
	gl->oglPushMatrix();
	gl->oglLoadIdentity();

	float w = static_cast<float>(uiWidth);
	float h = static_cast<float>(uiHeight);

	float left   = static_cast<float>(uiLeft);
	float top    = static_cast<float>(uiTop);
	float right  = static_cast<float>(uiRight);
	float bottom = static_cast<float>(uiBottom);

	float xm = (left) / w;
	float ym = (top) / h;
	float xmx = (right) / w;
	float ymx = (bottom) / h;

	gl->oglTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	gl->oglBegin(GL_QUADS);

	gl->oglTexCoord2f(xm, ymx);
	gl->oglVertex2f(left, bottom);

	gl->oglTexCoord2f(xm, ym);
	gl->oglVertex2f(left, top);

	gl->oglTexCoord2f(xmx, ym);
	gl->oglVertex2f(right, top);

	gl->oglTexCoord2f(xmx, ymx);
	gl->oglVertex2f(right, bottom);
	gl->oglEnd();

	gl->oglPopMatrix();

	// Restore context
	gl->owglMakeCurrent(oldhdc, oldctx);
}
