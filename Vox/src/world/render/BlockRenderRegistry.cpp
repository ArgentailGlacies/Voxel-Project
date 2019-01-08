
#include "BlockRenderRegistry.h"

#include "world/util/Limits.h"

#include <stdexcept>

vox::BlockRenderRegistry::BlockRenderRegistry() noexcept
{
	// Must ensure that the air block always exists in the system
	m_blocks.emplace_back();
}

vox::BlockRender & vox::BlockRenderRegistry::operator[](unsigned int id)
{
	if (id > MAX_BLOCK_TYPE)
		throw std::invalid_argument("Block id out of bounds");
	if (id >= m_blocks.size())
	{
		m_blocks.reserve(id);
		for (unsigned int i = m_blocks.size(); i <= id; ++i)
			m_blocks.emplace_back();
	}
	return m_blocks[id];
}
const vox::BlockRender & vox::BlockRenderRegistry::operator[](unsigned int id) const
{
	if (id >= m_blocks.size())
		throw std::invalid_argument("Block id out of bounds");
	return m_blocks[id];
}
