
#include "ecs/EntityRegistry.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::ecs
{
	constexpr ComponentMask EMPTY_MASK = 0x0;
	constexpr ComponentMask VALID_MASK = 0x1;

	TEST_CLASS(EntityRegistryTest)
	{
	public:
		TEST_METHOD(EntityRegistry_create)
		{
			EntityRegistry registry;
			const auto entityA = registry.create(VALID_MASK);
			const auto entityB = registry.create(VALID_MASK);

			Assert::AreNotEqual(entityA, entityB);
		}
		TEST_METHOD(EntityRegistry_destroy)
		{
			EntityRegistry registry;
			const auto entity = registry.create(VALID_MASK);

			Assert::IsTrue(registry.has(entity));
			registry.destroy(entity);
			Assert::IsFalse(registry.has(entity));
		}
		TEST_METHOD(EntityRegistry_destroyThenCreate)
		{
			EntityRegistry registry;
			const auto entityA = registry.create(VALID_MASK);
			const auto entityB = registry.create(VALID_MASK);
			registry.destroy(entityA);
			const auto entityC = registry.create(VALID_MASK);

			Assert::AreEqual(entityA, entityC);
		}

		TEST_METHOD(EntityRegistry_attach)
		{
			EntityRegistry registry;
			const auto entityA = registry.create(0b0010);
			const auto entityB = registry.create(0b0000);
			registry.attach(entityA, 0b0110);
			registry.attach(entityB, 0b1000);

			Assert::IsTrue(ComponentMask{ 0b0110 } == registry.get(entityA));
			Assert::IsTrue(ComponentMask{ 0b1000 } == registry.get(entityB));
		}
		TEST_METHOD(EntityRegistry_detach)
		{
			EntityRegistry registry;
			const auto entityA = registry.create(0b1010);
			const auto entityB = registry.create(0b0000);
			registry.detach(entityA, 0b0110);
			registry.detach(entityB, 0b1000);

			Assert::IsTrue(ComponentMask{ 0b1000 } == registry.get(entityA));
			Assert::IsTrue(ComponentMask{ 0b0000 } == registry.get(entityB));
		}

		TEST_METHOD(EntityRegistry_has)
		{
			EntityRegistry registry;
			const auto entityA = registry.create(VALID_MASK);
			const auto entityB = registry.create(EMPTY_MASK);

			Assert::IsTrue(registry.has(entityA));
			Assert::IsFalse(registry.has(entityB));
			Assert::IsFalse(registry.has(2));
		}
		TEST_METHOD(EntityRegistry_get)
		{
			EntityRegistry registry;
			const auto entityA = registry.create(VALID_MASK);
			const auto entityB = registry.create(VALID_MASK);
			registry.destroy(entityB);

			Assert::IsTrue(VALID_MASK == registry.get(entityA));
			Assert::IsTrue(EMPTY_MASK == registry.get(entityB));
			Assert::IsTrue(EMPTY_MASK == registry.get(2));
		}

		TEST_METHOD(EntityRegistry_find)
		{
			EntityRegistry registry;
			const auto entityA = registry.create(0b0010);
			const auto entityB = registry.create(0b0110);
			const auto entityC = registry.create(0b1101);
			const auto entitiesA = registry.findAllMatching(0b0000);
			const auto entitiesB = registry.findAllMatching(0b0010);
			const auto entitiesC = registry.findAllMatching(0b0110);

			Assert::AreEqual(3u, entitiesA.size());
			Assert::AreEqual(entityA, entitiesA[0]);
			Assert::AreEqual(entityB, entitiesA[1]);
			Assert::AreEqual(entityC, entitiesA[2]);
			Assert::AreEqual(2u, entitiesB.size());
			Assert::AreEqual(entityA, entitiesB[0]);
			Assert::AreEqual(entityB, entitiesB[1]);
			Assert::AreEqual(1u, entitiesC.size());
			Assert::AreEqual(entityB, entitiesC[0]);
		}
	};
}