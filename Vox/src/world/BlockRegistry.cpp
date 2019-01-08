
#include "BlockRegistry.h"

#include "world/util/Limits.h"

#include <plog/Log.h>

vox::BlockRegistry::BlockRegistry()
{
	// Must guarantee that the air block exists in the system
	m_blocks.push_back(Block{ BLOCK_AIR_NAME, BLOCK_AIR_ID, BlockPhase::GAS });
	m_names.emplace(BLOCK_AIR_NAME, BLOCK_AIR_ID);
	m_blocks[BLOCK_AIR_ID].m_lightFiltered = glm::uvec4{ MAX_BLOCK_LIGHT };
}

bool vox::BlockRegistry::has(unsigned int id) const
{
	if (id == BLOCK_AIR_ID)
		return true;
	return id < size() && m_blocks[id].m_id != BLOCK_AIR_ID;
}
bool vox::BlockRegistry::has(const std::string & name) const
{
	return m_names.find(name) != m_names.end();
}

const vox::Block & vox::BlockRegistry::operator[](unsigned int id) const
{
	return id >= size() ? m_blocks[0] : m_blocks[id];
}
const vox::Block & vox::BlockRegistry::operator[](const std::string & name) const
{
	const auto it = m_names.find(name);
	return it == m_names.end() ? m_blocks[0] : m_blocks[it->second];
}

vox::Block & vox::BlockRegistry::add(const std::string & name)
{
	for (unsigned int i = BLOCK_AIR_ID + 1; i < size(); ++i)
	{
		if (m_blocks[i].m_id == BLOCK_AIR_ID)
			return add(name, i);
	}
	return add(name, size());
}
vox::Block & vox::BlockRegistry::add(const std::string & name, unsigned int id)
{
	if (m_blocks.size() >= MAX_BLOCK_TYPE)
		throw std::runtime_error("Out of block space");
	if (name == BLOCK_AIR_NAME || id == BLOCK_AIR_ID)
		throw std::invalid_argument("Air block is reserved and cannot be overwritten");
	if (has(name))
		throw std::invalid_argument("Block " + name + " already exists");
	if (has(id))
		throw std::invalid_argument("Block (" + std::to_string(id) + ") already exists");

	while (size() <= id)
		m_blocks.push_back(Block{ BLOCK_AIR_NAME, BLOCK_AIR_ID, BlockPhase::GAS, {}, {}, {} });

	m_names[name] = id;
	return m_blocks[id] = Block{ name, id };
}
vox::Block & vox::BlockRegistry::get(const std::string & name)
{
	const auto it = m_names.find(name);
	if (it == m_names.end())
		throw std::invalid_argument("Block '" + name + "' does not exist");
	return m_blocks[it->second];
}
