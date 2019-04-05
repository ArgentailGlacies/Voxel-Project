
#include "gui/WidgetLoader.h"

#include <pugixml/pugixml.hpp>

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::gui
{
	TEST_CLASS(WidgetLoaderTest)
	{
	public:
		TEST_METHOD(WidgetLoader_loadBorder)
		{
			auto node = m_doc.append_child("border");
			node.append_attribute("inner").set_value(3.0f);
			node.append_attribute("outer").set_value(2.0f);

			m_loader.loadBorder(node);

			Assert::AreEqual(3.0f, m_widget.m_border.m_inner);
			Assert::AreEqual(2.0f, m_widget.m_border.m_outer);
		}
		TEST_METHOD(WidgetLoader_loadBoundingBox)
		{
			auto node = m_doc.append_child("bbox");
			node.append_attribute("size").set_value("40, 25.2");

			m_loader.loadBoundingBox(node);

			Assert::AreEqual({ 0.0f, 0.0f }, m_widget.m_bbox.m_pos);
			Assert::AreEqual({ 0.0f, 0.0f }, m_widget.m_bbox.m_size);
			Assert::AreEqual({ 40.0f, 25.2f }, m_widget.m_bbox.m_minSize);
		}
		TEST_METHOD(WidgetLoader_loadLink)
		{
			auto node = m_doc.append_child("link");
			node.append_attribute("target").set_value("other");
			node.append_attribute("ratio").set_value("0.75, 1.0");

			const auto & other = m_widgets["other"];
			m_loader.loadLink(node);

			Assert::IsTrue(&other == m_widget.m_link.m_target);
			Assert::AreEqual({ 0.75f, 1.0f }, m_widget.m_link.m_ratio);
		}
		TEST_METHOD(WidgetLoader_loadGroup)
		{
			auto node = m_doc.append_child("group");
			node.append_attribute("leader").set_value("other");

			const auto & other = m_widgets["other"];
			m_loader.loadGroup(node);

			Assert::IsTrue(&other == m_widget.m_group.m_leader);
			Assert::AreEqual(0u, m_widget.m_group.m_members.size());
			Assert::IsNull(other.m_link.m_target);
			Assert::AreEqual(1u, other.m_group.m_members.size());
		}
		TEST_METHOD(WidgetLoader_loadState)
		{
			auto node = m_doc.append_child("state");
			node.append_attribute("visible").set_value(false);
			node.append_attribute("locked").set_value(true);
			node.append_attribute("active").set_value(true);

			m_loader.loadState(node);

			Assert::IsFalse(m_widget.m_state.m_visible);
			Assert::IsTrue(m_widget.m_state.m_locked);
			Assert::IsTrue(m_widget.m_state.m_active);
		}

		TEST_METHOD(WidgetLoader_loadDefaultValues)
		{
			auto node = m_doc.append_child("widget");
			
			m_loader.load(node);

			Assert::AreEqual(0.0f, m_widget.m_border.m_inner);
			Assert::AreEqual(0.0f, m_widget.m_border.m_outer);
			Assert::AreEqual({ 0.0f, 0.0f }, m_widget.m_bbox.m_pos);
			Assert::AreEqual({ 0.0f, 0.0f }, m_widget.m_bbox.m_size);
			Assert::AreEqual({ 0.0f, 0.0f }, m_widget.m_bbox.m_minSize);
			Assert::IsNull(m_widget.m_link.m_target);
			Assert::AreEqual({ 0.0f, 0.0f }, m_widget.m_link.m_ratio);
			Assert::IsNull(m_widget.m_group.m_leader);
			Assert::IsTrue(m_widget.m_state.m_visible);
			Assert::IsFalse(m_widget.m_state.m_locked);
			Assert::IsFalse(m_widget.m_state.m_active);
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
			m_loader.load(node);

			Assert::AreEqual(0.0f, m_widget.m_border.m_inner);
			Assert::AreEqual(2.0f, m_widget.m_border.m_outer);
			Assert::AreEqual({ 0.0f, 0.0f }, m_widget.m_bbox.m_pos);
			Assert::AreEqual({ 0.0f, 0.0f }, m_widget.m_bbox.m_size);
			Assert::AreEqual({ 40.0f, 80.0f }, m_widget.m_bbox.m_minSize);
			Assert::IsTrue(&other == m_widget.m_link.m_target);
			Assert::AreEqual({ 0.0f, 0.0f }, m_widget.m_link.m_ratio);
			Assert::IsTrue(&other == m_widget.m_group.m_leader);
			Assert::IsTrue(m_widget.m_state.m_visible);
			Assert::IsFalse(m_widget.m_state.m_locked);
			Assert::IsTrue(m_widget.m_state.m_active);
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

			m_loader.loadChildren(m_doc);
			
			Assert::IsTrue(m_widgets.find("other") != m_widgets.end());
			Assert::IsTrue(m_widgets.find("child") != m_widgets.end());
			Assert::IsTrue(m_widgets["child"].m_state.m_active);
		}

		// ...

		TEST_METHOD(WidgetLoader_registerProcessors)
		{
			m_loader.registerProcessors();

			Assert::AreEqual(3u, m_widget.m_processors.size());
		}
		TEST_METHOD(WidgetLoader_registerMouseListeners)
		{
			m_loader.registerMouseListeners();

			Assert::AreEqual(0u, m_widget.m_listeners.size());
		}

	private:
		pugi::xml_document m_doc;

		Widgets m_widgets;
		Widget & m_widget = m_widgets["widget"];
		WidgetLoader m_loader{ m_widgets, m_widget };
	};
}