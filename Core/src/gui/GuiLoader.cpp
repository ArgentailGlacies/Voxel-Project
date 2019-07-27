
#include "GuiLoader.h"

#include "gui/WidgetLoader.h"

#include <plog/Log.h>
#include <pugixml/pugixml.hpp>

void core::GuiLoader::load(const util::File & file)
{
	pugi::xml_document doc;

	const auto result = doc.load_file(file.path().c_str());
	if (result.status == pugi::xml_parse_status::status_ok)
		load(doc);
	else
		LOG_WARNING << "Failed to load gui " << file.path();
}
void core::GuiLoader::load(const pugi::xml_document & doc)
{
	WidgetLoader{ m_assets, m_data }.load(doc.child("widgets"), m_root);
}
