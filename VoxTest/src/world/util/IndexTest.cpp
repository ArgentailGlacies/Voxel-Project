
#include "world/util/Index.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::util
{
	TEST_CLASS(IndexTest)
	{
	public:
		TEST_METHOD(Index_sizeof)
		{
			// Index determines the largest possible chunk. If the chunk size is to be changed, the
			// index data structure's size must also be changed.
			static_assert(sizeof(Index) == 2);
			static_assert(CHUNK_VOLUME<int> < (1 << 16));
		}

		TEST_METHOD(Index_toIndex)
		{
			Assert::AreEqual<int>(0, toIndex<int>({ 0, 0, 0 }));
			Assert::AreEqual<int>(1, toIndex<int>({ 1, 0, 0 }));
			Assert::AreEqual<int>(CHUNK_SIZE<int>, toIndex<int>({ 0, 1, 0 }));
			Assert::AreEqual<int>(CHUNK_AREA<int>, toIndex<int>({ 0, 0, 1 }));
			Assert::AreEqual<int>(14 * CHUNK_SIZE<int> + 5, toIndex<int>({ 5, 14, 0 }));
			Assert::AreEqual<int>(9 * CHUNK_AREA<int> + 7, toIndex<int>({ 7, 0, 9 }));

			Assert::AreEqual<int>(0, toIndex<int>({ 3, 1, 4 }, { 3, 1, 4 }, { 6, 5, 2 }));
			Assert::AreEqual<int>(1, toIndex<int>({ 0, 1, 0 }, { 0, 0, 0 }, { 1, 2, 1 }));
			Assert::AreEqual<int>(13, toIndex<int>({ 0, 0, 0 }, { -1, -1, -1 }, { 3, 3, 3 }));
			Assert::AreEqual<int>(76, toIndex<int>({ 1, 1, 6 }, { 1, -1, -6 }, { 2, 3, 17 }));
		}

		TEST_METHOD(Index_fromIndex)
		{
			Assert::AreEqual({ 0, 0, 0 }, fromIndex<int>(0));
			Assert::AreEqual({ 8, 0, 0 }, fromIndex<int>(8));
			Assert::AreEqual({ 0, 1, 0 }, fromIndex<int>(CHUNK_SIZE<int>));
			Assert::AreEqual({ 0, 0, 1 }, fromIndex<int>(CHUNK_AREA<int>));
			Assert::AreEqual({ 6, 2, 0 }, fromIndex<int>(2 * CHUNK_SIZE<int> + 6));
			Assert::AreEqual({ 0, 1, 7 }, fromIndex<int>(7 * CHUNK_AREA<int> + CHUNK_SIZE<int>));
		}
	};
}