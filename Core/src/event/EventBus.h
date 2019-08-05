#pragma once

#include "event/EventContainer.h"
#include "event/EventListener.h"

namespace core
{
	template<typename Event> using Callback = std::function<void(Event &)>;
	template<typename Event> using CallbackConst = std::function<void(const Event &)>;

	class EventBus
	{
	public:
		EventBus() = default;
		EventBus(const EventBus &) = delete;
		EventBus(EventBus &&) = delete;
		~EventBus() = default;

		EventBus & operator=(const EventBus &) = delete;
		EventBus & operator=(EventBus &&) = delete;

		/**
			Posts the provided event on the bus. All callbacks registered for that event will
			receive it, in the order determined by their priorities.

			@param event The event to post on the bus.
			@return The same event as passed in.
		*/
		template<typename Event> Event & post(Event & event) const;
		/**
			Posts the provided event on the bus. All callbacks registered for that event will
			receive it, in the order determined by their priorities.

			@param event The event to post on the bus.
			@return The same event as passed in.
		*/
		template<typename Event> Event post(Event && event) const;

		/**
			Registers a new callback on the bus, which will be invoked whenever an event of the
			specified type is posted. Callbacks are invoked sorted by their priorities, where a
			lower priority is invoked first, and higher priorities invoked last. In the case where
			callbacks have the same priority, the last registered callback is invoked last.

			@param priority The priority of the listener.
			@param callback The callback to register under the specified event.
			@return The listener associated with the callback.
		*/
		template<typename Event> Listener add(int priority, const Callback<Event> & callback);
		/**
			Registers a new callback on the bus, which will be invoked whenever an event of the
			specified type is posted. Callbacks registered without a priority will always be
			invoked after all other callbacks have been invoked. These callbacks are forbidden
			from modifying the event in any way.

			@param callback The callback to register under the specified event.
			@return The listener associated with the callback.
		*/
		template<typename Event> Listener add(const CallbackConst<Event> & callback);
		/**
			Removes the given listener from the bus, preventing the callback pointed to by the
			listener from being invoked whenever an event is posted.

			@param listener The listener to remove from the bus.
		*/
		void remove(const Listener & listener);

		// ...

		/**
			Counts the number of events of the specified type that has been registered within the
			event bus.

			@return The number of active listeners attached to the given event type.
		*/
		template<typename Event> unsigned int size() const;

	private:
		static constexpr int PRIORITY_MONITOR = std::numeric_limits<int>::max();

		template<typename Event> using Container = CallbackContainer<Callback<Event>>;
		template<typename Event> using ContainerConst = CallbackContainer<CallbackConst<Event>>;

		std::unordered_map<std::type_index, CallbackContainerPtr> m_containers;
		std::unordered_map<std::type_index, CallbackContainerPtr> m_containersConst;
	};
}

namespace core
{
	template<typename Event>
	inline Event & EventBus::post(Event & event) const
	{
		const auto type = std::type_index{ typeid(Event) };
		if (const auto it = m_containers.find(type); it != m_containers.end())
		{
			const auto & maps = static_cast<Container<Event>*>(it->second.get())->maps();
			for (const auto &[_, callbacks] : maps)
			for (const auto &[_, callback] : callbacks)
				callback(event);
		}
		if (const auto it = m_containersConst.find(type); it != m_containersConst.end())
		{
			const auto & maps = static_cast<ContainerConst<Event>*>(it->second.get())->maps();
			for (const auto &[_, callbacks] : maps)
			for (const auto &[_, callback] : callbacks)
				callback(event);
		}
		return event;
	}
	template<typename Event>
	inline Event EventBus::post(Event && event) const
	{
		return std::move(post(event));
	}

	template<typename Event>
	inline Listener EventBus::add(int priority, const Callback<Event> & callback)
	{
		const auto type = std::type_index{ typeid(Event) };
		auto it = m_containers.find(type);
		if (it == m_containers.end())
			it = m_containers.emplace(type, std::make_unique<Container<Event>>()).first;

		auto * container = static_cast<Container<Event>*>(it->second.get());
		const auto id = container->add(priority, callback);
		return Listener{ *this, typeid(Event), id, priority, false };
	}
	template<typename Event>
	inline Listener EventBus::add(const CallbackConst<Event> & callback)
	{
		const auto type = std::type_index{ typeid(Event) };
		auto it = m_containersConst.find(type);
		if (it == m_containersConst.end())
			it = m_containersConst.emplace(type, std::make_unique<ContainerConst<Event>>()).first;

		auto * container = static_cast<ContainerConst<Event>*>(it->second.get());
		const auto id = container->add(PRIORITY_MONITOR, callback);
		return Listener{ *this, typeid(Event), id, PRIORITY_MONITOR, true };
	}

	template<typename Event>
	inline unsigned int EventBus::size() const
	{
		unsigned int size = 0u;
		if (const auto it = m_containers.find(typeid(Event));  it != m_containers.end())
			size += static_cast<Container<Event>*>(it->second.get())->size();
		if (const auto it = m_containersConst.find(typeid(Event));  it != m_containersConst.end())
			size += static_cast<ContainerConst<Event>*>(it->second.get())->size();
		return size;
	}
}