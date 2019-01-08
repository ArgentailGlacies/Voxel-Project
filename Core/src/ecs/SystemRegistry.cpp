
#include "SystemRegistry.h"

core::SystemRegistry::SystemRegistry(
	const ComponentRegistry & components,
	const EntityRegistry & entities
) : m_components(components), m_entities(entities)
{}

void core::SystemRegistry::process(double t, double dt)
{
	for (auto & system : m_systems)
		system->process(m_entities.findAllMatching(system->getMask()), t, dt);
}
