
#include "ChunkStorage.h"

void vox::ChunkStorage::create(const glm::ivec3 & cpos)
{
	m_chunks[cpos].emplace(cpos.z, cpos);
}
void vox::ChunkStorage::destroy(const glm::ivec3 & cpos)
{
	if (const auto it = m_chunks.find(cpos); it != m_chunks.end())
	{
		it->second.erase(cpos.z);
		if (it->second.empty())
			m_chunks.erase(cpos);
	}
}
bool vox::ChunkStorage::has(const glm::ivec3 & cpos) const
{
	if (const auto it = m_chunks.find(cpos); it != m_chunks.end())
		return it->second.find(cpos.z) != it->second.end();
	return false;
}

vox::Chunk * vox::ChunkStorage::getExact(const glm::ivec3 & cpos) const
{
	if (const auto it = m_chunks.find(cpos); it != m_chunks.end())
	if (const auto it2 = it->second.find(cpos.z); it2 != it->second.end())
		return &it2->second;
	return nullptr;
}
vox::Chunk * vox::ChunkStorage::getAbove(const glm::ivec3 & cpos) const
{
	if (const auto it = m_chunks.find(cpos); it != m_chunks.end())
	{
		auto & column = it->second;
		const auto it2 = std::upper_bound(column.begin(), column.end(), cpos.z,
			[](const auto & lhs, const auto & rhs) { return lhs < rhs.first; }
		);
		return it2 == column.end() ? nullptr : &it2->second;
	}
	return nullptr;
}
vox::Chunk * vox::ChunkStorage::getBelow(const glm::ivec3 & cpos) const
{
	if (const auto it = m_chunks.find(cpos); it != m_chunks.end())
	{
		auto & column = it->second;
		auto it2 = std::lower_bound(column.begin(), column.end(), cpos.z,
			[](const auto & lhs, const auto & rhs) { return lhs.first < rhs; }
		);
		return it2 == column.begin() ? nullptr : &(--it2)->second;
	}
	return nullptr;
}

vox::Chunk * vox::ChunkStorage::getUppermost(const glm::ivec2 & cpos) const
{
	if (const auto it = m_chunks.find(cpos); it != m_chunks.end())
		return it->second.empty() ? nullptr : &it->second.rbegin()->second;
	return nullptr;
}
vox::Chunk * vox::ChunkStorage::getLowermost(const glm::ivec2 & cpos) const
{
	if (const auto it = m_chunks.find(cpos); it != m_chunks.end())
		return it->second.empty() ? nullptr : &it->second.begin()->second;
	return nullptr;
}

unsigned int vox::ChunkStorage::count() const
{
	unsigned int count = 0;
	for (const auto & [_, column] : m_chunks)
		count += column.size();
	return count;
}
