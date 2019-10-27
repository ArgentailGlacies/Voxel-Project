
#include "editor/EditorWorld.h"

#include "event/EventBus.h"
#include "mock/MockAssetRegistry.h"
#include "mock/MockUBORegistry.h"
#include "scene/Scene.h"

#include "Context.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::editor
{
	TEST_CLASS(EditorWorldTest)
	{
	public:
		TEST_METHOD(EditorWorld_setShape)
		{
			EditorWorld editor{ m_scene, m_bus };
			
			Assert::IsNull(editor.getShape());
			editor.setShape(&editor.m_shapePoint);
			Assert::IsNotNull(editor.getShape());
			editor.setShape(nullptr);
			Assert::IsNull(editor.getShape());
		}

	private:
		core::EventBus m_bus;
		core::AssetRegistry m_assets = core::mockAssetRegistry();
		core::UBORegistry m_ubos = core::mockUBORegistry();
		core::Scene m_scene{ m_assets, display(), m_ubos };
	};
}