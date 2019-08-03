
#include "Gui.h"

#include "gui/WidgetLoader.h"
#include "gui/WidgetProcessor.h"

#include <plog/Log.h>
#include <pugixml/pugixml.hpp>

const std::string core::Gui::STATE_BOOL = "STATE_BOOL";
const std::string core::Gui::STATE_FLOAT = "STATE_FLOAT";
const std::string core::Gui::STATE_STRING = "STATE_STRING";

// ...

core::Gui::Gui(const util::File & file, const AssetRegistry & assets)
	: m_script(file.path())
{
	m_script.execute("global " + STATE_BOOL + " = false;");
	m_script.execute("global " + STATE_FLOAT + " = 0.0f;");
	m_script.execute("global " + STATE_STRING + " = \"\";");

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

void core::Gui::process()
{
	process(m_root);
}
void core::Gui::process(Widget & widget)
{
	// Move the widget to the correct location on-screen
	gui::updatePosition(widget);
	
	// All child widgets may now be updated
	for (auto & child : widget.m_family.m_children)
		process(*child);

	// Recalculate the size of the widget
	gui::updateSize(widget);
}

void core::Gui::render() const
{
	render(m_root, glm::vec2{});
}
void core::Gui::render(const Widget & widget, const glm::vec2 & offset) const
{
	// Render all aspects of this specific widget
	for (const auto & renderer : widget.m_renderers)
		renderer(widget, offset);

	// Render all child widgets on top of this widget
	for (const auto & child : widget.m_family.m_children)
		render(*child, offset);
}
