#pragma once

#include "gui/Widget.h"

namespace core
{
	/**
		Whenever a widget is clicked by the user or otherwise activated via a keyboard shortcut,
		this event is posted on the current gui's event bus.
	*/
	struct WidgetActivate
	{
		WidgetActivate(Widget & widget) : m_widget(widget) {}

		// The widget which is to be activated.
		Widget & m_widget;
	};
}