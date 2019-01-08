#pragma once

#include "io/File.h"

#include <glm/vec3.hpp>
#include <pugixml/pugixml.hpp>

namespace vox
{
	struct BlockModel;
	class Side;

	class BlockModelLoader
	{
	public:
		BlockModelLoader() = delete;
		BlockModelLoader(BlockModel & model) : m_model(model) {}

		void loadModel(const util::File & file);
		void loadGeometry(const pugi::xml_node & node);
		void loadSide(const pugi::xml_node & node, const Side & side);
		void loadVertices(const pugi::xml_node & node, const Side & side);
		void loadFaces(const pugi::xml_node & node, const Side & side);

	private:
		BlockModel & m_model;
	};
}