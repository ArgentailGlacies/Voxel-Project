#pragma once

#include "world/data/BlockData.h"
#include "world/data/ChunkQuery.h"

#include <glm/vec3.hpp>
#include <map>

namespace vox
{
	class WorldQuery
	{
	public:
		WorldQuery() = default;
		WorldQuery(const WorldQuery &) = delete;
		WorldQuery(WorldQuery &&) = default;
		~WorldQuery() = default;

		WorldQuery & operator=(const WorldQuery &) = delete;
		WorldQuery & operator=(WorldQuery &&) = default;

		/**
			Retrieves the number of blocks being queried.

			@return The number of blocks in the query.
		*/
		inline auto count() const { return m_count; }

		/**
			Retrieves the number of chunks being queried.

			@return The number of chunks in the query.
		*/
		inline auto size() const { return m_data.size(); }
		/**
			Checks if the query contains any chunks or not.

			@return True iff there are no chunks in the query.
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
			Checks if the query contains the block at the specified position. The positoin is in
			world coordinates.

			@return True iff the query contains the specified chunk.
		*/
		bool has(const glm::ivec3 & pos) const;
		/**
			Adds the given chunk query and stores it at the given chunk position. The position must
			be specified in chunk coordinates. If another chunk query has already been added to the
			specified position, the old query is overwritten.

			@param pos The chunk postion to add to the query.
			@param query The query which should be associated with the specified position.
		*/
		void add(const glm::ivec3 & cpos, ChunkQuery && query);
		/**
			Adds the given block to the query. The position is specified in world coordinate. If the
			specified position has been added already, the old value will be overwritten.

			@param pos The postion to add to the query.
			@param data The data which should be associated with the position in the query.
		*/
		void add(const glm::ivec3 & pos, const BlockData & data = {});

	private:
		/**
			When writing to a world, the most costly operation is light generation and propagation.
			By prioritizing chunks at the lower z-levels first, a faster approach for generating
			skylight may be utilized, which also costs less memory.
		*/
		struct Comparator
		{
			bool operator()(const glm::ivec3 & lhs, const glm::ivec3 & rhs) const
			{
				if (lhs.z < rhs.z)
					return true;
				if (lhs.z > rhs.z)
					return false;

				if (lhs.y < rhs.y)
					return true;
				if (lhs.y > rhs.y)
					return false;

				if (lhs.x < rhs.x)
					return true;
				if (lhs.x > rhs.x)
					return false;

				return false;
			}
		};

		std::map<glm::ivec3, ChunkQuery, Comparator> m_data;

		unsigned int m_count = 0;
	};
}