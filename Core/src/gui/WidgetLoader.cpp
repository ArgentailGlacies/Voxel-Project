
#include "WidgetLoader.h"

#include "asset/AssetRegistry.h"
#include "gui/internal/HandlerButton.h"
#include "gui/internal/HandlerLabel.h"
#include "gui/internal/HandlerSlider.h"
#include "gui/internal/Processor.h"
#include "gui/internal/RendererButton.h"
#include "gui/internal/RendererLabel.h"
#include "gui/internal/RendererPanel.h"
#include "gui/internal/RendererSlider.h"
#include "gui/Widget.h"
#include "script/Script.h"
#include "util/MathOperations.h"
#include "util/StringParsing.h"

#include <memory>
#include <plog/Log.h>

namespace
{
	const std::string NODE_WIDGET = "widget";

	/**
		The script executor is a simple functor object which executes the provided code under the
		provided script.
	*/
	class ScriptExecutor
	{
	public:
		ScriptExecutor(const core::Script & script, const std::string & code) : m_script(script), m_code(code) {}

		inline void operator()(core::Widget & widget) const { m_script.execute(m_code); }

	private:
		const core::Script & m_script;
		std::string m_code;
	};
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
		m_widgets[name] = &widget;

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
		widget.m_link.m_target = nullptr;
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
		for (auto * member : widget.m_group.m_members)
			it->second->m_group.m_members.push_back(member);
		widget.m_group.m_members.clear();
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
	else if (const auto data = node.child("label"))
		loadLabel(data, widget);
	else if (const auto data = node.child("panel"))
		loadPanel(data, widget);
	else if (const auto data = node.child("slider"))
		loadSlider(data, widget);
}

