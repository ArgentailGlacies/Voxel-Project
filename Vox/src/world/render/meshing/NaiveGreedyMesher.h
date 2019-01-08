#pragma once

#include "world/data/BlockData.h"
#include "world/render/TextureData.h"
#include "world/util/Limits.h"

#include <array>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace vox
{
	class BlockRegion;
	class BlockRenderRegistry;
	struct ChunkModel;
	class Side;

	/**
		The layer data represents block and texture data combined. When meshing chunks, only block
		faces which share the exact same layer data may be merged.
	*/
	class LayerData
	{
	public:
		LayerData() noexcept = default;
		LayerData(const BlockData & block, const TextureData & texture) noexcept
			: m_block(block), m_texture(texture)
		{}

		inline bool operator==(const LayerData & other) const noexcept
		{
			return m_block == other.m_block && m_texture == other.m_texture;
		}
		inline bool operator!=(const LayerData & other) const noexcept
		{
			return m_block != other.m_block || m_texture != other.m_texture;
		}

		inline bool empty() const { return m_block == 0 && m_texture == 0; }
		inline auto block() const { return m_block; }
		inline auto texture() const { return m_texture; }

		inline void clear() { m_block = 0; m_texture = 0; }

	private:
		BlockData m_block;
		TextureData m_texture;
	};
	/**
		A layer is a collection of data gathered from a chunk and its surroundings. When meshing a
		chunk, the chunk is split into multiple layers. The chunk faces are then extracted from
		each layer.
	*/
	class Layer
	{
	public:
		/**
			Assigns the data at the specified position in the layer. The caller is responsible for
			ensuring that the position is valid.

			@param pos The position to store the provided data at.
			@param data The data to store at the specified position.
		*/
		void set(const glm::uvec2 & pos, const LayerData & data);
		/**
			Retrieves the data stored at the specified position in the layer. The caller is
			responsible for ensuring that the position is valid.

			@param pos The position to read from.
			@return The data stored at the specified position.
		*/
		LayerData & get(const glm::uvec2 & pos);
		const LayerData & get(const glm::uvec2 & pos) const;
		
		/**
			Removes a section of the layer. Once any sections have been extracted, the user should
			not change the data stored in the layer at all.

			@param data [out] The layer data that was extracted.
			@param position [out] The position at which the layer data started
			@param size [out] The total size of the segment that was extracted.
			@return True iff any data was extracted at all.
		*/
		bool extract(LayerData & data, glm::uvec2 & position, glm::uvec2 & size);

	private:
		bool nextExtractionPosition(glm::uvec2 & pos) const;
		glm::uvec2 findExtractionSize() const;

		std::array<LayerData, CHUNK_AREA<size_t>> m_data;
		glm::uvec2 m_pos{};
	};

	// ...

	class NaiveGreedyMesher
	{
	public:
		NaiveGreedyMesher() = delete;
		NaiveGreedyMesher(
			const BlockRenderRegistry & blocks,
			const BlockRegion & data,
			ChunkModel & model
		) noexcept : m_blocks(blocks), m_data(data), m_model(model) {}

		/**
			Performs the entire meshing task, meshes the entire chunk. Will not generate geometry
			for blocks that are occluded.
		*/
		void mesh();
		
		/**
			Constructs a layer along the axis of the specified side, at the specified layer depth.

			@param depth The distance into into the chunk along the axis determined by the side.
			@param side The side holding the normal vector of the layer plane.
			@return The layer at the given depth, along the normal vector specified by the side.
		*/
		Layer getLayer(unsigned int depth, const Side & side) const;
		/**
			Retrieves the data at the specified position in the internal block region. If the block
			should not be rendered from the specified side, no data will be returned.

			@param pos The position within the block region to read from.
			@param side The side of the block to read.
		*/
		LayerData getData(const glm::ivec3 & pos, const Side & side) const;

	private:
		void meshLayer(unsigned int depth, Layer && layer, const Side & side);
		void meshFace(
			const LayerData & face,
			const glm::vec3 & pos,
			const glm::vec3 & size,
			const Side & side
		);

		const BlockRenderRegistry & m_blocks;
		const BlockRegion & m_data;
		ChunkModel & m_model;

		unsigned int m_totalVertices = 0;
	};
}