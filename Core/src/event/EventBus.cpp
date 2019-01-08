
#include "EventBus.h"

void core::EventBus::remove(const Listener & listener)
{
	const auto & container = listener.monitor() ? m_containersConst : m_containers;
	const auto it = container.find(listener.type());
	if (it == container.end())
		return;

	it->second->remove(listener.priority(), listener.id());
}
