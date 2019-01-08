
#include "ecs/ComponentRegistry.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::ecs
{
	TEST_CLASS(ComponentRegistryTest)
	{
	public:
		TEST_METHOD(ComponentRegistry_add)
		{
			ComponentRegistry registry;

			Assert::IsFalse(registry.has<int>());
			registry.add<int>();
			Assert::IsTrue(registry.has<int>());
			Assert::ExpectException<std::invalid_argument>([&]() { registry.add<int>(); });
		}

		TEST_METHOD(ComponentRegistry_get)
		{
			ComponentRegistry registry;
			registry.add<int>();
			registry.add<float>();
			const auto & componentA = registry.get<int>();
			const auto & componentB = registry.get<float>();

			Assert::IsTrue(&componentA == &registry.get<int>());
			Assert::IsTrue(&componentB == &registry.get<float>());
			Assert::ExpectException<std::invalid_argument>([&]() { registry.get<double>(); });
		}
		TEST_METHOD(ComponentRegistry_has)
		{
			ComponentRegistry registry;
			registry.add<int>();

			Assert::IsTrue(registry.has<int>());
			Assert::IsFalse(registry.has<float>());
		}

		TEST_METHOD(ComponentRegistry_mask)
		{
			ComponentRegistry registry;
			registry.add<int>();
			registry.add<float>();
			registry.add<double>();

			const auto maskA = registry.mask<int, double>();
			const auto maskB = registry.mask<float>();
			const auto maskC = registry.mask<double>();

			Assert::AreEqual(2u, maskA.count());
			Assert::AreEqual(1u, maskB.count());
			Assert::AreEqual(1u, maskC.count());
			Assert::AreEqual(0u, (maskA & maskB).count());
			Assert::AreEqual(1u, (maskA & maskC).count());
			Assert::AreEqual(0u, (maskB & maskC).count());
			Assert::ExpectException<std::invalid_argument>([&]() { registry.mask<bool>(); });
		}
	};
}