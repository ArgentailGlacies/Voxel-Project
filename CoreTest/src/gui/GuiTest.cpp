
#include "gui/Gui.h"

#include "core/Resources.h"
#include "io/File.h"
#include "io/Folder.h"
#include "mock/MockAssetRegistry.h"
#include "script/ModuleRegistry.h"
#include "script/Script.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace
{
	const util::Folder folder = "test_file";
	const util::File file = folder.file("gui.xml");
}

namespace core::gui
{
	TEST_CLASS(GuiTest)
	{
	public:
		GuiTest() { initialize(); }
		~GuiTest() { deinitialize(); }

		TEST_METHOD(Gui_ctor)
		{
			Gui gui{ m_assets, m_modules, file };

			Assert::IsTrue(gui.has("root.foo"));
			Assert::IsFalse(gui.has("whatever"));
		}

		TEST_METHOD(Gui_setVisible)
		{
			Gui gui{ m_assets, m_modules, file };

			Assert::IsTrue(gui.isVisible("root.foo"));
			gui.setVisible("root.foo", false);
			Assert::IsFalse(gui.isVisible("root.foo"));

			Assert::IsFalse(gui.isVisible("whatever"));
		}
		TEST_METHOD(Gui_setLocked)
		{
			Gui gui{ m_assets, m_modules, file };

			Assert::IsFalse(gui.isLocked("root.foo"));
			gui.setLocked("root.foo", true);
			Assert::IsTrue(gui.isLocked("root.foo"));

			Assert::IsTrue(gui.isLocked("whatever"));
		}

		TEST_METHOD(Gui_setBool)
		{
			Gui gui{ m_assets, m_modules, file };
			gui.setBool("root.foo", true);
			gui.setBool("whatever", true);

			Assert::AreEqual(true, gui.getBool("root.foo"));
			Assert::AreEqual(false, gui.getBool("whatever"));
		}
		TEST_METHOD(Gui_setFloat)
		{
			Gui gui{ m_assets, m_modules, file };
			gui.setFloat("root.foo", 3.14f);
			gui.setFloat("whatever", 4.0f);

			Assert::AreEqual(3.14f, gui.getFloat("root.foo"), 0.0f);
			Assert::AreEqual(0.0f, gui.getFloat("whatever"), 0.0f);
		}
		TEST_METHOD(Gui_setString)
		{
			Gui gui{ m_assets, m_modules, file };
			gui.setString("root.foo", "some text");
			gui.setString("whatever", "world");

			Assert::AreEqual({ "some text" }, gui.getString("root.foo"));
			Assert::AreEqual({ "" }, gui.getString("whatever"));
		}

		// ...

		TEST_METHOD(Gui_scriptIntegration)
		{
			Gui gui{ m_assets, m_modules, file };

			Assert::IsTrue(gui.executeScript("GUI")); // TODO: See #29
			Assert::IsTrue(gui.executeScript("WIDGET")); // TODO: See #29
			Assert::IsTrue(gui.executeScript("foo()"));
			Assert::IsTrue(gui.executeScript("bar()"));
		}

	private:
		void initialize()
		{
			folder.create();
			file.write(R"(
				<widgets>
					<widget name="foo" />
					<widget name="bar" />
				</widgets>
			)");

			m_modules.add(res::script::GUI, [](auto & script) { script.execute("def foo(){}"); });
			m_modules.add(res::script::GUI_REGISTRY, [](auto & script) { script.execute("def bar(){}"); });
		}
		void deinitialize()
		{
			folder.erase(true);
		}

		AssetRegistry m_assets = mockAssetRegistry();
		ModuleRegistry m_modules;
	};
}