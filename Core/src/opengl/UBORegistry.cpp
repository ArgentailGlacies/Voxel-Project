
#include "UBORegistry.h"

bool core::UBORegistry::has(const std::string & block) const
{
	return m_ubos.find(block) != m_ubos.end();
}
bool core::UBORegistry::has(const std::string & block, const std::string & member) const
{
	return has(block) ? getBlock(block).has(member) : false;
}

bool core::UBORegistry::build(const std::string & block)
{
	return has(block) ? getBlock(block).build() : false;
}

std::unordered_map<std::string, unsigned int> core::UBORegistry::bindings() const
{
	std::unordered_map<std::string, unsigned int> bindings;
	for (const auto & [name, ubo] : m_ubos)
		bindings.emplace(name, ubo.port());
	return bindings;
}

core::UBO & core::UBORegistry::getBlock(const std::string & block)
{
	auto it = m_ubos.find(block);
	if (it == m_ubos.end())
		it = m_ubos.emplace(block, m_ubos.size()).first;
	return it->second;
}
const core::UBO & core::UBORegistry::getBlock(const std::string & block) const
{
	return m_ubos.find(block)->second;
}
