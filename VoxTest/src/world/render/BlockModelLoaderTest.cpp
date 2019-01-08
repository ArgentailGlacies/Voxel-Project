
#include "world/render/BlockModel.h"
#include "world/render/BlockModelLoader.h"
#include "world/util/Side.h"

#include <pugixml/pugixml.hpp>

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::render
{
	TEST_CLASS(BlockModelLoaderTest)
	{
	public:
		TEST_METHOD(BlockModelLoader_loadVertices)
		{
			const auto side = Side::POS_X;

			pugi::xml_document doc;
			pugi::xml_node node = doc.append_child("node");
			appendVertex(node, "0, 0, 0", "0, 0, 1", "0, 0, 0");
			appendVertex(node, "1, 0, 0", "0, 0, 1", "1, 0, 0");
			appendVertex(node, "0, 1, 0", "0, 0, 1", "0, 1, 0");

			BlockModel model;
			BlockModelLoader loader{ model };
			loader.loadVertices(node, side);

			Assert::AreEqual(3u, model.m_vertices[side.id()].size());
			Assert::AreEqual({ 1, 0, 0 }, model.m_vertices[side.id()][1u].m_position);
			Assert::AreEqual({ 0, 0, 1 }, model.m_vertices[side.id()][1u].m_normal);
			Assert::AreEqual({ 1, 0, 0 }, model.m_vertices[side.id()][1u].m_uv);
		}

		TEST_METHOD(BlockModelLoader_loadFaces)
		{
			const auto side = Side::POS_X;

			pugi::xml_document doc;
			pugi::xml_node node = doc.append_child("node");
			appendFace(node, "0, 2, 1");
			appendFace(node, "5, 6, 7, 8");

			BlockModel model;
			BlockModelLoader loader{ model };
			loader.loadFaces(node, side);

			Assert::AreEqual(9u, model.m_indices[side.id()].size());
			Assert::AreEqual(0u, model.m_indices[side.id()][0u]);
			Assert::AreEqual(2u, model.m_indices[side.id()][1u]);
			Assert::AreEqual(1u, model.m_indices[side.id()][2u]);
			Assert::AreEqual(5u, model.m_indices[side.id()][3u]);
			Assert::AreEqual(6u, model.m_indices[side.id()][4u]);
			Assert::AreEqual(7u, model.m_indices[side.id()][5u]);
			Assert::AreEqual(5u, model.m_indices[side.id()][6u]);
			Assert::AreEqual(7u, model.m_indices[side.id()][7u]);
			Assert::AreEqual(8u, model.m_indices[side.id()][8u]);
		}

	private:
		void appendVertex(
			pugi::xml_node & node,
			const std::string & position,
			const std::string & normal,
			const std::string & uv
		)
		{
			auto vertex = node.append_child("vertex");
			vertex.append_attribute("position").set_value(position.c_str());
			vertex.append_attribute("normal").set_value(normal.c_str());
			vertex.append_attribute("uv").set_value(uv.c_str());
		}
		void appendFace(pugi::xml_node & node, const std::string & indices)
		{
			auto face = node.append_child("face");
			face.append_attribute("indices").set_value(indices.c_str());
		}
	};
}