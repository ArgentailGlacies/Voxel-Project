#pragma once

#include <pugixml/pugixml.hpp>
#include <string>
#include <unordered_map>

namespace core
{
	class AssetRegistry;
	class EventBus;
	class Script;
	struct Widget;

	/**
		The widget loader loads a widget and its entire family from a specific node. All widgets
		will be given their processors, renders, and listeners as well.
	*/
	class WidgetLoader
	{
	public:
		WidgetLoader() = delete;
		WidgetLoader(const AssetRegistry & assets, const Script & script, EventBus & bus) noexcept
			: m_assets(assets), m_script(script), m_bus(bus)
		{}

		/**
			Loads the data stored in the node as a widget node. Will load child widgets as well, if
			any are specified in the node.

			@param node The xml node containing widget data.
			@param widget The widget which should be loaded.
		*/
		void load(const pugi::xml_node & node, Widget & widget);

		/**
			Loads up all children of the widget, and loads the childrens' children, recursively.
			<widget name="childA" type="type" />
			<widget name="childB" type="type" />

			@param node The xml node containing widget data.
			@param widget The widget which should be loaded.
		*/
		void loadChildren(const pugi::xml_node & node, Widget & widget);

		/**
			Loads up the inner and outer border of a widget.
			<border inner="2" outer="3" />

			@param node The xml node containing widget data.
			@param widget The widget which should be loaded.
		*/
		void loadBorder(const pugi::xml_node & node, Widget & widget);
		/**
			Loads up the minimum size the widget can have.
			<bbox size="40, 25" />

			@param node The xml node containing widget data.
			@param widget The widget which should be loaded.
		*/
		void loadBoundingBox(const pugi::xml_node & node, Widget & widget);
		/**
			Loads a link between this widget and a different widget. The position of this widget
			will be specified in terms of the position of the link target position and the anchor
			point.
			<link target="widget" ratio="0.5, 1" />

			@param node The xml node containing widget data.
			@param widget The widget which should be loaded.
		*/
		void loadLink(const pugi::xml_node & node, Widget & widget);
		/**
			Assigns a widget as a leader of the group. If the widget has a leader as well, this
			widget will be added to that group instead.
			<group leader="widget" />

			@param node The xml node containing widget data.
			@param widget The widget which should be loaded.
		*/
		void loadGroup(const pugi::xml_node & node, Widget & widget);
		/**
			Loads up the state of the widget, whether it is visible, locked, and/or active.
			<state visible="true" locked="false" active="false" />

			@param node The xml node containing widget data.
			@param widget The widget which should be loaded.
		*/
		void loadState(const pugi::xml_node & node, Widget & widget);

		// ...

		/**
			Loads up all data which makes the widget unique and specialized to perform its task.

			@param node The xml node containing widget data.
			@param widget The widget which should be loaded.
		*/
		void loadSpecialization(const pugi::xml_node & node, Widget & widget);

		void loadButton(const pugi::xml_node & node, Widget & widget);
		void loadLabel(const pugi::xml_node & node, Widget & widget);
		void loadPanel(const pugi::xml_node & node, Widget & widget);
		void loadSlider(const pugi::xml_node & node, Widget & widget);

		// ...

		/**
			@return The map of all named widgets that have been loaded.
		*/
		inline auto getWidgets() const { return m_widgets; }

	private:
		/**
			Retrieves the fully qualified name for a widget with the given name and, if present, the
			given parent. The fully qualified name will be the name of the parent concatenated with
			the name of the child widget, seperated by a period character.

			@param name The name of the child widget. Must not contain any periods.
			@param parent The parent of the child widget, if it has any parents.
			@return The fully qualified name of the widget, on the form "parent.child".
		*/
		std::string getFullName(const std::string & name, const Widget * parent) const;

		/**
			Attempts to locate the widget with the given name. The name must not be a fully
			qualified name. If the parent is specified, the widget will be a child of the parent
			with the given name.

			@param name The name of the widget to located.
			@param parent The parent of the widget to locate.
			@return The widget, or nullptr if it could not be located.
		*/
		Widget * getWidget(const std::string & name, const Widget * parent) const;

		// ...

		const AssetRegistry & m_assets;
		const Script & m_script;
		EventBus & m_bus;

		std::unordered_map<std::string, Widget *> m_widgets;
	};
}
