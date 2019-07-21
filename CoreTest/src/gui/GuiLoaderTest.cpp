
#include "gui/GuiLoader.h"

#include "io/File.h"
#include "io/Folder.h"

#include <pugixml/pugixml.hpp>

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	TEST_CLASS(GuiLoaderTest)
	{
	public:
		GuiLoaderTest() { initialize(); }
		~GuiLoaderTest() { deinitialize(); }

		TEST_METHOD(GuiLoader_load)
		{
			GuiLoader loader{ m_data, m_root };
			loader.load("test_files/gui.xml");

			Assert::AreEqual(1u, m_root.m_family.m_children.size());
		}

	private:
		void initialize()
		{
			util::Folder{ "test_files" }.create();
			util::File{ "test_files/gui.xml" }.write(R"(
				<widgets>
					<widget name="foo" />
				</widgets>
			)");
		}
		void deinitialize()
		{
			util::Folder{ "test_files" }.erase(true);
		}

		GuiData m_data{ "gui" };
		Widget m_root;
	};
}