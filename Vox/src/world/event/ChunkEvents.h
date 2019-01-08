#pragma once

#include <glm/vec3.hpp>

namespace vox
{
	/**
		Dispatched when a chunk is created.
	*/
	struct ChunkCreated
	{
		/* The position of the chunk that was created, in chunk coordinates. */
		const glm::ivec3 m_cpos;
	};
	/**
		Dispatched when a chunk is removed.
	*/
	struct ChunkDestroyed
	{
		/* The position of the chunk that was removed, in chunk coordinates. */
		const glm::ivec3 m_cpos;
	};
	/**
		Dispatched when a chunk is modified.
	*/
	struct ChunkChanged
	{
		/* The position of the chunk that was changed, in chunk coordinates. */
		const glm::ivec3 m_cpos;
	};
	/**
		Dispatched when a chunk should have light calculations applied
	*/
	struct ChunkLighten
	{
		/* The position of the chunk that should be updated, in chunk coordinates. */
		const glm::ivec3 m_cpos;
	};
	/**
		Dispatched when a chunk should have shadow calculations applied
	*/
	struct ChunkDarken
	{
		/* The position of the chunk that should be updated, in chunk coordinates. */
		const glm::ivec3 m_cpos;
	};
}