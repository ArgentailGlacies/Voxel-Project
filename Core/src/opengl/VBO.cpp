
#include "VBO.h"

#include "opengl/OpenGL.h"

#include <plog/Log.h>
#include <unordered_map>

namespace
{
	std::unordered_map<core::BufferType, unsigned int> bindings;

	auto getBoundHandle(core::BufferType type)
	{
		return bindings[type];
	}
	bool setBoundHandle(core::BufferType type, unsigned int handle)
	{
		if (getBoundHandle(type) == handle)
			return false;
		bindings[type] = handle;
		glBindBuffer(static_cast<GLenum>(type), handle);
		return !HAS_GL_ERROR;
	}
}

core::VBO::VBO(BufferType type, BufferUse use)
	: m_type(type), m_use(use)
{
	glGenBuffers(1, &m_handle);
	HAS_GL_ERROR;
}
core::VBO::~VBO()
{
	glDeleteBuffers(1, &m_handle);
	HAS_GL_ERROR;

	if (getBoundHandle(m_type) == m_handle)
		setBoundHandle(m_type, 0u);
}

bool core::VBO::bind() const
{
	return setBoundHandle(m_type, m_handle);
}
bool core::VBO::unbind() const
{
	return setBoundHandle(m_type, 0u);
}
bool core::VBO::reset(BufferType type)
{
	return setBoundHandle(type, 0u);
}

bool core::VBO::reserve(unsigned int bytes, const void * data) const
{
	setBoundHandle(m_type, m_handle);
	glBufferData(static_cast<GLenum>(m_type), bytes, data, static_cast<GLenum>(m_use));
	return !HAS_GL_ERROR;
}
bool core::VBO::write(unsigned int bytes, const void * data, unsigned int offset) const
{
	setBoundHandle(m_type, m_handle);
	glBufferSubData(static_cast<GLenum>(m_type), offset, bytes, data);
	return !HAS_GL_ERROR;
}
bool core::VBO::read(unsigned int bytes, void * data, unsigned int offset) const
{
	setBoundHandle(m_type, m_handle);
	glGetBufferSubData(static_cast<GLenum>(m_type), offset, bytes, data);
	return !HAS_GL_ERROR;
}
