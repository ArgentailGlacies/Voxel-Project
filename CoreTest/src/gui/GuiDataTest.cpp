
#include "gui/GuiData.h"

#include "script/Script.h"
#include "script/ScriptUtil.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	TEST_CLASS(GuiDataTest)
	{
	public:
		GuiDataTest() { initializeScript(m_script); }

		TEST_METHOD(GuiData_initializeScript)
		{
			// Ensure widget state is initialized
			Assert::IsTrue(m_script.execute(STATE_BOOL));
			Assert::IsTrue(m_script.execute(STATE_FLOAT));
			Assert::IsTrue(m_script.execute(STATE_STRING));
		}

		TEST_METHOD(GuiData_setState)
		{
			setState(m_script, true);
			setState(m_script, 3.14f);
			setState(m_script, std::string{ "Hello World!" });

			Assert::AreEqual(true, util::get<bool>(m_script, STATE_BOOL));
			Assert::AreEqual(3.14f, util::get<float>(m_script, STATE_FLOAT), 0.001f);
			Assert::AreEqual({ "Hello World!" }, util::get<std::string>(m_script, STATE_STRING));
		}

	private:
		Script m_script{ "script" };
	};
}