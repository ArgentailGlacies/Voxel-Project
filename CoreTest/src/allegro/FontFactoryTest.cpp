
#include "allegro/FontFactory.h"

#include "mock/MockFont.h"

#include "Common.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::allegro
{
	TEST_CLASS(FontLoaderTest)
	{
	public:
		TEST_CLASS_INITIALIZE(initialize)
		{
			util::Folder{ "test" }.create();
			util::File{ "test/font.ttf" }.write(builtin::FONT_DATA, builtin::FONT_SIZE);
			util::File{ "test/font.xml" }.write(R"( <font size="18" path="test/font.ttf" /> )");
		}
		TEST_CLASS_CLEANUP(cleanup)
		{
			util::Folder{ "test" }.erase(true);
		}

		TEST_METHOD(FontLoader_load)
		{
			FontLoader factory;


		}
	};
}