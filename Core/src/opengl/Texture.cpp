
#include "Texture.h"

#include "opengl/OpenGL.h"

#include <mutex>
#include <unordered_map>

namespace
{
	std::mutex mutex;
	std::unordered_map<core::TextureType, unsigned int> bindings;

	auto getBoundHandle(core::TextureType type)
	{
		return bindings[type];
	}
	bool setBoundHandle(core::TextureType type, unsigned int handle)
	{
		if (getBoundHandle(type) == handle)
			return false;
		bindings[type] = handle;
		glBindTexture(static_cast<GLenum>(type), handle);
		return !HAS_GL_ERROR;
	}
}

core::Texture::Texture(TextureType type, DataFormat data, ColorFormat color)
	: m_type(type), m_data(data), m_color(color)
{
	glGenTextures(1, &m_handle);
	HAS_GL_ERROR;
}
core::Texture::~Texture()
{
	glDeleteTextures(1, &m_handle);
	HAS_GL_ERROR;

	std::lock_guard guard{ mutex };
	if (getBoundHandle(m_type) == m_handle)
		setBoundHandle(m_type, 0u);
}

bool core::Texture::bind() const
{
	std::lock_guard guard{ mutex };
	return setBoundHandle(m_type, m_handle);
}
bool core::Texture::unbind() const
{
	std::lock_guard guard{ mutex };
	return setBoundHandle(m_type, 0u);
}

bool core::Texture::reserve(unsigned int size, DataFormat format, ColorFormat color, const void * data) const
{
	std::lock_guard guard{ mutex };
	setBoundHandle(m_type, m_handle);
	glTexImage1D(
		static_cast<GLenum>(m_type), 0, static_cast<GLint>(m_color), size, 0,
		static_cast<GLenum>(color), static_cast<GLenum>(format), data
	);
	return !HAS_GL_ERROR;
}
bool core::Texture::reserve(const glm::uvec2 & size, DataFormat format, ColorFormat color, const void * data) const
{
	std::lock_guard guard{ mutex };
	setBoundHandle(m_type, m_handle);
	glTexImage2D(
		static_cast<GLenum>(m_type), 0, static_cast<GLint>(m_color), size.x, size.y, 0,
		static_cast<GLenum>(color), static_cast<GLenum>(format), data
	);
	return !HAS_GL_ERROR;
}
bool core::Texture::reserve(const glm::uvec3 & size, DataFormat format, ColorFormat color, const void * data) const
{
	std::lock_guard guard{ mutex };
	setBoundHandle(m_type, m_handle);
	glTexImage3D(
		static_cast<GLenum>(m_type), 0, static_cast<GLint>(m_color), size.x, size.y, size.z, 0,
		static_cast<GLenum>(color), static_cast<GLenum>(format), data
	);
	return !HAS_GL_ERROR;
}

bool core::Texture::write(unsigned int offset, unsigned int size, DataFormat format, ColorFormat color, const void * data) const
{
	std::lock_guard guard{ mutex };
	setBoundHandle(m_type, m_handle);
	glTexSubImage1D(
		static_cast<GLenum>(m_type), 0, offset, size,
		static_cast<GLenum>(color), static_cast<GLenum>(format), data
	);
	return !HAS_GL_ERROR;
}
bool core::Texture::write(const glm::uvec2 & offset, const glm::uvec2 & size, DataFormat format, ColorFormat color, const void * data) const
{
	std::lock_guard guard{ mutex };
	setBoundHandle(m_type, m_handle);
	glTexSubImage2D(
		static_cast<GLenum>(m_type), 0, offset.x, offset.y, size.x, size.y,
		static_cast<GLenum>(color), static_cast<GLenum>(format), data
	);
	return !HAS_GL_ERROR;
}
bool core::Texture::write(const glm::uvec3 & offset, const glm::uvec3 & size, DataFormat format, ColorFormat color, const void * data) const
{
	std::lock_guard guard{ mutex };
	setBoundHandle(m_type, m_handle);
	glTexSubImage3D(
		static_cast<GLenum>(m_type), 0, offset.x, offset.y, offset.z, size.x, size.y, size.z,
		static_cast<GLenum>(color), static_cast<GLenum>(format), data
	);
	return !HAS_GL_ERROR;
}

bool core::Texture::generateMipmaps() const
{
	std::lock_guard guard{ mutex };
	setBoundHandle(m_type, m_handle);
	glGenerateMipmap(static_cast<GLenum>(m_type));
	return !HAS_GL_ERROR;
}

bool core::Texture::setBorderColor(const glm::vec4 & color) const
{
	std::lock_guard guard{ mutex };
	setBoundHandle(m_type, m_handle);
	glTexParameterfv(static_cast<GLenum>(m_type), GL_TEXTURE_BORDER_COLOR, &color[0]);
	return !HAS_GL_ERROR;
}
bool core::Texture::setFilter(FilterType min, FilterType mag) const
{
	std::lock_guard guard{ mutex };
	setBoundHandle(m_type, m_handle);
	glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min));
	glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag));
	return !HAS_GL_ERROR;
}
bool core::Texture::setWrap(WrapType type) const
{
	std::lock_guard guard{ mutex };
	setBoundHandle(m_type, m_handle);
	glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_S, static_cast<GLint>(type));
	glTexParameteri(static_cast<GLenum>(m_type), GL_TEXTURE_WRAP_T, static_cast<GLint>(type));
	return !HAS_GL_ERROR;
}
