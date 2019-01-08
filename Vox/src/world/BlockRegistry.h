#pragma once

#include "world/Block.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace vox
{
	const std::string BLOCK_AIR_NAME = "air";
	const unsigned int BLOCK_AIR_ID = 0;

	class BlockRegistry
	{
	public:
		BlockRegistry();
		BlockRegistry(const BlockRegistry &) = delete;
		BlockRegistry(BlockRegistry &&) = default;
		~BlockRegistry() = default;

		BlockRegistry & operator=(const BlockRegistry &) = delete;
		BlockRegistry & operator=(BlockRegistry &&) = delete;

		/**
			Checks if the block with the given id exists within the block registry. Only blocks that
			have explicitly been added will be considered as existing.

			@return True iff the block exists.
		*/
		bool has(unsigned int id) const;
		/**
			Checks if the block with the given name exists within the block registry. Only blocks
			that have explicitly been added will be considered as existing.

			@return True iff the block exists.
		*/
		bool has(const std::string & name) const;

		/**
			Accesses the block with the given id. If the block with the given id does not exist, the
			air block is returned instead.

			@param id The id of the block to access.
			@return The block with the given id or the air block.
		*/
		const Block & operator[](unsigned int id) const;
		/**
			Accesses the block with the given name. If the block with the given name does not exist,
			the air block is returned instead.

			@param name The name of the block to access.
			@return The block with the given name or the air block.
		*/
		const Block & operator[](const std::string & name) const;

		/**
			Retrieves the number of blocks stored in the registry.

			@return The number of registered blocks.
		*/
		inline auto size() const { return m_blocks.size(); }

		/**
			Adds a block with the provided name to the registry. The name must be unique. If another
			block with the given name exists, the exception std::invalid_argument is thrown. If
			there is not enough space to allocated one more block, std::runtime_error is throw.

			@param name The name of the new block to add.
			@return A reference to the block. The reference will be invalid after the next add call.
		*/
		Block & add(const std::string & name);
		/**
			Adds a block with the provided name and id to the registry. The name and id must be
			unique. If another block with the given name or id exists, the exception
			std::invalid_argument is thrown. If there is not enough space to allocated one more
			block, std::runtime_error is throw.

			@param name The name of the new block to add.
			@param id The id of the new block to add.
			@return A reference to the block. The reference will be invalid after the next add call.
		*/
		Block & add(const std::string & name, unsigned int id);
		/**
			Retrieves the block with the given name if it exists within the system. If the block
			does not exist, the std::invalid_argument exception is thrown.

			@param name The name of the block to retrieve.
			@return The block if it exists.
		*/
		Block & get(const std::string & name);

	private:
		std::vector<Block> m_blocks;
		std::unordered_map<std::string, unsigned int> m_names;
	};
}