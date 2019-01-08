
#include "world/data/BlockRegion.h"
#include "world/render/BlockRenderRegistry.h"
#include "world/render/ChunkModel.h"
#include "world/render/meshing/NaiveGreedyMesher.h"
#include "world/util/Side.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::render
{
	TEST_CLASS(NaiveGreedyMesherTest)
	{
	public:
		NaiveGreedyMesherTest() { prepare(); }

		TEST_METHOD(LayerData_limitations)
		{
			// Must require the size of the layer data to be a specific size to hold
			// all relevant data when meshing chunks.
			static_assert(8 == sizeof(LayerData));
		}

		// ...

		TEST_METHOD(Layer_set)
		{
			const LayerData dataA{ BlockData{ 42 }, TextureData{ 6 } };
			const LayerData dataB{ BlockData{ 43 }, TextureData{ 6 } };

			Layer layer;
			layer.set({ 3, 4 }, dataA);
			layer.set({ 7, 0 }, dataB);

			Assert::IsTrue(dataA == layer.get({ 3, 4 }));
			Assert::IsTrue(dataB == layer.get({ 7, 0 }));
		}
		TEST_METHOD(Layer_extract)
		{
			const LayerData dataA{ BlockData{ 42 }, TextureData{ 6 } };
			const LayerData dataB{ BlockData{ 43 }, TextureData{ 6 } };

			Layer layer;
			layer.set({ 0, 0 }, dataA);
			layer.set({ 0, 1 }, dataA);
			layer.set({ 0, 2 }, dataA);
			layer.set({ 1, 0 }, dataB);
			layer.set({ 2, 0 }, dataB);
			layer.set({ 3, 8 }, dataA);

			LayerData outData;
			glm::uvec2 outPos, outSize;

			Assert::IsTrue(layer.extract(outData, outPos, outSize));
			Assert::IsTrue(dataA == outData);
			Assert::AreEqual({ 0, 0 }, outPos);
			Assert::AreEqual({ 1, 3 }, outSize);
			Assert::IsTrue(layer.extract(outData, outPos, outSize));
			Assert::IsTrue(dataB == outData);
			Assert::AreEqual({ 1, 0 }, outPos);
			Assert::AreEqual({ 2, 1 }, outSize);
			Assert::IsTrue(layer.extract(outData, outPos, outSize));
			Assert::IsTrue(dataA == outData);
			Assert::AreEqual({ 3, 8 }, outPos);
			Assert::AreEqual({ 1, 1 }, outSize);
			Assert::IsFalse(layer.extract(outData, outPos, outSize));
		}

		// ...

		TEST_METHOD(NaiveGreedyMesher_mesh)
		{
			NaiveGreedyMesher mesher{ m_blocks, m_data, m_model };
			mesher.mesh();
			
			// Should generate 22 faces
			Assert::AreEqual(88u, m_model.m_mesh.vertices().size());
			Assert::AreEqual(132u, m_model.m_mesh.indices().size());
		}

		TEST_METHOD(NaiveGreedyMesher_getLayer)
		{
			NaiveGreedyMesher mesher{ m_blocks, m_data, m_model };
			const auto layer = mesher.getLayer(3, Side::POS_X);

			Assert::AreEqual(2u, layer.get({ 1, 3 }).block().getType());
			Assert::AreEqual(1u, layer.get({ 1, 4 }).block().getType());
			Assert::IsTrue(layer.get({ 0, 4 }).empty());
		}
		TEST_METHOD(NaiveGreedyMesher_getData)
		{
			NaiveGreedyMesher mesher{ m_blocks, m_data, m_model };
			const auto dataA = mesher.getData({ 3, 1, 4 }, Side::POS_Y);
			const auto dataB = mesher.getData({ 4, 0, 4 }, Side::POS_Z);
			const auto dataC = mesher.getData({ 4, 0, 4 }, Side::NEG_X);

			Assert::IsFalse(dataA.empty());
			Assert::IsFalse(dataB.empty());
			Assert::IsTrue(dataC.empty());
			Assert::AreEqual(1u, dataA.block().getType());
			Assert::AreEqual(2u, dataB.block().getType());
		}

	private:
		void prepare()
		{
			m_data.write({ 3, 0, 4 }, BlockData{ 1, Light{} });
			m_data.write({ 3, 1, 4 }, BlockData{ 1, Light{} });
			m_data.write({ 3, 1, 3 }, BlockData{ 2, Light{} });
			m_data.write({ 4, 0, 4 }, BlockData{ 2, Light{} });
			m_data.write({ 5, 1, 4 }, BlockData{ 2, Light{} });
			
			for (const auto & side : Side::FACES)
			{
				m_blocks[0].m_occlude[side.id()] = false;
				m_blocks[1].m_model.m_vertices[side.id()] = { {}, {}, {}, {} };
				m_blocks[2].m_model.m_vertices[side.id()] = { {}, {}, {}, {} };
				m_blocks[1].m_model.m_indices[side.id()] = { 0, 0, 0, 0, 0, 0 };
				m_blocks[2].m_model.m_indices[side.id()] = { 0, 0, 0, 0, 0, 0 };
			}
		}

		BlockRenderRegistry m_blocks;
		BlockRegion m_data{ glm::ivec3{ -1 }, glm::uvec3{ CHUNK_SIZE<unsigned int> + 2 } };
		ChunkModel m_model;
	};
}