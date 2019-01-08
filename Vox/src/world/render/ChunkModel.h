#pragma once

#include "opengl/Mesh.h"
#include "scene/SceneNode.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace vox
{
	struct ChunkVertex
	{
		glm::vec3 m_position;
		glm::vec3 m_normal;
		glm::vec3 m_uv;
		glm::vec4 m_color;
		unsigned int m_texture;
	};

	/**
		The chunk model contains all meshes required to fully render the whole chunk geometry. It
		will also keep track of the renders the chunk needs to be fully rendered.
	*/
	struct ChunkModel
	{
		ChunkModel()
		{
			m_mesh.addAttribute({ 0, 3, 0, sizeof(ChunkVertex), core::DataFormat::FLOAT });
			m_mesh.addAttribute({ 1, 3, 12, sizeof(ChunkVertex), core::DataFormat::FLOAT });
			m_mesh.addAttribute({ 2, 3, 24, sizeof(ChunkVertex), core::DataFormat::FLOAT });
			m_mesh.addAttribute({ 3, 4, 36, sizeof(ChunkVertex), core::DataFormat::FLOAT });
			m_mesh.addAttribute({ 4, 1, 52, sizeof(ChunkVertex), core::DataFormat::UNSIGNED_INT });
		}

		inline bool empty() const { return m_mesh.empty(); }

		core::Mesh<ChunkVertex> m_mesh;
		core::SceneEntry m_transform;
		core::SceneEntry m_render;
	};
}