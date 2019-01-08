
#include "Query.h"

#include "util/MathOperations.h"
#include "util/Raytrace.h"
#include "world/Block.h"
#include "world/util/Limits.h"

namespace
{
	inline auto getBlockData(const vox::Block & block)
	{
		vox::BlockData data;
		data.setType(block.m_id);
		data.setLight(block.m_lightEmitted);
		return data;
	}
}

vox::WorldQuery vox::readBlock(const glm::ivec3 & pos)
{
	return writeBlock(Block{}, pos);
}
vox::WorldQuery vox::readCylinder(const glm::ivec3 & start, const glm::ivec3 & end, util::Axis axis)
{
	return writeCylinder(Block{}, start, end, axis);
}
vox::WorldQuery vox::readEllipse(const glm::ivec3 & start, const glm::ivec3 & end)
{
	return writeEllipse(Block{}, start, end);
}
vox::WorldQuery vox::readRectangle(const glm::ivec3 & start, const glm::ivec3 & end)
{
	return writeRectangle(Block{}, start, end);
}
vox::WorldQuery vox::readLine(const glm::ivec3 & start, const glm::ivec3 & end)
{
	return writeLine(Block{}, start, end);
}
vox::WorldQuery vox::readSphere(const glm::ivec3 & center, unsigned int diameter)
{
	return writeSphere(Block{}, center, diameter);
}

vox::WorldQuery vox::writeBlock(const Block & block, const glm::ivec3 & pos)
{
	WorldQuery query;
	query.add(pos, getBlockData(block));
	return query;
}
vox::WorldQuery vox::writeCylinder(const Block & block, const glm::ivec3 & start, const glm::ivec3 & end, util::Axis axis)
{
	const auto min = util::min(start, end);
	const auto max = util::max(start, end);
	const auto cmin = min >> CHUNK_LG<int>;
	const auto cmax = max >> CHUNK_LG<int>;

	const auto dim = axis == util::Axis::Z ? glm::uvec2{ 0, 1 } : axis == util::Axis::Y ? glm::uvec2{ 0, 2 } : glm::uvec2{ 1, 2 };
	const auto size = max - min + 1;

	const auto blockData = getBlockData(block);

	WorldQuery query;
	glm::ivec3 cpos;
	for (cpos.z = cmin.z; cpos.z <= cmax.z; ++cpos.z)
	for (cpos.y = cmin.y; cpos.y <= cmax.y; ++cpos.y)
	for (cpos.x = cmin.x; cpos.x <= cmax.x; ++cpos.x)
	{
		const auto lower = util::max(min - cpos * CHUNK_SIZE<int>, glm::ivec3{});
		const auto upper = util::min(max - cpos * CHUNK_SIZE<int>, glm::ivec3{ CHUNK_BIT<int> });

		const auto center = glm::vec3{ min - cpos * CHUNK_SIZE<int> } + 0.5f * glm::vec3{ size };

		ChunkQuery chunkQuery;

		glm::ivec3 pos;
		for (pos.z = lower.z; pos.z <= upper.z; ++pos.z)
		for (pos.y = lower.y; pos.y <= upper.y; ++pos.y)
		for (pos.x = lower.x; pos.x <= upper.x; ++pos.x)
		{
			const auto& delta = util::pow(2.0f * (0.5f + glm::vec3{ pos } - center) / glm::vec3{ size }, 2.0f);
			if (delta[dim.x] + delta[dim.y] <= 1.0f)
				chunkQuery.add(pos, blockData);
		}

		query.add(cpos, std::move(chunkQuery));
	}
	return query;
}
vox::WorldQuery vox::writeEllipse(const Block & block, const glm::ivec3 & start, const glm::ivec3 & end)
{
	const auto min = util::min(start, end);
	const auto max = util::max(start, end);
	const auto cmin = min >> CHUNK_LG<int>;
	const auto cmax = max >> CHUNK_LG<int>;

	const auto size = max - min + 1;

	const auto blockData = getBlockData(block);

	WorldQuery query;
	glm::ivec3 cpos;
	for (cpos.z = cmin.z; cpos.z <= cmax.z; ++cpos.z)
	for (cpos.y = cmin.y; cpos.y <= cmax.y; ++cpos.y)
	for (cpos.x = cmin.x; cpos.x <= cmax.x; ++cpos.x)
	{
		const auto lower = util::max(min - cpos * CHUNK_SIZE<int>, glm::ivec3{});
		const auto upper = util::min(max - cpos * CHUNK_SIZE<int>, glm::ivec3{ CHUNK_BIT<int> });

		const auto center = glm::vec3{ min - cpos * CHUNK_SIZE<int> } + 0.5f * glm::vec3{ size };

		ChunkQuery chunkQuery;

		glm::ivec3 pos;
		for (pos.z = lower.z; pos.z <= upper.z; ++pos.z)
		for (pos.y = lower.y; pos.y <= upper.y; ++pos.y)
		for (pos.x = lower.x; pos.x <= upper.x; ++pos.x)
		{
			const auto delta = util::pow(2.0f * (0.5f + glm::vec3{ pos } - center) / glm::vec3{ size }, 2.0f);
			if (delta.x + delta.y + delta.z <= 1.0f)
				chunkQuery.add(pos, blockData);
		}

		query.add(cpos, std::move(chunkQuery));
	}
	return query;
}
vox::WorldQuery vox::writeRectangle(const Block & block, const glm::ivec3 & start, const glm::ivec3 & end)
{
	const auto min = util::min(start, end);
	const auto max = util::max(start, end);
	const auto cmin = min >> CHUNK_LG<int>;
	const auto cmax = max >> CHUNK_LG<int>;

	const auto blockData = getBlockData(block);

	WorldQuery query;
	glm::ivec3 cpos;
	for (cpos.z = cmin.z; cpos.z <= cmax.z; ++cpos.z)
	for (cpos.y = cmin.y; cpos.y <= cmax.y; ++cpos.y)
	for (cpos.x = cmin.x; cpos.x <= cmax.x; ++cpos.x)
	{
		const auto lower = util::max(min - cpos * CHUNK_SIZE<int>, glm::ivec3{});
		const auto upper = util::min(max - cpos * CHUNK_SIZE<int>, glm::ivec3{ CHUNK_BIT<int> });

		ChunkQuery chunkQuery;
		chunkQuery.add(lower, upper, blockData);
		query.add(cpos, std::move(chunkQuery));
	}
	return query;
}
vox::WorldQuery vox::writeLine(const Block & block, const glm::ivec3 & start, const glm::ivec3 & end)
{
	const auto blockData = getBlockData(block);

	WorldQuery query;
	for (RaytraceBresenham ray{ start, end }; ray.valid(); ray.next())
		query.add(ray.getPos(), blockData);
	return query;
}
vox::WorldQuery vox::writeSphere(const Block & block, const glm::ivec3 & center, unsigned int diameter)
{
	if (diameter == 0u)
		return writeBlock(block, center);
	const auto low = static_cast<int>((diameter - 1u) >> 1u);
	const auto high = static_cast<int>(diameter >> 1u);
	return writeEllipse(block, center - low, center + high);
}