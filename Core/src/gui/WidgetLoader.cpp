
#include "WidgetLoader.h"

#include "gui/Gui.h"
#include "gui/WidgetListener.h"
#include "gui/WidgetProcessor.h"
#include "gui/WidgetRenderer.h"
#include "util/StringParsing.h"

#include <plog/Log.h>

namespace
{
	const std::string NODE_WIDGET = "widget";
}

// ...

void core::WidgetLoader::load(const pugi::xml_node & node, Widget & widget)
{
	loadHeader(node, widget);

	if (const auto data = node.child("border"))
		loadBorder(data, widget);
	if (const auto data = node.child("bbox"))
		loadBoundingBox(data, widget);
	if (const auto data = node.child("link"))
		loadLink(data, widget);
	if (const auto data = node.child("group"))
		loadGroup(data, widget);
	if (const auto data = node.child("state"))
		loadState(data, widget);

	loadChildren(node, widget);
}

void core::WidgetLoader::loadHeader(const pugi::xml_node & node, Widget & widget)
{
	widget.m_name = node.attribute("name").as_string();
	widget.m_type = node.attribute("type").as_string();
}
void core::WidgetLoader::loadBorder(const pugi::xml_node & node, Widget & widget)
{
	widget.m_border.m_inner = node.attribute("inner").as_float();
	widget.m_border.m_outer = node.attribute("outer").as_float();
}
void core::WidgetLoader::loadBoundingBox(const pugi::xml_node & node, Widget & widget)
{
	const util::Parser<glm::vec2> parser;

	widget.m_bbox.m_minSize = parser.parse(node.attribute("size").as_string());
}
void core::WidgetLoader::loadChildren(const pugi::xml_node & node, Widget & widget)
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
			auto & w = m_widgets[name];
			w.m_family.m_leader = &widget;
			widget.m_family.m_members.push_back(&w);
			load(child, w);
		}
	}
}
void core::WidgetLoader::loadLink(const pugi::xml_node & node, Widget & widget)
{
	const util::Parser<glm::vec2> parser;
	const std::string target = node.attribute("target").as_string();

	if (target.empty())
		widget.m_link.m_target = widget.m_family.m_leader;
	else if (const auto it = m_widgets.find(target); it == m_widgets.end())
		LOG_WARNING << "Cannot link to nonexisting widget " << target;
	else
		widget.m_link.m_target = &it->second;
	widget.m_link.m_ratio = parser.parse(node.attribute("ratio").as_string());
}
void core::WidgetLoader::loadGroup(const pugi::xml_node & node, Widget & widget)
{
	const auto leader = node.attribute("leader").as_string();
	if (const auto it = m_widgets.find(leader); it == m_widgets.end())
		LOG_WARNING << "Cannot form group with nonexisting widget " << leader;
	else
	{
		widget.m_group.m_leader = &it->second;
		widget.m_group.m_leader->m_group.m_members.push_back(&widget);
	}
}
void core::WidgetLoader::loadState(const pugi::xml_node & node, Widget & widget)
{
	widget.m_state.m_visible = node.attribute("visible").as_bool(true);
	widget.m_state.m_locked = node.attribute("locked").as_bool();
	widget.m_state.m_active = node.attribute("active").as_bool();
}
