#pragma once

#define ALLEGRO_UNSTABLE

#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>
#include <gl/GL.h>

#define HAS_GL_ERROR ::core::hasOpenGLError(__func__, __LINE__)

namespace core
{
	bool hasOpenGLError(const char * function, unsigned int line);
}