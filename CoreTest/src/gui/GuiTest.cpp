
#include "gui/Gui.h"

#include "mock/MockGui.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	TEST_CLASS(GuiTest)
	{
	public:
		TEST_METHOD(Gui_handleMousePress)
		{
			auto gui = mockGui();

			Assert::Fail();
		}
	};
}