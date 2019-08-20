#pragma once

#include <string>

namespace core
{
	/**
		Represents all possible built-in widget types. Users may define new widget types if more
		types are required.
	*/
	namespace WidgetType
	{
		/**
			Simple interactible button.
		*/
		const std::string BUTTON = "button";
		/**
			Displays a simple text.
		*/
		const std::string LABEL = "label";
		/**
			Simple panel, groups a collection of other widgets into a single unit.
		*/
		const std::string PANEL = "panel";
		/**
			Changes state in a continuous manner between two values.
		*/
		const std::string SLIDER = "slider";
		/**
			Changes state based on user text input.
		*/
		const std::string TEXTBOX = "textbox";
	}
}