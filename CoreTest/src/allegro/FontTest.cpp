
#include "allegro/Font.h"

#include "io/Folder.h"
#include "mock/MockFont.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace
{
	const std::string file = "testfont.ttf";
}

namespace core::allegro
{
	TEST_CLASS(FontTest)
	{
	public:
		FontTest() { util::File{ file }.write(builtin::FONT_DATA, builtin::FONT_SIZE); }
		~FontTest() { util::File{ file }.erase(); }

		TEST_METHOD(Font_prepare)
		{
			Font font;
			font.prepare(file, Font::Style::REGULAR, 12.0f);

			const auto handleA = font.handle(Font::Style::REGULAR);	// loaded font
			const auto handleB = font.handle(Font::Style::BOLD);	// built-in font
			const auto handleC = font.handle(Font::Style::ITALIC);	// built-in font

			Assert::IsFalse(handleA == handleB);
			Assert::IsTrue(handleB == handleC);
		}

		TEST_METHOD(Font_handle)
		{
			Font font;
			font.prepare(file, Font::Style::REGULAR, 12.0f);

			const auto handleA = font.handle(Font::Style::REGULAR, Font::Flag::NORMAL, 1.0f);
			const auto handleB = font.handle(Font::Style::REGULAR, Font::Flag::NORMAL, 1.5f);
			const auto handleC = font.handle(Font::Style::BOLD);

			Assert::IsNotNull(handleA);
			Assert::IsNotNull(handleB);
			Assert::IsNotNull(handleC);
			Assert::IsFalse(handleA == handleB);
			Assert::IsFalse(handleA == handleC);
			Assert::IsFalse(handleB == handleC);
		}
	};
}