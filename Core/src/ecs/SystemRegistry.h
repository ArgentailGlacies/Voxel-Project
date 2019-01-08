#pragma once

#include "ecs/ComponentRegistry.h"
#include "ecs/EntityRegistry.h"
#include "ecs/System.h"

#include <memory>
#include <vector>

namespace core
{
	class SystemRegistry
	{
	public:
		SystemRegistry() = delete;
		SystemRegistry(const ComponentRegistry & components, const EntityRegistry & entities);
		SystemRegistry(const SystemRegistry &) = delete;
		SystemRegistry(SystemRegistry &&) = default;
		~SystemRegistry() = default;

		SystemRegistry & operator=(const SystemRegistry &) = delete;
		SystemRegistry & operator=(SystemRegistry &&) = delete;

		/**
			Adds a new system to the registry. The system will be automatically processed, given
			the appropriate entities to operate on.
		*/
		template<typename System, typename ...Params> void add(const Params & ...params);

		/**
			Runs through all systems, processing them once. All systems will be provided with the
			correct entities. The systems will have to select and operate on components as they see
			fit.

			@param t The time since the program started, in seconds.
			@param dt The time to simulate, in seconds.
		*/
		void process(double t, double dt);

	private:
		const ComponentRegistry & m_components;
		const EntityRegistry & m_entities;

		std::vector<std::unique_ptr<SystemBase>> m_systems;
	};
}

namespace core
{
	template<typename System, typename ...Params>
	inline void SystemRegistry::add(const Params & ...params)
	{
		m_systems.emplace_back(std::make_unique<System>(m_components, params...));
	}
}