
#include "GuiRegistry.h"

core::GuiRegistry::GuiRegistry(EventBus & bus, Scene & scene)
	: m_scene(scene)
{
}

bool core::GuiRegistry::open(const util::File & file)
{
	return false;
}
bool core::GuiRegistry::close(const util::File & file)
{
	return false;
}
