
#include "ecs/ECS.h"
#include "mock/MockComponent.h"
#include "mock/MockSystem.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::ecs
{
	TEST_CLASS(ECSTest)
	{
	public:
		TEST_METHOD(ECS_process)
		{
			ECS ecs;
			ecs.addComponent<MockComponent>();
			ecs.addSystem<MockSystemSingleComponentWithParam>(MockComponent{ true, 13 });
			const auto entity = ecs.createEntity<MockComponent>();
			const auto & data = ecs.getEntityData<MockComponent>(entity);

			// Should not crash when entities have been added to the system
			Assert::IsFalse(data.m_bool);
			Assert::AreNotEqual(13, data.m_int);
			ecs.process(0.0, 1.0);
			Assert::IsTrue(data.m_bool);
			Assert::AreEqual(13, data.m_int);
		}

		TEST_METHOD(ECS_addComponent)
		{
			ECS ecs;

			// Adding components for the first time cause no problem
			ecs.addComponent<MockComponent>();

			// Cannot add the same component multiple times
			Assert::ExpectException<std::invalid_argument>(
				[&]() { ecs.addComponent<MockComponent>(); }
			);
		}
		TEST_METHOD(ECS_addSystem)
		{
			ECS ecs;

			// Cannot add system before all its components have been added
			Assert::ExpectException<std::invalid_argument>(
				[&]() { ecs.addSystem<MockSystemSingleComponent>(); }
			);

			// After component has been added, system should add just fine
			ecs.addComponent<MockComponent>();
			ecs.addSystem<MockSystemSingleComponent>();
		}

		TEST_METHOD(ECS_createEntity)
		{
			ECS ecs;
			ecs.addComponent<int>();
			ecs.addComponent<bool>();
			ecs.addComponent<MockComponent>();

			// Can create as many entities as desired with a wide range of components
			const auto entityA = ecs.createEntity<MockComponent>();
			const auto entityB = ecs.createEntity<MockComponent, int, bool>();
			Assert::AreNotEqual(entityA, entityB);

			// Cannot add entities with components that have not been added
			Assert::ExpectException<std::invalid_argument>([&]() { ecs.createEntity<float>(); });
		}
		TEST_METHOD(ECS_removeEntity)
		{
			ECS ecs;
			ecs.addComponent<int>();
			const auto entityA = ecs.createEntity<int>();
			const auto entityB = ecs.createEntity<int>();
			ecs.destroyEntity(entityA);

			Assert::IsFalse(ecs.hasEntity(entityA));
			Assert::IsTrue(ecs.hasEntity(entityB));
		}
		TEST_METHOD(ECS_hasEntity)
		{
			ECS ecs;
			ecs.addComponent<int>();
			const auto entity = ecs.createEntity<int>();

			Assert::IsTrue(ecs.hasEntity(entity));
			Assert::IsFalse(ecs.hasEntity(1));
		}

		TEST_METHOD(ECS_attachEntityComponent)
		{
			ECS ecs;
			ecs.addComponent<int>();
			ecs.addComponent<float>();
			ecs.addComponent<double>();
			const auto entity = ecs.createEntity<int>();
			
			// Can attach components the entity does not already have
			Assert::IsFalse(ecs.hasEntityData<float>(entity));
			Assert::IsFalse(ecs.hasEntityData<double>(entity));
			ecs.attachEntityComponent<float, double>(entity);
			Assert::IsTrue(ecs.hasEntityData<float>(entity));
			Assert::IsTrue(ecs.hasEntityData<double>(entity));
		}
		TEST_METHOD(ECS_detachEntityComponent)
		{
			ECS ecs;
			ecs.addComponent<int>();
			ecs.addComponent<float>();
			ecs.addComponent<double>();
			const auto entity = ecs.createEntity<int, float, double>();

			// Can attach components the entity does not already have
			Assert::IsTrue(ecs.hasEntityData<float>(entity));
			Assert::IsTrue(ecs.hasEntityData<double>(entity));
			ecs.detachEntityComponent<float, double>(entity);
			Assert::IsFalse(ecs.hasEntityData<float>(entity));
			Assert::IsFalse(ecs.hasEntityData<double>(entity));
		}

		TEST_METHOD(ECS_hasEntityData)
		{
			ECS ecs;
			ecs.addComponent<int>();
			ecs.addComponent<float>();
			const auto entity = ecs.createEntity<int>();

			// Accessing components added to the system works alright
			Assert::IsTrue(ecs.hasEntityData<int>(entity));
			Assert::IsFalse(ecs.hasEntityData<float>(entity));

			// Accessing components not added cannot be done
			Assert::ExpectException<std::invalid_argument>(
				[&]() { ecs.hasEntityData<double>(entity); }
			);
		}
		TEST_METHOD(ECS_getEntityData)
		{
			ECS ecs;
			ecs.addComponent<int>();
			ecs.addComponent<float>();
			const auto entity = ecs.createEntity<int>();

			// Accessing entity data is fine when given valid component
			ecs.getEntityData<int>(entity);

			// Accessing entity data which is not valid cannot be done
			Assert::ExpectException<std::invalid_argument>(
				[&]() { ecs.getEntityData<float>(entity); }
			);
		}
	};
}