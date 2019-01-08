
#include "StateManager.h"

core::StateManager::StateManager(Engine & engine)
	: m_engine(engine)
{}

void core::StateManager::process()
{
	if (m_next != nullptr)
	{
		m_current = std::move(m_next);
		m_current->initialize(m_engine);
	}
	if (m_current != nullptr)
		m_current->process();
}
