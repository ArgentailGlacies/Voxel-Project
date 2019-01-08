
#include "ChunkQuery.h"

#include "util/MathOperations.h"

#include <algorithm>

vox::ChunkQuery::ChunkQuery()
	: m_min(CHUNK_SIZE<unsigned int>), m_max(0)
{}

bool vox::ChunkQuery::has(const glm::uvec3 & pos) const
{
	const auto index = toIndex(pos);
	const auto it = std::lower_bound(m_data.begin(), m_data.end(), index,
		[](const auto & lhs, const auto & rhs) { return lhs.m_index < rhs; }
	);
	return it != m_data.end() && it->m_index == index;
}

void vox::ChunkQuery::add(const glm::uvec3 & pos, const BlockData & data)
{
	bounds(pos, pos);
	insert(toIndex(pos), data);
}
void vox::ChunkQuery::add(const glm::uvec3 & start, const glm::uvec3 & end, const BlockData & data)
{
	const auto min = util::min(start, end);
	const auto max = util::max(start, end);
	bounds(min, max);

	glm::uvec3 pos;
	for (pos.z = min.z; pos.z <= max.z; ++pos.z)
	for (pos.y = min.y; pos.y <= max.y; ++pos.y)
	for (pos.x = min.x; pos.x <= max.x; ++pos.x)
		insert(toIndex(pos), data);
}
void vox::ChunkQuery::insert(Index index, const BlockData & data)
{
	if (empty() || m_data.back().m_index < index)
		m_data.emplace_back(index, data);
	else
	{
		auto it = std::lower_bound(m_data.begin(), m_data.end(), index,
			[](const auto & lhs, const auto & rhs) { return lhs.m_index < rhs; }
		);
		if (it->m_index == index)
			it->m_data = data;
		else
			m_data.emplace(it, index, data);
	}
}

void vox::ChunkQuery::bounds(const glm::uvec3 & min, const glm::uvec3 & max)
{
	m_min = util::min(m_min, min);
	m_max = util::max(m_max, max);
}
