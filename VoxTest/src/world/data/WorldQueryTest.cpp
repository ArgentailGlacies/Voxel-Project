
#include "world/data/WorldQuery.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::data
{
	TEST_CLASS(WorldQueryTest)
	{
	public:
		TEST_METHOD(WorldQuery_add)
		{
			ChunkQuery chunkQuery;
			chunkQuery.add({ 3, 1, 4 });

			WorldQuery queryA, queryB;
			queryA.add({ 0, 1, -1 }, std::move(chunkQuery));
			queryA.add({ 1, -3, 1 }, ChunkQuery{});
			queryB.add({ 0, 0, 0 });
			queryB.add({ 0, 0, 1 });
			queryB.add({ -1616, 5161, 826 });

			validate(queryA, 1, 1);
			validate(queryB, 2, 3);
		}

		TEST_METHOD(WorldQuery_has)
		{
			WorldQuery query;
			query.add({ 0, 0, 0 });
			query.add({ -161, 416, 6177 });

			Assert::IsFalse(query.has({ 0, 1, 0 }));
			Assert::IsTrue(query.has({ 0, 0, 0 }));
			Assert::IsTrue(query.has({ -161, 416, 6177 }));
		}

		TEST_METHOD(WorldQuery_optimalChunkOrder)
		{
			WorldQuery query;
			query.add(glm::ivec3{ 0, 0, 0 } * CHUNK_SIZE<int>);
			query.add(glm::ivec3{ -2, 0, 3 } * CHUNK_SIZE<int>);
			query.add(glm::ivec3{ 0, 0, 2 } * CHUNK_SIZE<int>);
			query.add(glm::ivec3{ -1, 0, 1 } * CHUNK_SIZE<int>);
			query.add(glm::ivec3{ 0, 2, -1 } * CHUNK_SIZE<int>);
			query.add(glm::ivec3{ 3, 1, 4 } * CHUNK_SIZE<int>);
			query.add(glm::ivec3{ 2, 1, 1 } * CHUNK_SIZE<int>);

			auto it = query.begin();
			Assert::AreEqual({ 0, 2, -1 }, (it++)->first);
			Assert::AreEqual({ 0, 0, 0 }, (it++)->first);
			Assert::AreEqual({ -1, 0, 1 }, (it++)->first);
			Assert::AreEqual({ 2, 1, 1 }, (it++)->first);
			Assert::AreEqual({ 0, 0, 2 }, (it++)->first);
			Assert::AreEqual({ -2, 0, 3 }, (it++)->first);
			Assert::AreEqual({ 3, 1, 4 }, (it++)->first);
			Assert::IsTrue(query.end() == it);
		}

	private:
		void validate(const WorldQuery & query, unsigned int size, unsigned int count)
		{
			Assert::AreEqual(size, query.size());
			Assert::AreEqual(count, query.count());
		}
	};
}