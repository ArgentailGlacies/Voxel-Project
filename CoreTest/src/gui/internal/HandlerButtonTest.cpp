
#include "gui/internal/HandlerButton.h"

#include "gui/Widget.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	TEST_CLASS(HandlerButtonTest)
	{
	public:
		TEST_METHOD(HandlerButton_action)
		{
			int counter = 0;

			Widget widget;
			HandlerButton handler{ [&counter]() { counter = 7; } };

			handler.action(widget);
			Assert::IsTrue(widget.m_value.m_bool);
			Assert::AreEqual(7, counter);
		}

		TEST_METHOD(HandlerButtonCheckbox_action)
		{
			int counter = 0;

			Widget widget;
			HandlerButtonCheckbox handler{ [&counter]() { counter++; } };

			handler.action(widget);
			Assert::IsTrue(widget.m_value.m_bool);
			Assert::AreEqual(1, counter);
			handler.action(widget);
			Assert::IsFalse(widget.m_value.m_bool);
			Assert::AreEqual(2, counter);
		}

		TEST_METHOD(HandlerButtonRadio_action)
		{
			int counter = 0;

			Widget widgetA, widgetB;
			widgetA.m_group.m_members = { &widgetA, &widgetB };
			widgetA.m_group.m_leader = widgetB.m_group.m_leader = &widgetA;
			HandlerButtonRadio handler{ [&counter]() { counter++; } };

			handler.action(widgetA);
			Assert::IsTrue(widgetA.m_value.m_bool);
			Assert::IsFalse(widgetB.m_value.m_bool);
			Assert::AreEqual(1, counter);
			handler.action(widgetB);
			Assert::IsFalse(widgetA.m_value.m_bool);
			Assert::IsTrue(widgetB.m_value.m_bool);
			Assert::AreEqual(2, counter);

			handler.action(widgetB);
			Assert::AreEqual(2, counter);
		}
	};
}