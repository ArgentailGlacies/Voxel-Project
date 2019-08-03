
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

		TEST_METHOD(WidgetActionButton_invoke)
		{
			Widget widget;

			WidgetActionButton{ m_script, "counter += 1" }(widget);
			Assert::IsTrue(widget.m_value.m_bool);
			Assert::AreEqual(1, util::get<int>(m_script, "counter"));
		}
		TEST_METHOD(WidgetActionButtonCheckbox_invoke)
		{
			Widget widget;

			WidgetActionButtonCheckbox{ m_script, "counter += 1" }(widget);
			Assert::IsTrue(widget.m_value.m_bool);
			Assert::AreEqual(1, util::get<int>(m_script, "counter"));

			WidgetActionButtonCheckbox{ m_script, "counter += 1" }(widget);
			Assert::IsFalse(widget.m_value.m_bool);
			Assert::AreEqual(2, util::get<int>(m_script, "counter"));
		}
		TEST_METHOD(WidgetActionButtonRadio_invoke)
		{
			Widget widgetA, widgetB;
			widgetA.m_group.m_members.push_back(&widgetA);
			widgetA.m_group.m_members.push_back(&widgetB);
			widgetA.m_group.m_leader = &widgetA;
			widgetB.m_group.m_leader = &widgetA;

			WidgetActionButtonRadio{ m_script, "counter += 1" }(widgetA);
			Assert::IsTrue(widgetA.m_value.m_bool);
			Assert::IsFalse(widgetB.m_value.m_bool);
			Assert::AreEqual(1, util::get<int>(m_script, "counter"));

			WidgetActionButtonRadio{ m_script, "counter += 1" }(widgetB);
			Assert::IsFalse(widgetA.m_value.m_bool);
			Assert::IsTrue(widgetB.m_value.m_bool);
			Assert::AreEqual(2, util::get<int>(m_script, "counter"));

			WidgetActionButtonRadio{ m_script, "counter += 1" }(widgetB);
			Assert::IsFalse(widgetA.m_value.m_bool);
			Assert::IsTrue(widgetB.m_value.m_bool);
			Assert::AreEqual(2, util::get<int>(m_script, "counter"));
		}

	private:
		void initialize()
		{
			m_script.execute("global counter = 0;");
		}

		Script m_script{ "script" };
	};
}