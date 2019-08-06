
#include "gui/internal/HandlerButton.h"

#include "gui/Widget.h"
#include "script/Script.h"
#include "script/ScriptUtil.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	TEST_CLASS(HandlerButtonTest)
	{
	public:
		HandlerButtonTest() { initialize(); }

		TEST_METHOD(HandlerButton_action)
		{
			Widget widget;
			HandlerButton handler{ m_script, "counter = 7" };

			handler.action(widget);
			Assert::IsTrue(widget.m_value.m_bool);
			Assert::AreEqual(7, util::get<int>(m_script, "counter"));
		}

		TEST_METHOD(HandlerButtonCheckbox_action)
		{
			Widget widget;
			HandlerButtonCheckbox handler{ m_script, "counter += 1" };

			handler.action(widget);
			Assert::IsTrue(widget.m_value.m_bool);
			Assert::AreEqual(1, util::get<int>(m_script, "counter"));
			handler.action(widget);
			Assert::IsFalse(widget.m_value.m_bool);
			Assert::AreEqual(2, util::get<int>(m_script, "counter"));
		}

		TEST_METHOD(HandlerButtonRadio_action)
		{
			Widget widgetA, widgetB;
			widgetA.m_group.m_members = { &widgetA, &widgetB };
			widgetA.m_group.m_leader = widgetB.m_group.m_leader = &widgetA;
			HandlerButtonRadio handler{ m_script, "counter += 1" };

			handler.action(widgetA);
			Assert::IsTrue(widgetA.m_value.m_bool);
			Assert::IsFalse(widgetB.m_value.m_bool);
			Assert::AreEqual(1, util::get<int>(m_script, "counter"));
			handler.action(widgetB);
			Assert::IsFalse(widgetA.m_value.m_bool);
			Assert::IsTrue(widgetB.m_value.m_bool);
			Assert::AreEqual(2, util::get<int>(m_script, "counter"));

			handler.action(widgetB);
			Assert::AreEqual(2, util::get<int>(m_script, "counter"));
		}

	private:
		void initialize()
		{
			m_script.execute("global counter = 0");
		}

		Script m_script{ "script" };
	};
}