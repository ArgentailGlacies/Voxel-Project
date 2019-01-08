
#include "io/Folder.h"
#include "world/BlockRegistry.h"
#include "world/data/BlockData.h"
#include "world/data/BlockRegion.h"
#include "world/render/BlockTexture.h"
#include "world/render/BlockTextureAtlas.h"
#include "world/render/BlockTextureLoader.h"
#include "world/render/TextureData.h"
#include "world/util/Side.h"

#include <allegro5/bitmap_io.h>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::render
{
	TEST_CLASS(BlockTextureLoaderTest)
	{
	public:
		BlockTextureLoaderTest() { initialize(); prepare(); }
		~BlockTextureLoaderTest() { deinitialize(); }

		TEST_METHOD(BlockTextureLoader_loadDefaultTexture)
		{
			pugi::xml_document doc;
			pugi::xml_node node = doc.append_child("node");
			node.append_attribute("path").set_value("test_files/bitmapA.png");
			loadTexture(node);

			Assert::AreEqual(1u, m_atlas.size());
		}
		TEST_METHOD(BlockTextureLoader_loadOverlayTexture)
		{
			pugi::xml_document doc;
			pugi::xml_node node = doc.append_child("node");
			node.append_attribute("path").set_value("test_files/bitmapA.png");
			node.append_attribute("overlay").set_value("test_files/bitmapB.png");
			loadTexture(node);

			Assert::AreEqual(1u, m_atlas.size());
		}

		TEST_METHOD(BlockTextureLoader_loadDefaultConnection)
		{
			pugi::xml_document doc;
			pugi::xml_node node = doc.append_child("node");
			node.append_attribute("type").set_value("default");
			node.append_attribute("offset").set_value("2");
			loadConnection(node);

			const auto data = read({ 0, 0, 0 }, Side::POS_Z);
			Assert::AreEqual(2u, data.getTexture());
			Assert::AreEqual(1u, data.getMeta());
		}
		TEST_METHOD(BlockTextureLoader_loadRandomConnection)
		{
			pugi::xml_document doc;
			pugi::xml_node node = doc.append_child("node");
			node.append_attribute("type").set_value("random");
			node.append_attribute("offset").set_value("10");
			node.append_attribute("count").set_value("43");
			loadConnection(node);

			const auto data = read({ 0, 0, 0 }, Side::POS_Z);
			Assert::AreEqual(10u, data.getTexture());
			Assert::AreEqual(43u, data.getMeta());
		}
		TEST_METHOD(BlockTextureLoader_loadRepeatConnection)
		{
			pugi::xml_document doc;
			pugi::xml_node node = doc.append_child("node");
			node.append_attribute("type").set_value("repeat");
			node.append_attribute("row").set_value("4");
			node.append_attribute("count").set_value("8");
			loadConnection(node);

			const auto dataA = read({ 0, 0, 0 }, Side::POS_Z);
			const auto dataB = read({ 1, 5, 0 }, Side::POS_Z);
			const auto dataC = read({ 3, 0, 4 }, Side::POS_Z);
			Assert::AreEqual(4u, dataA.getTexture());
			Assert::AreEqual(1u, dataB.getTexture());
			Assert::AreEqual(7u, dataC.getTexture());
			Assert::AreEqual(1u, dataA.getMeta());
			Assert::AreEqual(1u, dataB.getMeta());
			Assert::AreEqual(1u, dataC.getMeta());
		}
		/*TEST_METHOD(BlockTextureLoader_loadConnectAllConnection)
		{
			pugi::xml_document doc;
			pugi::xml_node node = doc.append_child("node");
			node.append_attribute("type").set_value("connect-all");
			node.append_attribute("blocks").set_value("stone");
			loadConnection(node);

			const auto dataA = read({ 1, 1, 0 }, Side::POS_Z);
			const auto dataB = read({ 1, 2, 0 }, Side::POS_Z);
			const auto dataC = read({ 2, 1, 0 }, Side::POS_Z);
			Assert::Fail();
		}*/
		TEST_METHOD(BlockTextureLoader_loadConnectHorConnection)
		{
			pugi::xml_document doc;
			pugi::xml_node node = doc.append_child("node");
			node.append_attribute("type").set_value("connect-horizontal");
			node.append_attribute("count").set_value("3");
			node.append_attribute("blocks").set_value("grass, dirt");
			node.append_attribute("match").set_value("true");
			loadConnection(node);

			const auto dataA = read({ 4, 1, 1 }, Side::NEG_X);
			const auto dataB = read({ 4, 0, 1 }, Side::NEG_X);
			const auto dataC = read({ 5, 1, 1 }, Side::POS_X);
			Assert::AreEqual(6u, dataA.getTexture());
			Assert::AreEqual(3u, dataB.getTexture());
			Assert::AreEqual(0u, dataC.getTexture());
			Assert::AreEqual(3u, dataA.getMeta());
			Assert::AreEqual(3u, dataB.getMeta());
			Assert::AreEqual(3u, dataC.getMeta());
			Assert::AreEqual(dataA.getHash(), dataB.getHash());
		}
		TEST_METHOD(BlockTextureLoader_loadConnectVerConnection)
		{
			pugi::xml_document doc;
			pugi::xml_node node = doc.append_child("node");
			node.append_attribute("type").set_value("connect-vertical");
			node.append_attribute("count").set_value("3");
			node.append_attribute("blocks").set_value("dirt");
			node.append_attribute("match").set_value("true");
			loadConnection(node);

			const auto dataA = read({ 4, 1, 1 }, Side::NEG_X);
			const auto dataB = read({ 5, 1, 1 }, Side::POS_X);
			const auto dataC = read({ 5, 1, 2 }, Side::POS_X);
			Assert::AreEqual(0u, dataA.getTexture());
			Assert::AreEqual(6u, dataB.getTexture());
			Assert::AreEqual(3u, dataC.getTexture());
			Assert::AreEqual(3u, dataA.getMeta());
			Assert::AreEqual(3u, dataB.getMeta());
			Assert::AreEqual(3u, dataC.getMeta());
			Assert::AreEqual(dataB.getHash(), dataC.getHash());
		}
		
	private:
		void initialize()
		{
			util::Folder{ "test_files" }.create();

			ALLEGRO_BITMAP * bitmap = al_create_bitmap(32, 32);
			al_save_bitmap("test_files/bitmapA.png", bitmap);
			al_save_bitmap("test_files/bitmapB.png", bitmap);
			al_destroy_bitmap(bitmap);
		}
		void deinitialize()
		{
			util::Folder{ "test_files" }.erase(true);
		}
		void prepare()
		{
			m_registry.add("stone");
			m_registry.add("dirt");
			m_registry.add("grass");

			// Five stone shaped as + in the xy-plane
			m_region.write({ 0, 1, 0 }, BlockData{ m_registry["stone"].m_id, Light{} });
			m_region.write({ 1, 0, 0 }, BlockData{ m_registry["stone"].m_id, Light{} });
			m_region.write({ 1, 1, 0 }, BlockData{ m_registry["stone"].m_id, Light{} });
			m_region.write({ 1, 2, 0 }, BlockData{ m_registry["stone"].m_id, Light{} });
			m_region.write({ 2, 1, 0 }, BlockData{ m_registry["stone"].m_id, Light{} });

			// Three dirt shaped as - in the yz-plane, plus two stone above and below center
			m_region.write({ 4, 0, 1 }, BlockData{ m_registry["dirt"].m_id, Light{} });
			m_region.write({ 4, 1, 1 }, BlockData{ m_registry["dirt"].m_id, Light{} });
			m_region.write({ 4, 2, 1 }, BlockData{ m_registry["dirt"].m_id, Light{} });
			m_region.write({ 4, 1, 0 }, BlockData{ m_registry["stone"].m_id, Light{} });
			m_region.write({ 4, 1, 2 }, BlockData{ m_registry["stone"].m_id, Light{} });

			// Three dirt shaped as | in the yz-plane, plus two stone left and right of center
			m_region.write({ 5, 1, 0 }, BlockData{ m_registry["dirt"].m_id, Light{} });
			m_region.write({ 5, 1, 1 }, BlockData{ m_registry["dirt"].m_id, Light{} });
			m_region.write({ 5, 1, 2 }, BlockData{ m_registry["dirt"].m_id, Light{} });
			m_region.write({ 5, 0, 1 }, BlockData{ m_registry["stone"].m_id, Light{} });
			m_region.write({ 5, 2, 1 }, BlockData{ m_registry["stone"].m_id, Light{} });

			// Column of two stone, two dirt and one grass
			m_region.write({ 7, 1, 0 }, BlockData{ m_registry["stone"].m_id, Light{} });
			m_region.write({ 7, 1, 1 }, BlockData{ m_registry["stone"].m_id, Light{} });
			m_region.write({ 7, 1, 2 }, BlockData{ m_registry["dirt"].m_id, Light{} });
			m_region.write({ 7, 1, 3 }, BlockData{ m_registry["dirt"].m_id, Light{} });
			m_region.write({ 7, 1, 4 }, BlockData{ m_registry["grass"].m_id, Light{} });
		}

		void loadTexture(const pugi::xml_node & node)
		{
			BlockTextureLoader{ m_registry, m_atlas, m_texture }.loadTexture(node);
		}
		void loadConnection(const pugi::xml_node & node)
		{
			BlockTextureLoader{ m_registry, m_atlas, m_texture }.loadConnection(node);
		}
		TextureData read(const glm::ivec3 & pos, const Side & side)
		{
			return m_texture.m_lookup(m_region, pos, side);
		}

		BlockRegistry m_registry;
		BlockRegion m_region;
		BlockTexture m_texture;
		BlockTextureAtlas m_atlas;
	};
}