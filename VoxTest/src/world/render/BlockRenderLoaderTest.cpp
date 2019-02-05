
#include "allegro/Bitmap.h"
#include "io/Folder.h"
#include "world/BlockRegistry.h"
#include "world/data/BlockLoader.h"
#include "world/render/BlockRender.h"
#include "world/render/BlockRenderLoader.h"
#include "world/render/BlockRenderRegistry.h"
#include "world/render/BlockTextureAtlas.h"
#include "world/util/Side.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::render
{
	TEST_CLASS(BlockRenderLoaderTest)
	{
	public:
		BlockRenderLoaderTest() { initialize(); }
		~BlockRenderLoaderTest() { deinitialize(); }

		TEST_METHOD(BlockRenderLoader_loadBlock)
		{
			BlockRegistry m_registry;
			BlockRenderRegistry m_blocks;
			BlockTextureAtlas m_atlas;

			BlockLoader{ m_registry }.loadBlock(m_file);
			BlockRenderLoader{ "", m_registry, m_blocks, m_atlas }.loadBlock(m_file);

			const auto & constant = m_blocks;
			Assert::IsTrue(constant[m_registry["air"].m_id].m_occlude[Side::POS_X.id()]);
			Assert::IsTrue(constant[m_registry["test_block"].m_id].m_occlude[Side::POS_Y.id()]);
			Assert::IsTrue(constant[m_registry["test_block:foo"].m_id].m_occlude[Side::POS_Z.id()]);
			Assert::IsTrue(constant[m_registry["test_block:bar"].m_id].m_occlude[Side::OTHER.id()]);
		}

	private:
		void initialize()
		{
			m_file.write(R"(<variant/><variant name="foo"/><variant name="bar"/>)");
		}
		void deinitialize()
		{
			m_file.erase();
		}

		util::File m_file{ "test_block.xml" };
	};

	TEST_CLASS(BlockRenderVariantLoaderTest)
	{
	public:
		BlockRenderVariantLoaderTest() { initialize(); }
		~BlockRenderVariantLoaderTest() { deinitialize(); }

		TEST_METHOD(BlockRenderVariantLoader_loadVariant)
		{
			pugi::xml_document doc;
			pugi::xml_node variant = doc.append_child("variant");

			BlockRender block;
			BlockRenderVariantLoader{ "test", m_registry, m_atlas, block }.loadVariant(variant);

			for (const auto & side : Side::ALL)
			{
				Assert::AreEqual(0u, block.m_model.m_vertices[side.id()].size());
				Assert::AreEqual(0u, block.m_model.m_indices[side.id()].size());
				Assert::AreEqual(0u, block.m_texture[side.id()].m_handle);
				Assert::IsFalse(nullptr == block.m_texture[side.id()].m_lookup);
				Assert::IsTrue(block.m_occlude[side.id()]);
			}
		}

		TEST_METHOD(BlockRenderVariantLoader_loadModel)
		{
			pugi::xml_document doc;
			pugi::xml_node variant = doc.append_child("variant");
			pugi::xml_node node = variant.append_child("model");
			node.append_attribute("path").set_value("model.xml");

			BlockRender block;
			BlockRenderVariantLoader{ "test", m_registry, m_atlas, block }.loadVariant(variant);

			Assert::AreEqual(4u, block.m_model.m_vertices[Side::POS_Z.id()].size());
			Assert::AreEqual(3u, block.m_model.m_vertices[Side::NEG_Z.id()].size());
			Assert::AreEqual(6u, block.m_model.m_indices[Side::POS_Z.id()].size());
			Assert::AreEqual(3u, block.m_model.m_indices[Side::NEG_Z.id()].size());
		}
		TEST_METHOD(BlockRenderVariantLoader_loadTexture)
		{
			pugi::xml_document doc;
			pugi::xml_node variant = doc.append_child("variant");
			pugi::xml_node node = variant.append_child("texture");
			node.append_attribute("all").set_value("textureA.xml");
			node.append_attribute("pos_z").set_value("textureB.xml");
			node.append_attribute("neg_z").set_value("textureC.xml");
		
			BlockRender block;
			BlockRenderVariantLoader{ "test", m_registry, m_atlas, block }.loadVariant(variant);

			Assert::AreEqual(0u, block.m_texture[Side::POS_X.id()].m_handle);
			Assert::AreEqual(0u, block.m_texture[Side::POS_Y.id()].m_handle);
			Assert::AreEqual(1u, block.m_texture[Side::POS_Z.id()].m_handle);
			Assert::AreEqual(0u, block.m_texture[Side::NEG_X.id()].m_handle);
			Assert::AreEqual(0u, block.m_texture[Side::NEG_Y.id()].m_handle);
			Assert::AreEqual(2u, block.m_texture[Side::NEG_Z.id()].m_handle);
			Assert::AreEqual(0u, block.m_texture[Side::OTHER.id()].m_handle);
		}
		TEST_METHOD(BlockRenderVariantLoader_loadData)
		{
			pugi::xml_document doc;
			pugi::xml_node variant = doc.append_child("variant");
			pugi::xml_node occlude = variant.append_child("occlude");
			occlude.append_attribute("sides").set_value("false");
			occlude.append_attribute("pos_x").set_value("true");
			occlude.append_attribute("neg_y").set_value("true");
			occlude.append_attribute("pos_z").set_value("false");
			occlude.append_attribute("other").set_value("false");

			BlockRender block;
			BlockRenderVariantLoader{ "test", m_registry, m_atlas, block }.loadVariant(variant);

			Assert::IsTrue(block.m_occlude[Side::POS_X.id()]);
			Assert::IsFalse(block.m_occlude[Side::POS_Y.id()]);
			Assert::IsFalse(block.m_occlude[Side::POS_Z.id()]);
			Assert::IsFalse(block.m_occlude[Side::NEG_X.id()]);
			Assert::IsTrue(block.m_occlude[Side::NEG_Y.id()]);
			Assert::IsTrue(block.m_occlude[Side::NEG_Z.id()]);
			Assert::IsFalse(block.m_occlude[Side::OTHER.id()]);
		}

	private:
		void initialize()
		{
			util::Folder{ "test" }.create();
			util::Folder{ "test/models" }.create();
			util::Folder{ "test/textures" }.create();
			util::File{ "test/textures/textureA.xml" }.write(R"(<texture path="test/bitmapA.png" />)");
			util::File{ "test/textures/textureB.xml" }.write(R"(<texture path="test/bitmapB.png" />)");
			util::File{ "test/textures/textureC.xml" }.write(R"(<texture path="test/bitmapC.png" />)");
			util::File{ "test/models/model.xml" }.write(R"(
			<geometry>
				<pos_z>
					<vertices>
						<vertex position="0, 0, 0" normal="0, 0, 1" uv="0, 0, 0" />
						<vertex position="1, 0, 0" normal="0, 0, 1" uv="1, 0, 0" />
						<vertex position="1, 1, 0" normal="0, 0, 1" uv="1, 1, 0" />
						<vertex position="0, 1, 0" normal="0, 0, 1" uv="0, 1, 0" />
					</vertices>
					<faces>
						<face indices="0, 1, 2, 3" />
					</faces>
				</pos_z>
				<neg_z>
					<vertices>
						<vertex position="0, 0, 0" normal="0, 0, 1" uv="0, 0, 0" />
						<vertex position="1, 0, 0" normal="0, 0, 1" uv="1, 0, 0" />
						<vertex position="0.5, 1, 0" normal="0, 0, 1" uv="0.5, 1, 0" />
					</vertices>
					<faces>
						<face indices="0, 1, 2" />
					</faces>
				</neg_z>
			</geometry>
			)");

			core::Bitmap bitmap{ 32, 32 };
			bitmap.save("test/bitmapA.png");
			bitmap.save("test/bitmapB.png");
			bitmap.save("test/bitmapC.png");
		}
		void deinitialize()
		{
			util::Folder{ "test" }.erase(true);
		}

		BlockRegistry m_registry;
		BlockTextureAtlas m_atlas;
	};
}