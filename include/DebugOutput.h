#ifndef DEBUGOUTPUT_H
#define DEBUGOUTPUT_H

#include "Common.h"

#include <iostream>
#include <string>

const GLuint userEventID = 3636;

#ifndef NDEBUG
	#define glCheckError() _glCheckError(__FILE__, __LINE__)
#else
	#define glCheckError()
#endif

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
