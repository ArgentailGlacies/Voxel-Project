
#include "SpriteFactory.h"

#include "util/StringParsing.h"

#include <plog/Log.h>
#include <pugixml/pugixml.hpp>

namespace
{
	constexpr const char * NODE_BITMAP = "bitmap";
	constexpr const char * NODE_FRAMES = "frames";

	constexpr const char * ATTR_PATH = "path";
	constexpr const char * ATTR_POS = "pos";
	constexpr const char * ATTR_SIZE = "size";
	constexpr const char * ATTR_CENTER = "center";
	constexpr const char * ATTR_TINT = "tint";
}

std::unique_ptr<core::Sprite> core::SpriteLoader::load(const util::File & file)
{
	LOG_INFO << "Loading sprite " << file.path() << "...";
	pugi::xml_document doc;
	doc.load_file(file.path().c_str());

	auto sprite = std::make_unique<Sprite>();
	if (const auto node = doc.child(NODE_BITMAP))
		sprite->load(node.attribute(ATTR_PATH).as_string());
	for (auto node = doc.child(NODE_FRAMES).first_child(); node; node = node.next_sibling())
	{
		util::Parser<glm::vec2> vec2;
		util::Parser<glm::vec4> vec4;

		const auto attrPos = vec2.parse(node.attribute(ATTR_POS).as_string());
		const auto attrSize = vec2.parse(node.attribute(ATTR_SIZE).as_string());
		const auto attrCenter = vec2.parse(node.attribute(ATTR_CENTER).as_string());
		const auto attrTint = vec4.parse(node.attribute(ATTR_TINT).as_string(), glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

		Sprite::Frame frame;
		frame.m_pos = attrPos;
		frame.m_size = attrSize;
		frame.m_center = attrCenter;
		frame.m_tint = { attrTint.r, attrTint.g, attrTint.b, attrTint.a };

		sprite->addFrame(node.name(), frame);
	}

	return sprite;
}
