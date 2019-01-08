#pragma once

#include "world/BlockRegistry.h"

namespace vox
{
	inline BlockRegistry mockBlockRegistry()
	{
		BlockRegistry registry;

		auto & stone = registry.add("solid");
		stone.m_lightAbsorbed = glm::uvec4{ 31, 31, 31, 31 };
		stone.m_lightFiltered = glm::uvec4{ 31, 31, 31, 31 };

		auto & light = registry.add("light");
		light.m_lightAbsorbed = glm::uvec4{ 31, 31, 31, 31 };
		light.m_lightEmitted = glm::uvec4{ 17, 5, 23, 31 };
		light.m_lightFiltered = glm::uvec4{ 31, 31, 31, 31 };

		auto & glow = registry.add("glow");
		glow.m_lightAbsorbed = glm::uvec4{ 31, 31, 31, 31 };
		glow.m_lightEmitted = glm::uvec4{ 31, 31, 31, 0 };
		glow.m_lightFiltered = glm::uvec4{ 31, 31, 31, 31 };

		return registry;
	}
}