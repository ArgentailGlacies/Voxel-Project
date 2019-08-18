
#include "gui/internal/HandlerLabel.h"

#include "event/EventBus.h"
#include "event/Events.h"
#include "gui/Widget.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	TEST_CLASS(HandlerLabelTest)
	{
	public:
		TEST_METHOD(HandlerLabel_action)
		{
			Widget widget;
			HandlerLabel handler{ []() {}, m_bus, widget };

			simulateUnichar('a');
			handler.action(widget);
			simulateUnichar('b');
			simulateUnichar('c');
			simulateUnichar('\b');
			simulateUnichar('d');
			simulateMouseClick();
			simulateUnichar('e');
			handler.action(widget);
			simulateKeyboardPress(KeyboardKey::ESCAPE);
			simulateUnichar('f');

			Assert::AreEqual({ "bd" }, widget.m_value.m_string);
		}

	private:
		void simulateMouseClick()
		{
			m_bus.post(MousePress{ MouseButton::LEFT, {}, {}, 1.0f });
		}
		void simulateKeyboardPress(KeyboardKey key)
		{
			m_bus.post(KeyPress{ key, 0 });
		}
		void simulateUnichar(int codepoint)
		{
			m_bus.post(KeyUnichar{ codepoint });
		}

		EventBus m_bus;
	};
}