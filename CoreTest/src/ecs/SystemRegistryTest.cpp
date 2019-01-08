
#include "ecs/SystemRegistry.h"
#include "mock/MockComponentRegistry.h"
#include "mock/MockSystem.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::ecs
{
	TEST_CLASS(SystemRegistryTest)
	{
	public:
		TEST_METHOD(SystemRegistry_add)
		{
			bool processed = false;

			SystemRegistry registry{ m_components, m_entities };
			registry.add<MockSystemCallback<bool>>([&](auto &) { processed = true; });
			registry.process(0.0, 0.0);

			Assert::IsTrue(processed);
		}

		TEST_METHOD(SystemRegistry_process)
		{
			Entities entities;

			SystemRegistry registry{ m_components, m_entities };
			registry.add<MockSystemCallback<int>>([&](auto & e) { entities = e; });
			registry.add<MockSystemDoubleComponentWithParam>(3.14f, 42);

			const auto entityA = m_entities.create(m_components.mask<float>());
			const auto entityB = m_entities.create(m_components.mask<int>());
			const auto entityC = m_entities.create(m_components.mask<int, float>());
			registry.process(0.0, 1.0);

			Assert::AreEqual(2u, entities.size());
			Assert::AreEqual(entityB, entities[0]);
			Assert::AreEqual(entityC, entities[1]);
			Assert::AreEqual(0, m_components.get<int>()[entityB]);
			Assert::AreEqual(42, m_components.get<int>()[entityC]);
		}

	private:
		ComponentRegistry m_components = mockComponentRegistry();
		EntityRegistry m_entities;
	};
}