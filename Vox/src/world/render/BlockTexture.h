#pragma once

#include "world/render/TextureData.h"

#include <functional>
#include <glm/vec3.hpp>

namespace vox
{
	class BlockRegion;
	class Side;

	struct BlockTexture
	{
		std::function<TextureData(
			const BlockRegion & data,
			const glm::ivec3 & pos,
			const Side & side
		)> m_lookup = [](auto &, auto &, auto &) { return TextureData{}; };

		unsigned int m_handle = 0u;
	};
}