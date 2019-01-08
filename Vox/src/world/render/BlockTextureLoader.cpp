
#include "BlockTextureLoader.h"

#include "util/StringParsing.h"
#include "world/BlockRegistry.h"
#include "world/data/BlockRegion.h"
#include "world/render/BlockTexture.h"
#include "world/render/BlockTextureAtlas.h"
#include "world/util/Limits.h"
#include "world/util/Side.h"

#include <allegro5/allegro.h>
#include <glm/gtx/hash.hpp>
#include <plog/Log.h>

namespace
{
	constexpr const char * NODE_TEXTURE = "texture";
	constexpr const char * NODE_CONNECTION = "connection";

	constexpr const char * ATTR_TEXTURE_PATH = "path";
	constexpr const char * ATTR_TEXTURE_OVERLAY = "overlay";
	constexpr const char * ATTR_TEXTURE_ROW = "row";
	constexpr const char * ATTR_CONNECTION_TYPE = "type";
	constexpr const char * ATTR_CONNECTION_BLOCKS = "blocks";
	constexpr const char * ATTR_CONNECTION_DIRECTION = "direction";
	constexpr const char * ATTR_CONNECTION_OFFSET = "offset";
	constexpr const char * ATTR_CONNECTION_COUNT = "count";
	constexpr const char * ATTR_CONNECTION_ROW = "row";
	constexpr const char * ATTR_CONNECTION_MATCH = "match";

	constexpr const char * VALUE_CONNECTION_DEFAULT = "default";
	constexpr const char * VALUE_CONNECTION_RANDOM = "random";
	constexpr const char * VALUE_CONNECTION_REPEAT = "repeat";
	constexpr const char * VALUE_CONNECTION_CONNECT_ALL = "connect-all";
	constexpr const char * VALUE_CONNECTION_CONNECT_HORIZONTAL = "connect-horizontal";
	constexpr const char * VALUE_CONNECTION_CONNECT_VETRICAL = "connect-vertical";

	// ...

	inline unsigned int calculateHash(const glm::ivec3 & pos, const vox::Side & side)
	{
		return std::hash<glm::ivec3>()(pos) ^ side.id();
	}
	
	inline std::vector<bool> obtainKeys(
		const std::vector<std::string> & blocks,
		const vox::BlockRegistry & registry
	)
	{
		std::vector<bool> keys(registry.size(), false);
		for (auto & block : blocks)
			keys[registry[block].m_id] = true;
		return keys;
	}

	inline unsigned int mapAll(
		const vox::BlockRegion & region,
		const glm::ivec3 & pos,
		const vox::Side & side,
		const std::vector<bool> & keys
	)
	{
		static const unsigned int mappings[] = {
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
		};

		return mappings[
			keys[region.read(pos - side.tangentH() + side.tangentV()).getType()] << 0 |
			keys[region.read(pos + side.tangentV()).getType()] << 1 |
			keys[region.read(pos + side.tangentH() + side.tangentV()).getType()] << 2 |
			keys[region.read(pos + side.tangentH()).getType()] << 3 |
			keys[region.read(pos + side.tangentH() - side.tangentV()).getType()] << 4 |
			keys[region.read(pos - side.tangentV()).getType()] << 5 |
			keys[region.read(pos - side.tangentH() - side.tangentV()).getType()] << 6 |
			keys[region.read(pos - side.tangentH()).getType()] << 7
		];
	}
	inline unsigned int mapHorizontal(
		const vox::BlockRegion & region,
		const glm::ivec3 & pos,
		const vox::Side & side,
		const std::vector<bool> & keys
	)
	{
		static const unsigned int mappings[] = { 0, 3, 1, 2 };

		return mappings[
			keys[region.read(pos + side.tangentH()).getType()] << 0 |
			keys[region.read(pos - side.tangentH()).getType()] << 1
		];
	}
	inline unsigned int mapVertical(
		const vox::BlockRegion & region,
		const glm::ivec3 & pos,
		const vox::Side & side,
		const std::vector<bool> & keys
	)
	{
		static const unsigned int mappings[] = { 0, 3, 1, 2 };

		return mappings[
			keys[region.read(pos + side.tangentV()).getType()] << 0 |
			keys[region.read(pos - side.tangentV()).getType()] << 1
		];
	}
}

void vox::BlockTextureLoader::load(const ::util::File & file)
{
	pugi::xml_document doc;
	doc.load_file(file.path().c_str());

	loadTexture(doc.child(NODE_TEXTURE));
	loadConnection(doc.child(NODE_CONNECTION));
}

void vox::BlockTextureLoader::loadTexture(const pugi::xml_node & texture)
{
	const std::string path = texture.attribute(ATTR_TEXTURE_PATH).as_string();
	const std::string overlay = texture.attribute(ATTR_TEXTURE_OVERLAY).as_string();
	const auto row = texture.attribute(ATTR_TEXTURE_ROW).as_bool(true);

	if (overlay.empty())
		m_texture.m_handle = m_atlas.attach(path, row);
	else
	{
		LOG_INFO << "Building overlay from '" << overlay << "' to '" << path << "'";
		auto base = gsl::make_not_null(al_load_bitmap(path.c_str()));
		auto over = gsl::make_not_null(al_load_bitmap(overlay.c_str()));

		auto * target = al_get_target_bitmap();
		al_set_target_bitmap(base);
		al_draw_bitmap(over, 0.0f, 0.0f, 0);
		al_set_target_bitmap(target);
		al_destroy_bitmap(over);
		m_texture.m_handle = m_atlas.attach(std::move(base), row);
	}
}

