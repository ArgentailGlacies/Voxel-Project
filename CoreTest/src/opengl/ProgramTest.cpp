
#include "opengl/Program.h"
#include "opengl/Shader.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::opengl
{
	TEST_CLASS(ProgramTest)
	{
	public:
		TEST_METHOD(Program_ctor)
		{
			Program program;

			Assert::AreNotEqual(0u, program.handle());
		}

		TEST_METHOD(Program_bind)
		{
			Program program = build();

			Assert::IsTrue(program.bind());
			Assert::IsFalse(program.bind());
		}
		TEST_METHOD(Program_unbind)
		{
			Program program = build();
			program.bind();

			Assert::IsTrue(program.unbind());
			Assert::IsFalse(program.unbind());
		}

		TEST_METHOD(Program_attach)
		{
			Shader shader{ ShaderType::VERTEX };
			Program program;

			Assert::IsTrue(program.attach(shader));
		}
		TEST_METHOD(Program_link)
		{
			Program program;
			program.attach(vertex());
			program.attach(fragment());

			Assert::IsTrue(program.link());
		}

	private:
		Shader vertex()
		{
			Shader shader{ ShaderType::VERTEX };
			shader.compile("void main() { gl_Position = vec4(0, 0, 0, 1); }");
			return shader;
		}
		Shader fragment()
		{
			Shader shader{ ShaderType::FRAGMENT };
			shader.compile("void main() { gl_FragColor = vec4(0, 0, 0, 1); }");
			return shader;
		}
		Program build()
		{
			Program program;
			program.attach(vertex());
			program.attach(fragment());
			program.link();
			return program;
		}
	};
}