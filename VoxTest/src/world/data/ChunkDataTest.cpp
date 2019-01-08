
#include "world/data/ChunkData.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::data
{
	TEST_CLASS(ChunkDataFlatTest)
	{
	public:
		TEST_METHOD(ChunkDataFlat_write)
		{
			ChunkDataFlat data;
			data.write(6, { 0xDEADBEEF });

			Assert::AreEqual(0u, data.read(0).getData());
			Assert::AreEqual(0xDEADBEEF, data.read(6).getData());
		}

		TEST_METHOD(ChunkDataFlat_compress)
		{
			ChunkDataFlat flat;
			flat.write(5, { 0xDEADBEEF });
			flat.write(6, { 0xDEADBEEF });
			flat.write(7, { 0xC0FFEE4U });

			const auto compressed = flat.compress();
			Assert::AreEqual(0u, compressed.read(0).getData());
			Assert::AreEqual(0u, compressed.read(4).getData());
			Assert::AreEqual(0xDEADBEEF, compressed.read(5).getData());
			Assert::AreEqual(0xDEADBEEF, compressed.read(6).getData());
			Assert::AreEqual(0xC0FFEE4U, compressed.read(7).getData());
			Assert::AreEqual(0u, compressed.read(8).getData());
		}
	};

	TEST_CLASS(ChunkDataCompressedTest)
	{
	public:
		TEST_METHOD(ChunkDataCompressed_read)
		{
			ChunkDataCompressed data = generate();
			Assert::AreEqual(0u, data.read(0).getData());
			Assert::AreEqual(42u, data.read(6).getData());
		}

		TEST_METHOD(ChunkDataCompressed_expand)
		{
			const auto expanded = generate().expand();
			Assert::AreEqual(0u, expanded.read(0).getData());
			Assert::AreEqual(0u, expanded.read(5).getData());
			Assert::AreEqual(42u, expanded.read(6).getData());
			Assert::AreEqual(11u, expanded.read(7).getData());
			Assert::AreEqual(11u, expanded.read(8).getData());
			Assert::AreEqual(0u, expanded.read(9).getData());
		}

	private:
		ChunkDataCompressed generate()
		{
			ChunkDataFlat data;
			data.write(6, 42);
			data.write(7, 11);
			data.write(8, 11);
			return data.compress();
		}
	};
}