
#include "world/render/ChunkRenderStorage.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::render
{
	TEST_CLASS(ChunkRenderStorageTest)
	{
	public:
		TEST_METHOD(ChunkRenderStorage_reserve)
		{
			ChunkRenderStorage storage;
			storage.reserve(glm::ivec3{ 3, 1, 4 });
			storage.reserve(glm::ivec3{ 2, 2, 3 });

			Assert::IsFalse(storage.has(glm::ivec3{ 0, 0, 0 }));
			Assert::IsTrue(storage.has(glm::ivec3{ 3, 1, 4 }));
			Assert::IsTrue(storage.has(glm::ivec3{ 2, 2, 3 }));
		}

		TEST_METHOD(ChunkRenderStorage_set)
		{
			const auto cpos = glm::ivec3{ 3, 1, 4 };

			ChunkRenderStorage storage;
			storage.reserve(cpos);
			storage.set(cpos, std::make_unique<ChunkModel>());
			storage.set(cpos, std::make_unique<ChunkModel>());

			const auto models = storage.getVisible();
			Assert::AreEqual(1u, models.size());
		}
		TEST_METHOD(ChunkRenderStorage_remove)
		{
			ChunkRenderStorage storage;
			storage.reserve(glm::ivec3{ 3, 1, 4 });
			storage.reserve(glm::ivec3{ 2, 2, 3 });

			storage.remove(glm::ivec3{ 3, 1, 4 });
			Assert::IsFalse(storage.has(glm::ivec3{ 3, 1, 4 }));
			Assert::IsTrue(storage.has(glm::ivec3{ 2, 2, 3 }));
		}

		TEST_METHOD(ChunkRenderStorage_get)
		{
			const auto cposA = glm::ivec3{ 3, 1, 4 };
			const auto cposB = glm::ivec3{ 0, 1, 4 };
			auto model = std::make_unique<ChunkModel>();
			auto * ptr = model.get();

			ChunkRenderStorage storage;
			storage.reserve(cposA);
			storage.reserve(cposB);
			storage.set(cposA, std::move(model));

			Assert::IsTrue(ptr == storage.get(cposA));
			Assert::IsTrue(nullptr == storage.get(cposB));
			Assert::IsTrue(nullptr == storage.get(glm::ivec3{}));
		}
	};
}