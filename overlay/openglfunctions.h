#ifndef MUMBLE_OVERLAY_OPENGLFUNCTIONS_H_
#define MUMBLE_OVERLAY_OPENGLFUNCTIONS_H_

/// For declaring function pointer types
#define TDEF(ret, name, arg) typedef ret (__stdcall * t##name) arg
/// For declaring function pointer variables
#define GLDEF(ret, name, arg) TDEF(ret, name, arg); t##name o##name = NULL
/// Injection, using the declared hook variable, function pointer, and hook to
/// my##name
#define INJECT(name) { o##name = reinterpret_cast<t##name>(GetProcAddress(hGL, #name)); if (o##name) { hh##name.setup(reinterpret_cast<voidFunc>(o##name), reinterpret_cast<voidFunc>(my##name)); o##name = (t##name) hh##name.call; } else { ods("OpenGL: No GetProc for %s", #name);} }

#include "openglGLreplication.h"

struct OpenGLFunctions {
	GLDEF(HGLRC, wglCreateContext, (HDC));
	GLDEF(HGLRC, wglGetCurrentContext, (void));
	GLDEF(HDC, wglGetCurrentDC, (void));

	GLDEF(void, glGenTextures, (GLsizei, GLuint *));
	GLDEF(void, glDeleteTextures, (GLsizei, GLuint *));
	GLDEF(void, glEnable, (GLenum));
	GLDEF(void, glDisable, (GLenum));
	GLDEF(void, glBlendFunc, (GLenum, GLenum));
	GLDEF(void, glColorMaterial, (GLenum, GLenum));
	GLDEF(void, glViewport, (GLint, GLint, GLsizei, GLsizei));
	GLDEF(void, glMatrixMode, (GLenum));
	GLDEF(void, glLoadIdentity, (void));
	GLDEF(void, glOrtho, (GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble));
	GLDEF(void, glBindTexture, (GLenum, GLuint));
	GLDEF(void, glPushMatrix, (void));
	GLDEF(void, glColor4ub, (GLubyte, GLubyte, GLubyte, GLubyte));
	GLDEF(void, glTranslatef, (GLfloat, GLfloat, GLfloat));
	GLDEF(void, glBegin, (GLenum));
	GLDEF(void, glEnd, (void));
	GLDEF(void, glTexCoord2f, (GLfloat, GLfloat));
	GLDEF(void, glVertex2f, (GLfloat, GLfloat));
	GLDEF(void, glPopMatrix, (void));
	GLDEF(void, glTexParameteri, (GLenum, GLenum, GLint));
	GLDEF(void, glTexEnvi, (GLenum, GLenum, GLint));
	GLDEF(void, glTexImage2D, (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *));
	GLDEF(void, glTexSubImage2D, (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *));
	GLDEF(void, glPixelStorei, (GLenum, GLint));
	GLDEF(void, wglMakeCurrent, (HDC, HGLRC));
	GLDEF(int, GetDeviceCaps, (HDC, int));

	/// This structure is considered valid if all of the
	bool isValid() {
		return owglCreateContext != NULL
				&& owglCreateContext != NULL
				&& owglGetCurrentContext != NULL
				&& owglGetCurrentDC != NULL
				&& oglGenTextures != NULL
				&& oglDeleteTextures != NULL
				&& oglEnable != NULL
				&& oglDisable != NULL
				&& oglBlendFunc != NULL
				&& oglColorMaterial != NULL
				&& oglViewport != NULL
				&& oglMatrixMode != NULL
				&& oglLoadIdentity != NULL
				&& oglOrtho != NULL
				&& oglBindTexture != NULL
				&& oglPushMatrix != NULL
				&& oglColor4ub != NULL
				&& oglTranslatef != NULL
				&& oglBegin != NULL
				&& oglEnd != NULL
				&& oglTexCoord2f != NULL
				&& oglVertex2f != NULL
				&& oglPopMatrix != NULL
				&& oglTexParameteri != NULL
				&& oglTexEnvi != NULL
				&& oglTexImage2D != NULL
				&& oglTexSubImage2D != NULL
				&& oglPixelStorei != NULL
				&& owglMakeCurrent != NULL
				&& oGetDeviceCaps != NULL;
	}
};

#endif
