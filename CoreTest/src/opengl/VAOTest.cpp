
#include "opengl/VAO.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::opengl
{
	TEST_CLASS(VAOTest)
	{
	public:
		TEST_METHOD(VAO_ctor)
		{
			VAO vao;

			Assert::AreNotEqual(0u, vao.handle());
		}

		TEST_METHOD(VAO_bind)
		{
			VAO vao;

			Assert::IsTrue(vao.bind());
			Assert::IsFalse(vao.bind());
		}
		TEST_METHOD(VAO_unbind)
		{
			VAO vao;
			vao.bind();

			Assert::IsTrue(vao.unbind());
			Assert::IsFalse(vao.unbind());
		}
	};
}