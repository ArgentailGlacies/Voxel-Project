#pragma once

#include "event/EventListener.h"
#include "gui/internal/Handler.h"

#include <string>

namespace core
{
	class Script;

	/**
		Labels may be changed by the user in certain cases, for example when the user has selected
		the label and starts typing on the keyboard. Once the label has been activated, the user can
		deactivate it by pressing escape, enter, or clicking a mouse button.
	*/
	class HandlerLabel : public Handler
	{
	public:
		HandlerLabel(const Callback & callback, EventBus & bus, Widget & widget);

		virtual void action(Widget & widget) override final { m_active = true; }
		virtual void process(Widget & widget) override final {}

	private:
		/**
			Appends the given codepoint to the string value in the given widget.

			@param widget The widget which should be updated.
			@param codepoint The character which should be appended to the current string value.
		*/
		void push(Widget & widget, int codepoint);
		/**
			Removes the last character in the stirng value of the given widget.

			@param widget The widget which should be updated.
		*/
		void pop(Widget & widget);

		// ...

		Callback m_callback;
		Listener m_unichar;
		Listener m_keyPress;
		Listener m_mousePress;

		bool m_active = false;
	};
}