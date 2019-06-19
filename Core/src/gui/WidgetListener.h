#pragma once

#include "event/EventBus.h"
#include "event/Events.h"
#include "gui/Widget.h"
#include "script/Script.h"

namespace core
{
	namespace gui
	{
		/**
			Updates a widget's hover state. If the parent widget is hovered and the mouse is 
			positioned over the widget, it is marked as hovered, false otherwise.

			@param event The event which should be processed.
			@param widget The widget which should be updated.
		*/
		void mouseMove(MouseMove & event, Widget & widget);
		/**
			Updates a widget's selected state. If the widget is hovered, the event is consumed to
			select the widget.

			@param event The event which should be processed.
			@param widget The widget which should be updated.
		*/
		void mousePress(MousePress & event, Widget & widget);
		/**
			Updates the widget's selected state. If the widget was selected and hovered, the event
			is consumed to post a widget click event on the provided bus.

			@param bus The bus the widget click event should be posted on.
			@param event The event which should be processed.
			@param widget The widget which should be updated.
		*/
		void mouseRelease(const EventBus & bus, MouseRelease & event, Widget & widget);
	}
}