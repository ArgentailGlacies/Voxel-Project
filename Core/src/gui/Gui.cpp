
#include "Gui.h"

#include "core/Resources.h"
#include "gui/WidgetLoader.h"
#include "gui/WidgetProcessor.h"
#include "script/ModuleRegistry.h"
#include "script/ScriptUtil.h"

#include <plog/Log.h>
#include <pugixml/pugixml.hpp>
#include <string>

namespace
{
	std::string WIDGET;
}

core::Gui::Gui(const AssetRegistry & assets, const ModuleRegistry & modules, const ::util::File & file)
	: m_script(file.path())
{
	modules.apply(res::script::GUI, m_script);
	modules.apply(res::script::GUI_REGISTRY, m_script);
	util::addGlobalVariable(m_script, this, "GUI"); // TODO: See #29
	util::addGlobalVariable(m_script, &WIDGET, "WIDGET"); // TODO: See #29

	load(file, assets);
}

void core::Gui::load(const ::util::File & file, const AssetRegistry & assets)
{
	pugi::xml_document doc;

	const auto result = doc.load_file(file.path().c_str());
	if (result.status == pugi::xml_parse_status::status_ok)
	{
		doc.child("widgets").append_attribute("name").set_value("root");
		WidgetLoader loader{ assets, m_script, m_bus };
		loader.load(doc.child("widgets"), m_root);
		m_widgets = loader.getWidgets();
	}
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

core::Widget * core::Gui::getWidget(const std::string & name) const
{
	const auto it = m_widgets.find(name);
	return it == m_widgets.end() ? nullptr : it->second;
}

bool core::Gui::has(const std::string & widget) const
{
	return getWidget(widget) != nullptr;
}

bool core::Gui::isVisible(const std::string & widget) const
{
	if (const auto * w = getWidget(widget))
		return w->m_state.m_visible;
	return false;
}
void core::Gui::setVisible(const std::string & widget, bool visible)
{
	if (auto * w = getWidget(widget))
		w->m_state.m_visible = visible;
}

bool core::Gui::isLocked(const std::string & widget) const
{
	if (const auto * w = getWidget(widget))
		return w->m_state.m_locked;
	return true;
}
void core::Gui::setLocked(const std::string & widget, bool locked)
{
	if (auto * w = getWidget(widget))
		w->m_state.m_locked = locked;
}

void core::Gui::setBool(const std::string & widget, bool value)
{
	if (auto * w = getWidget(widget))
		w->m_value.m_bool = value;
}
void core::Gui::setFloat(const std::string & widget, float value)
{
	if (auto * w = getWidget(widget))
		w->m_value.m_float = value;
}
void core::Gui::setString(const std::string & widget, const std::string & value)
{
	if (auto * w = getWidget(widget))
		w->m_value.m_string = value;
}

bool core::Gui::getBool(const std::string & widget) const
{
	if (const auto * w = getWidget(widget))
		return w->m_value.m_bool;
	return false;
}
float core::Gui::getFloat(const std::string & widget) const
{
	if (const auto * w = getWidget(widget))
		return w->m_value.m_float;
	return 0.0f;
}
std::string core::Gui::getString(const std::string & widget) const
{
	if (const auto * w = getWidget(widget))
		return w->m_value.m_string;
	return "";
}
