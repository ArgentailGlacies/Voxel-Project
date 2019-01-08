
#include "WorldQuery.h"

#include "world/util/Limits.h"

bool vox::WorldQuery::has(const glm::ivec3 & pos) const
{
	const auto cpos = pos >> CHUNK_LG<int>;
	const auto bpos = pos & CHUNK_BIT<int>;

	const auto it = m_data.find(cpos);
	return it == m_data.end() ? false : it->second.has(bpos);
}

void vox::WorldQuery::add(const glm::ivec3 & cpos, ChunkQuery && query)
{
	const auto oldCount = has(cpos) ? m_data[cpos].size() : 0;
	const auto newCount = m_count + query.size() - oldCount;

	if (query.empty() || newCount >= MAX_WORLD_QUERY)
		return;
	m_data[cpos] = std::move(query);
	m_count = newCount;
}
void vox::WorldQuery::add(const glm::ivec3 & pos, const BlockData & data)
{
	if (m_count >= MAX_WORLD_QUERY)
		return;
	const auto cpos = pos >> CHUNK_LG<int>;
	const auto bpos = pos & CHUNK_BIT<int>;
	auto & query = m_data[cpos];
	m_count -= query.size();
	query.add(bpos, data);
	m_count += query.size();
}