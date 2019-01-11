#pragma once

#include "world/util/Limits.h"

#include <glm/vec3.hpp>
#include <stdint.h>

namespace vox
{
	using Index = uint16_t;

	/**
		Returns the positional index of the given position. The size of the chunk is assumed to be
		a standard chunk. It is the caller's responsibility to ensure that the provided position is
		within the chunk volume.

		The chunk data is assumed to ordered according to data[z][y][x].

		@param pos The position within the chunk.
		@return The index of the position within a chunk of standard size.
	*/
	template<typename T> constexpr Index toIndex(const glm::tvec3<T> & pos);
	/**
		Returns the positional index of the given position. The size and offset of the region is
		specified by the caller. It is the caller's responsibility to ensure that the provided
		position is within the region volume.

		The region data is assumed to ordered according to data[z][y][x].

		@param pos The position within the region.
		@return The index of the position within a region.
	*/
	template<typename T> constexpr Index toIndex(
		const glm::tvec3<T> & pos,
		const glm::tvec3<T> & offset,
		const glm::tvec3<T> & size
	);

	/**
		Returns the position of the provided index. The size of the chunk is assumed to be the
		standard chunk size. It is the caller's responsibility to ensure that the provided index is
		within the chunk volume.

		The chunk data is assumed to ordered according to data[z][y][x].

		@param index The index within the chunk.
		@return The position of the index within a chunk of standard size.
	*/
	template<typename T> constexpr glm::tvec3<T> fromIndex(Index index);
}

namespace vox
{
	template<typename T>
	constexpr Index toIndex(const glm::tvec3<T> & pos)
	{
		return (pos.z * CHUNK_SIZE<T> + pos.y) * CHUNK_SIZE<T> + pos.x;
	}
	template<typename T>
	constexpr Index toIndex(
		const glm::tvec3<T> & pos,
		const glm::tvec3<T> & offset,
		const glm::tvec3<T> & size)
	{
		const auto delta = pos - offset;
		return (delta.z * size.y + delta.y) * size.x + delta.x;
	}

	template<typename T>
	constexpr glm::tvec3<T> fromIndex(Index index)
	{
		return {
			index % CHUNK_SIZE<T>,
			(index / CHUNK_SIZE<T>) % CHUNK_SIZE<T>,
			index / CHUNK_AREA<T>
		};
	}
}