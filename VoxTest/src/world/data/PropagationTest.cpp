
#include "world/BlockRegistry.h"
#include "world/data/BlockRegion.h"
#include "world/data/ChunkQuery.h"
#include "world/data/ChunkStorage.h"
#include "world/data/Propagation.h"
#include "world/util/Side.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::data
{
	TEST_CLASS(PropagationTest)
	{
	public:
		PropagationTest() { prepare(); }

		TEST_METHOD(PropagationBase_offset)
		{
			const auto source = toIndex<int>({ 3, 1, 4 });

			auto writer = write({});
			auto propagator = construct<PropagationBase>(writer);
			Assert::AreEqual<int>(toIndex<int>({ 4, 1, 4 }), propagator.offset(source, Side::POS_X));
			Assert::AreEqual<int>(toIndex<int>({ 3, 1, 3 }), propagator.offset(source, Side::NEG_Z));
		}

		// ...

		TEST_METHOD(PropagationLight_applyBlockEffect)
		{
			auto writer = write({});
			auto propagator = construct<PropagationLight>(writer);
			Assert::AreEqual(8u, propagator.applyBlockEffect(8, 0, 10));
			Assert::AreEqual(5u, propagator.applyBlockEffect(8, 3, 10));
			Assert::AreEqual(5u, propagator.applyBlockEffect(8, 3, 6));
			Assert::AreEqual(3u, propagator.applyBlockEffect(8, 3, 3));
		}

		TEST_METHOD(PropagationLight_spread)
		{
			auto writer = write({});
			construct<PropagationLight>(writer).spread();

			validate(writer, { 8, 10, 10 }, { 5, 10, 15, 20 });
			validate(writer, { 10, 10, 10 }, { 5, 10, 15, 20 });
			validate(writer, { 11, 10, 10 }, { 0, 0, 0, 0 });
			validate(writer, { 10, 11, 10 }, { 4, 9, 8, 0 });
			validate(writer, { 12, 10, 10 }, { 1, 6, 11, 17 });
			validate(writer, { 12, 10, 0 }, { 0, 0, 3, 18 });
		}

		// ...

		TEST_METHOD(PropagationDark_applyBlockEffect)
		{
			auto writer = write({});
			auto propagator = construct<PropagationDark>(writer);
			
			Light light{ 3, 1, 4, 12 };
			Light current{ 5, 1, 2, 3 };
			propagator.applyBlockEffect(light, current, { 0, 0, 2, 0 });
			Assert::AreEqual<glm::uvec4>({ 0, 0, 0, 3}, light);
			Assert::AreEqual<glm::uvec4>({ 5, 1, 2, 0 }, current);
		}

		TEST_METHOD(PropagationDark_spread)
		{
			auto writer = write({});
			construct<PropagationLight>(writer).spread();
			writer.write({ 10, 10, 10 }, BlockData{});
			construct<PropagationDark>(writer).spread();

			validate(writer, { 8, 10, 10 }, { 5, 10, 15, 20 });
			validate(writer, { 10, 10, 10 }, { 0, 0, 0, 0 });
			validate(writer, { 10, 9, 10 }, { 0, 0, 0, 0 });
			validate(writer, { 12, 10, 10 }, { 0, 0, 0, 0 });
		}

		// ...

		TEST_METHOD(Propagation_spreadDarkThenLight)
		{
			auto writer = write({});
			construct<PropagationLight>(writer).spread();
			writer.write({ 10, 10, 10 }, BlockData{});
			construct<PropagationDark>(writer).spread();
			construct<PropagationLight>(writer).spread();

			validate(writer, { 8, 10, 10 }, { 5, 10, 15, 20 });
			validate(writer, { 10, 10, 10 }, { 3, 8, 13, 18 });
			validate(writer, { 10, 9, 10 }, { 2, 7, 12, 17 });
			validate(writer, { 12, 10, 10 }, { 0, 4, 9, 15 });
		}

	private:
		void prepare()
		{
			auto & filter = m_blocks.add("filter");
			filter.m_lightAbsorbed = glm::uvec4{ 0 };
			filter.m_lightEmitted = glm::uvec4{ 0 };
			filter.m_lightFiltered = glm::uvec4{ 24, 16, 8, 0 };
			auto & light = m_blocks.add("light");
			light.m_lightAbsorbed = glm::uvec4{ MAX_BLOCK_LIGHT };
			light.m_lightEmitted = glm::uvec4{ 5, 10, 15, 20 };
			light.m_lightFiltered = glm::uvec4{ MAX_BLOCK_LIGHT };
			auto & solid = m_blocks.add("solid");
			solid.m_lightAbsorbed = glm::uvec4{ MAX_BLOCK_LIGHT };
			solid.m_lightEmitted = glm::uvec4{ 0 };
			solid.m_lightFiltered = glm::uvec4{ MAX_BLOCK_LIGHT };

			m_chunks.create({ 0, 0, 0 });

			ChunkQuery query;
			query.add({ 11, 10, 10 }, BlockData{ m_blocks["solid"].m_id, Light{} });
			query.add({ 10, 10, 10 }, BlockData{ m_blocks["light"].m_id, Light{ 5, 10, 15, 20 } });
			query.add({ 8, 10, 10 }, BlockData{ m_blocks["light"].m_id, Light{ 5, 10, 15, 20 } });
			query.add({ 10, 11, 10 }, BlockData{ m_blocks["filter"].m_id, Light{} });
			m_chunks.getExact({ 0, 0, 0 })->write(query);
		}
			
		Chunk::Writer write(const glm::ivec3 & cpos)
		{
			return m_chunks.getExact(cpos)->write();
		}
		template<typename Propagator>
		inline auto construct(Chunk::Writer & writer)
		{
			return Propagator{
				PropagationContext{  m_bus, m_blocks, m_chunks, writer },
				writer.chunk().getPosition()
			};
		}
		void validate(Chunk::Writer & writer, const glm::ivec3 & pos, const Light & light)
		{
			Assert::AreEqual<glm::uvec4>(light, writer.read(pos).getLight());
		}

		core::EventBus m_bus;
		BlockRegistry m_blocks;
		ChunkStorage m_chunks;
	};
}