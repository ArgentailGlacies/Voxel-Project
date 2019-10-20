
#include "editor/shape/Shape.h"

#include "scene/Scene.h"
#include "mock/MockAssetRegistry.h"
#include "mock/MockUBORegistry.h"

#include "Common.h"
#include "Context.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::editor
{
	TEST_CLASS(ShapeTest)
	{
	public:
		TEST_METHOD(Shape_stretch)
		{
			bool meshed = false;
			glm::ivec3 start = {};
			glm::ivec3 end = {};

			Shape shape{ m_scene, [&](auto & from, auto & to) { meshed = true; start = from; end = to; return nullptr; } };
			shape.stretch({ -2, 1, 4 }, { 6, -2, 10 });

			Assert::IsTrue(meshed);
			Assert::AreEqual({ -4, 2, -3 }, start);
			Assert::AreEqual({ 4, -1, 3 }, end);
		}

	private:
		core::AssetRegistry m_assets = core::mockAssetRegistry();
		core::UBORegistry m_ubos = core::mockUBORegistry();
		core::Scene m_scene{ m_assets, display(), m_ubos };
	};
}