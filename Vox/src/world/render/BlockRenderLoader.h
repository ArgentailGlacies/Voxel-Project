#pragma once

#include "io/File.h"
#include "io/Folder.h"

#include <pugixml/pugixml.hpp>

namespace vox
{
	class BlockRegistry;
	struct BlockRender;
	class BlockRenderRegistry;
	class BlockTextureAtlas;

	class BlockRenderLoader
	{
	public:
		BlockRenderLoader() = delete;
		BlockRenderLoader(
			const util::Folder & root,
			const BlockRegistry & registry,
			BlockRenderRegistry & blocks,
			BlockTextureAtlas & atlas
		) noexcept : m_registry(registry), m_blocks(blocks), m_atlas(atlas), m_root(root) {}

		void loadBlocks(const util::Folder & folder) const;
		void loadBlock(const util::File & file) const;

	private:
		const BlockRegistry & m_registry;
		BlockRenderRegistry & m_blocks;
		BlockTextureAtlas & m_atlas;

		util::Folder m_root;
	};

	class BlockRenderVariantLoader
	{
	public:
		BlockRenderVariantLoader() = delete;
		BlockRenderVariantLoader(
			const util::Folder & root,
			const BlockRegistry & registry, 
			BlockTextureAtlas & atlas,
			BlockRender & block
		) noexcept : m_registry(registry), m_atlas(atlas), m_block(block), m_root(root) {}

		void loadVariant(const pugi::xml_node & variant) const;
		void loadModel(const pugi::xml_node & model) const;
		void loadTexture(const pugi::xml_node & texture) const;
		void loadData(const pugi::xml_node & variant) const;

	private:
		const BlockRegistry & m_registry;
		BlockTextureAtlas & m_atlas;
		BlockRender & m_block;

		util::Folder m_root;
	};
}