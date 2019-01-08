
#include "world/BlockRegistry.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world
{
	TEST_CLASS(BlockRegistryTest)
	{
	public:
		TEST_METHOD(BlockRegistry_has)
		{
			BlockRegistry registry;
			registry.add("foo", 4);

			Assert::IsTrue(registry.has("air"));
			Assert::IsTrue(registry.has("foo"));
			Assert::IsFalse(registry.has("bar"));
			Assert::IsTrue(registry.has(0));
			Assert::IsFalse(registry.has(1));
			Assert::IsTrue(registry.has(4));
		}

		TEST_METHOD(BlockRegistry_add)
		{
			BlockRegistry registry;
			const auto blockA = registry.add("foo");
			const auto blockB = registry.add("bar");

			Assert::AreEqual(3u, registry.size());
			Assert::AreEqual(BLOCK_AIR_ID + 1, blockA.m_id);
			Assert::AreEqual(BLOCK_AIR_ID + 2, blockB.m_id);
			Assert::AreEqual<std::string>("foo", blockA.m_name);
			Assert::AreEqual<std::string>("bar", blockB.m_name);

			Assert::ExpectException<std::invalid_argument>([&]() { registry.add("foo"); });
			Assert::ExpectException<std::invalid_argument>([&]() { registry.add(BLOCK_AIR_NAME); });
		}
		TEST_METHOD(BlockRegistry_insert)
		{
			BlockRegistry registry;
			const auto blockA = registry.add("foo", 4);
			const auto blockB = registry.add("bar", 2);

			Assert::AreEqual(5u, registry.size());
			Assert::AreEqual(4u, blockA.m_id);
			Assert::AreEqual(2u, blockB.m_id);
			Assert::AreEqual<std::string>("foo", blockA.m_name);
			Assert::AreEqual<std::string>("bar", blockB.m_name);

			Assert::ExpectException<std::invalid_argument>([&]() { registry.add("foo", 9); });
			Assert::ExpectException<std::invalid_argument>([&]() { registry.add("baz", 4); });
			Assert::ExpectException<std::invalid_argument>(
				[&]() { registry.add(BLOCK_AIR_NAME, 5); }
			);
			Assert::ExpectException<std::invalid_argument>(
				[&]() { registry.add("valid", BLOCK_AIR_ID); }
			);
		}

		TEST_METHOD(BlockRegistry_get)
		{
			BlockRegistry registry;
			const auto blockA = registry.add("foo", 3);
			const auto blockB = registry.add("bar", 7);
			const auto blockAir = registry[BLOCK_AIR_ID];

			Assert::IsTrue(blockA == registry[3]);
			Assert::IsTrue(blockA == registry["foo"]);
			Assert::IsTrue(blockB == registry[7]);
			Assert::IsTrue(blockB == registry["bar"]);
			Assert::IsTrue(blockAir == registry[2]);
			Assert::IsTrue(blockAir == registry[8]);
			Assert::IsTrue(blockAir == registry[42]);
			Assert::IsTrue(blockAir == registry["invalid"]);
			Assert::IsTrue(blockA == registry.get("foo"));
			Assert::IsTrue(blockB == registry.get("bar"));
			Assert::ExpectException<std::invalid_argument>([&]() { registry.get("invalid"); });
		}
	};
}