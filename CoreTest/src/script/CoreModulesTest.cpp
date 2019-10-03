
#include "script/CoreModules.h"

#include "gui/GuiRegistry.h"
#include "mock/MockAssetRegistry.h"
#include "mock/MockUBORegistry.h"
#include "script/Script.h"

#include "Context.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::script
{
	TEST_CLASS(CoreModulesTest)
	{
	public:
		TEST_METHOD(ModuleGlm_bind)
		{
			ModuleGlm{}.bind(m_script);

			// Ensure that vector functionality has been added correctly
			Assert::IsTrue(m_script.execute(R"( var vecA = vec2() )"));
			Assert::IsTrue(m_script.execute(R"( var vecB = vec2(2.5f, -2) )"));
			Assert::IsTrue(m_script.execute(R"( var vecC = ivec3(0, 1, 2) )"));
			Assert::IsTrue(m_script.execute(R"( var vecD = vec4(-0.1f, 3, 1, 0) )"));

			Assert::IsTrue(m_script.execute(
				"vecA + vecB;"
				"vecA - vecB;"
				"vecA * vecB;"
				"vecA / vecB;"
			));
			Assert::IsTrue(m_script.execute(
				"vecA += 1;"
				"vecA -= vecB;"
				"vecA *= 0;"
				"vecA /= 2.0f;"
			));
		}

		TEST_METHOD(ModuleFileSystem_bind)
		{
			ModuleFileSystem{}.bind(m_script);

			// Ensure that file functionality has been added correctly
			Assert::IsTrue(m_script.execute(R"( var fileA = File() )"));
			Assert::IsTrue(m_script.execute(R"( var fileB = File("foobar") )"));

			// Ensure that folder functionality has been added correctly
			Assert::IsTrue(m_script.execute(R"( var folder = Folder("foobar") )"));
		}

		TEST_METHOD(ModuleGui_bind)
		{
			AssetRegistry assets = mockAssetRegistry();
			UBORegistry ubos = mockUBORegistry();
			EventBus bus;
			Scene scene{ assets, display(), ubos };
			GuiRegistry guis{ assets, display(), bus, scene };
			Gui gui{ assets, "" };

			// ...

			ModuleFileSystem{}.bind(m_script);
			ModuleGui{}.bind(m_script, guis, gui);

			// Ensure that global variables have been added to correct namespace
			Assert::IsTrue(m_script.execute(R"( GUI_REGISTRY )"));
			Assert::IsTrue(m_script.execute(R"( GUI )"));
			Assert::IsTrue(m_script.execute(R"( WIDGET )"));

			// Ensure that gui registry functionality has been added correctly
			Assert::IsTrue(m_script.execute(R"( GUI_REGISTRY.open(File("foobar")) )"));
			Assert::IsTrue(m_script.execute(R"( GUI_REGISTRY.close(File("foobar")) )"));

			// Ensure that gui functionality has been added correctly
			Assert::IsTrue(m_script.execute(R"( GUI.has("widget") )"));

			Assert::IsTrue(m_script.execute(R"( GUI.isVisible("widget") )"));
			Assert::IsTrue(m_script.execute(R"( GUI.isLocked("widget") )"));
			Assert::IsTrue(m_script.execute(R"( GUI.setVisible("widget", true) )"));
			Assert::IsTrue(m_script.execute(R"( GUI.setLocked("widget", true) )"));

			Assert::IsTrue(m_script.execute(R"( GUI.getBool("widget") )"));
			Assert::IsTrue(m_script.execute(R"( GUI.getFloat("widget") )"));
			Assert::IsTrue(m_script.execute(R"( GUI.getString("widget") )"));
			Assert::IsTrue(m_script.execute(R"( GUI.setBool("widget", true) )"));
			Assert::IsTrue(m_script.execute(R"( GUI.setFloat("widget", 0.0f) )"));
			Assert::IsTrue(m_script.execute(R"( GUI.setString("widget", "") )"));
		}

	private:
		Script m_script{ "script" };
	};
}