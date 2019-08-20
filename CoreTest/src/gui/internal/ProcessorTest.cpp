
#include "gui/internal/Processor.h"

#include "event/EventBus.h"
#include "gui/internal/Handler.h"
#include "mock/MockWidget.h"

#include <glm/vec3.hpp>

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	class MockHandler : public Handler
	{
	public:
		MockHandler(const std::function<void()> & callback) : m_callback(callback) {}

		virtual void action(Widget & widget) override final { m_callback(); }
		virtual void process(Widget & widget) override final {}

	private:
		std::function<void()> m_callback;
	};

	TEST_CLASS(ProcessorTest)
	{
	public:
		ProcessorTest() { initialize(); }

		TEST_METHOD(Processor_mouseMove)
		{
			// Parent is hovered
			simulateMouseMove({ 40.0f, 40.0f });
			Assert::IsTrue(m_parent.m_state.m_hovered);
			Assert::IsFalse(m_widgetA.m_state.m_hovered);
			Assert::IsFalse(m_widgetB.m_state.m_hovered);

			// Parent and widget A is hovered
			simulateMouseMove({ 20.0f, 15.0f });
			Assert::IsTrue(m_parent.m_state.m_hovered);
			Assert::IsTrue(m_widgetA.m_state.m_hovered);
			Assert::IsFalse(m_widgetB.m_state.m_hovered);

			// Parent, widget A and widget B is hovered
			simulateMouseMove({ 27.0f, 15.0f });
			Assert::IsTrue(m_parent.m_state.m_hovered);
			Assert::IsTrue(m_widgetA.m_state.m_hovered);
			Assert::IsTrue(m_widgetB.m_state.m_hovered);

			// Nothing is hovered
			simulateMouseMove({ 60.0f, 40.0f });
			Assert::IsFalse(m_parent.m_state.m_hovered);
			Assert::IsFalse(m_widgetA.m_state.m_hovered);
			Assert::IsFalse(m_widgetB.m_state.m_hovered);
		}

		TEST_METHOD(Processor_mousePress)
		{
			// Parent is selected
			simulateMousePress({ 40.0f, 40.0f });
			Assert::IsTrue(m_parent.m_state.m_selected);
			Assert::IsFalse(m_widgetA.m_state.m_selected);
			Assert::IsFalse(m_widgetB.m_state.m_selected);

			// Only widget A is selected
			simulateMousePress({ 20.0f, 15.0f });
			Assert::IsFalse(m_parent.m_state.m_selected);
			Assert::IsTrue(m_widgetA.m_state.m_selected);
			Assert::IsFalse(m_widgetB.m_state.m_selected);

			// Only widget B is selected
			simulateMousePress({ 27.0f, 15.0f });
			Assert::IsFalse(m_parent.m_state.m_selected);
			Assert::IsFalse(m_widgetA.m_state.m_selected);
			Assert::IsTrue(m_widgetB.m_state.m_selected);

			// Nothing is selected
			simulateMousePress({ 60.0f, 40.0f });
			Assert::IsFalse(m_parent.m_state.m_selected);
			Assert::IsFalse(m_widgetA.m_state.m_selected);
			Assert::IsFalse(m_widgetB.m_state.m_selected);
		}

		TEST_METHOD(Processor_mouseRelease)
		{
			glm::ivec3 counter{};
			m_parent.m_handler = std::make_unique<MockHandler>([&]() { counter.x++; });
			m_widgetA.m_handler = std::make_unique<MockHandler>([&]() { counter.y++; });
			m_widgetB.m_handler = std::make_unique<MockHandler>([&]() { counter.z++; });

			// Parent is activated
			simulateMouseRelease({ 40.0f, 40.0f });
			Assert::AreEqual({ 1, 0, 0 }, counter);

			// Only widget A is activated
			simulateMouseRelease({ 20.0f, 15.0f });
			Assert::AreEqual({ 1, 1, 0 }, counter);

			// Only widget B is activated
			simulateMouseRelease({ 27.0f, 15.0f });
			Assert::AreEqual({ 1, 1, 1 }, counter);

			// Nothing is activated
			simulateMouseRelease({ 60.0f, 40.0f });
			Assert::AreEqual({ 1, 1, 1 }, counter);
		}

	private:
		void initialize()
		{
			m_parent.m_bbox.m_size = { 50.0f, 50.0f };
			m_widgetA.m_family.m_parent = &m_parent;
			m_widgetA.m_bbox.m_pos = { 20.0f, 10.0f };
			m_widgetA.m_bbox.m_size = { 10.0f, 10.0f };
			m_widgetB.m_family.m_parent = &m_parent;
			m_widgetB.m_bbox.m_pos = { 25.0f, 10.0f };
			m_widgetB.m_bbox.m_size = { 10.0f, 10.0f };

			// Widgets must be processed in reverse order of creation!
			m_widgetB.m_processor = std::make_unique<Processor>(m_widgetB, m_bus);
			m_widgetA.m_processor = std::make_unique<Processor>(m_widgetA, m_bus);
			m_parent.m_processor = std::make_unique<Processor>(m_parent, m_bus);
		}

		void simulateMouseMove(const glm::vec2 position)
		{
			m_bus.post(MouseMove{ position, {}, {}, {} });
		}
		void simulateMousePress(const glm::vec2 position)
		{
			simulateMouseMove(position);
			m_bus.post(MousePress{ MouseButton::LEFT, position, {}, 1.0f });
		}
		void simulateMouseRelease(const glm::vec2 position)
		{
			simulateMousePress(position);
			m_bus.post(MouseRelease{ MouseButton::LEFT, position, {}, 1.0f });
		}

		EventBus m_bus;
		Widget m_parent, m_widgetA, m_widgetB;
	};
}