#pragma once

#include "world/data/Chunk.h"

#include <glm/gtx/hash.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <map>
#include <unordered_map>

namespace vox
{
	class ChunkStorage
	{
	public:
		ChunkStorage() = default;
		ChunkStorage(const ChunkStorage &) = delete;
		ChunkStorage(ChunkStorage &&) = delete;
		~ChunkStorage() = default;

		ChunkStorage & operator=(const ChunkStorage &) = delete;
		ChunkStorage & operator=(ChunkStorage &&) = delete;

		/**
			Generates a new chunk at the provided position. The position has to be specified in
			chunk coordinates. If another chunk has already been created at that position, this
			method will do nothing.

			@param cpos The position to create the chunk at.
		*/
		void create(const glm::ivec3 & cpos);
		/**
			Destroys the chunk at the provided position. The position has to be specified in chunk
			coordinates. If no chunk exists at that position, this method will do nothing.

			@param cpos The position to destroy a chunk from.
		*/
		void destroy(const glm::ivec3 & cpos);
		/**
			Checks if a chunk exists at the specified position. The position has to be specified in
			chunk coordinates.

			@param cpost The position to check if contains a chunk.
			@return True iff a chunk exists at the specified position.
		*/
		bool has(const glm::ivec3 & cpos) const;
		/**
			Retrieves the chunk at the specified position. The position has to be specified in chunk
			coordinates.

			@param cpost The position to retrieve a chunk from.
			@return The chunk if it exists, otherwise nullptr.
		*/
		Chunk * getExact(const glm::ivec3 & cpos) const;
		/**
			Retrieves the chunk above the specified position. The position has to be specified in
			chunk coordinates.

			@param cpost The position to search above for a chunk.
			@return The chunk above the specified position if it exists, otherwise nullptr.
		*/
		Chunk * getAbove(const glm::ivec3 & cpos) const;
		/**
			Retrieves the chunk below the specified position. The position has to be specified in
			chunk coordinates.

			@param cpost The position to search below for a chunk.
			@return The chunk below the specified position if it exists, otherwise nullptr.
		*/
		Chunk * getBelow(const glm::ivec3 & cpos) const;
		/**
			Retrieves the uppermost chunk at the specified position. The position must be specified
			in chunk coordinates.

			@param cpos The position to look at.
			@return The uppermost chunk if it exists, otherwise nullptr.
		*/
		Chunk * getUppermost(const glm::ivec2 & cpos) const;
		/**
			Retrieves the lowermost chunk at the specified position. The position must be specified
			in chunk coordinates.

			@param cpos The position to look at.
			@return The lowermost chunk if it exists, otherwise nullptr.
		*/
		Chunk * getLowermost(const glm::ivec2 & cpos) const;

		// ...

		/**
			Retrieves the number of chunks stored within the storage.

			@return The number of chunks.
		*/
		unsigned int count() const;

	private:
		using Column = std::map<int, Chunk>;
		using Grid = std::unordered_map<glm::ivec2, Column>;

		mutable Grid m_chunks;
	};
}