
#include "EventListener.h"

#include "event/EventBus.h"

core::Listener::Listener(
	EventBus & bus, const std::type_index & type,
	unsigned int id, int priority, bool monitor
) noexcept
	: m_bus(&bus), m_type(type), m_id(id), m_priority(priority), m_monitor(monitor)
{}
core::Listener::Listener(Listener && other) noexcept
	: m_type(other.m_type)
{
	*this = std::move(other);
}
core::Listener::~Listener() noexcept
{
	if (m_bus != nullptr)
		m_bus->remove(*this);
}

core::Listener & core::Listener::operator=(Listener && other) noexcept
{
	m_bus = other.m_bus;
	m_type = other.m_type;
	m_id = other.m_id;
	m_priority = other.m_priority;
	m_monitor = other.m_monitor;

	other.m_bus = nullptr;
	return *this;
}
