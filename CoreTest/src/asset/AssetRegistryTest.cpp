
#include "asset/AssetRegistry.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::asset
{
	TEST_CLASS(AssetRegistryTest)
	{
	public:
		TEST_METHOD(AssetRegistry_has)
		{
			AssetRegistry registry;
			registry.add<int>("key", []() { return std::make_unique<int>(); });

			Assert::IsTrue(registry.has<int>("key"));
			Assert::IsFalse(registry.has<float>("key"));
			Assert::IsFalse(registry.has<int>("invalid"));
		}
			
		TEST_METHOD(AssetRegistry_add)
		{
			AssetRegistry registry;
			registry.add<float>("keyA", []() { return std::make_unique<float>(3.14f); });
			registry.add<int>("keyA", []() { return std::make_unique<int>(1); });
			registry.add<int>("keyB", []() { return std::make_unique<int>(42); });
			registry.add<int>("keyB", []() { return std::make_unique<int>(8); });

			Assert::AreEqual<float>(3.14f, registry.get<float>("keyA"));
			Assert::AreEqual<int>(1, registry.get<int>("keyA"));
			Assert::AreEqual<int>(8, registry.get<int>("keyB"));
		}
		TEST_METHOD(AssetRegistry_get)
		{
			AssetRegistry registry;
			registry.add<int>("key", []() { return std::make_unique<int>(3); });

			auto & referenceA = registry.get<int>("key");
			auto & referenceB = registry.get<int>("invalid");
			Assert::IsFalse(referenceA.empty());
			Assert::IsTrue(referenceB.empty());
			Assert::AreEqual({ "key" }, referenceA.name());
		}
	};
}