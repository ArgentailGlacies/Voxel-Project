#pragma once

#include <functional>
#include <map>
#include <memory>

namespace core
{
	class CallbackContainerBase
	{
	public:
		virtual void remove(int priority, unsigned int id) = 0;
	};
	using CallbackContainerPtr = std::unique_ptr<CallbackContainerBase>;

	// ...

	template<typename Callback>
	class CallbackContainer : public CallbackContainerBase
	{
	public:
		unsigned int size() const;
		unsigned int add(int priority, const Callback & callback);
		virtual inline void remove(int priority, unsigned int id) override final;

		inline const auto & maps() const { return m_maps; }

	private:
		using CallbackMap = std::map<unsigned int, Callback>;

		std::map<int, CallbackMap> m_maps;
		unsigned int m_uniqueId = 0u;
	};
}

namespace core
{
	template<typename Callback>
	inline unsigned int CallbackContainer<Callback>::size() const
	{
		unsigned int size = 0u;
		for (const auto & it : m_maps)
			size += it.second.size();
		return size;
	}
	template<typename Callback>
	inline unsigned int CallbackContainer<Callback>::add(
		int priority, const Callback & callback
	)
	{
		m_maps[priority][m_uniqueId] = callback;
		return m_uniqueId++;
	}
	template<typename Callback>
	inline void CallbackContainer<Callback>::remove(int priority, unsigned int id)
	{
		m_maps[priority].erase(id);
	}
}