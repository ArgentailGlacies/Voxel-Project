
#include "OpenGL.h"

#include <plog/Log.h>

bool core::hasOpenGLError(const char * function, unsigned int line)
{
	switch (glGetError())
	{
	case GL_NO_ERROR:
		return false;

	case GL_INVALID_ENUM:
		LOG_WARNING << "Invalid enum at " << function << "@" << line;
		break;
	case GL_INVALID_OPERATION:
		LOG_WARNING << "Invalid operation at " << function << "@" << line;
		break;
	case GL_OUT_OF_MEMORY:
		LOG_WARNING << "Out of memory at " << function << "@" << line;
		break;
	case GL_INVALID_VALUE:
		LOG_WARNING << "Invalid value at " << function << "@" << line;
		break;

	default:
		LOG_WARNING << "Unknown error at " << function << "@" << line;
	}
	return true;
}
