
#include "Attribute.h"

#include "opengl/OpenGL.h"

#include <plog/Log.h>

core::Attribute::Attribute(
	unsigned int index,
	unsigned int count,
	unsigned int offset,
	unsigned int stride,
	DataFormat format
) : m_index(index), m_count(count), m_offset(offset), m_stride(stride), m_format(format)
{}

void core::Attribute::enable() const
{
	glEnableVertexAttribArray(m_index);
	HAS_GL_ERROR;

	switch (m_format)
	{
	case DataFormat::BYTE:
	case DataFormat::INT:
	case DataFormat::SHORT:
	case DataFormat::UNSIGNED_BYTE:
	case DataFormat::UNSIGNED_INT:
	case DataFormat::UNSIGNED_SHORT:
		glVertexAttribIPointer(m_index, m_count, static_cast<GLenum>(m_format), m_stride, (const GLvoid*)m_offset);
		HAS_GL_ERROR;
		break;
	case DataFormat::FLOAT:
		glVertexAttribPointer(m_index, m_count, static_cast<GLenum>(m_format), GL_FALSE, m_stride, (const GLvoid*)m_offset);
		HAS_GL_ERROR;
		break;
	default:
		LOG_WARNING << "Unsupported data format " << static_cast<GLenum>(m_format);
	}
}
void core::Attribute::disable() const
{
	glDisableVertexAttribArray(m_index);
	HAS_GL_ERROR;
}
