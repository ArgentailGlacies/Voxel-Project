#pragma once

#include <glm/vec4.hpp>
#include <string>

namespace vox
{
	enum class BlockPhase
	{
		SOLID,
		LIQUID,
		GAS
	};

	struct Block
	{
		std::string m_name;
		unsigned int m_id;

		BlockPhase m_phase;

		glm::uvec4 m_lightAbsorbed; // Will absorb this much light
		glm::uvec4 m_lightEmitted;  // Will output this much light
		glm::uvec4 m_lightFiltered; // Will allow at most this much light to go through
	};
	
	inline bool operator==(const Block & lhs, const Block & rhs) { return lhs.m_id == rhs.m_id; }
	inline bool operator!=(const Block & lhs, const Block & rhs) { return lhs.m_id != rhs.m_id; }
}