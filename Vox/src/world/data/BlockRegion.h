#pragma once

#include "world/data/BlockData.h"

#include <glm/vec3.hpp>
#include <vector>

namespace vox
{
	class BlockRegion
	{
	public:
		BlockRegion() noexcept;
		BlockRegion(const glm::ivec3 & pos, const glm::uvec3 & size) noexcept;
		BlockRegion(const BlockRegion &) = delete;
		BlockRegion(BlockRegion &&) noexcept = default;
		~BlockRegion() noexcept = default;

		BlockRegion & operator=(const BlockRegion &) = delete;
		BlockRegion & operator=(BlockRegion &&) noexcept = default;

		/**
			Reads the block at the specified position. The position should be specified relative to
			the zero coordinate. The position of the region is factored in internally. It is the
			caller's responsibility to ensure that the provided position is valid.

			@param pos The position to read the data from.
			@return The block data stored at the specified position.
		*/
		BlockData read(const glm::ivec3 & pos) const;

		/**
			Writes the given block data to the specified position. The position should be specified
			relative to the zero coordinate. The position of the region is factored in internally.
			It is the caller's responsibility to ensure that the position is valid.

			@param pos The position to store the data at.
			@oaram data The data to write to the specified position.
		*/
		void write(const glm::ivec3 & pos, BlockData data);

	private:
		std::vector<BlockData> m_data;

		glm::ivec3 m_pos;
		glm::ivec3 m_size;
	};
}