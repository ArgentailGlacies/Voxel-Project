
#include "Propagation.h"

#include "util/MathOperations.h"
#include "world/BlockRegistry.h"
#include "world/data/ChunkStorage.h"
#include "world/event/ChunkEvents.h"
#include "world/util/Side.h"

namespace
{
	/**
		Retrieves the chunk which is offset in the specified direction from the chunk at the
		provided position. If the direction to look in is downwards, if any chunks exists below the
		specified position, that chunk is retrieved regardless if there is any empty space between
		that and the current chunk.

		@param context The context for light or darkness propagation.
		@param side The direction in which the retrieval should be.
		@param cpos The position of the chunk to retrieve from.
	*/
	vox::Chunk * getChunk(
		const vox::PropagationContext & context,
		const vox::Side & side,
		const glm::ivec3 & cpos
	)
	{
		if (side == vox::Side::NEG_Z)
			return context.m_chunks.getBelow(cpos);
		else
			return context.m_chunks.getExact(cpos + side.normal());
	}

	/**
		Marks the chunk off to the provided direction for light or dark propagation. If the chunk
		is below the current one, and the side is downwards, the chunk which is below (regardless of
		distance to it) will be selected, rather than the empty space between chunks.

		@param EventType ChunkLighten or ChunkDarken.
		@param context The context for light or darkness propagation.
		@param side The direction in which the spreading should be.
		@param cpos The position of the chunk to spread from.
	*/
	template<typename EventType>
	void spreadToChunk(
		const vox::PropagationContext & context,
		const vox::Side & side,
		const glm::ivec3 & cpos
	)
	{
		if (side == vox::Side::NEG_Z)
			context.m_bus.post(EventType{ context.m_chunks.getBelow(cpos)->getPosition() });
		else
			context.m_bus.post(EventType{ cpos + side.normal() });
	}
}

vox::Index vox::PropagationBase::offset(Index source, const Side & side) const
{
	return offset(fromIndex<int>(source), side);
}
vox::Index vox::PropagationBase::offset(const glm::ivec3 & source, const Side & side) const
{
	return toIndex(source + side.normal());
}

// ...

void vox::PropagationLight::spread()
{
	auto & writer = m_context.m_writer;

	Index index;
	Light light;
	while (writer.chunk().popLightPropagation(index, light))
	{
		// When a light is not specified, the block's light value is already set
		if (light.r == 0 && light.g == 0 && light.b == 0 && light.a == 0)
		{
			light = writer.read(index).getLight();
			if (light.r | light.g | light.b | light.a)
				spreadFrom(writer, index, light);
		}
		// Otherwise, light is must be written to block
		else
			spreadTo(writer, index, light);
	}
}
void vox::PropagationLight::spreadTo(Chunk::Writer & writer, Index index, Light light)
{
	auto data = m_context.m_writer.read(index);
	const auto & block = m_context.m_blocks[data.getType()];
	const auto old = data.getLight();
	
	light = applyBlockEffect(light, 1u + block.m_lightAbsorbed, block.m_lightFiltered);
	if (light.r <= old.r && light.g <= old.g && light.b <= old.b && light.a <= old.a)
		return;

	data.setLight(util::max(old, light));
	writer.writeFast(index, data);
	writer.chunk().pushLightPropagation(index, {});
}
void vox::PropagationLight::spreadFrom(Chunk::Writer & writer, Index index, Light light)
{
	for (const auto & side : Side::FACES)
	{
		const int edgeUpper = side.positive() ? CHUNK_SIZE<int> - 1 : 0;
		const int edgeLower = side.positive() ? 0 : CHUNK_SIZE<int> - 1;

		if (side == Side::NEG_Z)
			light.a++;

		auto pos = fromIndex<int>(index);
		if (pos[side.dimension()[0]] == edgeUpper)
		{
			if (auto * chunk = getChunk(m_context, side, m_cpos))
			{
				pos[side.dimension()[0]] = edgeLower;
				chunk->pushLightPropagation(toIndex(pos), light);
				spreadToChunk<ChunkLighten>(m_context, side, m_cpos);
			}
		}
		else
			spreadTo(writer, offset(pos, side), light);
	}
}

unsigned int vox::PropagationLight::applyBlockEffect(
	unsigned char in, unsigned char absorbed, unsigned char filtered
) const
{
	return in < absorbed ? 0 : util::min<unsigned char>(in - absorbed, filtered);
}
vox::Light vox::PropagationLight::applyBlockEffect(
	const Light & in, const Light & absorbed, const Light & filtered
) const
{
	return Light{
		applyBlockEffect(in.r, absorbed.r, filtered.r),
		applyBlockEffect(in.g, absorbed.g, filtered.g),
		applyBlockEffect(in.b, absorbed.b, filtered.b),
		applyBlockEffect(in.a, absorbed.a, filtered.a)
	};
}

// ...

void vox::PropagationDark::spread()
{
	auto & writer = m_context.m_writer;

	Index index;
	Light light;
	bool edgecase;
	while (writer.chunk().popDarkPropagation(index, light, edgecase))
	{
		if (edgecase)
			spreadTo(writer, index, light);
		else
			spreadFrom(writer, index, light);
	}
}
void vox::PropagationDark::spreadTo(Chunk::Writer & writer, Index index, Light light)
{
	auto data = m_context.m_writer.read(index);
	const auto & block = m_context.m_blocks[data.getType()];
	auto current = data.getLight();

	applyBlockEffect(light, current, block.m_lightEmitted);
	data.setLight(current);
	writer.writeFast(index, data);

	if (light.r != 0 || light.g != 0 || light.b != 0 || light.a != 0)
		writer.chunk().pushDarkPropagation(index, light, false);
	if (current.r != 0 || current.g != 0 || current.b != 0 || current.a != 0)
		writer.chunk().pushLightPropagation(index, {});
}
void vox::PropagationDark::spreadFrom(Chunk::Writer & writer, Index index, Light light)
{
	for (const auto & side : Side::FACES)
	{
		const int edgeUpper = side.positive() ? CHUNK_SIZE<int> - 1 : 0;
		const int edgeLower = side.positive() ? 0 : CHUNK_SIZE<int> - 1;

		if (side == Side::NEG_Z)
			light.a++;

		auto pos = fromIndex<int>(index);
		if (pos[side.dimension()[0]] == edgeUpper)
		{
			if (auto * chunk = getChunk(m_context, side, m_cpos))
			{
				pos[side.dimension()[0]] = edgeLower;
				chunk->pushDarkPropagation(toIndex(pos), light, true);
				spreadToChunk<ChunkDarken>(m_context, side, m_cpos);
			}
		}
		else
			spreadTo(writer, offset(pos, side), light);
	}
}

void vox::PropagationDark::applyBlockEffect(
	unsigned char & in, unsigned char & current, unsigned char emitted
) const
{
	if (in > current && current > emitted)
	{
		in = current;
		current = emitted;
	}
	else
		in = 0;
}
void vox::PropagationDark::applyBlockEffect(
	Light & in, Light & current, const Light & emitted
) const
{
	applyBlockEffect(in.r, current.r, emitted.r);
	applyBlockEffect(in.g, current.g, emitted.g);
	applyBlockEffect(in.b, current.b, emitted.b);
	applyBlockEffect(in.a, current.a, emitted.a);
}
