#pragma once

#include "gui/Widget.h"

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
		WidgetLoader(Widgets & widgets, Widget & widget) : m_widgets(widgets), m_widget(widget) {}

		/**
			Loads the data stored in the node as a widget node. Will load child widgets as well, if
			any are specified in the node.

			@param node The xml node containing generic widget data.
		*/
		void load(const pugi::xml_node & node);

		void loadBorder(const pugi::xml_node & node);
		void loadBoundingBox(const pugi::xml_node & node);
		void loadLink(const pugi::xml_node & node);
		void loadGroup(const pugi::xml_node & node);
		void loadState(const pugi::xml_node & node);

		// Injects special functionality for button widgets.
		
		void loadButton(const pugi::xml_node & node);
		void loadSlider(const pugi::xml_node & node);

		// ...

		/**
			Registers a processor for recalculating widget size.
			Registers a processor for recalculating widget position.
			Registers a processor for updating all children widgets.
		*/
		void registerProcessors();
		/**
			Registers a listener for mouse moving, to update widget hover state.
			Registers a listener for mouse clicks, to update widget press state.
			Registers a listener for mouse release, to update widget active state.
		*/
		void registerMouseListeners();

	private:
		Widgets & m_widgets;
		Widget & m_widget;
	};
}
