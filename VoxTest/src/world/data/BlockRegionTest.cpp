
#include "world/data/BlockRegion.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::data
{
	TEST_CLASS(BlockRegionTest)
	{
	public:
		TEST_METHOD(BlockRegion_write)
		{
			BlockRegion regionA;
			BlockRegion regionB{ glm::ivec3{ -1 }, glm::uvec3{ 4 } };
			regionA.write({ 3, 1, 4 }, 42);
			regionB.write({ 2, 1, -1 }, 42);

			Assert::AreEqual(0u, regionA.read({ 0, 0, 0 }).getData());
			Assert::AreEqual(42u, regionA.read({ 3, 1, 4 }).getData());
			Assert::AreEqual(0u, regionB.read({ 0, 0, 0 }).getData());
			Assert::AreEqual(42u, regionB.read({ 2, 1, -1 }).getData());
		}
	};
}