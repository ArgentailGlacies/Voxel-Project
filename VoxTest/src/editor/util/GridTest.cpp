
#include "editor/util/Grid.h"

#include "scene/Scene.h"
#include "mock/MockAssetRegistry.h"
#include "mock/MockUBORegistry.h"

#include "CppUnitTest.h"
#include "Context.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::editor
{
	TEST_CLASS(GridTest)
	{
	public:
		TEST_METHOD(Grid_ctor)
		{
			Grid grid{ m_scene };
		}

	private:
		core::AssetRegistry m_assets = core::mockAssetRegistry();
		core::UBORegistry m_ubos = core::mockUBORegistry();
		core::Scene m_scene{ m_assets, display(), m_ubos };
	};
}