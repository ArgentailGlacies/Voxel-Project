#pragma once

#include "io/File.h"
#include "io/Folder.h"

#include <pugixml/pugixml.hpp>

namespace vox
{
	struct Block;
	class BlockRegistry;

	class BlockLoader
	{
	public:
		BlockLoader() = delete;
		BlockLoader(BlockRegistry & registry) noexcept : m_registry(registry) {}

		void loadBlocks(const util::Folder & folder) const;
		void loadBlock(const util::File & file) const;

	private:
		BlockRegistry & m_registry;
	};

	class BlockVariantLoader
	{
	public:
		BlockVariantLoader() = delete;
		BlockVariantLoader(Block & block) : m_block(block) {}

		void loadVariant(const pugi::xml_node & variant) const;
		void loadPhase(const pugi::xml_node & variant) const;
		void loadLight(const pugi::xml_node & variant) const;

	private:
		Block & m_block;
	};
}