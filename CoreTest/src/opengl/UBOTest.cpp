
#include "opengl/UBO.h"

#include <glm/mat4x4.hpp>

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::opengl
{
	TEST_CLASS(UBOTest)
	{
	public:
		TEST_METHOD(UBO_ctor)
		{
			UBO ubo{ 0u };

			Assert::AreNotEqual(0u, ubo.handle());
		}

		TEST_METHOD(UBO_bind)
		{
			UBO ubo{ 0u };

			Assert::IsTrue(ubo.bind());
			Assert::IsFalse(ubo.bind());
		}
		TEST_METHOD(UBO_unbind)
		{
			UBO ubo{ 0u };
			ubo.bind();

			Assert::IsTrue(ubo.unbind());
			Assert::IsFalse(ubo.unbind());
		}

		TEST_METHOD(UBO_build)
		{
			UBO ubo{ 0u };

			Assert::IsTrue(ubo.build());
		}

		TEST_METHOD(UBO_has)
		{
			UBO ubo{ 0u };

			Assert::IsFalse(ubo.has("foo"));
			ubo.add<int>("foo");
			Assert::IsTrue(ubo.has("foo"));
		}
		TEST_METHOD(UBO_add)
		{
			UBO ubo{ 0u };

			Assert::IsTrue(ubo.add<glm::mat4>("matrix"));
			Assert::IsFalse(ubo.add<glm::mat4>("matrix"));
		}
		TEST_METHOD(UBO_set)
		{
			UBO ubo{ 0u };
			ubo.add<glm::ivec3>("vector");
			ubo.build();

			Assert::IsTrue(ubo.set("vector", glm::ivec3{ 3, 1, 4 }));
			Assert::IsFalse(ubo.set("vector", 3.14f));
			Assert::IsFalse(ubo.set("invalid", 0));
		}
		TEST_METHOD(UBO_get)
		{
			UBO ubo{ 0u };
			ubo.add<int>("field");
			ubo.build();
			ubo.set("field", 42);

			int data;
			Assert::IsTrue(ubo.get("field", data));
			Assert::AreEqual(42, data);
		}
	};
}