
#include "BlockLoader.h"

#include "util/StringParsing.h"
#include "world/Block.h"
#include "world/BlockRegistry.h"
#include "world/util/Limits.h"

#include <plog/Log.h>

namespace
{
	constexpr const char * NODE_LIGHT = "light";
	constexpr const char * NODE_VARIANT = "variant";

	constexpr const char * ATTR_LIGHT_EMIT = "emit";
	constexpr const char * ATTR_LIGHT_ABSORB = "absorb";
	constexpr const char * ATTR_LIGHT_FILTER = "filter";
	constexpr const char * ATTR_PHASE = "phase";
	constexpr const char * ATTR_VARIANT_NAME = "name";

	constexpr const char * VALUE_PHASE_GAS = "gas";
	constexpr const char * VALUE_PHASE_LIQUID = "liquid";
	constexpr const char * VALUE_PHASE_SOLID = "solid";
}

// ...

void vox::BlockLoader::loadBlocks(const util::Folder & folder) const
{
	for (const auto & it : folder.files())
		loadBlock(it);
	for (const auto & it : folder.folders())
		loadBlocks(it);
}
void vox::BlockLoader::loadBlock(const util::File & file) const
{
	LOG_INFO << "Loading blocks from " << file.path() << "...";

	pugi::xml_document doc;
	doc.load_file(file.path().c_str());

	for (auto variant = doc.first_child(); variant; variant = variant.next_sibling())
	{
		if (std::strcmp(NODE_VARIANT, variant.name()) == 0)
		{
			const std::string attrName = variant.attribute(ATTR_VARIANT_NAME).as_string();
			const std::string blockName = file.name() + (attrName.empty() ? "" : ":" + attrName);

			LOG_INFO << "Loading block variant " << blockName << "...";
			if (m_registry.has(blockName))
				BlockVariantLoader{ m_registry.get(blockName) }.loadVariant(variant);
			else
				BlockVariantLoader{ m_registry.add(blockName) }.loadVariant(variant);
		}
	}
}

// ...

void vox::BlockVariantLoader::loadVariant(const pugi::xml_node & variant) const
{
	loadPhase(variant);
	loadLight(variant);
}
void vox::BlockVariantLoader::loadPhase(const pugi::xml_node & variant) const
{
	const std::string attrPhase = variant.attribute(ATTR_PHASE).as_string(VALUE_PHASE_SOLID);

	if (attrPhase == VALUE_PHASE_GAS)
		m_block.m_phase = BlockPhase::GAS;
	else if (attrPhase == VALUE_PHASE_LIQUID)
		m_block.m_phase = BlockPhase::LIQUID;
	else if (attrPhase == VALUE_PHASE_SOLID)
		m_block.m_phase = BlockPhase::SOLID;
	else
		LOG_WARNING << "Unknown block phase " << attrPhase;
}
void vox::BlockVariantLoader::loadLight(const pugi::xml_node & variant) const
{
	const auto light = variant.child(NODE_LIGHT);

	const std::string attrAbsorb = light.attribute(ATTR_LIGHT_ABSORB).as_string();
	const std::string attrEmit = light.attribute(ATTR_LIGHT_EMIT).as_string();
	const std::string attrFilter = light.attribute(ATTR_LIGHT_FILTER).as_string();

	util::Parser<glm::uvec4> parser;
	m_block.m_lightAbsorbed = parser.parse(attrAbsorb, glm::uvec4{ MAX_BLOCK_LIGHT });
	m_block.m_lightEmitted = parser.parse(attrEmit, glm::uvec4{});
	m_block.m_lightFiltered = parser.parse(attrFilter, glm::uvec4{ MAX_BLOCK_LIGHT });
}
