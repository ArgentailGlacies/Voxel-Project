
#include "Gui.h"

#include "gui/WidgetLoader.h"
#include "gui/WidgetProcessor.h"

#include <plog/Log.h>
#include <pugixml/pugixml.hpp>

core::Gui::Gui(const util::File & file, const AssetRegistry & assets)
	: m_script(file.path())
{
	load(file, assets);
}

void core::Gui::load(const util::File & file, const AssetRegistry & assets)
{
	pugi::xml_document doc;

	const auto result = doc.load_file(file.path().c_str());
	if (result.status == pugi::xml_parse_status::status_ok)
		WidgetLoader{ assets, m_script, m_bus }.load(doc.child("widgets"), m_root);
	else
		LOG_WARNING << "Failed to load gui " << file.path();
}

void core::Gui::process(Widget & widget)
{
	// Move the widget to the correct location on-screen and assign the correct size to it
	if (&m_root != &widget)
	{
		gui::updateSize(widget);
		gui::updatePosition(widget);
	}

	// All child widgets may now be updated
	for (auto & child : widget.m_family.m_children)
		process(*child);

	// Perform all aspectes of this specific widget
	if (widget.m_handler)
		widget.m_handler->process(widget);
}
void core::Gui::render(const Widget & widget, const glm::vec2 & offset) const
{
	// Render all aspects of this specific widget
	if (widget.m_renderer)
		widget.m_renderer->render(widget, offset);

	// Render all child widgets on top of this widget
	for (const auto & child : widget.m_family.m_children)
		render(*child, offset);
}
