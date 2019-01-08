
#include "opengl/VBO.h"

#include "Common.h"

#include <glm/mat4x4.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::opengl
{
	TEST_CLASS(VBOTest)
	{
	public:
		TEST_METHOD(VBO_ctor)
		{
			VBO vbo{ BufferType::ARRAY_BUFFER };

			Assert::AreNotEqual(0u, vbo.handle());
		}

		TEST_METHOD(VBO_bind)
		{
			VBO vbo{ BufferType::ARRAY_BUFFER };

			Assert::IsTrue(vbo.bind());
			Assert::IsFalse(vbo.bind());
		}
		TEST_METHOD(VBO_unbind)
		{
			VBO vbo{ BufferType::ARRAY_BUFFER };
			vbo.bind();

			Assert::IsTrue(vbo.unbind());
			Assert::IsFalse(vbo.unbind());
		}

		TEST_METHOD(VBO_reserve)
		{
			VBO vbo{ BufferType::ARRAY_BUFFER };

			Assert::IsTrue(vbo.reserve(64u));
		}
		TEST_METHOD(VBO_write)
		{
			VBO vbo{ BufferType::ARRAY_BUFFER };

			const auto data = glm::mat4{ 1.0f };
			Assert::IsTrue(vbo.reserve(sizeof(data)));
			Assert::IsTrue(vbo.write(64, &data));
		}
		TEST_METHOD(VBO_read)
		{
			VBO vbo{ BufferType::ARRAY_BUFFER };

			const auto dataA = glm::mat4{ 1.0f };
			auto dataB = glm::mat4{ 0.0f };
			Assert::IsTrue(vbo.reserve(sizeof(dataA)));
			Assert::IsTrue(vbo.write(sizeof(dataA), &dataA));
			Assert::IsTrue(vbo.read(sizeof(dataB), &dataB));
			Assert::AreEqual(dataA, dataB);
		}
	};
}