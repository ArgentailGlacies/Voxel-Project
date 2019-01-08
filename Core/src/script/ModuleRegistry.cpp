
#include "ModuleRegistry.h"

#include <plog/Log.h>

bool core::ModuleRegistry::has(const std::string & identifier) const
{
	return m_modules.find(identifier) != m_modules.end();
}
void core::ModuleRegistry::add(const std::string & identifier, const Module & module)
{
	m_modules[identifier] = module;
}

void core::ModuleRegistry::apply(const std::string & identifier, Script & script)
{
	if (const auto it = m_modules.find(identifier); it != m_modules.end())
		it->second(script);
	else
		LOG_WARNING << "Could not find '" << identifier << "' for '" << script.name() << "'";
}
