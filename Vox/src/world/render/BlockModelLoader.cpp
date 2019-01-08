
#include "BlockModelLoader.h"

#include "util/StringParsing.h"
#include "world/render/BlockModel.h"
#include "world/util/Side.h"

namespace
{
	constexpr const char * NODE_GEOMETRY = "geometry";
	constexpr const char * NODE_VERTICES = "vertices";
	constexpr const char * NODE_FACES = "faces";

	constexpr const char * ATTR_FACE_INDICES = "indices";
	constexpr const char * ATTR_VERTEX_POSITION = "position";
	constexpr const char * ATTR_VERTEX_NORMAL = "normal";
	constexpr const char * ATTR_VERTEX_UV = "uv";
}

void vox::BlockModelLoader::loadModel(const util::File & file)
{
	pugi::xml_document doc;
	doc.load_file(file.path().c_str());

	loadGeometry(doc.child(NODE_GEOMETRY));
}
void vox::BlockModelLoader::loadGeometry(const pugi::xml_node & node)
{
	for (auto side = node.first_child(); side; side = side.next_sibling())
		loadSide(side, Side::fromName(side.name()));
}
void vox::BlockModelLoader::loadSide(const pugi::xml_node & node, const Side & side)
{
	loadVertices(node.child(NODE_VERTICES), side);
	loadFaces(node.child(NODE_FACES), side);
}
void vox::BlockModelLoader::loadVertices(const pugi::xml_node & node, const Side & side)
{
	auto & vertices = m_model.m_vertices[side.id()];
	for (auto vertex = node.first_child(); vertex; vertex = vertex.next_sibling())
	{
		util::Parser<glm::vec3> parser;
		const auto position = parser.parse(vertex.attribute(ATTR_VERTEX_POSITION).as_string());
		const auto normal = parser.parse(vertex.attribute(ATTR_VERTEX_NORMAL).as_string());
		const auto uv = parser.parse(vertex.attribute(ATTR_VERTEX_UV).as_string());

		vertices.push_back({ position, normal, uv });
	}
}
void vox::BlockModelLoader::loadFaces(const pugi::xml_node & node, const Side & side)
{
	auto & indices = m_model.m_indices[side.id()];
	for (auto face = node.first_child(); face; face = face.next_sibling())
	{
		util::Parser<std::vector<unsigned int>> parser;
		const auto parsed = parser.parse(face.attribute(ATTR_FACE_INDICES).as_string());

		for (size_t i = 2; i < parsed.size(); ++i)
		{
			indices.push_back(parsed[0]);
			indices.push_back(parsed[i - 1]);
			indices.push_back(parsed[i]);
		}
	}
}
