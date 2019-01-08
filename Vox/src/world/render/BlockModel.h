#pragma once

#include <glm/vec3.hpp>
#include <vector>

namespace vox
{
	struct BlockVertex
	{
		glm::vec3 m_position;
		glm::vec3 m_normal;
		glm::vec3 m_uv;
	};

	struct BlockModel
	{
		std::vector<BlockVertex> m_vertices[7];
		std::vector<unsigned int> m_indices[7];
	};
}