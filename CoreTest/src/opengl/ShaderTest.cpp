
#include "opengl/Shader.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::opengl
{
	TEST_CLASS(ShaderTest)
	{
	public:
		TEST_METHOD(Shader_ctor)
		{
			Shader shader{ ShaderType::VERTEX };

			Assert::AreNotEqual(0u, shader.handle());
		}

		TEST_METHOD(Shader_compile)
		{
			const std::string code = R"(
				#version 330 core
				
				out vec4 color;
					
				void main()
				{
					color = vec4(0, 0, 0, 1);
				}
			)";

			Shader shader{ ShaderType::VERTEX };

			Assert::IsTrue(shader.compile(code));
		}
	};
}