
#include "opengl/ProgramFactory.h"
#include "Context.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::opengl
{
	TEST_CLASS(ProgramLoaderTest)
	{
	public:
		ProgramLoaderTest() { init(); }
		~ProgramLoaderTest() { deinit(); }

		TEST_METHOD(ProgramLoader_load)
		{
			UBORegistry registry;
			ProgramLoader loader;
			auto programA = loader.load("test/programA.xml", registry);
			auto programB = loader.load("test/programB.xml", registry);

			Assert::IsTrue(programA->bind());
			Assert::IsTrue(programB->bind());
		}

	private:
		void init()
		{
			util::Folder{ "test" }.create();

			util::File{ "test/programA.xml" }.write(R"(
				<shader type="fragment">
				void main()
				{
					gl_FragColor = vec4(0, 0, 0, 1);
				}
				</shader>
				<shader type="vertex">
				void main()
				{
					gl_Position  = vec4(0, 0, 0, 1);
				}
				</shader>
			)");

			util::File{ "test/programB.xml" }.write(R"(
				<shader type="vertex" file="test/program.vs" />
				<shader type="fragment" file="test/program.fs" />
			)");
			util::File{ "test/program.fs" }.write(R"(
				void main()
				{
					gl_FragColor = vec4(0, 0, 0, 1);
				}
			)");
			util::File{ "test/program.vs" }.write(R"(
				void main()
				{
					gl_Position  = vec4(0, 0, 0, 1);
				}
			)");
		}
		void deinit()
		{
			util::Folder{ "test" }.erase(true);
		}
	};
}