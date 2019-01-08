#pragma once

#include "ecs/Component.h"
#include "ecs/Entity.h"

#include <vector>

namespace core
{
	class EntityRegistry
	{
	public:
		EntityRegistry() = default;
		EntityRegistry(const EntityRegistry &) = delete;
		EntityRegistry(EntityRegistry &&) = delete;
		~EntityRegistry() = default;

		EntityRegistry & operator=(const EntityRegistry &) = delete;
		EntityRegistry & operator=(EntityRegistry &&) = delete;

		/**
			Generates a new entity. All entities will be given a unique id. When entities are
			removed, their ids will be available for reuse. As such, care should be taken to not
			store entities beyond their expected lifetimes. If the entity is created without
			components, it will not be considered active until at least one component has been
			attached to it.

			@param mask The components the entity should contain at initialization.
			@return A unique entity.
		*/
		Entity create(const ComponentMask & mask = 0);
		/**
			Removes the entity from the system, making its id available for reuse.

			@param entity The entity to remove from the system.
		*/
		void destroy(Entity entity);
		
		/**
			Attaches all the provided components to the specified entity if it exists.

			@param entity The entity the components should be attached to.
			@param mask The components to attach to the entity.
		*/
		void attach(Entity entity, const ComponentMask & mask);
		/**
			Detaches all the provided components from the specified entity if it exists.

			@param entity The entity the components should be detached from.
			@param mask The components to detach from the entity.
		*/
		void detach(Entity entity, const ComponentMask & mask);

		/**
			Checks if the specified entity exists within the system.

			@return True iff the given entity exists.
		*/
		bool has(Entity entity) const;
		/**
			Retrieves the component mask for the given entity, or an empty mask is the entity does
			not exist.

			@param entity The entity to retrieve the mask from.
			@return The component mask for the given entity.
		*/
		ComponentMask get(Entity entity) const;

		/**
			Collects all entities which has at least all components specified in the mask. If an
			entity has more components than specified in the mask, that entity is collected. If the
			entity lacks at least one of the components specified in the mask, the entity is
			ignored.

			@param mask The mask to match against the entities.
			@return A collection of all entities matching the specified mask.
		*/
		Entities findAllMatching(const ComponentMask & mask) const;

	private:
		std::vector<ComponentMask> m_entities;
		std::vector<Entity> m_available;
	};
}