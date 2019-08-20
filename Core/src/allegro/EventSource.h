#pragma once

#include <gsl/pointers>

struct ALLEGRO_EVENT_SOURCE;

namespace core
{
	class Display;

	using EventSourcePtr = gsl::not_null<ALLEGRO_EVENT_SOURCE*>;

	/**
		Retrieves the event source associated with the given display. Any user interaction with the
		given display will be fired from this source.

		@param display The display to obtain the event source from.
	*/
	EventSourcePtr getDisplayEventSource(const Display & display);
	/**
		Retrieves the event source associated with the keyboard. All user inputs via the keyboard
		will be fired from this source.
	*/
	EventSourcePtr getKeyboardEventSource();
	/**
		Retrieves the event source associated with the mouse. All user inputs via the mouse will be
		fired from this source.
	*/
	EventSourcePtr getMouseEventSource();
}