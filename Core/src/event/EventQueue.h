#pragma once

#include <gsl/pointers>
#include <unordered_set>

struct ALLEGRO_EVENT_QUEUE;
struct ALLEGRO_EVENT_SOURCE;

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

		void add(gsl::not_null<ALLEGRO_EVENT_SOURCE*> source);

		void process() const;

	private:
		const EventBus & m_bus;

		gsl::not_null<ALLEGRO_EVENT_QUEUE*> m_handle;
	};
}