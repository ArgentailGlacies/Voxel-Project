
#include "script/ModuleRegistry.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::script
{
	TEST_CLASS(ModuleRegistryTest)
	{
	public:
		TEST_METHOD(ModuleRegistry_add)
		{
			ModuleRegistry registry;
			registry.add("identifier", [](auto &) {});

			Assert::IsTrue(registry.has("identifier"));
		}

		TEST_METHOD(ModuleRegistry_apply)
		{
			bool added = false;

			Script script{ "script" };
			ModuleRegistry registry;
			registry.add("identifier", [&added](auto &) { added = true; });
			registry.apply("identifier", script);

			Assert::IsTrue(added);
		}
	};
}