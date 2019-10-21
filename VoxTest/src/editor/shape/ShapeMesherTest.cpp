
#include "editor/shape/Shape.h"

#include "CppUnitTest.h"
#include "Context.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::editor
{
	TEST_CLASS(ShapeMesherTest)
	{
	public:
		TEST_METHOD(ShapeMesher_meshPoint)
		{
			auto mesh = meshPoint({ 1, 2, 3 });

			Assert::AreEqual(24u, mesh->indices().size());
		}

		TEST_METHOD(ShapeMesher_meshRectangle)
		{
			auto mesh = meshRectangle({ -1, -2, -3 }, { 1, 2, 3 });

			Assert::AreEqual(24u, mesh->indices().size());
		}
	};
}