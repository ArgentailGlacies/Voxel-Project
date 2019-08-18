
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
		TEST_METHOD(HandlerSlider_incrementer)
		{
			Widget widgetA, widgetB;
			HandlerSlider handler{ callback(3), { 0.0f, 5.0f, 2.5f, 0.1f } };

			// Incrementing applies callback
			handler.incrementer(widgetA)(widgetB);
			verifyCallback(3);

			// Incrementing rounds values correctly
			widgetA.m_value.m_float = 3.14f;
			handler.incrementer(widgetA)(widgetB);
			Assert::AreEqual(3.2f, widgetA.m_value.m_float, 0.01f);

			// Incrementing beyond maximum forces to nearest allowed value
			widgetA.m_value.m_float = 5.0f;
			handler.incrementer(widgetA)(widgetB);
			Assert::AreEqual(5.0f, widgetA.m_value.m_float, 0.01f);
		}
		TEST_METHOD(HandlerSlider_decrementer)
		{
			Widget widgetA, widgetB;
			HandlerSlider handler{ callback(9), { 0.0f, 5.0f, 2.5f, 0.1f } };

			// Decrementing applies callback
			handler.decrementer(widgetA)(widgetB);
			verifyCallback(9);

			// Decrementing rounds values correctly
			widgetA.m_value.m_float = 3.14f;
			handler.decrementer(widgetA)(widgetB);
			Assert::AreEqual(3.0f, widgetA.m_value.m_float, 0.01f);

			// Decrementing beyond minimum forces to nearest allowed value
			widgetA.m_value.m_float = 0.0f;
			handler.decrementer(widgetA)(widgetB);
			Assert::AreEqual(0.0f, widgetA.m_value.m_float, 0.01f);
		}

		TEST_METHOD(HandlerSliderBar_processHorizontal)
		{
			Widget parent, widget;
			widget.m_family.m_parent = &parent;
			widget.m_bbox.m_size = { 100.0f, 10.0f };
			widget.m_state.m_selected = true;
			HandlerSlider root{ callback(5), { 0.0f, 10.0f, 9.0f, 0.1f } };
			HandlerSliderBar handler{ root, m_bus, true };

			simulateMouseMovement({ 31.0f, 0.0f });
			handler.process(widget);
			Assert::AreEqual(5.6f, parent.m_value.m_float, 0.01f);
			verifyCallback(5);

			simulateMouseMovement({ 92.0f, 0.0f });
			handler.process(widget);
			Assert::AreEqual(9.8f, parent.m_value.m_float, 0.01f);
			verifyCallback(5);
		}
		TEST_METHOD(HandlerSliderBar_processVertical)
		{
			Widget parent, widget;
			widget.m_family.m_parent = &parent;
			widget.m_bbox.m_size = { 10.0f, 100.0f };
			widget.m_state.m_selected = true;
			HandlerSlider root{ callback(6), { 0.0f, 10.0f, 9.0f, 0.1f } };
			HandlerSliderBar handler{ root, m_bus, false };

			simulateMouseMovement({ 0.0f, 69.0f });
			handler.process(widget);
			Assert::AreEqual(5.6f, parent.m_value.m_float, 0.01f);
			verifyCallback(6);

			simulateMouseMovement({ 0.0f, 8.0f });
			handler.process(widget);
			Assert::AreEqual(9.8f, parent.m_value.m_float, 0.01f);
			verifyCallback(6);
		}

	private:
		Handler::Callback callback(int value)
		{
			return [this, value](Widget &) { m_callbackValue = value; };
		}
		void verifyCallback(int value)
		{
			Assert::AreEqual(value, m_callbackValue);
		}

		void simulateMouseMovement(const glm::vec2 & position)
		{
			m_bus.post(MouseMove{ position, {}, {}, {} });
		}

		int m_callbackValue = 0;
		EventBus m_bus;
	};
}