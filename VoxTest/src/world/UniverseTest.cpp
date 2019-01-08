
#include "mock/MockAssetRegistry.h"
#include "mock/MockUBORegistry.h"
#include "asset/AssetUtil.h"
#include "core/Resources.h"
#include "scene/Scene.h"
#include "world/Query.h"
#include "world/Universe.h"
#include "world/World.h"

#include "Context.h"
#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::universe
{
	TEST_CLASS(UniverseTest)
	{
	public:
		UniverseTest() { initialize(); }
		~UniverseTest() { deinitialize(); }

		TEST_METHOD(Universe_prepare)
		{
			// Simulate a game restart by destroying the universe, then creating a new one
			Assert::IsFalse(m_data.file("blocks.xml").exists());
			{ Universe{ m_data, m_scene }.prepare(m_assets); }
			{ Universe{ m_data, m_scene }.prepare(m_assets); }
			Assert::IsTrue(m_data.file("blocks.xml").exists());
		}

		TEST_METHOD(Universe_createWorld)
		{
			Universe universe{ m_data, m_scene };

			Assert::IsFalse(universe.hasWorld("world"));
			const auto & world = universe.createWorld("world");
			Assert::IsTrue(universe.hasWorld("world"));
			Assert::IsTrue(&universe.getWorld("world") == &world);

			// Cannot create a world which already exists
			Assert::ExpectException<std::invalid_argument>([&]() {universe.createWorld("world"); });
		}
		TEST_METHOD(Universe_destroyWorld)
		{
			Universe universe{ m_data, m_scene };
			universe.createWorld("world");

			Assert::IsTrue(universe.hasWorld("world"));
			universe.destroyWorld("world");
			Assert::IsFalse(universe.hasWorld("world"));
		}
		TEST_METHOD(Universe_getWorld)
		{
			Universe universe{ m_data, m_scene };
			universe.createWorld("foo");
			universe.createWorld("bar");

			auto & worldA = universe.getWorld("foo");
			auto & worldB = universe.getWorld("bar");
			Assert::IsTrue(&worldA != &worldB);
			Assert::ExpectException<std::invalid_argument>([&]() { universe.getWorld("baz"); });
		}

		TEST_METHOD(Universe_changeWorld)
		{
			Universe universe{ m_data, m_scene };
			universe.prepare(m_assets);

			auto & blocks = universe.getBlockRegistry();
			auto & world = universe.createWorld("world");
			world.write(writeBlock(blocks["glowstone"], glm::ivec3{ 0, 0, 10 }));
			world.finish();

			validate(world, { 0, 0, 10 }, blocks["glowstone"].m_id, { 31, 31, 31, 0 });
			validate(world, { 0, 0, 11 }, blocks["air"].m_id, { 30, 30, 30, 31 });
		}

	private:
		void initialize()
		{
			m_data.create();
			m_data.folder("blocks").create();
			m_data.file("blocks/glowstone.xml").write("<variant><light emit=\"31, 31, 31, 0\" /></variant>");

			core::setupBuilder<BlockTextureAtlasBuilder, core::Texture>(
				m_assets, core::res::asset::BLOCK_TEXTURE_ATLAS
			);
		}
		void deinitialize()
		{
			m_data.erase(true);
		}

		void validate(
			const World & world,
			const glm::ivec3 & pos,
			unsigned int block,
			const Light & light)
		{
			auto query = readBlock(pos);
			world.read(query);

			const auto data = query.begin()->second.begin()->m_data;
			Assert::AreEqual(block, data.getType());
			Assert::AreEqual<glm::uvec4>(light, data.getLight());
		}

		util::Folder m_data{ "test" };
		core::AssetRegistry m_assets = core::mockAssetRegistry();
		core::Scene m_scene{ m_assets, display(), core::mockUBORegistry() };
	};
}