void vox::BlockTextureLoader::loadConnection(const pugi::xml_node & connection)
{
	const std::string attrType = connection.attribute(ATTR_CONNECTION_TYPE).as_string(VALUE_CONNECTION_DEFAULT);
	if (attrType == VALUE_CONNECTION_DEFAULT)
		loadDefaultConnection(connection);
	else if (attrType == VALUE_CONNECTION_RANDOM)
		loadRandomConnection(connection);
	else if (attrType == VALUE_CONNECTION_REPEAT)
		loadRepeatConnection(connection);
	else if (attrType == VALUE_CONNECTION_CONNECT_ALL)
		loadConnectAllConnection(connection);
	else if (attrType == VALUE_CONNECTION_CONNECT_HORIZONTAL)
		loadConnectHorConnection(connection);
	else if (attrType == VALUE_CONNECTION_CONNECT_VETRICAL)
		loadConnectVerConnection(connection);
	else
		LOG_WARNING << "Unknown texture connection " << attrType;
}
void vox::BlockTextureLoader::loadDefaultConnection(const pugi::xml_node & node)
{
	const auto offset = node.attribute(ATTR_CONNECTION_OFFSET).as_uint();
	const auto handle = m_texture.m_handle + offset;

	m_texture.m_lookup = [handle](auto & region, auto & pos, auto & side)
	{
		return TextureData{ handle, 1, 0 };
	};
}
void vox::BlockTextureLoader::loadRandomConnection(const pugi::xml_node & node)
{
	const auto offset = node.attribute(ATTR_CONNECTION_OFFSET).as_uint();
	const auto count = node.attribute(ATTR_CONNECTION_COUNT).as_uint(1);
	const auto handle = m_texture.m_handle + offset;

	m_texture.m_lookup = [handle, count](auto & region, auto & pos, auto & side)
	{
		return TextureData{ handle, count, calculateHash(pos, side) };
	};
}
void vox::BlockTextureLoader::loadRepeatConnection(const pugi::xml_node & node)
{
	const auto offset = node.attribute(ATTR_CONNECTION_OFFSET).as_uint();
	const auto row = node.attribute(ATTR_CONNECTION_ROW).as_uint(1);
	const auto count = node.attribute(ATTR_CONNECTION_COUNT).as_uint(1);
	const auto handle = m_texture.m_handle + offset;

	m_texture.m_lookup = [handle, row, count](auto & region, auto & pos, auto & side)
	{
		const auto index = (pos.x + row * (CHUNK_SIZE<int> - 1 - pos.y)) % count;
		return TextureData{ handle + index, 1, 0 };
	};
}
void vox::BlockTextureLoader::loadConnectAllConnection(const pugi::xml_node & node)
{
}
void vox::BlockTextureLoader::loadConnectHorConnection(const pugi::xml_node & node)
{
	const auto blocks = util::split(node.attribute(ATTR_CONNECTION_BLOCKS).as_string(), ", ");
	const auto keys = obtainKeys(blocks, m_registry);
	const auto offset = node.attribute(ATTR_CONNECTION_OFFSET).as_uint();
	const auto count = node.attribute(ATTR_CONNECTION_COUNT).as_uint(1);
	const auto match = node.attribute(ATTR_CONNECTION_MATCH).as_bool();
	const auto handle = m_texture.m_handle + offset;

	// TODO: Change 'match' to 'mode' and introduce repeated mode

	if (match)
	{
		m_texture.m_lookup = [handle, count, keys](auto & region, auto & pos, auto & side)
		{
			const auto i = mapHorizontal(region, pos, side, keys);
			const auto s = i == 3 ? pos + side.tangentH() : i == 1 ? pos - side.tangentH() : pos;
			return TextureData{ handle + i * count, count, calculateHash(s, side) };
		};
	}
	else
	{
		m_texture.m_lookup = [handle, count, keys](auto & region, auto & pos, auto & side)
		{
			const auto i = mapHorizontal(region, pos, side, keys);
			return TextureData{ handle + i * count, count, calculateHash(pos, side) };
		};
	}
}
void vox::BlockTextureLoader::loadConnectVerConnection(const pugi::xml_node & node)
{
	const auto blocks = util::split(node.attribute(ATTR_CONNECTION_BLOCKS).as_string(), ", ");
	const auto keys = obtainKeys(blocks, m_registry);
	const auto offset = node.attribute(ATTR_CONNECTION_OFFSET).as_uint();
	const auto count = node.attribute(ATTR_CONNECTION_COUNT).as_uint(1);
	const auto match = node.attribute(ATTR_CONNECTION_MATCH).as_bool();
	const auto handle = m_texture.m_handle + offset;

	// TODO: Change 'match' to 'mode' and introduce repeated mode

	if (match)
	{
		m_texture.m_lookup = [handle, count, keys](auto & region, auto & pos, auto & side)
		{
			const auto i = mapVertical(region, pos, side, keys);
			const auto s = i == 3 ? pos + side.tangentV() : i == 1 ? pos - side.tangentV() : pos;
			return TextureData{ handle + i * count, count, calculateHash(s, side) };
		};
	}
	else
	{
		m_texture.m_lookup = [handle, count, keys](auto & region, auto & pos, auto & side)
		{
			const auto i = mapVertical(region, pos, side, keys);
			return TextureData{ handle + i * count, count, calculateHash(pos, side) };
		};
	}
}
