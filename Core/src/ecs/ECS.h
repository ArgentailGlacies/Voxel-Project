#pragma once

#include "ComponentRegistry.h"
#include "EntityRegistry.h"
#include "SystemRegistry.h"

namespace core
{
	class ECS
	{
	public:
		ECS() = default;
		ECS(const ECS &) = delete;
		ECS(ECS &&) = delete;
		~ECS() = default;

		ECS & operator=(const ECS &) = delete;
		ECS & operator=(ECS &&) = delete;

		/**
			Processes all registered systems once.

			@param t The time since the program started, in seconds.
			@param dt The time to simulate, in seconds.
		*/
		inline void process(double t, double dt) { m_systems.process(t, dt); }

		/**
			Adds the specific component. A new data table will be created for the component as well,
			which will hold data for all entities with this component.

			@param Component	The component to add to the system. The component must be default-
								constructable.
		*/
		template<typename Component> void addComponent();
		/**
			Adds the specified system. The system components must be registered individually before
			the system is added. Any new entities will be added to the system provided the entity
			has all required components.

			@param System The system type which should be registered.
			@param params The parameters which are passed as parameters for the system constructor.
		*/
		template<typename System, typename ...Params> void addSystem(const Params & ...params);

		/**
			Creates a new entity and binds it to the provided components. Any entities that are
			not deleted once the ecs system is deleted are automatically cleaned up.

			@param Components A list of all the components the entity should be created with.
			@return The newly created entity.
		*/
		template<typename ...Components> Entity createEntity();
		/**
			Removes the entity from the entire system.

			@param entity The entity to remove.
		*/
		inline void destroyEntity(Entity entity) { return m_entities.destroy(entity); }
		/**
			Checks if the given entity exists within the system.

			@return True iff the entity exists.
		*/
		inline bool hasEntity(Entity entity) const { return m_entities.has(entity); }

		/**
			Attaches the specified components to the specified entity, if it exists.

			@param Components The components to attach to the entity.
			@param entity The entity the components should be attached to.
		*/
		template<typename ...Components> void attachEntityComponent(Entity entity);
		/**
			Detaches the specified components from the specified entity, if it exists.

			@param Components The components to detach from the entity.
			@param entity The entity the components should be detached from.
		*/
		template<typename ...Components> void detachEntityComponent(Entity entity);

		/**
			Checks whether the provided component is attached to the given entity or not.

			@param entity The entity to test.
			@return True iff the component is attached to the entity.
		*/
		template<typename Component> bool hasEntityData(Entity entity) const;
		/**
			Retrieves the component data stored in the given entity. If the entity does not contain
			the specified component, an std::invalid_argument exception is thrown.

			@param entity The entity to retrieve the data from.
			@return The data of the given component.
		*/
		template<typename Component> Component & getEntityData(Entity entity) const;

	private:
		ComponentRegistry m_components;
		EntityRegistry m_entities;
		SystemRegistry m_systems{ m_components, m_entities };
	};
}

namespace core
{
	template<typename Component>
	inline void ECS::addComponent()
	{
		m_components.add<Component>();
	}
	template<typename System, typename ...Params>
	inline void ECS::addSystem(const Params & ...params)
	{
		m_systems.add<System>(params...);
	}

	template<typename ...Components>
	inline Entity ECS::createEntity()
	{
		return m_entities.create(m_components.mask<Components...>());
	}
	
	template<typename ...Components>
	inline void ECS::attachEntityComponent(Entity entity)
	{
		m_entities.attach(entity, m_components.mask<Components...>());
	}
	template<typename ...Components>
	inline void ECS::detachEntityComponent(Entity entity)
	{
		m_entities.detach(entity, m_components.mask<Components...>());
	}

	template<typename Component>
	inline bool ECS::hasEntityData(Entity entity) const
	{
		return m_entities.get(entity)[m_components.id<Component>()];
	}
	template<typename Component>
	inline Component & ECS::getEntityData(Entity entity) const
	{
		if (!hasEntityData<Component>(entity))
			throw std::invalid_argument("Missing component in entity " + std::to_string(entity));
		return m_components.get<Component>()[entity];
	}
}