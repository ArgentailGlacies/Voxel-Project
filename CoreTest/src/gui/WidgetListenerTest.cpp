
#include "gui/WidgetListener.h"

#include "mock/MockWidget.h"

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
			Assert::IsTrue(m_root.m_state.m_hovered);
			Assert::IsFalse(m_widgetA.m_state.m_hovered);
			Assert::IsFalse(m_widgetB.m_state.m_hovered);

			// Move over a first child
			mouseMove({ 30.0f, 22.0f });
			Assert::IsTrue(m_root.m_state.m_hovered);
			Assert::IsTrue(m_widgetA.m_state.m_hovered);
			Assert::IsFalse(m_widgetB.m_state.m_hovered);

			// Move over second child
			mouseMove({ 70.0f, 50.0f });
			Assert::IsTrue(m_root.m_state.m_hovered);
			Assert::IsFalse(m_widgetA.m_state.m_hovered);
			Assert::IsTrue(m_widgetB.m_state.m_hovered);

			// Move over child but outside parent
			mouseMove({ 90.0f, 20.0f });
			Assert::IsFalse(m_root.m_state.m_hovered);
			Assert::IsFalse(m_widgetA.m_state.m_hovered);
			Assert::IsFalse(m_widgetB.m_state.m_hovered);
		}
		TEST_METHOD(WidgetListener_mousePress)
		{
			// Select parent
			mousePress(m_root.m_bbox.m_pos);
			Assert::IsTrue(m_root.m_state.m_selected);
			Assert::IsFalse(m_widgetA.m_state.m_selected);

			// Select child
			mousePress(m_widgetA.m_bbox.m_pos);
			Assert::IsFalse(m_root.m_state.m_selected);
			Assert::IsTrue(m_widgetA.m_state.m_selected);
		}
		TEST_METHOD(WidgetListener_mouseRelease)
		{
			bool r = false, a = false, b = false;
			m_root.m_actions.push_back([&](auto &) { r = true; });
			m_widgetA.m_actions.push_back([&](auto &) { a = true; });
			m_widgetB.m_actions.push_back([&](auto &) { b = true; });

			// Release parent
			mousePress(m_root.m_bbox.m_pos);
			mouseRelease(m_root.m_bbox.m_pos);
			Assert::IsFalse(m_root.m_state.m_selected);
			Assert::IsFalse(m_widgetA.m_state.m_selected);

			// Release child
			mousePress(m_widgetA.m_bbox.m_pos);
			mouseRelease(m_widgetA.m_bbox.m_pos);
			Assert::IsFalse(m_root.m_state.m_selected);
			Assert::IsFalse(m_widgetA.m_state.m_selected);

			// Release child while selected, but not while hovering
			mousePress(m_widgetB.m_bbox.m_pos);
			Assert::IsTrue(m_widgetB.m_state.m_selected);
			mouseRelease({});
			Assert::IsFalse(m_widgetB.m_state.m_selected);

			// Root and widgetA did something, widgetB did not
			Assert::IsTrue(r);
			Assert::IsTrue(a);
			Assert::IsFalse(b);
		}

	private:
		inline void mouseMove(const glm::vec2 & position)
		{
			// Execution order: children in reverse creation order, then parent
			MouseMove event = { position, {}, {}, {} };
			gui::mouseMove(event, m_root);
			gui::mouseMove(event, m_widgetA);
			gui::mouseMove(event, m_widgetB);
		}
		inline void mousePress(const glm::vec2 & position)
		{
			// Simulate mouse move first
			mouseMove(position);

			// Execution order: children in reverse creation order, then parent
			MousePress event = { MouseButton::LEFT, position, {}, 0.0f };
			gui::mousePress(event, m_widgetB);
			gui::mousePress(event, m_widgetA);
			gui::mousePress(event, m_root);
		}
		inline void mouseRelease(const glm::vec2 & position)
		{
			// Execution order: children in reverse creation order, then parent
			MouseRelease event = { MouseButton::LEFT, position, {}, 0.0f };
			gui::mouseRelease(event, m_widgetB);
			gui::mouseRelease(event, m_widgetA);
			gui::mouseRelease(event, m_root);
		}

		Widget m_root = mockWidget({}, { 80.0f, 60.0f });
		Widget & m_widgetA = mockWidget(m_root, { 10.0f, 20.0f }, { 25.0f, 5.0f });
		Widget & m_widgetB = mockWidget(m_root, { 40.0f, 10.0f }, { 60.0f, 50.0f });
	};
}