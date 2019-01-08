
#include "EntityRegistry.h"

core::Entity core::EntityRegistry::create(const ComponentMask & mask)
{
	if (!m_available.empty())
	{
		const auto entity = m_available.back();
		m_available.pop_back();
		m_entities[entity] = mask;
		return entity;
	}
	else
	{
		m_entities.push_back(mask);
		return m_entities.size() - 1;
	}
}
void core::EntityRegistry::destroy(Entity entity)
{
	if (has(entity))
	{
		m_entities[entity].reset();
		m_available.push_back(entity);
	}
}

void core::EntityRegistry::attach(Entity entity, const ComponentMask & mask)
{
	if (entity < m_entities.size())
		m_entities[entity] |= mask;
}
void core::EntityRegistry::detach(Entity entity, const ComponentMask & mask)
{
	if (entity < m_entities.size())
		m_entities[entity] &= ~mask;
}

bool core::EntityRegistry::has(Entity entity) const
{
	return entity < m_entities.size() && m_entities[entity].any();
}
core::ComponentMask core::EntityRegistry::get(Entity entity) const
{
	return entity < m_entities.size() ? m_entities[entity] : 0;
}

core::Entities core::EntityRegistry::findAllMatching(const ComponentMask & mask) const
{
	Entities entities;
	for (Entity entity = 0; entity < m_entities.size(); ++entity)
	{
		if (((m_entities[entity] & mask) ^ mask).none())
			entities.push_back(entity);
	}
	return entities;
}
