#pragma once

#include "ecs/Component.h"
#include "ecs/ComponentData.h"

#include <memory>
#include <typeindex>
#include <unordered_map>

namespace core
{
	class ComponentRegistry
	{
	public:
		ComponentRegistry() = default;
		ComponentRegistry(const ComponentRegistry &) = delete;
		ComponentRegistry(ComponentRegistry &&) = default;
		~ComponentRegistry() = default;

		ComponentRegistry & operator=(const ComponentRegistry &) = delete;
		ComponentRegistry & operator=(ComponentRegistry &&) = delete;

		/**
			Adds the specified component to the registry, allowing it to be accessed.
		*/
		template<typename Component> void add();
		/**
			Retrieves the data of the specified component. If the component does not exist, the
			exception std::invalid_argument is thrown.

			@return The data associated with the given component.
		*/
		template<typename Component> ComponentData<Component> & get() const;
		/**
			Retrieves the id of the specified component. If the component does not exist, the
			exception std::invalid_argument is thrown.

			@return The id associated with the given component.
		*/
		template<typename Component> unsigned int id() const;
		/**
			Checks if the provided component exists in the registry.

			@return True iff the component has been added to the registry.
		*/
		template<typename Component> bool has() const;

		/**
			Retrieves the component mask that matches the provided components. If any of the
			components do not exist, the exception std::invalid_argument is thrown.

			@return The mask for the specified components.
		*/
		template<typename ...Components> ComponentMask mask() const;

	private:
		using ComponentDataPtr = std::unique_ptr<ComponentDataBase>;
		using ComponentEntry = std::pair<unsigned int, ComponentDataPtr>;

		/**
			Searches through the registered components, retrieveing the id and the data storage
			associated with the requested component. If the component was not found, the exception
			std::invalid_argument is thrown.

			@return The component id and data pair.
		*/
		template<typename Component> const ComponentEntry & find() const;

		template<typename Component> void mask(ComponentMask & mask) const;
		template<typename Component, typename Next, typename ...Remaining>
		void mask(ComponentMask & mask) const;

		std::unordered_map<std::type_index, ComponentEntry> m_data;
	};
}

namespace core
{
	template<typename Component>
	inline const ComponentRegistry::ComponentEntry & ComponentRegistry::find() const
	{
		const auto it = m_data.find(typeid(Component));
		if (it == m_data.end())
			throw std::invalid_argument("Requested compoment has not been added");
		return it->second;
	}

	template<typename Component>
	inline void ComponentRegistry::add()
	{
		if (has<Component>())
			throw std::invalid_argument("Requested compoment has already been added");
		auto entry = std::make_pair(m_data.size(), std::make_unique<ComponentData<Component>>());
		m_data[typeid(Component)] = std::move(entry);
	}
	template<typename Component>
	inline ComponentData<Component> & ComponentRegistry::get() const
	{
		return *static_cast<ComponentData<Component>*>(find<Component>().second.get());
	}
	template<typename Component>
	inline unsigned int ComponentRegistry::id() const
	{
		return find<Component>().first;
	}
	template<typename Component>
	inline bool ComponentRegistry::has() const
	{
		return m_data.find(typeid(Component)) != m_data.end();
	}

	template<typename ...Components>
	inline ComponentMask ComponentRegistry::mask() const
	{
		ComponentMask mask;
		this->mask<Components...>(mask);
		return mask;
	}
	template<typename Component>
	inline void ComponentRegistry::mask(ComponentMask & mask) const
	{
		mask.set(id<Component>());
	}
	template<typename Component, typename Next, typename ...Remaining>
	inline void ComponentRegistry::mask(ComponentMask & mask) const
	{
		this->mask<Component>(mask);
		this->mask<Next, Remaining...>(mask);
	}
}