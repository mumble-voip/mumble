#ifndef MUMBLE_AVAIL_H_
#define MUMBLE_AVAIL_H_

#define AVAIL_ALL_GLSYM ( \
	AVAIL(glTexImage2D) && \
	AVAIL(glCreateShader) && \
	AVAIL(glLinkProgram) && \
	AVAIL(glActiveTexture) && \
	AVAIL(glViewport) && \
	AVAIL(glEnableClientState) && \
	AVAIL(glOrtho) && \
	AVAIL(glGetIntegerv) && \
	AVAIL(glCompileShader) && \
	AVAIL(glGetString) && \
	AVAIL(glPopAttrib) && \
	AVAIL(glPushClientAttrib) && \
	AVAIL(glAttachShader) && \
	AVAIL(glEnd) && \
	AVAIL(glGetBooleanv) && \
	AVAIL(glDisableClientState) && \
	AVAIL(glUniform1i) && \
	AVAIL(glRenderMode) && \
	AVAIL(glIsTexture) && \
	AVAIL(glTexEnvi) && \
	AVAIL(glGetTexParameterfv) && \
	AVAIL(glPopMatrix) && \
	AVAIL(glDisable) && \
	AVAIL(glBindTexture) && \
	AVAIL(glPushAttrib) && \
	AVAIL(glUseProgram) && \
	AVAIL(glCreateProgram) && \
	AVAIL(glDeleteTextures) && \
	AVAIL(glBegin) && \
	AVAIL(glVertex2f) && \
	AVAIL(glMatrixMode) && \
	AVAIL(glGenTextures) && \
	AVAIL(glGetError) && \
	AVAIL(glBlendFunc) && \
	AVAIL(glPopClientAttrib) && \
	AVAIL(glBindBuffer) && \
	AVAIL(glShaderSource) && \
	AVAIL(glEnable) && \
	AVAIL(glGetShaderInfoLog) && \
	AVAIL(glPixelStorei) && \
	AVAIL(glTexParameterfv) && \
	AVAIL(glTexSubImage2D) && \
	AVAIL(glTexCoord2f) && \
	AVAIL(glGetUniformLocation) && \
	AVAIL(glColorMaterial) && \
	AVAIL(glPushMatrix) && \
	AVAIL(glTexParameteri) && \
	AVAIL(glLoadIdentity) && \
	1)

#endif
