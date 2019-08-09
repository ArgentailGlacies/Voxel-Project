
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
			util::File{ "test/font-bold.ttf" }.write(builtin::FONT_DATA, builtin::FONT_SIZE);
			util::File{ "test/font.xml" }.write(R"(
				<font>
					<style size="18" path="test/font.ttf" />
					<style size="18" type="bold" path="test/font-bold.ttf" />
				</font>
			)");
		}
		TEST_CLASS_CLEANUP(cleanup)
		{
			util::Folder{ "test" }.erase(true);
		}

		TEST_METHOD(FontLoader_load)
		{
			FontLoader factory;
			auto font = factory.load("test/font.xml");

			const auto handleA = font->handle(Font::Style::REGULAR);
			const auto handleB = font->handle(Font::Style::BOLD);
			const auto handleC = font->handle(Font::Style::ITALIC);

			Assert::IsFalse(handleA == handleB);
			Assert::IsFalse(handleA == handleC);
			Assert::IsFalse(handleB == handleC);
		}
	};
}