void core::WidgetLoader::loadButton(const pugi::xml_node & node, Widget & widget)
{
	const std::string type = node.attribute("type").as_string("generic");
	const std::string action = node.child("script").attribute("action").as_string();
	const std::string sprite = node.child("renderer").attribute("button").as_string();

	// Load action
	const ScriptExecutor callback{ m_script, action };

	if (type == "generic")
		widget.m_handler = std::make_unique<HandlerButton>(callback);
	else if (type == "checkbox")
		widget.m_handler = std::make_unique<HandlerButtonCheckbox>(callback);
	else if (type == "radio")
		widget.m_handler = std::make_unique<HandlerButtonRadio>(callback);
	else
		LOG_WARNING << "Unknown button type " << type;

	// Load renderer
	widget.m_renderer = std::make_unique<RendererButton>(m_assets.get<Sprite>(sprite));

	// Load processor
	widget.m_processor = std::make_unique<Processor>(widget, m_bus);
}
void core::WidgetLoader::loadLabel(const pugi::xml_node & node, Widget & widget)
{
	const util::Parser<glm::vec4> parser;
	const auto renderer = node.child("renderer");

	const auto dynamic = node.attribute("mutable").as_bool();
	const std::string action = node.child("script").attribute("action").as_string();
	const std::string pretext = renderer.attribute("pretext").as_string();
	const std::string posttext = renderer.attribute("posttext").as_string();

	Text::Style style;
	style.m_font = renderer.attribute("font").as_string();
	style.m_size = renderer.attribute("size").as_float(1.0f);
	style.m_color = parser.parse(renderer.attribute("color").as_string(), glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });
	style.m_bold = renderer.attribute("bold").as_bool();
	style.m_italic = renderer.attribute("italic").as_bool();
	style.m_monochrome = renderer.attribute("monochrome").as_bool();
	style.m_strikethrough = renderer.attribute("strikethrough").as_bool();
	style.m_underline = renderer.attribute("underline").as_bool();

	// Load action
	if (dynamic)
		widget.m_handler = std::make_unique<HandlerLabel>(ScriptExecutor{ m_script, action }, m_bus, widget);

	// Load renderer
	widget.m_renderer = std::make_unique<RendererLabel>(m_assets, widget, style, pretext, posttext);

	// Load processor
	if (dynamic)
		widget.m_processor = std::make_unique<Processor>(widget, m_bus);
}
void core::WidgetLoader::loadPanel(const pugi::xml_node & node, Widget & widget)
{
	const std::string sprite = node.child("renderer").attribute("background").as_string();

	// Load renderer
	widget.m_renderer = std::make_unique<RendererPanel>(m_assets.get<Sprite>(sprite));
}
void core::WidgetLoader::loadSlider(const pugi::xml_node & node, Widget & widget)
{
	const std::string type = node.attribute("type").as_string("horizontal");
	const std::string action = node.child("script").attribute("action").as_string();
	const std::string spriteBar = node.child("renderer").attribute("button_bar").as_string();
	const std::string spriteIncrement = node.child("renderer").attribute("button_increment").as_string();
	const std::string spriteDecrement = node.child("renderer").attribute("button_decrement").as_string();

	HandlerSlider::Data data;
	data.m_min = node.child("data").attribute("min").as_float(0.0f);
	data.m_max = node.child("data").attribute("max").as_float(0.0f);
	data.m_center = node.child("data").attribute("center").as_float(0.5f * (data.m_min + data.m_max));
	data.m_step = node.child("data").attribute("step").as_float(0.0f);

	const auto horizontal = type == "horizontal";

	// Load children
	auto & bar = *widget.m_family.m_children.emplace_back(std::make_unique<Widget>());
	auto & increment = *widget.m_family.m_children.emplace_back(std::make_unique<Widget>());
	auto & decrement = *widget.m_family.m_children.emplace_back(std::make_unique<Widget>());
	auto & label = *widget.m_family.m_children.emplace_back(std::make_unique<Widget>());

	increment.m_family.m_parent = &widget;
	decrement.m_family.m_parent = &widget;
	label.m_family.m_parent = &widget;
	bar.m_family.m_parent = &widget;

	const auto max = util::max(widget.m_bbox.m_minSize.x, widget.m_bbox.m_minSize.y);
	const auto min = util::min(widget.m_bbox.m_minSize.x, widget.m_bbox.m_minSize.y);
	increment.m_bbox.m_minSize = { min, min };
	decrement.m_bbox.m_minSize = { min, min };

	if (horizontal)
	{
		bar.m_bbox.m_minSize = { max - 2.0f * min, min };

		bar.m_link.m_target = &decrement;
		bar.m_link.m_ratio = { 1.0f, 0.5f };
		increment.m_link.m_target = &bar;
		increment.m_link.m_ratio = { 1.0f, 0.5f };
	}
	else
	{
		bar.m_bbox.m_minSize = { min, max - 2.0f * min };

		bar.m_link.m_target = &increment;
		bar.m_link.m_ratio = { 0.5f, 1.0f };
		decrement.m_link.m_target = &bar;
		decrement.m_link.m_ratio = { 0.5f, 1.0f };
	}
	label.m_bbox.m_minSize = bar.m_bbox.m_minSize;
	label.m_link.m_target = &bar;
	label.m_link.m_ratio = { 0.5f, 0.5f };

	// Load action
	auto handler = std::make_unique<HandlerSlider>(ScriptExecutor{ m_script, action }, data);
	bar.m_handler = std::make_unique<HandlerSliderBar>(handler->slider(widget), m_bus, horizontal);
	increment.m_handler = std::make_unique<HandlerButton>(handler->incrementer(widget));
	decrement.m_handler = std::make_unique<HandlerButton>(handler->decrementer(widget));
	label.m_handler = std::make_unique<HandlerLabel>(handler->translator(widget), m_bus, widget);

	widget.m_handler = std::move(handler);

	// Load renderer
	bar.m_renderer = std::make_unique<RendererSlider>(data, m_assets.get<Sprite>(spriteBar), horizontal);
	increment.m_renderer = std::make_unique<RendererButton>(m_assets.get<Sprite>(spriteIncrement));
	decrement.m_renderer = std::make_unique<RendererButton>(m_assets.get<Sprite>(spriteDecrement));
	label.m_renderer = std::make_unique<RendererLabel>(m_assets, widget, Text::Style{ "overlock" }, "", "");

	// Load processor
	bar.m_processor = std::make_unique<Processor>(bar, m_bus);
	increment.m_processor = std::make_unique<Processor>(increment, m_bus);
	decrement.m_processor = std::make_unique<Processor>(decrement, m_bus);
	label.m_processor = std::make_unique<Processor>(label, m_bus);
}
