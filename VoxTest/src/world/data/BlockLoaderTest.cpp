
#include "world/Block.h"
#include "world/BlockRegistry.h"
#include "world/data/BlockLoader.h"
#include "world/util/Limits.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::data
{
	TEST_CLASS(BlockLoaderTest)
	{
	public:
		BlockLoaderTest() { m_file.write(R"(<variant/><variant name="foo"/><variant name="bar"/>)"); }
		~BlockLoaderTest() { m_file.erase(); }

		TEST_METHOD(BlockLoader_loadBlock)
		{
			BlockRegistry registry;
			BlockLoader{ registry }.loadBlock(m_file);

			Assert::IsTrue(registry.has("test_block"));
			Assert::IsTrue(registry.has("test_block:foo"));
			Assert::IsTrue(registry.has("test_block:bar"));
		}

	private:
		util::File m_file{ "test_block.xml" };
	};

	TEST_CLASS(BlockVariantLoaderTest)
	{
	public:
		TEST_METHOD(BlockVariantLoader_loadVariant)
		{
			pugi::xml_document doc;
			pugi::xml_node variant = doc.append_child("variant");

			Block block;
			BlockVariantLoader{ block }.loadVariant(variant);

			Assert::IsTrue(BlockPhase::SOLID == block.m_phase);
			Assert::AreEqual(glm::uvec4{ MAX_BLOCK_LIGHT }, block.m_lightAbsorbed);
			Assert::AreEqual(glm::uvec4{}, block.m_lightEmitted);
			Assert::AreEqual(glm::uvec4{ MAX_BLOCK_LIGHT }, block.m_lightFiltered);
		}

		TEST_METHOD(BlockVariantLoader_loadPhase)
		{
			pugi::xml_document doc;
			pugi::xml_node variant = doc.append_child("variant");
			variant.append_attribute("phase").set_value("liquid");

			Block block;
			BlockVariantLoader{ block }.loadPhase(variant);

			Assert::IsTrue(BlockPhase::LIQUID == block.m_phase);
		}
		TEST_METHOD(BlockVariantLoader_loadLight)
		{
			pugi::xml_document doc;
			pugi::xml_node variant = doc.append_child("variant");
			pugi::xml_node light = variant.append_child("light");
			light.append_attribute("absorb").set_value("4, 5, 9, 12");
			light.append_attribute("emit").set_value("3, 10, 6, 8");
			light.append_attribute("filter").set_value("1, 11, 2, 7");

			Block block;
			BlockVariantLoader{ block }.loadLight(variant);

			Assert::AreEqual({ 4, 5, 9, 12 }, block.m_lightAbsorbed);
			Assert::AreEqual({ 3, 10, 6, 8 }, block.m_lightEmitted);
			Assert::AreEqual({ 1, 11, 2, 7 }, block.m_lightFiltered);
		}
	};
}