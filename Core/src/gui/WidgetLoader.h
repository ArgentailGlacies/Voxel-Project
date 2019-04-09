#pragma once

#include "event/EventBus.h"
#include "gui/Widget.h"
#include "script/Script.h"

#include <pugixml/pugixml.hpp>

namespace core
{
	/**
		The widget loader loads a widget and its entire family from a specific node. All widgets
		will be given their processors, renders, and listeners as well.
	*/
	class WidgetLoader
	{
	public:
		WidgetLoader() = delete;
		WidgetLoader(const Script & script, EventBus & bus, Widgets & widgets, Widget & widget)
			: m_script(script), m_bus(bus), m_widgets(widgets), m_widget(widget) {}

		/**
			Loads the data stored in the node as a widget node. Will load child widgets as well, if
			any are specified in the node.

			@param node The xml node containing generic widget data.
		*/
		void load(const pugi::xml_node & node);

		/**
			Loads up the inner and outer border of a widget.

			<border inner="2" outer="3" />
		*/
		void loadBorder(const pugi::xml_node & node);
		/**
			Loads up the minimum size the widget can have.

			<bbox size="40, 25" />
		*/
		void loadBoundingBox(const pugi::xml_node & node);
		/**
			Loads up all children of the widget, and loads the childrens' children, recursively.

			<widget name="childA" type="type" />
			<widget name="childB" type="type" />
			...
		*/
		void loadChildren(const pugi::xml_node & node);
		/**
			Loads a link between this widget and a different widget. The position of this widget
			will be specified in terms of the position of the link target position and the anchor
			point.

			<link target="widget" ratio="0.5, 1" />
		*/
		void loadLink(const pugi::xml_node & node);
		/**
			Assigns a widget as a leader of the group. If the widget has a leader as well, this
			widget will be added to that group instead.

			<group leader="widget" />
		*/
		void loadGroup(const pugi::xml_node & node);
		/**
			Loads up the state of the widget, whether it is visible, locked, and/or active.

			<state visible="true" locked="false" active="false" />
		*/
		void loadState(const pugi::xml_node & node);

		// ...

		/**
			Registers a processor for recalculating widget size, position, and one to update
			children. Invoking all processors for a widget, should cause all processors for all
			child widgets to be invoked as well.
		*/
		void registerProcessors();
		/**
			Registers a listener for mouse move, press and release events in order to properly
			process widget hover, press and active state.
		*/
		void registerMouseListeners();

		// ...
		
		void initAsButton(const pugi::xml_node & node);
		void initAsSlider(const pugi::xml_node & node);

	private:
		const Script & m_script;
		EventBus & m_bus;
		Widgets & m_widgets;
		Widget & m_widget;
	};
}
