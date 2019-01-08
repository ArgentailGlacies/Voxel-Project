
#include "world/data/BlockRegion.h"
#include "world/data/Chunk.h"
#include "world/data/ChunkQuery.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::data
{
	TEST_CLASS(ChunkTest)
	{
	public:
		ChunkTest()
		{
			Chunk::Writer writer = m_chunk.write();
			writer.write({ 2, 0, 0 }, BlockData{ 1, Light{ 3, 1, 4, 12 } });
			writer.write({ 2, 0, 0 }, BlockData{ 1, Light{} });
			writer.write({ 3, 0, 0 }, BlockData{ 2, Light{} });
			writer.write({ 5, 0, 0 }, BlockData{ 3, Light{} });
		}

		TEST_METHOD(Chunk_empty)
		{
			ChunkQuery query;
			query.add({ 0, 0, 0 }, BlockData{ 1, {} });

			Chunk chunkA{ {} };
			Chunk chunkB{ {} };
			chunkB.write(query);

			Assert::IsTrue(chunkA.empty());
			Assert::IsFalse(chunkB.empty());
		}

		TEST_METHOD(Chunk_readQuery)
		{
			ChunkQuery query;
			query.add({ 0, 0, 0 });
			query.add({ 2, 0, 0 });
			query.add({ 3, 0, 0 });
			query.add({ 5, 0, 0 });

			m_chunk.read(query);

			unsigned int index = 0;
			for (const auto & it : query)
				Assert::AreEqual(index++, it.m_data.getType());
		}
		TEST_METHOD(Chunk_readRegion)
		{
			BlockRegion regionA{ { 0, 0, 0 }, { 4, 4, 4 } };
			BlockRegion regionB{ glm::ivec3{ -1 }, glm::ivec3{ CHUNK_SIZE<int> + 2 } };
			m_chunk.read(regionA, { 2, 0, 0 }, { 0, 0, 0 }, { 4, 4, 4 });
			m_chunk.read(regionB, { 0, 0, 0 }, { 0, 0, 0 }, glm::ivec3{ CHUNK_SIZE<int> });

			Assert::AreEqual(0u, regionA.read({ 2, 1, 3 }).getType());
			Assert::AreEqual(1u, regionA.read({ 0, 0, 0 }).getType());
			Assert::AreEqual(2u, regionA.read({ 1, 0, 0 }).getType());
			Assert::AreEqual(3u, regionA.read({ 3, 0, 0 }).getType());
			Assert::AreEqual(0u, regionB.read({ 0, 0, 0 }).getType());
			Assert::AreEqual(1u, regionB.read({ 2, 0, 0 }).getType());
			Assert::AreEqual(2u, regionB.read({ 3, 0, 0 }).getType());
			Assert::AreEqual(3u, regionB.read({ 5, 0, 0 }).getType());
		}

		TEST_METHOD(Chunk_writeQuery)
		{
			ChunkQuery query;
			query.add({ 0, 0, 0 }, BlockData{ 51, {} });
			query.add({ 2, 0, 0 }, BlockData{ 13, {} });
			query.add({ 5, 8, 4 }, BlockData{ 72, {} });
			m_chunk.write(query);

			Chunk::Reader reader = m_chunk.read();
			Assert::AreEqual(51u, reader.read({ 0, 0, 0 }).getType());
			Assert::AreEqual(13u, reader.read({ 2, 0, 0 }).getType());
			Assert::AreEqual(2u, reader.read({ 3, 0, 0 }).getType());
			Assert::AreEqual(72u, reader.read({ 5, 8, 4 }).getType());
		}
		TEST_METHOD(Chunk_writeLight)
		{
			Index index;
			Light data;
			bool _;

			Assert::IsTrue(m_chunk.popLightPropagation(index, data));
			Assert::AreEqual<int>(toIndex<int>({ 2, 0, 0 }), index);
			Assert::AreEqual<glm::uvec4>({ 0, 0, 0, 0 }, data); // Should NOT propagate written data

			Assert::IsTrue(m_chunk.popDarkPropagation(index, data, _));
			Assert::AreEqual<int>(toIndex<int>({ 2, 0, 0 }), index);
			Assert::AreEqual<glm::uvec4>({ 3, 1, 4, 12 }, data); // MUST propagate written data

			Assert::IsFalse(m_chunk.popLightPropagation(index, data));
			Assert::IsFalse(m_chunk.popDarkPropagation(index, data, _));
		}
		
		TEST_METHOD(Chunk_pushLightPropagation)
		{
			Chunk chunk{ {} };
			chunk.pushLightPropagation(42, { 3, 1, 4, 0 });

			Index index;
			Light data;
			Assert::IsTrue(chunk.popLightPropagation(index, data));
			Assert::IsFalse(chunk.popLightPropagation(index, data));

			Assert::AreEqual<int>(42, index);
			Assert::AreEqual<glm::uvec4>({ 3, 1, 4, 0 }, data);
		}
		TEST_METHOD(Chunk_pushDarkPropagation)
		{
			Chunk chunk{ {} };
			chunk.pushDarkPropagation(42, { 3, 1, 4, 0 }, true);

			Index index;
			Light data;
			bool _;
			Assert::IsTrue(chunk.popDarkPropagation(index, data, _));
			Assert::IsFalse(chunk.popDarkPropagation(index, data, _));

			Assert::AreEqual<int>(42, index);
			Assert::AreEqual<glm::uvec4>({ 3, 1, 4, 0 }, data);
		}

	private:
		Chunk m_chunk{ {} };
	};

	TEST_CLASS(ChunkReaderTest)
	{
	public:
		ChunkReaderTest()
		{
			Chunk::Writer writer = m_chunk.write();
			writer.write(42, 72);
			writer.write({ 3, 1, 4 }, 16);
		}

		TEST_METHOD(ChunkReader_read)
		{
			Chunk::Reader reader = m_chunk.read();

			Assert::AreEqual(72u, reader.read(42).getData());
			Assert::AreEqual(16u, reader.read({ 3, 1, 4 }).getData());
		}

	private:
		Chunk m_chunk{ {} };
	};
	TEST_CLASS(ChunkWriterTest)
	{
	public:
		TEST_METHOD(ChunkWriter_write)
		{
			Chunk chunk{ {} };
			Chunk::Writer writer = chunk.write();
			writer.write(171, 31);
			writer.write({ 3, 1, 4 }, 42);

			Assert::AreEqual(31u, writer.read(171).getData());
			Assert::AreEqual(42u, writer.read({ 3, 1, 4 }).getData());
		}
	};
}