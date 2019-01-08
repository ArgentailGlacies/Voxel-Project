
#include "Mesh.h"

#include "opengl/OpenGL.h"

void core::MeshBase::render(RenderMode mode, unsigned int count) const
{
	m_vao.bind();
	glDrawElements(static_cast<GLenum>(mode), count, GL_UNSIGNED_INT, nullptr);
	HAS_GL_ERROR;
	m_vao.unbind();
}

bool core::MeshBase::build(
	unsigned int inum, const void * idata,
	unsigned int vnum, const void * vdata, unsigned int vsize
)
{
	if (inum == 0 || vnum == 0 || idata == nullptr || vdata == nullptr)
		return false;
	m_vao.bind();

	m_indexUBO.reserve(inum * sizeof(unsigned int), idata);
	m_vertexUBO.reserve(vnum * vsize, vdata);
	for (const auto & attribute : m_attributes)
		attribute.enable();

	m_vao.unbind();
	return true;
}