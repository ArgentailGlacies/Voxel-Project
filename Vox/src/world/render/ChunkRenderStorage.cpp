
#include "ChunkRenderStorage.h"

bool vox::ChunkRenderStorage::has(const glm::ivec3 & cpos) const
{
	return m_models.find(cpos) != m_models.end();
}

void vox::ChunkRenderStorage::reserve(const glm::ivec3 & cpos)
{
	m_models[cpos] = nullptr;
}
void vox::ChunkRenderStorage::set(const glm::ivec3 & cpos, std::unique_ptr<ChunkModel> && model)
{
	if (has(cpos))
		m_models[cpos] = std::move(model);
}
void vox::ChunkRenderStorage::remove(const glm::ivec3 & cpos)
{
	m_models.erase(cpos);
}

vox::ChunkModel * vox::ChunkRenderStorage::get(const glm::ivec3 & cpos) const
{
	const auto it = m_models.find(cpos);
	return it == m_models.end() || it->second == nullptr ? nullptr : it->second.get();
}

std::unordered_map<glm::ivec3, vox::ChunkModel*> vox::ChunkRenderStorage::getVisible() const
{
	std::unordered_map<glm::ivec3, ChunkModel*> models;
	for (const auto & [cpos, ptr] : m_models)
	{
		if (ptr != nullptr)
			models[cpos] = ptr.get();
	}
	return models;
}