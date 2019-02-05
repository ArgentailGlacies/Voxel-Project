#pragma once

#include "allegro/EventSource.h"

#include <unordered_set>

struct ALLEGRO_EVENT_QUEUE;

namespace core
{
	class EventBus;

	class EventQueue
	{
	public:
		EventQueue() = delete;
		EventQueue(const EventBus & bus);
		EventQueue(const EventQueue &) = delete;
		EventQueue(EventQueue &&) = delete;
		~EventQueue();

		EventQueue & operator=(const EventQueue &) = delete;
		EventQueue & operator=(EventQueue &&) = delete;

		void add(EventSourcePtr source);

		void process() const;

	private:
		const EventBus & m_bus;

		ALLEGRO_EVENT_QUEUE * m_handle;
	};
}