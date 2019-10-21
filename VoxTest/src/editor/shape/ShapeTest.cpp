
#include "editor/shape/Shape.h"

#include "scene/Scene.h"
#include "mock/MockAssetRegistry.h"
#include "mock/MockUBORegistry.h"

#include "Common.h"
#include "Context.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace
{
	class MockShape : public vox::Shape
	{
	public:
		MockShape(core::Scene & scene) : Shape(scene) {}

		mutable glm::ivec3 m_from = {};
		mutable glm::ivec3 m_to = {};
		mutable bool m_meshed = false;

	private:
		virtual vox::WorldQuery query(const glm::ivec3 & from, const glm::ivec3 & to, const vox::Block & block) const override final
		{
			return {};
		}
		virtual vox::ShapeMeshPtr mesh(const glm::ivec3 & from, const glm::ivec3 & to) const override final
		{
			m_from = from;
			m_to = to;
			m_meshed = true;
			return nullptr;
		}
	};
}

namespace vox::editor
{
	TEST_CLASS(ShapeTest)
	{
	public:
		TEST_METHOD(Shape_stretch)
		{
			MockShape shape{ m_scene};
			shape.stretch({ -2, 1, 4 }, { 6, -2, 10 });

			Assert::IsTrue(shape.m_meshed);
			Assert::AreEqual({ -4, 2, -3 }, shape.m_from);
			Assert::AreEqual({ 4, -1, 3 }, shape.m_to);
		}

	private:
		core::AssetRegistry m_assets = core::mockAssetRegistry();
		core::UBORegistry m_ubos = core::mockUBORegistry();
		core::Scene m_scene{ m_assets, display(), m_ubos };
	};
}