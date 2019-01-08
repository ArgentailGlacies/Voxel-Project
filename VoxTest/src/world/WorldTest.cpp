
#include "mock/MockBlockRegistry.h"
#include "world/World.h"
#include "world/Query.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world
{
	TEST_CLASS(WorldTest)
	{
	public:
		TEST_METHOD(World_createChunks)
		{
			m_world.write(writeBlock(m_blocks["solid"], { 0, 0, 0 }));
			m_world.finish();

			Assert::AreEqual(27u, m_world.countChunks());
		}
		TEST_METHOD(World_destroyChunks)
		{
			m_world.write(writeBlock(m_blocks["solid"], { 0, 0, 0 }));
			m_world.finish();
			m_world.write(writeBlock(m_blocks["air"], { 0, 0, 0 }));
			m_world.finish();

			Assert::AreEqual(0u, m_world.countChunks());
		}

		TEST_METHOD(World_modifyBlocks)
		{
			m_world.write(writeBlock(m_blocks["solid"], { -5, 6177, 61 }));
			m_world.finish();

			validate({ -5, 6177, 62 }, "air", { 0, 0, 0, 31 });
			validate({ -5, 6177, 61 }, "solid", { 0, 0, 0, 0 });
			validate({ -5, 6177, 60 }, "air", { 0, 0, 0, 30 });
		}
		TEST_METHOD(World_modifyLights)
		{
			m_world.write(writeBlock(m_blocks["light"], { -5, 7, 2 }));
			m_world.finish();

			validate({ -5, 7, 2 }, "light", { 17, 5, 23, 31 });
			validate({ -5, 7, 1 }, "air", { 16, 4, 22, 31 });
			validate({ 2, 7, 2 }, "air", { 10, 0, 16, 31 });
		}
		TEST_METHOD(World_modifyDarkness)
		{
			m_world.write(writeBlock(m_blocks["light"], { -5, 7, 2 }));
			m_world.write(writeBlock(m_blocks["light"], { 13, 7, 2 }));
			m_world.finish();
			m_world.write(writeBlock(m_blocks["air"], { -5, 7, 2 }));
			m_world.finish();

			validate({ -5, 7, 2 }, "air", { 0, 0, 5, 31 });
			validate({ -1, 7, 2 }, "air", { 3, 0, 9, 31 });
			validate({ 0, 7, 2 }, "air", { 4, 0, 10, 31 });
			validate({ 12, 7, 2 }, "air", { 16, 4, 22, 31 });
		}

		TEST_METHOD(World_lightBlockTest)
		{
			m_world.write(writeBlock(m_blocks["glow"], { 0, 0, 10 }));
			m_world.finish();

			validate({ 0, 0, 10 }, "glow", { 31, 31, 31, 0 });
			validate({ 0, 0, 11 }, "air", { 30, 30, 30, 31 });
		}
		TEST_METHOD(World_lightSurfaceTest)
		{
			m_world.write(writeRectangle(m_blocks["solid"], { 0, 0, 0 }, { 31, 31, 0 }));
			m_world.write(writeBlock(m_blocks["air"], { 25, 15, 0 }));
			m_world.finish();

			validate({ -1, 15, -1 }, "air", { 0, 0, 0, 31 });
			validate({ 0, 15, -1 }, "air", { 0, 0, 0, 30 });
			validate({ 10, 15, -1 }, "air", { 0, 0, 0, 20 });
			validate({ 20, 15, -1 }, "air", { 0, 0, 0, 26 });
		}
		TEST_METHOD(World_lightChunkTest)
		{
			m_world.write(writeRectangle(m_blocks["solid"], { 0, 0, 0 }, { 31, 31, 31 }));
			m_world.write(writeBlock(m_blocks["light"], { 16, 16, 0 }));
			m_world.write(writeBlock(m_blocks["air"], { 16, 0, 2 }));
			m_world.finish();

			validate({ 16, 16, -1 }, "air", { 16, 4, 22, 31 });
			validate({ 16, 0, -1 }, "air", { 0, 0, 6, 30 });
			validate({ 16, -1, 0 }, "air", { 0, 0, 4, 31 });
			validate({ 16, -1, 2 }, "air", { 0, 0, 2, 31 });
			validate({ 16, 0, 2 }, "air", { 0, 0, 1, 30 });
		}
		TEST_METHOD(World_lightShadowTest)
		{
			m_world.write(writeRectangle(m_blocks["solid"], { 0, 0, 5 }, { 31, 31, 5 }));
			m_world.write(writeRectangle(m_blocks["solid"], { -50, -50, 1000 }, { 50, 50, 1000 }));
			m_world.finish();

			validate({ 0, 0, 10 }, "air", { 0, 0, 0, 0 });
		}

		TEST_METHOD(World_readChunkMeshData)
		{
			m_world.write(writeLine(m_blocks["solid"], { -1, 0, 0 }, { 0, 0, 0 }));
			m_world.finish();

			const auto region = m_world.readChunkMeshData({ 0, 0, 0 });
			Assert::AreEqual(m_blocks["solid"].m_id, region.read({ 0, 0, 0 }).getType());
			Assert::AreEqual(m_blocks["solid"].m_id, region.read({ -1, 0, 0 }).getType());
			Assert::AreEqual(m_blocks["air"].m_id, region.read({ 1, 0, 0 }).getType());
		}

	private:
		void validate(const glm::ivec3 & pos, const std::string & block, const Light & light)
		{
			auto query = readBlock(pos);
			m_world.read(query);
			
			const auto data = query.begin()->second.begin()->m_data;
			Assert::AreEqual(m_blocks[block].m_id, data.getType());
			Assert::AreEqual<glm::uvec4>(light, data.getLight());
		}

		BlockRegistry m_blocks = mockBlockRegistry();
		World m_world{ m_blocks };
	};
}