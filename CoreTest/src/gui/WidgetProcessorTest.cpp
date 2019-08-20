
#include "gui/WidgetProcessor.h"

#include "mock/MockWidget.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	TEST_CLASS(WidgetProcessorTest)
	{
	public:
		TEST_METHOD(WidgetProcessor_updatePosition)
		{
			auto parent = mockWidget({ 40.0f, 40.0f }, { 320.0f, 240.0f });
			auto & widgetA = mockWidget(parent, {}, { 50.0f, 60.0f });
			auto & widgetB = mockWidget(widgetA, { 0.5f, 1.0f }, {}, { 20.0f, 50.0f });
			parent.m_border.m_inner = 5.0f;
			widgetA.m_border.m_outer = 1.0f;
			widgetB.m_border.m_outer = 2.0f;

			gui::updatePosition(parent);	// No link, no parent - does not move
			gui::updatePosition(widgetA);	// No link, has parent, does move
			gui::updatePosition(widgetB);	// Has link, does move

			Assert::AreEqual({ 40.0f, 40.0f }, parent.m_bbox.m_pos);
			Assert::AreEqual({ 45.0f, 45.0f }, widgetA.m_bbox.m_pos);
			Assert::AreEqual({ 60.0f, 107.0f }, widgetB.m_bbox.m_pos);
		}
		TEST_METHOD(WidgetProcessor_updateSize)
		{
			auto parent = mockWidget({ 40.0f, 30.0f }, {});
			auto & widgetA = mockWidget(parent, { 42.0f, 32.0f }, {});
			auto & widgetB = mockWidget(parent, { 87.0f, 35.0f }, {});
			parent.m_border.m_inner = 2.0f;
			widgetA.m_bbox.m_minSize = { 40.0f, 20.0f };
			widgetB.m_bbox.m_minSize = { 30.0f, 40.0f };
			widgetB.m_border.m_outer = 5.0f;

			gui::updateSize(widgetB);	// Uses min size
			gui::updateSize(widgetA);	// Uses min size
			gui::updateSize(parent);	// Uses children sizes

			Assert::AreEqual({ 82.0f, 50.0f }, parent.m_bbox.m_size);
			Assert::AreEqual({ 40.0f, 20.0f }, widgetA.m_bbox.m_size);
			Assert::AreEqual({ 30.0f, 40.0f }, widgetB.m_bbox.m_size);
		}
	};
}