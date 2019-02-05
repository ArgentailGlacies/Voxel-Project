
#include "ui/Display.h"

#include "Common.h"
#include "Context.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::ui
{
	TEST_CLASS(DisplayTest)
	{
	public:
		TEST_METHOD(Display_initialize)
		{
			Assert::IsNotNull(display().handle());
		}

		TEST_METHOD(Display_isFullscreen)
		{
			Assert::IsFalse(display().isFullscreen());
		}
		TEST_METHOD(Display_getSize)
		{
			Assert::AreEqual({ 320, 240 }, display().getSize());
		}
		TEST_METHOD(Display_getAspectRatio)
		{
			Assert::AreEqual(1.333f, display().getAspectRatio(), 0.001f);
		}
	};
}