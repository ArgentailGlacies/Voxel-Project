
#include "gui/internal/HandlerSlider.h"

#include "event/EventBus.h"
#include "event/events.h"
#include "gui/Widget.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	TEST_CLASS(HandlerSliderTest)
	{
	public:
		TEST_METHOD(HandlerSlider_increment)
		{
			Widget widget;
			HandlerSlider handler{ [](Widget &) {}, { 0.0f, 5.0f, 2.5f, 0.1f } };

			handler.increment(widget);
			Assert::Fail();
		}

		TEST_METHOD(HandlerSliderButton_actionIncrement)
		{
			int counter = 0;

			Widget parent, widget;
			parent.m_value.m_float = 3.14f;
			widget.m_family.m_parent = &parent;
			HandlerSlider root{ [&counter](Widget &) { counter = 4; }, { 0.0f, 5.0f, 2.5f, 0.1f } };
			HandlerSliderButton handler{ root, true };

			handler.action(widget);
			Assert::AreEqual(3.2f, parent.m_value.m_float, 0.01f);
			Assert::AreEqual(4, counter);
		}
		TEST_METHOD(HandlerSliderButton_actionDecrement)
		{
			int counter = 0;

			Widget parent, widget;
			parent.m_value.m_float = 0.02f;
			widget.m_family.m_parent = &parent;
			HandlerSlider root{ [&counter](Widget &) { counter = 9; }, { 0.0f, 5.0f, 2.5f, 0.1f } };
			HandlerSliderButton handler{ root, false };

			handler.action(widget);
			Assert::AreEqual(0.0f, parent.m_value.m_float, 0.01f);
			Assert::AreEqual(9, counter);
		}

		TEST_METHOD(HandlerSliderBar_processHorizontal)
		{
			int counter = 0;

			Widget parent, widget;
			widget.m_family.m_parent = &parent;
			widget.m_bbox.m_size = { 100.0f, 10.0f };
			widget.m_state.m_selected = true;
			HandlerSlider root{ [&counter](Widget &) { counter = 5; }, { 0.0f, 10.0f, 9.0f, 0.1f } };
			HandlerSliderBar handler{ root, m_bus, true };

			simulateMouseMovement({ 31.0f, 0.0f });
			handler.process(widget);
			Assert::AreEqual(5.6f, parent.m_value.m_float, 0.01f);
			Assert::AreEqual(5, counter);

			simulateMouseMovement({ 92.0f, 0.0f });
			handler.process(widget);
			Assert::AreEqual(9.8f, parent.m_value.m_float, 0.01f);
			Assert::AreEqual(5, counter);
		}
		TEST_METHOD(HandlerSliderBar_processVertical)
		{
			int counter = 0;

			Widget parent, widget;
			widget.m_family.m_parent = &parent;
			widget.m_bbox.m_size = { 10.0f, 100.0f };
			widget.m_state.m_selected = true;
			HandlerSlider root{ [&counter](Widget &) { counter = 5; }, { 0.0f, 10.0f, 9.0f, 0.1f } };
			HandlerSliderBar handler{ root, m_bus, false };

			simulateMouseMovement({ 0.0f, 69.0f });
			handler.process(widget);
			Assert::AreEqual(5.6f, parent.m_value.m_float, 0.01f);
			Assert::AreEqual(5, counter);

			simulateMouseMovement({ 0.0f, 8.0f });
			handler.process(widget);
			Assert::AreEqual(9.8f, parent.m_value.m_float, 0.01f);
			Assert::AreEqual(5, counter);
		}

	private:
		void simulateMouseMovement(const glm::vec2 & position)
		{
			m_bus.post(MouseMove{ position, {}, {}, {} });
		}

		EventBus m_bus;
	};
}