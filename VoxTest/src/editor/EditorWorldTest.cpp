
#include "editor/EditorWorld.h"

#include "event/EventBus.h"
#include "mock/MockAssetRegistry.h"
#include "mock/MockUBORegistry.h"
#include "scene/Scene.h"

#include "Context.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace
{
	class MockShape : public vox::Shape
	{
	public:
		MockShape(core::Scene & scene) : Shape(scene) {}

	private:
		virtual vox::WorldQuery query(const glm::ivec3 & from, const glm::ivec3 & to, const vox::Block & block) const override final { return {}; }
		virtual vox::ShapeMeshPtr mesh(const glm::ivec3 & from, const glm::ivec3 & to) const override final { return nullptr; }
	};
}

namespace vox::editor
{
	TEST_CLASS(EditorWorldTest)
	{
	public:
		TEST_METHOD(EditorWorld_setShape)
		{
			MockShape shape{ m_scene };
			EditorWorld editor{ m_scene, m_bus };
			
			Assert::IsNull(editor.getShape());
			editor.setShape(&shape);
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