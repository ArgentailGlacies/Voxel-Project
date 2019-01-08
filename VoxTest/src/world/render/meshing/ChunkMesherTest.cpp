
#include "mock/MockBlockRegistry.h"
#include "world/render/BlockRenderRegistry.h"
#include "world/render/meshing/ChunkMesher.h"
#include "world/World.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::render
{
	TEST_CLASS(ChunkMesherTest)
	{
	public:
		TEST_METHOD(ChunkMesher_work)
		{
			ChunkMesher mesher{ m_world, m_blocks };
			mesher.pushTask(glm::ivec3{});
			mesher.finish();

			ChunkMesher::Result result;
			Assert::IsTrue(mesher.popResult(result));
			Assert::IsTrue(nullptr != result.m_model);
			Assert::IsFalse(mesher.popResult(result));
		}

	private:
		World m_world{ mockBlockRegistry() };
		BlockRenderRegistry m_blocks;
	};
}