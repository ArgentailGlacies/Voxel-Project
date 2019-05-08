
#include "gui/WidgetLoader.h"

#include <pugixml/pugixml.hpp>

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	TEST_CLASS(WidgetLoaderTest)
	{
	public:
		TEST_METHOD(WidgetLoader_loadHeader)
		{
			auto & widget = m_widgets["widget"];
			m_loader.loadHeader(addWidget(m_doc, "name", "type"), widget);

			Assert::AreEqual({ "name" }, widget.m_name);
			Assert::AreEqual({ "type" }, widget.m_type);
		}
		TEST_METHOD(WidgetLoader_loadBorder)
		{
			Widget::Border border;
			border.m_inner = 2.0f;
			border.m_outer = 3.0f;

			auto & widget = m_widgets["widget"];
			m_loader.loadBorder(addBorder(m_doc, border), widget);

			Assert::AreEqual(border.m_inner, widget.m_border.m_inner);
			Assert::AreEqual(border.m_outer, widget.m_border.m_outer);
		}
		TEST_METHOD(WidgetLoader_loadBoundingBox)
		{
			Widget::BoundingBox bbox;
			bbox.m_minSize = { 40.0f, 25.2f };

			auto & widget = m_widgets["widget"];
			m_loader.loadBoundingBox(addBoundingBox(m_doc, bbox), widget);

			Assert::AreEqual(bbox.m_pos, widget.m_bbox.m_pos);
			Assert::AreEqual(bbox.m_size, widget.m_bbox.m_size);
			Assert::AreEqual(bbox.m_minSize, widget.m_bbox.m_minSize);
		}
		TEST_METHOD(WidgetLoader_loadGroup)
		{
			auto & leader = add("leader");

			auto & widget = m_widgets["widget"];
			m_loader.loadGroup(addGroup(m_doc, leader), widget);

			Assert::IsTrue(&leader == widget.m_group.m_leader);
			Assert::AreEqual(0u, widget.m_group.m_members.size());
			Assert::IsNull(leader.m_link.m_target);
			Assert::AreEqual(1u, leader.m_group.m_members.size());
		}
		TEST_METHOD(WidgetLoader_loadLink)
		{
			Widget::Link link;
			link.m_target = &add("target");
			link.m_ratio = { 0.75f, 1.0f };

			auto & widget = m_widgets["widget"];
			m_loader.loadLink(addLink(m_doc, link), widget);

			Assert::IsTrue(link.m_target == widget.m_link.m_target);
			Assert::AreEqual(link.m_ratio, widget.m_link.m_ratio);
		}
		TEST_METHOD(WidgetLoader_loadState)
		{
			Widget::State state;
			state.m_visible = false;
			state.m_active = true;
			state.m_locked = true;

			auto & widget = m_widgets["widget"];
			m_loader.loadState(addState(m_doc, state), widget);

			Assert::AreEqual(state.m_visible, widget.m_state.m_visible);
			Assert::AreEqual(state.m_active, widget.m_state.m_locked);
			Assert::AreEqual(state.m_locked, widget.m_state.m_active);
		}

		TEST_METHOD(WidgetLoader_loadDefaultValues)
		{
			auto node = m_doc.append_child("widget");
			
			auto & widget = m_widgets["widget"];
			m_loader.load(node, widget);

			Assert::AreEqual(0.0f, widget.m_border.m_inner);
			Assert::AreEqual(0.0f, widget.m_border.m_outer);
			Assert::AreEqual({ 0.0f, 0.0f }, widget.m_bbox.m_pos);
			Assert::AreEqual({ 0.0f, 0.0f }, widget.m_bbox.m_size);
			Assert::AreEqual({ 0.0f, 0.0f }, widget.m_bbox.m_minSize);
			Assert::IsNull(widget.m_link.m_target);
			Assert::AreEqual({ 0.0f, 0.0f }, widget.m_link.m_ratio);
			Assert::IsNull(widget.m_group.m_leader);
			Assert::IsTrue(widget.m_state.m_visible);
			Assert::IsFalse(widget.m_state.m_locked);
			Assert::IsFalse(widget.m_state.m_active);
		}
		TEST_METHOD(WidgetLoader_loadSpecialValues)
		{
			auto node = m_doc.append_child("widget");
			node.append_child("border").append_attribute("outer").set_value("2");
			node.append_child("bbox").append_attribute("size").set_value("40, 80");
			node.append_child("group").append_attribute("leader").set_value("other");
			node.append_child("link").append_attribute("target").set_value("other");
			node.append_child("state").append_attribute("active").set_value("true");

			const auto & other = m_widgets["other"];
			auto & widget = m_widgets["widget"];
			m_loader.load(node, widget);

			Assert::AreEqual(0.0f, widget.m_border.m_inner);
			Assert::AreEqual(2.0f, widget.m_border.m_outer);
			Assert::AreEqual({ 0.0f, 0.0f }, widget.m_bbox.m_pos);
			Assert::AreEqual({ 0.0f, 0.0f }, widget.m_bbox.m_size);
			Assert::AreEqual({ 40.0f, 80.0f }, widget.m_bbox.m_minSize);
			Assert::IsTrue(&other == widget.m_link.m_target);
			Assert::AreEqual({ 0.0f, 0.0f }, widget.m_link.m_ratio);
			Assert::IsTrue(&other == widget.m_group.m_leader);
			Assert::IsTrue(widget.m_state.m_visible);
			Assert::IsFalse(widget.m_state.m_locked);
			Assert::IsTrue(widget.m_state.m_active);
		}

		TEST_METHOD(WidgetLoader_loadChild)
		{
			// Multiple children can be loaded simultanously, cannot overwrite earlier widgets
			auto childA = m_doc.append_child("widget");
			childA.append_attribute("name").set_value("child");
			childA.append_child("state").append_attribute("active").set_value(true);

			auto childB = m_doc.append_child("widget");
			childB.append_attribute("name").set_value("child");
			childB.append_child("state").append_attribute("active").set_value(false);

			m_doc.append_child("widget").append_attribute("name").set_value("other");

			auto & widget = m_widgets["widget"];
			m_loader.loadChildren(m_doc, widget);
			
			Assert::IsTrue(m_widgets.find("other") != m_widgets.end());
			Assert::IsTrue(m_widgets.find("child") != m_widgets.end());
			Assert::IsTrue(m_widgets["child"].m_state.m_active);
		}
		TEST_METHOD(WidgetLoader_loadWidgetHierarchy)
		{
			auto parent = addWidget(m_doc, "parent", "panel");
			auto childA = addWidget(parent, "childA", "panel");
			auto childB = addWidget(parent, "childB", "button");
			auto childC = addWidget(childA, "childC", "button");

			auto & widget = m_widgets["widget"];
			m_loader.load(m_doc, widget);

			Assert::AreEqual({ "parent" }, m_widgets["parent"].m_name);
			Assert::AreEqual({ "childA" }, m_widgets["childA"].m_name);
			Assert::AreEqual({ "childB" }, m_widgets["childB"].m_name);
			Assert::AreEqual({ "childC" }, m_widgets["childC"].m_name);

			Assert::AreEqual(2u, m_widgets["parent"].m_family.m_members.size());
			Assert::AreEqual(1u, m_widgets["childA"].m_family.m_members.size());
			Assert::AreEqual(0u, m_widgets["childB"].m_family.m_members.size());
			Assert::AreEqual(0u, m_widgets["childC"].m_family.m_members.size());

			Assert::IsTrue(&widget == m_widgets["parent"].m_family.m_leader);
			Assert::IsTrue(&m_widgets["parent"] == m_widgets["childA"].m_family.m_leader);
			Assert::IsTrue(&m_widgets["parent"] == m_widgets["childB"].m_family.m_leader);
			Assert::IsTrue(&m_widgets["childA"] == m_widgets["childC"].m_family.m_leader);
		}

	private:
		Widget & add(const std::string & name)
		{
			m_widgets[name].m_name = name;
			return m_widgets[name];
		}

		pugi::xml_node addBorder(pugi::xml_node & widget, const Widget::Border & border)
		{
			auto node = widget.append_child("border");
			node.append_attribute("inner").set_value(border.m_inner);
			node.append_attribute("outer").set_value(border.m_outer);
			return node;
		}
		pugi::xml_node addBoundingBox(pugi::xml_node & widget, const Widget::BoundingBox & bbox)
		{
			const auto str = std::to_string(bbox.m_minSize.x) + ", " + std::to_string(bbox.m_minSize.y);
			auto node = widget.append_child("bbox");
			node.append_attribute("size").set_value(str.c_str());
			return node;
		}
		pugi::xml_node addGroup(pugi::xml_node & widget, const Widget & leader)
		{
			auto node = widget.append_child("group");
			node.append_attribute("leader").set_value(leader.m_name.c_str());
			return node;
		}
		pugi::xml_node addLink(pugi::xml_node & widget, const Widget::Link & link)
		{
			const auto str = std::to_string(link.m_ratio.x) + ", " + std::to_string(link.m_ratio.y);
			auto node = widget.append_child("link");
			node.append_attribute("target").set_value(link.m_target->m_name.c_str());
			node.append_attribute("ratio").set_value(str.c_str());
			return node;
		}
		pugi::xml_node addState(pugi::xml_node & widget, const Widget::State & state)
		{
			auto node = widget.append_child("state");
			node.append_attribute("visible").set_value(state.m_visible);
			node.append_attribute("active").set_value(state.m_active);
			node.append_attribute("locked").set_value(state.m_locked);
			return node;
		}
		pugi::xml_node addWidget(pugi::xml_node & widget, const std::string & name, const std::string & type)
		{
			auto node = widget.append_child("widget");
			node.append_attribute("name").set_value(name.c_str());
			node.append_attribute("type").set_value(type.c_str());
			return node;
		}

		// ...

		pugi::xml_document m_doc;

		Widgets m_widgets;
		WidgetLoader m_loader{ m_widgets };
	};
}