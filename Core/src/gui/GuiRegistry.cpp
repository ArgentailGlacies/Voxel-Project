
#include "GuiRegistry.h"

#include "gui/GuiLoader.h"

core::GuiRegistry::GuiRegistry(EventBus & bus, Scene & scene)
	: m_scene(scene)
{
}

bool core::GuiRegistry::open(const util::File & file)
{
	if (!file.exists() || m_guis.find(file) != m_guis.end())
		return false;

	m_guis.emplace(file, file.name()).first->second.load(file);
	return true;
}
bool core::GuiRegistry::close(const util::File & file)
{
	if (m_guis.find(file) == m_guis.end())
		return false;

	m_guis.erase(file);
	return true;
}
