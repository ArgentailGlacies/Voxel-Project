#pragma once

#include "ecs/Component.h"
#include "ecs/ComponentRegistry.h"
#include "ecs/EntityRegistry.h"

namespace core
{
	class SystemBase
	{
	public:
		SystemBase() = delete;
		SystemBase(const ComponentRegistry & components, const ComponentMask & mask)
			: m_components(components), m_mask(mask) {}
		SystemBase(const SystemBase &) = delete;
		SystemBase(SystemBase &&) = delete;
		virtual ~SystemBase() = default;

		SystemBase & operator=(const SystemBase &) = delete;
		SystemBase & operator=(SystemBase &&) = delete;

		/**
			Processes the system a single time. Only the provided entities should be processed by
			the system.

			@param entities The entities that should be processed by this system.
			@param t The current time in seconds since the program started.
			@param dt The amount of time in seconds that should be simulated.
		*/
		virtual void process(const Entities & entities, double t, double dt) = 0;

		/**
			Retrieves the mask holding the components that are used within this system.

			@return The component mask for this system.
		*/
		inline const auto & getMask() const { return m_mask; }

	protected:
		/**
			Retrieves the raw data for all components. This should normally not be used, specify
			instead which components should be accessed.

			@return The raw data container containing all entity data.
		*/
		inline const auto & getData() const { return m_components; }
		/**
			Retrieves the data associated with the given component. The system may modify the data
			in whichever way it desires. If the requested component is not added to the system,
			the exception std::invalid_argument is thrown.

			@return The data of the specified component.
		*/
		template<typename Component> ComponentData<Component> & getData() const;

	private:
		const ComponentRegistry & m_components;
		const ComponentMask m_mask;
	};

	template<typename ...Components>
	class System : public SystemBase
	{
	public:
		System(const ComponentRegistry & registry)
			: SystemBase(registry, registry.mask<Components...>()) {}
	};
}

namespace core
{
	template<typename Component>
	inline ComponentData<Component> & SystemBase::getData() const
	{
		if (!m_mask[m_components.id<Component>()])
			throw std::invalid_argument("Requested component is not part of system");
		return m_components.get<Component>();
	}
}