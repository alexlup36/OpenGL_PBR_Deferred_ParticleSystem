#ifndef DEBUGOUTPUT_H
#define DEBUGOUTPUT_H

#include "Common.h"

#include <iostream>
#include <string>

#define glCheckError() _glCheckError(__FILE__, __LINE__)

// ----------------------------------------------------------------------------

GLenum _glCheckError(const char *file, int line);
void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam);

// ----------------------------------------------------------------------------

#endif // DEBUGOUTPUT_H
