
#include "FontFactory.h"

#include <plog/Log.h>
#include <pugixml/pugixml.hpp>

namespace
{
	constexpr const char * NODE_FONT = "font";

	constexpr const char * ATTR_PATH = "path";
	constexpr const char * ATTR_SIZE = "size";
	constexpr const char * ATTR_TYPE = "type";
}

std::unique_ptr<core::Font> core::FontLoader::load(const util::File & file)
{
	LOG_INFO << "Loading font " << file.path() << "...";
	pugi::xml_document doc;
	doc.load_file(file.path().c_str());

	auto font = std::make_unique<Font>();
	for (auto node = doc.child(NODE_FONT).first_child(); node; node = node.next_sibling())
	{
		const std::string path = node.attribute(ATTR_PATH).as_string();
		const std::string type = node.attribute(ATTR_TYPE).as_string("regular");
		const auto size = node.attribute(ATTR_SIZE).as_float();

		if (type == "regular")
			font->prepare(path, Font::Style::REGULAR, size);
		else if (type == "bold")
			font->prepare(path, Font::Style::BOLD, size);
		else if (type == "bold-italic")
			font->prepare(path, Font::Style::BOLD_ITALIC, size);
		else if (type == "italic")
			font->prepare(path, Font::Style::ITALIC, size);
		else
			LOG_WARNING << "Unknown font style " << type;
	}
	return font;
}
