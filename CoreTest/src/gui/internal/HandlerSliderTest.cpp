
#include "gui/internal/HandlerSlider.h"

#include "event/EventBus.h"
#include "event/events.h"
#include "gui/Widget.h"
#include "script/Script.h"
#include "script/ScriptUtil.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	TEST_CLASS(HandlerSliderTest)
	{
	public:
		HandlerSliderTest() { initialize(); }

		TEST_METHOD(HandlerSlider_action)
		{
			Widget widget;
			HandlerSlider handler{ m_script, "counter = 7", {} };

			handler.action(widget);
			Assert::AreEqual(7, util::get<int>(m_script, "counter"));
		}

		TEST_METHOD(HandlerSliderButton_actionIncrement)
		{
			Widget parent, widget;
			parent.m_value.m_float = 3.14f;
			widget.m_family.m_parent = &parent;
			HandlerSlider root{ m_script, "counter = 4", { 0.0f, 5.0f, 2.5f, 0.1f } };
			HandlerSliderButton handler{ root, true };

			handler.action(widget);
			Assert::AreEqual(3.2f, parent.m_value.m_float, 0.01f);
			Assert::AreEqual(4, util::get<int>(m_script, "counter"));
		}
		TEST_METHOD(HandlerSliderButton_actionDecrement)
		{
			Widget parent, widget;
			parent.m_value.m_float = 0.02f;
			widget.m_family.m_parent = &parent;
			HandlerSlider root{ m_script, "counter = 9", { 0.0f, 5.0f, 2.5f, 0.1f } };
			HandlerSliderButton handler{ root, false };

			handler.action(widget);
			Assert::AreEqual(0.0f, parent.m_value.m_float, 0.01f);
			Assert::AreEqual(9, util::get<int>(m_script, "counter"));
		}

		TEST_METHOD(HandlerSliderBar_processHorizontal)
		{
			Widget parent, widget;
			widget.m_family.m_parent = &parent;
			widget.m_bbox.m_size = { 100.0f, 10.0f };
			widget.m_state.m_selected = true;
			HandlerSlider root{ m_script, "counter = 5", { 0.0f, 10.0f, 9.0f, 0.1f } };
			HandlerSliderBar handler{ root, m_bus, true };

			simulateMouseMovement({ 31.0f, 0.0f });
			handler.process(widget);
			Assert::AreEqual(5.6f, parent.m_value.m_float, 0.01f);
			Assert::AreEqual(5, util::get<int>(m_script, "counter"));

			simulateMouseMovement({ 92.0f, 0.0f });
			handler.process(widget);
			Assert::AreEqual(9.8f, parent.m_value.m_float, 0.01f);
			Assert::AreEqual(5, util::get<int>(m_script, "counter"));
		}
		TEST_METHOD(HandlerSliderBar_processVertical)
		{
			Widget parent, widget;
			widget.m_family.m_parent = &parent;
			widget.m_bbox.m_size = { 10.0f, 100.0f };
			widget.m_state.m_selected = true;
			HandlerSlider root{ m_script, "counter = 5", { 0.0f, 10.0f, 9.0f, 0.1f } };
			HandlerSliderBar handler{ root, m_bus, false };

			simulateMouseMovement({ 0.0f, 31.0f });
			handler.process(widget);
			Assert::AreEqual(5.6f, parent.m_value.m_float, 0.01f);
			Assert::AreEqual(5, util::get<int>(m_script, "counter"));

			simulateMouseMovement({ 0.0f, 92.0f });
			handler.process(widget);
			Assert::AreEqual(9.8f, parent.m_value.m_float, 0.01f);
			Assert::AreEqual(5, util::get<int>(m_script, "counter"));
		}

	private:
		void initialize()
		{
			m_script.execute("global counter = 0");
		}
		void simulateMouseMovement(const glm::vec2 & position)
		{
			m_bus.post(MouseMove{ position, {}, {}, {} });
		}

		EventBus m_bus;
		Script m_script{ "script" };
	};
}