
#include "gui/WidgetAction.h"

#include "gui/Widget.h"
#include "script/Script.h"
#include "script/ScriptUtil.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	TEST_CLASS(WidgetActionTest)
	{
	public:
		WidgetActionTest() { initialize(); }

		TEST_METHOD(WidgetActionSliderBar_invoke)
		{
			Widget widget;

			WidgetActionSliderBar{ m_script, "counter += 1" }(widget);
			Assert::AreEqual(1, util::get<int>(m_script, "counter"));
		}
		TEST_METHOD(WidgetActionSliderButton_invoke)
		{
			Widget widget;
			widget.m_value.m_float = 3.14f;

			WidgetActionSliderButton{ widget, m_script, "counter += 1", { 0.0f, 10.0f, 9.0f, 0.1f }, true }(widget);
			Assert::AreEqual(1, util::get<int>(m_script, "counter"));
			Assert::AreEqual(3.2f, widget.m_value.m_float, 0.01f);
		}

	private:
		void initialize()
		{
			m_script.execute("global counter = 0;");
		}

		Script m_script{ "script" };
	};
}