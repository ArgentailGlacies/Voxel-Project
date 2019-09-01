
#include "editor/util/Cursor.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::editor
{
	TEST_CLASS(CursorTest)
	{
	public:
		TEST_METHOD(Cursor_getLockedPosition)
		{
			Cursor cursorA, cursorB;
			cursorA.setPosition({ -3, 4, 5 });
			cursorA.lockPosition(true);
			cursorA.setPosition({ 1, 2, 3 });
			cursorB.setPosition({ 2, 4, 5 });

			Assert::AreEqual({ -3, 4, 5 }, cursorA.getLockedPosition());
			Assert::AreEqual({ 2, 4, 5 }, cursorB.getLockedPosition());
		}
		TEST_METHOD(Cursor_getCurrentPosition)
		{
			Cursor cursorA, cursorB;
			cursorA.setPosition({ 2, 3, 5 });
			cursorB.lockPosition(true);
			cursorB.setPosition({ -3, 2, -4 });

			Assert::AreEqual({ 2, 3, 5 }, cursorA.getCurrentPosition());
			Assert::AreEqual({ -3, 2, -4 }, cursorB.getCurrentPosition());
		}

		TEST_METHOD(Cursor_lockAxis)
		{
			Cursor cursorA, cursorB;
			cursorA.lockPosition(true);
			cursorA.setPosition({ 3, -1, 4 });
			cursorA.lockAxis(util::Axis::X, true);
			cursorB.setPosition({ 1, 1, 1 });
			cursorB.lockPosition(true);
			cursorB.setPosition({ 4, 2, -5 });
			cursorB.lockAxis(util::Axis::Y, true);
			cursorB.lockAxis(util::Axis::Z, true);

			Assert::AreEqual({ 0, -1, 4 }, cursorA.getCurrentPosition());
			Assert::AreEqual({ 4, 1, 1 }, cursorB.getCurrentPosition());
		}
	};
}