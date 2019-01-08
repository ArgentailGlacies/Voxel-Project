
#include "Shader.h"

#include "opengl/OpenGL.h"

#include <plog/Log.h>

core::Shader::Shader(ShaderType type)
{
	m_handle = glCreateShader(static_cast<GLenum>(type));
	HAS_GL_ERROR;
}
core::Shader::Shader(Shader && other)
{
	*this = std::move(other);
}
core::Shader::~Shader()
{
	glDeleteShader(m_handle);
	HAS_GL_ERROR;
}

core::Shader & core::Shader::operator=(Shader && other)
{
	if (this != &other)
		std::swap(m_handle, other.m_handle);
	return *this;
}

bool core::Shader::compile(const std::string & code) const
{
	const char* data = code.c_str();
	glShaderSource(m_handle, 1, &data, nullptr);
	glCompileShader(m_handle);

	GLint result;
	glGetShaderiv(m_handle, GL_COMPILE_STATUS, &result);
	GLint length;
	glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &length);
	std::string message{ "", static_cast<unsigned int>(length) };
	glGetShaderInfoLog(m_handle, length, nullptr, &message[0]);

	if (!result)
	{
		LOG_WARNING << "\n" << message;
		return false;
	}
	return !HAS_GL_ERROR;
}
