
#include "WidgetLoader.h"

#include "asset/AssetRegistry.h"
#include "gui/GuiEvents.h"
#include "gui/WidgetListener.h"
#include "gui/WidgetRenderer.h"
#include "util/StringParsing.h"

#include <plog/Log.h>

namespace
{
	const std::string NODE_WIDGET = "widget";

	/**
		Assigns the bool state in the global widget state field.
	*/
	void setState(const core::Script & script, bool state)
	{
		script.execute(core::GuiData::STATE_BOOL + " = " + (state ? "true" : "false") + ";");
	}
	/**
		Assigns the float state in the global widget state field.
	*/
	void setState(const core::Script & script, float state)
	{
		script.execute(core::GuiData::STATE_BOOL + " = " + std::to_string(state) + ";");
	}
	/**
		Assigns the string state in the global widget state field.
	*/
	void setState(const core::Script & script, const std::string & state)
	{
		script.execute(core::GuiData::STATE_BOOL + " = \"" + util::replaceAll(state, "\"", "\\\"") + "\";");
	}
}

// ...

void core::WidgetLoader::load(const pugi::xml_node & node, Widget & widget)
{
	// Can only store widgets with unique names
	const std::string name = node.attribute("name").as_string();
	if (name.empty())
		LOG_WARNING << "Cannot load widget without a name";
	else if (const auto it = m_widgets.find(name); it != m_widgets.end())
		LOG_WARNING << "Cannot overwrite existing widget " << name;
	else
		m_widgets[node.attribute("name").as_string()] = &widget;

	// Must load normal data and specialization before children
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
	loadSpecialization(node, widget);
	loadChildren(node, widget);
}

void core::WidgetLoader::loadChildren(const pugi::xml_node & node, Widget & widget)
{
	for (auto child = node.first_child(); child; child = child.next_sibling())
	{
		if (NODE_WIDGET != child.name())
			continue;

		auto & ptr = widget.m_family.m_children.emplace_back(std::make_unique<Widget>());
		ptr->m_family.m_parent = &widget;
		load(child, *ptr);
	}
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
void core::WidgetLoader::loadLink(const pugi::xml_node & node, Widget & widget)
{
	const util::Parser<glm::vec2> parser;
	const std::string target = node.attribute("target").as_string();

	if (target.empty())
		widget.m_link.m_target = widget.m_family.m_parent;
	else if (const auto it = m_widgets.find(target); it == m_widgets.end())
		LOG_WARNING << "Cannot link to nonexisting widget " << target;
	else
		widget.m_link.m_target = it->second;
	widget.m_link.m_ratio = parser.parse(node.attribute("ratio").as_string());
}
void core::WidgetLoader::loadGroup(const pugi::xml_node & node, Widget & widget)
{
	const auto leader = node.attribute("leader").as_string();
	if (const auto it = m_widgets.find(leader); it == m_widgets.end())
		LOG_WARNING << "Cannot form group with nonexisting widget " << leader;
	else
	{
		widget.m_group.m_leader = it->second;
		widget.m_group.m_leader->m_group.m_members.push_back(&widget);
	}
}
void core::WidgetLoader::loadState(const pugi::xml_node & node, Widget & widget)
{
	widget.m_state.m_visible = node.attribute("visible").as_bool(true);
	widget.m_state.m_locked = node.attribute("locked").as_bool();
}

// ...

void core::WidgetLoader::loadSpecialization(const pugi::xml_node& node, Widget& widget)
{
	if (const auto data = node.child("button"))
		loadButton(data, widget);
}

void core::WidgetLoader::loadButton(const pugi::xml_node & node, Widget & widget)
{
	const std::string type = node.attribute("type").as_string("generic");
	const std::string action = node.child("script").attribute("action").as_string();
	const std::string sprite = node.child("renderer").attribute("button").as_string();

	const auto & script = m_data.getScript();
	widget.m_scripts["action"] = action;

	CallbackConst<WidgetActivate> callback = [](const WidgetActivate &) {};
	if (type == "generic")
	{
		callback = [&widget, &script](const WidgetActivate & event) {
			if (&event.m_widget != &widget)
				return;

			setState(script, true);
			script.execute(widget.m_scripts["action"]);
		};
	}
	else if (type == "checkbox")
	{
		callback = [&widget, &script](const WidgetActivate & event) {
			if (&event.m_widget != &widget)
				return;

			widget.m_value.m_bool = !widget.m_value.m_bool;

			setState(script, widget.m_value.m_bool);
			script.execute(widget.m_scripts["action"]);
		};
	}
	else if (type == "radio")
	{
		callback = [&widget, &script](const WidgetActivate & event) {
			if (&event.m_widget != &widget)
				return;

			for (auto & w : widget.m_group.m_leader->m_group.m_members)
				w->m_value.m_bool = false;
			widget.m_value.m_bool = true;

			setState(script, true);
			script.execute(widget.m_scripts["action"]);
		};
	}
	else
		LOG_WARNING << "Unknown button type " << type;

	const auto asset = m_assets.get<Sprite>(sprite);
	widget.m_renderers.push_back(gui::WidgetRendererButton{ asset });

	registerStandardListeners(widget);
	widget.m_listeners.push_back(m_data.getBus().add<WidgetActivate>(callback));
}
void core::WidgetLoader::loadSlider(const pugi::xml_node & node, Widget & widget)
{
}
void core::WidgetLoader::loadLabel(const pugi::xml_node & node, Widget & widget)
{
}
void core::WidgetLoader::loadTextbox(const pugi::xml_node & node, Widget & widget)
{
}

// ...

void core::WidgetLoader::registerStandardListeners(Widget & widget)
{
	widget.m_listeners.push_back(m_data.getBus().add<MouseMove>(0,
		[&widget](auto & event) { gui::mouseMove(event, widget); }
	));
	widget.m_listeners.push_back(m_data.getBus().add<MousePress>(0,
		[&widget](auto & event) { gui::mousePress(event, widget); }
	));
	widget.m_listeners.push_back(m_data.getBus().add<MouseRelease>(0,
		[this, &widget](auto & event) { gui::mouseRelease(m_data.getBus(), event, widget); }
	));
}
