#pragma once

#include "ecs/Entity.h"
#include "mock/MockComponent.h"

#include <functional>

namespace core
{
	/**
		Simple system which executes some sort of callback when invoked by system registry.
	*/
	template<typename ...Types>
	class MockSystemCallback : public System<Types...>
	{
	public:
		using Callback = std::function<void(const Entities & entities)>;

		MockSystemCallback(const ComponentRegistry & components, const Callback & callback)
			: System(components), m_callback(callback) {}

		virtual void process(const Entities & entities, double t, double dt) override final
		{
			m_callback(entities);
		}

	private:
		Callback m_callback;
	};

	/**
		Simple system which requires entities to take in one component.
	*/
	class MockSystemSingleComponent : public System<MockComponent>
	{
	public:
		MockSystemSingleComponent(const ComponentRegistry & components)
			: System(components) {}

		virtual void process(const Entities & entities, double t, double dt) override final {}
	};

	/**
		Simple system which requires entities to take in one component, and will write its
		parameter to the entity data.
	*/
	class MockSystemSingleComponentWithParam : public System<MockComponent>
	{
	public:
		MockSystemSingleComponentWithParam(
			const ComponentRegistry & components,
			const MockComponent & param
		) : System(components), m_param(param) {}

		virtual void process(const Entities & entities, double t, double dt) override final
		{
			auto & data = getData<MockComponent>();
			for (const auto & entity : entities)
				data[entity] = m_param;
		}

	private:
		MockComponent m_param;
	};

	/**
		Simple system which requires entities to take in two components.
	*/
	class MockSystemDoubleComponent : public System<float, int>
	{
	public:
		MockSystemDoubleComponent(const core::ComponentRegistry & components)
			: System(components) {}

		virtual void process(const core::Entities & entities, double t, double dt) override final {}

		template<typename Component> 
		inline auto & getData() const { return System::getData<Component>(); }
	};

	/**
		Simple system which requires entities to take in two components, and will write its
		parameters to the entity data.
	*/
	class MockSystemDoubleComponentWithParam : public System<float, int>
	{
	public:
		MockSystemDoubleComponentWithParam(
			const ComponentRegistry & components,
			const float & paramA,
			const int & paramB
		) : System(components), m_float(paramA), m_int(paramB) {}

		virtual void process(const Entities & entities, double t, double dt) override final
		{
			for (const auto & entity : entities)
			{
				getData<float>()[entity] = m_float;
				getData<int>()[entity] = m_int;
			}
		}

	private:
		float m_float;
		int m_int;
	};
}