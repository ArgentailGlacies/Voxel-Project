
#include "world/data/ChunkQuery.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::data
{
	TEST_CLASS(ChunkQueryTest)
	{
	public:
		TEST_METHOD(ChunkQuery_add)
		{
			ChunkQuery queryA, queryB, queryC;
			queryA.add({ 3, 1, 4 });
			queryB.add({ 2, 5, 4 }, { 4, 5, 1 });
			queryC.add({ 3, 1, 4 });
			queryC.add({ 0, 0, 0 });

			validate(queryA, 1, { 3, 1, 4 }, { 3, 1, 4 });
			validate(queryB, 12, { 2, 5, 1 }, { 4, 5, 4 });
			validate(queryC, 2, { 0, 0, 0 }, { 3, 1, 4 });
		}

		TEST_METHOD(ChunkQuery_has)
		{
			ChunkQuery query;
			query.add({ 3, 1, 4 });

			Assert::IsFalse(query.has({ 1, 2, 3 }));
			Assert::IsTrue(query.has({ 3, 1, 4 }));
		}

		TEST_METHOD(ChunkQuery_iterate)
		{
			ChunkQuery query;
			query.add({ 1, 0, 0 }, BlockData{ 7 }); // Will be overwritten
			query.add({ 5, 0, 0 }, BlockData{ 3 });
			query.add({ 1, 0, 0 }, BlockData{ 1 });
			query.add({ 3, 0, 0 }, BlockData{ 2 });
			validate(query, 3, { 1, 0, 0 }, { 5, 0, 0 });

			unsigned int i = 0;
			for (const auto & it : query)
				Assert::AreEqual(++i, it.m_data.getData());
		}

	private:
		void validate(
			const ChunkQuery & query,
			unsigned int size,
			const glm::uvec3 & min,
			const glm::uvec3 & max
		) const
		{
			Assert::AreEqual(size, query.size());
			Assert::AreEqual(min, query.min());
			Assert::AreEqual(max, query.max());
		}
	};
}