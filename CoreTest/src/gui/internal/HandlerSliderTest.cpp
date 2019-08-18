
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
			Assert::AreEqual({ "3.20" }, widgetA.m_value.m_string);

			// Incrementing beyond maximum forces to nearest allowed value
			widgetA.m_value.m_float = 5.0f;
			handler.incrementer(widgetA)(widgetB);
			Assert::AreEqual(5.0f, widgetA.m_value.m_float, 0.01f);
			Assert::AreEqual({ "5.00" }, widgetA.m_value.m_string);
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
			Assert::AreEqual({ "3.00" }, widgetA.m_value.m_string);

			// Decrementing beyond minimum forces to nearest allowed value
			widgetA.m_value.m_float = 0.0f;
			handler.decrementer(widgetA)(widgetB);
			Assert::AreEqual(0.0f, widgetA.m_value.m_float, 0.01f);
			Assert::AreEqual({ "0.00" }, widgetA.m_value.m_string);
		}

		TEST_METHOD(HandlerSlider_translator)
		{
			Widget widgetA, widgetB;
			HandlerSlider handler{ callback(8), { 0.0f, 5.0f, 2.5f, 0.1f } };

			// Translating applies callback
			handler.translator(widgetA)(widgetB);
			verifyCallback(8);

			// Translating valid values work correctly
			widgetA.m_value.m_string = "4.2";
			handler.translator(widgetA)(widgetB);
			Assert::AreEqual(4.2f, widgetA.m_value.m_float, 0.01f);

			// Translating valid values work correctly
			widgetA.m_value.m_string = "invalid-value";
			handler.translator(widgetA)(widgetB);
			Assert::AreEqual(0.0f, widgetA.m_value.m_float, 0.01f);
		}

		TEST_METHOD(HandlerSlider_slider)
		{
			Widget widgetA, widgetB;
			HandlerSlider handler{ callback(7), { 0.0f, 5.0f, 2.5f, 0.1f } };

			// Slider applies callback
			handler.slider(widgetA)(widgetB);
			verifyCallback(7);

			// Slider translates correctly from factor
			widgetB.m_value.m_float = 0.23f;
			handler.slider(widgetA)(widgetB);
			Assert::AreEqual(1.2f, widgetA.m_value.m_float, 0.01f);

			widgetB.m_value.m_float = 0.92f;
			handler.slider(widgetA)(widgetB);
			Assert::AreEqual(4.6f, widgetA.m_value.m_float, 0.01f);
		}

		TEST_METHOD(HandlerSliderBar_callback)
		{
			Widget widget;
			HandlerSliderBar handler{ callback(5), m_bus, true };

			// Does not apply callback when not selected
			handler.process(widget);
			verifyCallback(0);

			// Applies callback when selected
			widget.m_state.m_selected = true;
			handler.process(widget);
			verifyCallback(5);
		}
		TEST_METHOD(HandlerSliderBar_processHorizontal)
		{
			Widget widget;
			widget.m_bbox.m_pos = { 10.0f, 10.0f };
			widget.m_bbox.m_size = { 80.0f, 10.0f };
			widget.m_state.m_selected = true;
			HandlerSliderBar handler{ callback(5), m_bus, true };

			// Slider bar applies correct factor to widget
			simulateMouseMovement({ 31.0f, 0.0f });
			handler.process(widget);
			Assert::AreEqual(0.2625f, widget.m_value.m_float, 0.001f);
		}
		TEST_METHOD(HandlerSliderBar_processVertical)
		{
			Widget widget;
			widget.m_bbox.m_pos = { 10.0f, 10.0f };
			widget.m_bbox.m_size = { 10.0f, 80.0f };
			widget.m_state.m_selected = true;
			HandlerSliderBar handler{ callback(6), m_bus, false };

			// Slider bar applies correct factor to widget
			simulateMouseMovement({ 0.0f, 31.0f });
			handler.process(widget);
			Assert::AreEqual(0.7375f, widget.m_value.m_float, 0.001f);
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