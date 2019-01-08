
#include "world/data/ChunkStorage.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::data
{
	TEST_CLASS(ChunkStorageTest)
	{
	public:
		TEST_METHOD(ChunkStorage_create)
		{
			ChunkStorage storage;
			storage.create({ 3, -1, 4 });

			Assert::IsFalse(storage.has({ 0, 0, 0 }));
			Assert::IsTrue(storage.has({ 3, -1, 4 }));
		}
		TEST_METHOD(ChunkStorage_destroy)
		{
			ChunkStorage storage;
			storage.create({ 3, -1, 4 });
			storage.destroy({ 3, -1, 4 });

			Assert::IsFalse(storage.has({ 3, -1, 4 }));
		}

		TEST_METHOD(ChunkStorage_getExact)
		{
			ChunkStorage storage;
			storage.create({ 1, 2, 3 });

			Assert::IsNull(storage.getExact({ 2, 4, 1 }));
			Assert::IsNotNull(storage.getExact({ 1, 2, 3 }));
		}
		TEST_METHOD(ChunkStorage_getAbove)
		{
			ChunkStorage storage;
			storage.create({ 0, 0, 1 });
			storage.create({ 0, 0, 3 });
			storage.create({ 0, 0, -4 });

			Assert::IsTrue(storage.getAbove({ 0, 0, -16 }) == storage.getExact({ 0, 0, -4 }));
			Assert::IsTrue(storage.getAbove({ 0, 0, -4 }) == storage.getExact({ 0, 0, 1 }));
			Assert::IsTrue(storage.getAbove({ 0, 0, 2 }) == storage.getExact({ 0, 0, 3 }));
			Assert::IsNull(storage.getAbove({ 0, 0, 3 }));
		}
		TEST_METHOD(ChunkStorage_getBelow)
		{
			ChunkStorage storage;
			storage.create({ 0, 0, 1 });
			storage.create({ 0, 0, 3 });
			storage.create({ 0, 0, -4 });

			Assert::IsNull(storage.getBelow({ 0, 0, -5 }));
			Assert::IsTrue(storage.getBelow({ 0, 0, -3 }) == storage.getExact({ 0, 0, -4 }));
			Assert::IsTrue(storage.getBelow({ 0, 0, 2 }) == storage.getExact({ 0, 0, 1 }));
			Assert::IsTrue(storage.getBelow({ 0, 0, 5 }) == storage.getExact({ 0, 0, 3 }));
		}
		TEST_METHOD(ChunkStorage_getUppermost)
		{
			ChunkStorage storage;
			storage.create({ 0, 0, 1 });
			storage.create({ 0, 0, 3 });
			storage.create({ 0, 0, -4 });

			Assert::IsNull(storage.getUppermost({ 1, 2 }));
			Assert::IsTrue(storage.getUppermost({ 0, 0 }) == storage.getExact({ 0, 0, 3 }));
		}
		TEST_METHOD(ChunkStorage_getLowermost)
		{
			ChunkStorage storage;
			storage.create({ 0, 0, 1 });
			storage.create({ 0, 0, 3 });
			storage.create({ 0, 0, -4 });

			Assert::IsNull(storage.getUppermost({ 1, 2 }));
			Assert::IsTrue(storage.getLowermost({ 0, 0 }) == storage.getExact({ 0, 0, -4 }));
		}

		TEST_METHOD(ChunkStorage_count)
		{
			ChunkStorage storage;
			storage.create({ 3, 1, 6 });
			storage.create({ 3, 1, -3 });
			storage.create({ 2,-6, 9 });

			Assert::AreEqual(3u, storage.count());
		}
	};
}