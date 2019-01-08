
#include "opengl/UBORegistry.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::opengl
{
	TEST_CLASS(UBORegistryTest)
	{
	public:
		TEST_METHOD(UBORegistry_has)
		{
			UBORegistry registry;
			registry.add<int>("block", "member");

			Assert::IsTrue(registry.has("block"));
			Assert::IsFalse(registry.has("invalid"));
			Assert::IsTrue(registry.has("block", "member"));
			Assert::IsFalse(registry.has("block", "invalid"));
		}
		TEST_METHOD(UBORegistry_add)
		{
			UBORegistry registry;

			Assert::IsTrue(registry.add<int>("block", "foo"));
			Assert::IsFalse(registry.add<int>("block", "foo"));
		}
		TEST_METHOD(UBORegistry_build)
		{
			UBORegistry registry;
			registry.add<int>("block", "member");

			Assert::IsTrue(registry.build("block"));
			Assert::IsFalse(registry.build("invalid"));
		}

		TEST_METHOD(UBORegistry_bindings)
		{
			UBORegistry registry;
			registry.add<int>("block", "member");
			registry.build("block");

			const auto bindings = registry.bindings();
			Assert::IsFalse(bindings.find("block") == bindings.end());
			Assert::IsTrue(bindings.find("invalid") == bindings.end());
		}
	};
}