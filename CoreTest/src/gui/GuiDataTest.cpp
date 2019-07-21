
#include "gui/GuiData.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	TEST_CLASS(GuiDataTest)
	{
	public:
		TEST_METHOD(GuiData_ctor)
		{
			GuiData data{ "gui" };
			
			Assert::IsTrue(data.getScript().execute(GuiData::STATE_BOOL));
			Assert::IsTrue(data.getScript().execute(GuiData::STATE_FLOAT));
			Assert::IsTrue(data.getScript().execute(GuiData::STATE_STRING));
		}

		TEST_METHOD(GuiData_bools)
		{
			GuiData data{ "gui" };
			data.setBool("foo", true);
			data.setBool("bar", false);

			Assert::IsTrue(data.getBool("foo"));
			Assert::IsFalse(data.getBool("bar"));
			Assert::IsFalse(data.getBool("invalid"));
		}
		TEST_METHOD(GuiData_floats)
		{
			GuiData data{ "gui" };
			data.setFloat("foo", 3.14f);
			data.setFloat("bar", -42.0f);

			Assert::AreEqual(3.14f, data.getFloat("foo"));
			Assert::AreEqual(-42.0f, data.getFloat("bar"));
			Assert::AreEqual(0.0f, data.getFloat("invalid"));
		}
		TEST_METHOD(GuiData_strings)
		{
			GuiData data{ "gui" };
			data.setString("foo", "Hello");
			data.setString("bar", "World!");

			Assert::AreEqual({ "Hello" }, data.getString("foo"));
			Assert::AreEqual({ "World!" }, data.getString("bar"));
			Assert::AreEqual({ "" }, data.getString("invalid"));
		}
		TEST_METHOD(GuiData_noFieldConflicts)
		{
			GuiData data{ "gui" };
			data.setBool("field", true);
			data.setFloat("field", 11.1f);
			data.setString("field", "text");

			Assert::IsTrue(data.getBool("field"));
			Assert::AreEqual(11.1f, data.getFloat("field"));
			Assert::AreEqual({ "text" }, data.getString("field"));
		}
	};
}