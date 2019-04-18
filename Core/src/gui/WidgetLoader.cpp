
#include "WidgetLoader.h"

#include "gui/Gui.h"
#include "gui/WidgetListener.h"
#include "gui/WidgetProcessor.h"
#include "util/StringParsing.h"

#include <plog/Log.h>

namespace
{
	const std::string NODE_WIDGET = "widget";
}

// ...

void core::WidgetLoader::load(const pugi::xml_node & node)
{
	m_widget.m_name = node.attribute("name").as_string();

	if (const auto data = node.child("border"))
		loadBorder(data);
	if (const auto data = node.child("bbox"))
		loadBoundingBox(data);
	if (const auto data = node.child("link"))
		loadLink(data);
	if (const auto data = node.child("group"))
		loadGroup(data);
	if (const auto data = node.child("state"))
		loadState(data);

	// Children must be loaded after regular widget data
	loadChildren(node);
}

void core::WidgetLoader::loadBorder(const pugi::xml_node & node)
{
	m_widget.m_border.m_inner = node.attribute("inner").as_float();
	m_widget.m_border.m_outer = node.attribute("outer").as_float();
}
void core::WidgetLoader::loadBoundingBox(const pugi::xml_node & node)
{
	const util::Parser<glm::vec2> parser;

	m_widget.m_bbox.m_minSize = parser.parse(node.attribute("size").as_string());
}
void core::WidgetLoader::loadChildren(const pugi::xml_node & node)
{
	for (auto child = node.first_child(); child; child = child.next_sibling())
	{
		if (NODE_WIDGET != child.name())
			continue;

		const std::string name = child.attribute("name").as_string();
		if (name.empty())
			LOG_WARNING << "Cannot load widget without a name";
		else if (const auto it = m_widgets.find(name); it != m_widgets.end())
			LOG_WARNING << "Cannot overwrite existing widget " << name;
		else
		{
			auto & widget = m_widgets[name];
			widget.m_family.m_leader = &m_widget;
			m_widget.m_family.m_members.push_back(&widget);
			WidgetLoader{ m_script, m_bus, m_widgets, widget }.load(child);
		}
	}
}
void core::WidgetLoader::loadLink(const pugi::xml_node & node)
{
	const util::Parser<glm::vec2> parser;
	const std::string target = node.attribute("target").as_string();

	if (target.empty())
		m_widget.m_link.m_target = m_widget.m_family.m_leader;
	else if (const auto it = m_widgets.find(target); it == m_widgets.end())
		LOG_WARNING << "Cannot link to nonexisting widget " << target;
	else
		m_widget.m_link.m_target = &it->second;
	m_widget.m_link.m_ratio = parser.parse(node.attribute("ratio").as_string());
}
void core::WidgetLoader::loadGroup(const pugi::xml_node & node)
{
	const auto leader = node.attribute("leader").as_string();
	if (const auto it = m_widgets.find(leader); it == m_widgets.end())
		LOG_WARNING << "Cannot form group with nonexisting widget " << leader;
	else
	{
		m_widget.m_group.m_leader = &it->second;
		m_widget.m_group.m_leader->m_group.m_members.push_back(&m_widget);
	}
}
void core::WidgetLoader::loadState(const pugi::xml_node & node)
{
	m_widget.m_state.m_visible = node.attribute("visible").as_bool(true);
	m_widget.m_state.m_locked = node.attribute("locked").as_bool();
	m_widget.m_state.m_active = node.attribute("active").as_bool();
}

// ...

void core::WidgetLoader::registerProcessors()
{
	m_widget.m_processors.push_back(gui::updatePosition);
	m_widget.m_processors.push_back(gui::updateChildren);
	m_widget.m_processors.push_back(gui::updateSize);
}
void core::WidgetLoader::registerMouseListeners()
{
	m_widget.m_listeners.push_back(m_bus.add<MouseMove>(0,
		[this](auto & event) { gui::mouseMove(event, m_widget); }
	));
	m_widget.m_listeners.push_back(m_bus.add<MousePress>(0,
		[this](auto & event) { gui::mousePress(event, m_widget); }
	));
	m_widget.m_listeners.push_back(m_bus.add<MouseRelease>(0,
		[this](auto & event) { gui::mouseRelease(m_script, event, m_widget); }
	));
}

// ...

void core::WidgetLoader::initAsButton(const pugi::xml_node & node)
{
}
void core::WidgetLoader::initAsSlider(const pugi::xml_node & node)
{
}
