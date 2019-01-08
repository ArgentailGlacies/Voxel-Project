
#include "BlockRegion.h"

#include "world/util/Index.h"
#include "world/util/Limits.h"

vox::BlockRegion::BlockRegion() noexcept
	: BlockRegion(glm::ivec3{}, glm::uvec3{ CHUNK_SIZE<unsigned int> })
{}
vox::BlockRegion::BlockRegion(const glm::ivec3 & pos, const glm::uvec3 & size) noexcept
	: m_pos(pos), m_size(size)
{
	m_data.resize(m_size.x * m_size.y * m_size.z, {});
}

vox::BlockData vox::BlockRegion::read(const glm::ivec3 & pos) const
{
	return m_data[toIndex(pos, m_pos, m_size)];
}

void vox::BlockRegion::write(const glm::ivec3 & pos, BlockData data)
{
	m_data[toIndex(pos, m_pos, m_size)] = data;
}
