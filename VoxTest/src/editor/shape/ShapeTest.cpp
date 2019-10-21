
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

		mutable glm::ivec3 m_queryFrom = {};
		mutable glm::ivec3 m_queryTo = {};
		mutable glm::ivec3 m_meshFrom = {};
		mutable glm::ivec3 m_meshTo = {};

	private:
		virtual vox::WorldQuery query(const glm::ivec3 & from, const glm::ivec3 & to, const vox::Block & block) const override final
		{
			m_queryFrom = from;
			m_queryTo = to;
			return {};
		}
		virtual vox::ShapeMeshPtr mesh(const glm::ivec3 & from, const glm::ivec3 & to) const override final
		{
			m_meshFrom = from;
			m_meshTo = to;
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
			shape.read();

			Assert::AreEqual({ -2, 1, 4 }, shape.m_queryFrom);
			Assert::AreEqual({ 6, -2, 10 }, shape.m_queryTo);
			Assert::AreEqual({ -4, 2, -3 }, shape.m_meshFrom);
			Assert::AreEqual({ 4, -1, 3 }, shape.m_meshTo);
		}

		TEST_METHOD(ShapePoint_query)
		{
			ShapePoint shape{ m_scene };
			shape.stretch({ 3, 1, 4 }, { -1, 3, 2 });

			const auto query = shape.read();
			Assert::AreEqual(1u, query.count());
			Assert::IsTrue(query.has({ -1, 3, 2 }));
		}

		TEST_METHOD(ShapeRectangle_query)
		{
			ShapeRectangle shape{ m_scene };
			shape.stretch({ 3, 1, 4 }, { -1, 3, 2 });

			const auto query = shape.read();
			Assert::AreEqual(45u, query.count());
		}

	private:
		core::AssetRegistry m_assets = core::mockAssetRegistry();
		core::UBORegistry m_ubos = core::mockUBORegistry();
		core::Scene m_scene{ m_assets, display(), m_ubos };
	};
}