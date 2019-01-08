
#include "UBO.h"

#include "opengl/OpenGL.h"

#include <plog/Log.h>

core::UBO::UBO(unsigned int port, BufferUse use)
	: m_port(port), m_size(0u), m_vbo{ BufferType::UNIFORM_BUFFER, use }
{}

bool core::UBO::build() const
{
	if (!m_vbo.reserve(m_size))
		return false;
	glBindBufferBase(GL_UNIFORM_BUFFER, m_port, handle());
	return !HAS_GL_ERROR;
}

bool core::UBO::has(const std::string & member) const
{
	return m_offsets.find(member) != m_offsets.end();
}
bool core::UBO::add(const std::string & member, unsigned int bytes)
{
	if (has(member))
	{
		LOG_WARNING << "Field '" << member << "' already exists in ubo '" << handle() << "'";
		return false;
	}
	m_offsets[member] = m_size;
	m_sizes[member] = bytes;
	m_size += bytes;
	LOG_INFO << "Added field '" << member << "' to ubo '" << handle() << "'";
	return true;
}
bool core::UBO::get(const std::string & member, unsigned int bytes, void * data) const
{
	const auto offset = m_offsets.find(member);
	const auto size = m_sizes.find(member);

	if (offset == end(m_offsets) || size == end(m_sizes))
		LOG_WARNING << "Field '" << member << "' was not found in ubo '" << handle() << "'";
	else if (size->second != bytes)
		LOG_WARNING << "Byte count '" << bytes << "' does not match expected size (" <<
		size->second << ") at field '" << member << "' in ubo '" << handle() << "'";
	else
		return m_vbo.read(bytes, data, offset->second);
	return false;
}
bool core::UBO::set(const std::string & member, unsigned int bytes, const void * data) const
{
	const auto offset = m_offsets.find(member);
	const auto size = m_sizes.find(member);

	if (offset == end(m_offsets) || size == end(m_sizes))
		LOG_WARNING << "Member '" << member << "' was not found in ubo '" << handle() << "'";
	else if (size->second != bytes)
		LOG_WARNING << "Byte count '" << bytes << "' does not match expected size (" <<
			size->second << ") at field '" << member << "' in ubo '" << handle() << "'";
	else
		return m_vbo.write(bytes, data, offset->second);
	return false;
}
