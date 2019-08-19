#include "gui/GuiRegistry.h"

#include "io/File.h"
#include "io/Folder.h"
#include "mock/MockAssetRegistry.h"
#include "mock/MockUBORegistry.h"

#include "Context.h"
#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	TEST_CLASS(GuiRegistryTest)
	{
	public:
		GuiRegistryTest() { initialize(); }
		~GuiRegistryTest() { deinitialize(); }

		TEST_METHOD(GuiRegistry_open)
		{
			GuiRegistry registry{ m_assets, display(), m_bus, m_scene };

			Assert::IsTrue(registry.open("test_files/guiA.xml"));
			Assert::IsTrue(registry.open("test_files/guiB.xml"));
			Assert::IsFalse(registry.open("test_files/guiA.xml"));
			Assert::IsFalse(registry.open("test_files/guiC.xml"));
		}
		TEST_METHOD(GuiRegistry_close)
		{
			GuiRegistry registry{ m_assets, display(), m_bus, m_scene };
			registry.open("test_files/guiA.xml");

			Assert::IsTrue(registry.close("test_files/guiA.xml"));
			Assert::IsFalse(registry.close("test_files/guiB.xml"));
			Assert::IsFalse(registry.close("test_files/guiA.xml"));
		}

	private:
		void initialize()
		{
			util::Folder{ "test_files" }.create();
			util::File{ "test_files/guiA.xml" }.write(R"(
				<widgets>
					<widget name="foo" />
				</widgets>
			)");
			util::File{ "test_files/guiB.xml" }.write(R"(
				<widgets>
					<widget name="bar" />
				</widgets>
			)");
		}
		void deinitialize()
		{
			util::Folder{ "test_files" }.erase(true);
		}

		AssetRegistry m_assets = mockAssetRegistry();
		UBORegistry m_ubos = mockUBORegistry();
		Scene m_scene{ m_assets, display(), m_ubos };
		EventBus m_bus;
	};
}