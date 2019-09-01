
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
			auto & widgetB = mockWidget(parent, {}, { 20.0f, 50.0f });
			auto & widgetC = mockWidget(parent, {}, { 10.0f, 10.0f });
			parent.m_border.m_inner = 5.0f;
			widgetA.m_border.m_outer = 1.0f;
			widgetA.m_link.m_ratio = { 0.0f, 0.5f };
			widgetB.m_border.m_outer = 2.0f;
			widgetB.m_link.m_target = &widgetA;
			widgetB.m_link.m_ratio = { 2.0f, 0.5f };
			widgetC.m_border.m_outer = 10.0f;
			widgetC.m_link.m_ratio = { 1.0f, 0.75f };

			gui::updatePosition(parent);	// No link, no parent - does not move
			gui::updatePosition(widgetA);	// Has parent link, does move
			gui::updatePosition(widgetB);	// Has sibling link, does move
			gui::updatePosition(widgetC);	// Has parent link, does move

			Assert::AreEqual({ 40.0f, 40.0f }, parent.m_bbox.m_pos);
			Assert::AreEqual({ 45.0f, 130.0f }, widgetA.m_bbox.m_pos);
			Assert::AreEqual({ 97.0f, 135.0f }, widgetB.m_bbox.m_pos);
			Assert::AreEqual({ 340.0f, 207.5f }, widgetC.m_bbox.m_pos);
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