#pragma once

#include "world/render/BlockRender.h"

#include <vector>

namespace vox
{
	class BlockRenderRegistry
	{
	public:
		BlockRenderRegistry() noexcept;
		BlockRenderRegistry(const BlockRenderRegistry &) = delete;
		BlockRenderRegistry(BlockRenderRegistry &&) = delete;
		~BlockRenderRegistry() noexcept = default;

		BlockRenderRegistry & operator=(const BlockRenderRegistry &) = delete;
		BlockRenderRegistry & operator=(BlockRenderRegistry &&) = delete;

		/**
			Accesses the block with the given id. If the block with the given id does not exist, it
			is created.

			@param id The id of the block render to access.
			@return The block render with the given id.
		*/
		BlockRender & operator[](unsigned int id);
		/**
			Accesses the block with the given name. If the block render does not exist, an
			std::invalid_argument exception is thrown.

			@param id The id of the block render to access.
			@return The block render with the given idr.
		*/
		const BlockRender & operator[](unsigned int id) const;

	private:
		std::vector<BlockRender> m_blocks;
	};
}