
#include "editor/shape/Shape.h"

#include "scene/Scene.h"
#include "mock/MockAssetRegistry.h"
#include "mock/MockUBORegistry.h"

#include "CppUnitTest.h"
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

			Shape shape{ m_scene, [&](auto, auto) { meshed = true; return nullptr; } };
			shape.stretch({}, {});

			Assert::IsTrue(meshed);
		}

	private:
		core::AssetRegistry m_assets = core::mockAssetRegistry();
		core::UBORegistry m_ubos = core::mockUBORegistry();
		core::Scene m_scene{ m_assets, display(), m_ubos };
	};
}