
#include "Gui.h"

#include "core/Resources.h"
#include "gui/WidgetLoader.h"
#include "gui/WidgetProcessor.h"
#include "script/ModuleRegistry.h"
#include "script/ScriptUtil.h"

#include <plog/Log.h>
#include <pugixml/pugixml.hpp>

core::Gui::Gui(const AssetRegistry & assets, const ModuleRegistry & modules, const ::util::File & file)
	: m_script(file.path())
{
	modules.apply(res::script::GUI, m_script);
	modules.apply(res::script::GUI_REGISTRY, m_script);
	util::addGlobalVariable(m_script, this, "GUI"); // TODO: See #29

	load(file, assets);
}

void core::Gui::load(const ::util::File & file, const AssetRegistry & assets)
{
	pugi::xml_document doc;

	const auto result = doc.load_file(file.path().c_str());
	if (result.status != pugi::xml_parse_status::status_ok)
		LOG_WARNING << "Failed to load gui " << file.path();
	else
	{
		WidgetLoader loader{ assets, m_script, m_bus };
		loader.load(doc.child("widgets"), m_root);
		m_widgets = loader.getWidgets();
	}
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
	// Do not render invisible widgets
	if (!widget.m_state.m_visible)
		return;

	// Render all aspects of this specific widget
	if (widget.m_renderer)
		widget.m_renderer->render(widget, offset);

	// Render all child widgets on top of this widget
	for (const auto & child : widget.m_family.m_children)
		render(*child, offset);
}

// ...

void core::Gui::resize(const glm::vec2 & size)
{
	m_root.m_bbox.m_minSize = size;
	m_root.m_bbox.m_size = size;
}

// ...

bool core::Gui::has(const std::string & widget) const
{
	return m_widgets.find(widget) != m_widgets.end();
}

bool core::Gui::isVisible(const std::string & widget) const
{
	if (const auto it = m_widgets.find(widget); it != m_widgets.end())
		return it->second->m_state.m_visible;
	return false;
}
void core::Gui::setVisible(const std::string & widget, bool visible)
{
	if (const auto it = m_widgets.find(widget); it != m_widgets.end())
		it->second->m_state.m_visible = visible;
}

bool core::Gui::isLocked(const std::string & widget) const
{
	if (const auto it = m_widgets.find(widget); it != m_widgets.end())
		return it->second->m_state.m_locked;
	return true;
}
void core::Gui::setLocked(const std::string & widget, bool locked)
{
	if (const auto it = m_widgets.find(widget); it != m_widgets.end())
		it->second->m_state.m_locked = locked;
}
