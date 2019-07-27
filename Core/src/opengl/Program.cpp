
#include "Program.h"

#include "opengl/OpenGL.h"
#include "opengl/Shader.h"

#include <mutex>
#include <plog/Log.h>

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
		glUseProgram(handle);
		return !HAS_GL_ERROR;
	}
}

core::Program::Program()
{
	m_handle = glCreateProgram();
	HAS_GL_ERROR;
}
core::Program::Program(Program && other)
{
	*this = std::move(other);
}
core::Program::~Program()
{
	glDeleteProgram(m_handle);
	HAS_GL_ERROR;
}

core::Program & core::Program::operator=(Program && other)
{
	if (this != &other)
		std::swap(m_handle, other.m_handle);
	return *this;
}

bool core::Program::bind() const
{
	std::lock_guard guard{ mutex };
	return setBoundHandle(m_handle);
}
bool core::Program::unbind() const
{
	std::lock_guard guard{ mutex };
	return setBoundHandle(0u);
}
bool core::Program::reset()
{
	std::lock_guard guard{ mutex };
	return setBoundHandle(0u);
}

bool core::Program::attach(const Shader & shader) const
{
	glAttachShader(m_handle, shader.handle());
	return !HAS_GL_ERROR;
}
bool core::Program::link() const
{
	glLinkProgram(m_handle);

	GLint result;
	glGetProgramiv(m_handle, GL_LINK_STATUS, &result);
	GLint length;
	glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &length);
	std::string message{ "", static_cast<unsigned int>(length) };
	glGetProgramInfoLog(m_handle, length, nullptr, &message[0]);

	if (!result)
	{
		LOG_WARNING << "\n" << message;
		return false;
	}
	return !HAS_GL_ERROR;
}
