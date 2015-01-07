
#ifndef MUMBLE_OVERLAY_OPENGLCONTEXT_H_
#define MUMBLE_OVERLAY_OPENGLCONTEXT_H_

#include "lib.h"
#include <time.h>

#include "openglGLreplication.h"

struct OpenGLFunctions;

class OpenGLOverlayContext : protected Pipe {
	public:
	OpenGLOverlayContext(HDC hdc, OpenGLFunctions* gl);
		void draw();

	protected:
		virtual void blit(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
		virtual void setRect();
		virtual void newTexture(unsigned int width, unsigned int height);

	private:
		HDC device;
		HGLRC ctx;
		GLuint texture;
		OpenGLFunctions* gl;

		clock_t timeT;
		unsigned int frameCount;

		void initContext();
};

#endif MUMBLE_OVERLAY_OPENGLCONTEXT_H_
