#pragma once

#include "world/data/BlockData.h"
#include "world/util/Index.h"

#include <glm/vec3.hpp>
#include <vector>

namespace vox
{
	class ChunkQuery
	{
	public:
		struct Data
		{
			Data() = default;
			Data(Index index, BlockData data) : m_index(index), m_data(data) {}

			Index m_index;
			BlockData m_data;
		};

		ChunkQuery();
		ChunkQuery(const ChunkQuery &) = delete;
		ChunkQuery(ChunkQuery &&) = default;
		~ChunkQuery() = default;

		ChunkQuery & operator=(const ChunkQuery &) = delete;
		ChunkQuery & operator=(ChunkQuery &&) = default;

		/**
			Retrieves the lowest corner of the bounding box containing all blocks in the query. The
			coordinate is specified relative to the position of the chunk the query is applied to.

			@return The lower bounding box corner.
		*/
		inline auto min() const { return m_min; }
		/**
			Retrieves the highest corner of the bounding box containing all blocks in the query. The
			coordinate is specified relative to the position of the chunk the query is applied to.

			@return The upper bounding box corner.
		*/
		inline auto max() const { return m_max; }
		/**
			Retrieves the number of blocks being queried.

			@return The number of blocks in the query.
		*/
		inline auto size() const { return m_data.size(); }
		/**
			Checks if the query contains any blocks or not.

			@return True iff there are no blocks in the query.
		*/
		inline auto empty() const { return m_data.empty(); }

		/**
			@return The iterator pointing to the first entry in the query.
		*/
		inline auto begin() { return m_data.begin(); }
		/**
			@return The iterator pointing to the first entry in the query.
		*/
		inline auto begin() const { return m_data.begin(); }
		/**
			@return The iterator pointing beyond the last entry in the query.
		*/
		inline auto end() { return m_data.end(); }
		/**
			@return The iterator pointing beyond the last entry in the query.
		*/
		inline auto end() const { return m_data.end(); }

		/**
			Checks if the query contains the specified position. The position is specified relative
			to the chunk the block is contained in.

			@return True iff the query contains the specified position.
		*/
		bool has(const glm::uvec3 & pos) const;
		/**
			Adds the given position to the query, optionally writing in a specified data value into
			the query. The position is specified relative to the chunk the block is contained in. If
			no data value is provided, the query defaults to air. If the position has already been
			added, the old data will be overwritten.

			@param pos The postion to add to the query.
			@param data The data which should be associated with the position in the query.
		*/
		void add(const glm::uvec3 & pos, const BlockData & data = {});
		/**
			Adds the given region to the query, optionally writing in a specified data value into
			the query. The positions are specified relative to the chunk the block is contained in.
			if no data value is provided, the query defaults to air. If any of the region positions
			have already been added, the old data will be overwritten.

			@param start One corner of the region to add.
			@param end Another corner of the region to add.
			@param data The data which should be associated with the region in the query.
		*/
		void add(const glm::uvec3 & start, const glm::uvec3 & end, const BlockData & data = {});

	private:
		void insert(Index index, const BlockData & data);
		void bounds(const glm::uvec3 & min, const glm::uvec3 & max);

		std::vector<Data> m_data;

		glm::uvec3 m_min;
		glm::uvec3 m_max;
	};
}