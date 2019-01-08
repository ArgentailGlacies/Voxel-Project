#pragma once

#include "io/File.h"

#include <pugixml/pugixml.hpp>

namespace vox
{
	class BlockRegistry;
	struct BlockTexture;
	class BlockTextureAtlas;

	class BlockTextureLoader
	{
	public:
		BlockTextureLoader() = delete;
		BlockTextureLoader(
			const BlockRegistry & registry,
			BlockTextureAtlas & atlas,
			BlockTexture & texture
		) noexcept : m_registry(registry), m_atlas(atlas), m_texture(texture) {}

		void load(const ::util::File & file);

		void loadTexture(const pugi::xml_node & texture);
		void loadConnection(const pugi::xml_node & connection);
		/**
			Loads a normal connection, where the texture index is the base texture index of the
			block texture plus the starting index. There are no special effects associated with
			this connection type.

			@attribute offset The offset index of the texture.

			@example <onnection type="default" offset="2" />
		*/
		void loadDefaultConnection(const pugi::xml_node & node);
		/**
			Loads a randomized connection, where the texture index is the base texture index plus a
			random offset. The offset is determined by the position and facing of the block face
			which is being textured.

			@attribute offset The offset index of the texture.
			@attribute count The number of possible tiles to chose from.

			@example <connection type="random" offset="4" count="6" />
		*/
		void loadRandomConnection(const pugi::xml_node & node);
		/**
			Loads a repeating connection, where the texture index is the base texture index plus a
			specific offset. The offset is determined by the position and facing of the block face
			which is being textured. The row size and total possible tiles should be powers of two.

			@attribute offset The offset index of the texture.
			@attribute count The total number of tiles forming the grid to repeat.
			@attribute row The number of tiles composing one row to repeat.

			@example <connection type="repeat" row="4" count="8" />
		*/
		void loadRepeatConnection(const pugi::xml_node & node);
		/**
			Not implemented.
		*/
		void loadConnectAllConnection(const pugi::xml_node & node);
		/**
			Loads a texture connected in the horizontal direction, where the texture index is the
			base texture index plus an offset determined by neighboring blocks. If matching is
			enabled, the outermost tiles will have the same offset as the center tile. Center tiles
			will always be randomized.

			@attribute offset The offset index of the texture.
			@attribute count The number of possible tiles to chose from.
			@attribute blocks A list of blocks that can be connected to.
			@attribute match Whether the edge tiles should match the center tile or not.

			@example <connection type="connect-horizontal" blocks="log" match="true" />
		*/
		void loadConnectHorConnection(const pugi::xml_node & node);
		/**
			Loads a texture connected in the vertical direction, where the texture index is the
			base texture index plus an offset determined by neighboring blocks. If matching is
			enabled, the outermost tiles will have the same offset as the center tile. Center tiles
			will always be randomized.

			@attribute offset The offset index of the texture.
			@attribute count The number of possible tiles to chose from.
			@attribute blocks A list of blocks that can be connected to.
			@attribute match Whether the edge tiles should match the center tile or not.

			@example <connection type="connect-vertical" blocks="dirt, grass" match="true" />
		*/
		void loadConnectVerConnection(const pugi::xml_node & node);

	private:
		const BlockRegistry & m_registry;
		BlockTextureAtlas & m_atlas;
		BlockTexture & m_texture;
	};
}