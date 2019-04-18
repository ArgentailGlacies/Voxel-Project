
#include "gui/WidgetListener.h"

#include "mock/MockWidget.h"
#include "script/ScriptUtil.h"

#include <pugixml/pugixml.hpp>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	TEST_CLASS(WidgetListenerTest)
	{
	public:
		TEST_METHOD(WidgetListener_mouseMove)
		{
			// Move over parent
			mouseMove({ 30.0f, 50.0f });
			Assert::IsTrue(m_root->m_state.m_hovered);
			Assert::IsFalse(m_widgetA->m_state.m_hovered);
			Assert::IsFalse(m_widgetB->m_state.m_hovered);

			// Move over a first child
			mouseMove({ 30.0f, 22.0f });
			Assert::IsTrue(m_root->m_state.m_hovered);
			Assert::IsTrue(m_widgetA->m_state.m_hovered);
			Assert::IsFalse(m_widgetB->m_state.m_hovered);

			// Move over second child
			mouseMove({ 70.0f, 50.0f });
			Assert::IsTrue(m_root->m_state.m_hovered);
			Assert::IsFalse(m_widgetA->m_state.m_hovered);
			Assert::IsTrue(m_widgetB->m_state.m_hovered);

			// Move over child but outside parent
			mouseMove({ 90.0f, 20.0f });
			Assert::IsFalse(m_root->m_state.m_hovered);
			Assert::IsFalse(m_widgetA->m_state.m_hovered);
			Assert::IsFalse(m_widgetB->m_state.m_hovered);
		}
		TEST_METHOD(WidgetListener_mousePress)
		{
			// Select parent
			mousePress(m_root->m_bbox.m_pos);
			Assert::IsTrue(m_root->m_state.m_selected);
			Assert::IsFalse(m_widgetA->m_state.m_selected);

			// Select child
			mousePress(m_widgetA->m_bbox.m_pos);
			Assert::IsFalse(m_root->m_state.m_selected);
			Assert::IsTrue(m_widgetA->m_state.m_selected);
		}
		TEST_METHOD(WidgetListener_mouseRelease)
		{
			Script script{ "script" };
			script.execute("var r = false; var a = false; var b = false;");
			m_root->m_scripts["on_click"] = "r = true;";
			m_widgetA->m_scripts["on_click"] = "a = true;";
			m_widgetB->m_scripts["on_click"] = "b = true;";

			// Release parent
			mousePress(m_root->m_bbox.m_pos);
			mouseRelease(script, m_root->m_bbox.m_pos);
			Assert::IsFalse(m_root->m_state.m_selected);
			Assert::IsFalse(m_widgetA->m_state.m_selected);

			// Release child
			mousePress(m_widgetA->m_bbox.m_pos);
			mouseRelease(script, m_widgetA->m_bbox.m_pos);
			Assert::IsFalse(m_root->m_state.m_selected);
			Assert::IsFalse(m_widgetA->m_state.m_selected);

			// Release child while selected, but not while hovering
			mousePress(m_widgetB->m_bbox.m_pos);
			Assert::IsTrue(m_widgetB->m_state.m_selected);
			mouseRelease(script, {});
			Assert::IsFalse(m_widgetB->m_state.m_selected);

			// Root and widgetA did something, widgetB did not
			Assert::IsTrue(util::get<bool>(script, "r"));
			Assert::IsTrue(util::get<bool>(script, "a"));
			Assert::IsFalse(util::get<bool>(script, "b"));
		}

	private:
		inline void mouseMove(const glm::vec2 & position)
		{
			// Execution order: children in reverse creation order, then parent
			MouseMove event = { position, {}, {}, {} };
			gui::mouseMove(event, *m_root);
			gui::mouseMove(event, *m_widgetA);
			gui::mouseMove(event, *m_widgetB);
		}
		inline void mousePress(const glm::vec2 & position)
		{
			// Simulate mouse move first
			mouseMove(position);

			// Execution order: children in reverse creation order, then parent
			MousePress event = { MouseButton::LEFT, position, {}, 0.0f };
			gui::mousePress(event, *m_widgetB);
			gui::mousePress(event, *m_widgetA);
			gui::mousePress(event, *m_root);
		}
		inline void mouseRelease(Script & script, const glm::vec2 & position)
		{
			// Execution order: children in reverse creation order, then parent
			MouseRelease event = { MouseButton::LEFT, position, {}, 0.0f };
			gui::mouseRelease(script, event, *m_widgetB);
			gui::mouseRelease(script, event, *m_widgetA);
			gui::mouseRelease(script, event, *m_root);
		}

		std::unique_ptr<Widget> m_root = mockWidget({}, { 80.0f, 60.0f });
		std::unique_ptr<Widget> m_widgetA = mockWidget(*m_root, { 10.0f, 20.0f }, { 25.0f, 5.0f });
		std::unique_ptr<Widget> m_widgetB = mockWidget(*m_root, { 40.0f, 10.0f }, { 60.0f, 50.0f });
	};
}