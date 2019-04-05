
#include "WidgetLoader.h"

#include "util/StringParsing.h"

void core::WidgetLoader::load(const pugi::xml_node & node)
{
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
void core::WidgetLoader::loadLink(const pugi::xml_node & node)
{
	const util::Parser<glm::vec2> parser;
	const auto it = m_widgets.find(node.attribute("target").as_string());

	m_widget.m_link.m_target = it == m_widgets.end() ? nullptr : &it->second;
	m_widget.m_link.m_ratio = parser.parse(node.attribute("ratio").as_string());
}
void core::WidgetLoader::loadGroup(const pugi::xml_node & node)
{
	const auto leader = node.attribute("leader").as_string();
	if (const auto it = m_widgets.find(leader); it != m_widgets.end())
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

void core::WidgetLoader::initAsButton(const pugi::xml_node & node)
{
}
void core::WidgetLoader::initAsSlider(const pugi::xml_node & node)
{
}

// ...

void core::WidgetLoader::registerProcessors()
{
}
void core::WidgetLoader::registerMouseListeners()
{
}