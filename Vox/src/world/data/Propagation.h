#pragma once

#include "event/EventBus.h"
#include "world/data/BlockData.h"
#include "world/data/Chunk.h"
#include "world/util/Index.h"

#include <glm/vec3.hpp>

namespace vox
{
	class BlockRegistry;
	class ChunkStorage;
	class Side;

	struct PropagationContext
	{
		const core::EventBus & m_bus;
		const BlockRegistry & m_blocks;
		const ChunkStorage & m_chunks;
		Chunk::Writer & m_writer;
	};

	class PropagationBase
	{
	public:
		PropagationBase(const PropagationContext & context, const glm::ivec3 & cpos) noexcept
			: m_context(context), m_cpos(cpos)
		{}

		Index offset(Index source, const Side & side) const;
		Index offset(const glm::ivec3 & source, const Side & side) const;

	protected:
		const PropagationContext m_context;
		const glm::ivec3 m_cpos;
	};

	class PropagationLight : private PropagationBase
	{
	public:
		PropagationLight(const PropagationContext & context, const glm::ivec3 & cpos) noexcept
			: PropagationBase(context, cpos)
		{}

		/**
			Performs the full light spreading algorithm over the entire chunk. Will also mark
			adjacent chunks for light propagation if the light should spread out to them.
		*/
		void spread();
		/**
			Spreads a given amount of light to a position within a chunk. If the light should be
			spread further, another light propagation node is appended to the light propagation
			queue for the chunk.

			@param writer The chunk data writer which is currently in use.
			@param index The position the light should be spread to.
			@param light The light amount which should be spread to the given position.
		*/
		void spreadTo(Chunk::Writer & writer, Index index, Light light);
		/**
			Spreads a given amount of light from a position within a chunk. If the light should be
			spread to adjacent chunks, the actual light which should be spread is appended to that
			chunk's propagation queue.

			@param writer The chunk data writer which is currently in use.
			@param index The position the light should be spread from.
			@param light The light amount which should be spread to the given position.
		*/
		void spreadFrom(Chunk::Writer & writer, Index index, Light light);

		/**
			Reduces the light values going into the block. Blocks can absorb certain light, or
			filter it such that only that much light at most can pass through it.

			@param in The amount of light going into the block.
			@param absorbed The amount of light the block should absorb.
			@param filtered The maximum amount of light the block allows to pass through itself.
			@return The amount of light actually going into the block.
		*/
		Light applyBlockEffect(const Light & in, const Light & absorbed, const Light & filtered) const;
		unsigned int applyBlockEffect(unsigned char in, unsigned char absorbed, unsigned char filtered) const;
	};

	class PropagationDark : private PropagationBase
	{
	public:
		PropagationDark(const PropagationContext & context, const glm::ivec3 & cpos) noexcept
			: PropagationBase(context, cpos)
		{}

		/**
			Performs the full darkness spreading algorithm over the entire chunk. Will also mark
			adjacent chunks for darkness propagation if the shadow should spread out to them.
		*/
		void spread();
		/**
			Spreads a given amount of shadow to a position within a chunk. If the shadow should be
			spread further, another shadow propagation node is appended to the light propagation
			queue for the chunk.

			@param writer The chunk data writer which is currently in use.
			@param index The position the shadow should be spread to.
			@param light The light amount which should be spread to the given position.
		*/
		void spreadTo(Chunk::Writer & writer, Index index, Light light);
		/**
			Spreads a given amount of shadow from a position within a chunk. If the shadow should be
			spread to adjacent chunks, the shadow which should be spread is appended to that chunk's
			propagation queue.

			@param writer The chunk data writer which is currently in use.
			@param index The position the shadow should be spread from.
			@param light The light amount which should be spread to the given position.
		*/
		void spreadFrom(Chunk::Writer & writer, Index index, Light light);

		/**
			Reduces the shadow values going into the block. Blocks may already be completely dark or
			may emit light. If the block's light level is greater than the shadow to propagate, the
			shadow does not enter the block. If the block emits light, the block's light is set to
			that level, and the shadow to propagate further is set the to block's old light level.

			@param in The amount of shadow going into the block.
			@param current The amount of light currently at the block position.
			@param emitted The amount of light emitted by the block.
		*/
		void applyBlockEffect(Light & in, Light & current, const Light & emitted) const;
		void applyBlockEffect(unsigned char & in, unsigned char & current, unsigned char emitted) const;
	};
}