
#include "BlockRenderLoader.h"

#include "world/BlockRegistry.h"
#include "world/render/BlockModelLoader.h"
#include "world/render/BlockRender.h"
#include "world/render/BlockRenderRegistry.h"
#include "world/render/BlockTextureLoader.h"
#include "world/util/Side.h"

#include <plog/Log.h>

namespace
{
	constexpr const char * NODE_VARIANT = "variant";
	constexpr const char * NODE_MODEL = "model";
	constexpr const char * NODE_TEXTURE = "texture";
	constexpr const char * NODE_OCCLUDE = "occlude";

	constexpr const char * ATTR_VARIANT_NAME = "name";
	constexpr const char * ATTR_MODEL_PATH = "path";
}

// ...

void vox::BlockRenderLoader::loadBlocks(const util::Folder & folder) const
{
	for (const auto & it : folder.files())
		loadBlock(it);
	for (const auto & it : folder.folders())
		loadBlocks(it);
}
void vox::BlockRenderLoader::loadBlock(const util::File & file) const
{
	LOG_INFO << "Loading block render from " << file.path() << "...";

	pugi::xml_document doc;
	doc.load_file(file.path().c_str());

	for (auto variant = doc.first_child(); variant; variant = variant.next_sibling())
	{
		if (std::strcmp(NODE_VARIANT, variant.name()) == 0)
		{
			const std::string attrName = variant.attribute(ATTR_VARIANT_NAME).as_string();
			const std::string blockName = file.name() + (attrName.empty() ? "" : ":" + attrName);

			LOG_INFO << "Loading block render variant " << blockName << "...";
			const auto id = m_registry[blockName].m_id;
			BlockRenderVariantLoader loader{ m_root, m_registry, m_atlas, m_blocks[id] };
			loader.loadVariant(variant);
		}
	}
}

// ...

void vox::BlockRenderVariantLoader::loadVariant(const pugi::xml_node & variant) const
{
	if (const auto node = variant.child(NODE_MODEL))
		loadModel(node);
	if (const auto node = variant.child(NODE_TEXTURE))
		loadTexture(node);
	loadData(variant);
}

void vox::BlockRenderVariantLoader::loadModel(const pugi::xml_node & model) const
{
	const auto attrFile = model.attribute(ATTR_MODEL_PATH).as_string();
	const auto file = m_root.folder("models").file(attrFile);
	if (!file.exists())
	{
		LOG_WARNING << "Cound not find model file " << file.path();
		return;
	}

	BlockModelLoader{ m_block.m_model }.loadModel(file);
}
void vox::BlockRenderVariantLoader::loadTexture(const pugi::xml_node & texture) const
{
	for (auto attr = texture.first_attribute(); attr; attr = attr.next_attribute())
	{
		const auto file = m_root.folder("textures").file(attr.as_string());
		if (!file.exists())
		{
			LOG_WARNING << "Cound not find texture file " << file.path();
			continue;
		}

		BlockTexture blockTexture;
		BlockTextureLoader{ m_registry, m_atlas, blockTexture }.load(file);
		for (const auto & side : Side::fromList(attr.name()))
			m_block.m_texture[side.id()] = blockTexture;
	}
}
void vox::BlockRenderVariantLoader::loadData(const pugi::xml_node & variant) const
{
	// Load face occlusion
	const auto occlude = variant.child(NODE_OCCLUDE);
	for (auto attr = occlude.first_attribute(); attr; attr = attr.next_attribute())
	{
		for (const auto & side : Side::fromList(attr.name()))
			m_block.m_occlude[side.id()] = attr.as_bool(true);
	}
}
