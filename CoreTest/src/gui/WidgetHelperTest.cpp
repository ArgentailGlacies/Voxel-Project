
#include "gui/WidgetHelper.h"

#include "mock/MockWidget.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	TEST_CLASS(WidgetHelperTest)
	{
	public:
		TEST_METHOD(WidgetHelper_isLocked)
		{
			auto grandparent = mockWidget({}, {});
			auto & parent = mockWidget(grandparent, {}, {});
			auto & widget = mockWidget(parent, {}, {});

			// If grandparent is locked, children and grandchildren are locked
			grandparent.m_state.m_locked = true;

			Assert::IsTrue(isLocked(grandparent));
			Assert::IsTrue(isLocked(parent));
			Assert::IsTrue(isLocked(widget));

			grandparent.m_state.m_locked = false;

			// If parent is locked, children are locked but grandparents are not
			parent.m_state.m_locked = true;

			Assert::IsFalse(isLocked(grandparent));
			Assert::IsTrue(isLocked(parent));
			Assert::IsTrue(isLocked(widget));

			parent.m_state.m_locked = false;

			// If widget is locked, parents are not
			widget.m_state.m_locked = true;

			Assert::IsFalse(isLocked(grandparent));
			Assert::IsFalse(isLocked(parent));
			Assert::IsTrue(isLocked(widget));

			widget.m_state.m_locked = false;
		}

		TEST_METHOD(WidgetHelper_isVisible)
		{
			auto grandparent = mockWidget({}, {});
			auto & parent = mockWidget(grandparent, {}, {});
			auto & widget = mockWidget(parent, {}, {});

			// If grandparent is not visible, children and grandchildren are not visible
			grandparent.m_state.m_visible = false;

			Assert::IsFalse(isVisible(grandparent));
			Assert::IsFalse(isVisible(parent));
			Assert::IsFalse(isVisible(widget));

			grandparent.m_state.m_visible = true;

			// If parent is not visible, children are not visible but grandparents are
			parent.m_state.m_visible = false;

			Assert::IsTrue(isVisible(grandparent));
			Assert::IsFalse(isVisible(parent));
			Assert::IsFalse(isVisible(widget));

			parent.m_state.m_visible = true;

			// If widget is not visible, parents are
			widget.m_state.m_visible = false;

			Assert::IsTrue(isVisible(grandparent));
			Assert::IsTrue(isVisible(parent));
			Assert::IsFalse(isVisible(widget));

			widget.m_state.m_visible = true;
		}
	};
}