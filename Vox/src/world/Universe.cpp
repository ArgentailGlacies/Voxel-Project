
#include "Universe.h"

#include "asset/AssetRegistry.h"
#include "core/Resources.h"
#include "scene/Scene.h"
#include "world/data/BlockLoader.h"
#include "world/render/BlockRenderLoader.h"

namespace
{
	const util::Folder DATA_BLOCKS = "blocks";
	const util::File DATA_BLOCK_REGISTRY = "blocks.xml";

	constexpr const char * ATTR_NAME = "name";
	constexpr const char * ATTR_ID = "id";
}

vox::Universe::Universe(const util::Folder & data, core::Scene & scene)
	: m_data(data), m_scene(scene)
{}

void vox::Universe::process()
{
	for (auto & [_, render] : m_renders)
		render.process();
}

void vox::Universe::prepare(const core::AssetRegistry & assets)
{
	m_textureAtlas = assets.get<core::Texture>(core::res::asset::BLOCK_TEXTURE_ATLAS);

	loadBlockRegistry();
	loadBlockRenderRegistry();
	storeBlockRegistry();
}

void vox::Universe::loadBlockRegistry()
{
	pugi::xml_document doc;
	doc.load_file(m_data.file(DATA_BLOCK_REGISTRY).path().c_str());

	// Load up all block ids first, to ensure that any new blocks will not claim used block ids
	for (auto child = doc.first_child(); child; child = child.next_sibling())
	{
		const std::string name = child.attribute(ATTR_NAME).as_string();
		const auto id = child.attribute(ATTR_ID).as_uint();

		m_blockRegistry.add(name, id);
	}

	// Now new blocks may be loaded up and added to the system
	BlockLoader{ m_blockRegistry }.loadBlocks(m_data.folder(DATA_BLOCKS));
}
void vox::Universe::storeBlockRegistry() const
{
	pugi::xml_document doc;
	for (unsigned int i = 1; i < m_blockRegistry.size(); ++i)
	{
		if (m_blockRegistry.has(i))
		{
			auto child = doc.append_child("block");
			child.append_attribute(ATTR_NAME).set_value(m_blockRegistry[i].m_name.c_str());
			child.append_attribute(ATTR_ID).set_value(m_blockRegistry[i].m_id);
		}
	}
	doc.save_file(m_data.file(DATA_BLOCK_REGISTRY).path().c_str());
}
void vox::Universe::loadBlockRenderRegistry()
{
	auto * atlas = reinterpret_cast<BlockTextureAtlas *>(m_textureAtlas.operator->());

	BlockRenderLoader loader{ m_data, m_blockRegistry, m_blockRenderRegistry, *atlas };
	loader.loadBlocks(m_data.folder(DATA_BLOCKS));

	atlas->build();
	atlas->setFilter(core::FilterType::LINEAR_MIPMAP_LINEAR, core::FilterType::NEAREST);
	atlas->generateMipmaps();
}

// ...

vox::World & vox::Universe::createWorld(const std::string & name, bool autosave)
{
	if (hasWorld(name))
		throw std::invalid_argument("World '" + name + "' already exists.");

	const auto itWorld = m_worlds.emplace(name, m_blockRegistry).first;
	const auto itRender = m_renders.emplace(std::piecewise_construct,
		std::forward_as_tuple(name),
		std::forward_as_tuple(*this, itWorld->second, m_scene)
	).first;
	return itWorld->second;
}
void vox::Universe::destroyWorld(const std::string & name)
{
	if (!hasWorld(name))
		return;

	m_renders.erase(name);
	m_worlds.erase(name);
}
bool vox::Universe::hasWorld(const std::string & name) const
{
	return m_worlds.find(name) != m_worlds.end();
}
vox::World & vox::Universe::getWorld(const std::string & name) const
{
	const auto it = m_worlds.find(name);
	if (it == m_worlds.end())
		throw std::invalid_argument("World '" + name + "' does not exist");
	return it->second;
}