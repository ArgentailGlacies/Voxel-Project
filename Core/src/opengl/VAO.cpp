
#include "VAO.h"

#include "opengl/OpenGL.h"

#include <mutex>

namespace
{
	std::mutex mutex;
	unsigned int binding = 0u;

	auto getBoundHandle()
	{
		return binding;
	}
	bool setBoundHandle(unsigned int handle)
	{
		if (getBoundHandle() == handle)
			return false;
		binding = handle;
		glBindVertexArray(handle);
		return !HAS_GL_ERROR;
	}
}

core::VAO::VAO()
{
	glGenVertexArrays(1, &m_handle);
	HAS_GL_ERROR;
}
core::VAO::~VAO()
{
	glDeleteVertexArrays(1, &m_handle);
	HAS_GL_ERROR;

	std::lock_guard guard{ mutex };
	if (getBoundHandle() == m_handle)
		setBoundHandle(0u);
}

bool core::VAO::bind() const
{
	std::lock_guard guard{ mutex };
	return setBoundHandle(m_handle);
}
bool core::VAO::unbind() const
{
	std::lock_guard guard{ mutex };
	return setBoundHandle(0u);
}
