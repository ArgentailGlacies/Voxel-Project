
#include "EventSource.h"

#include "ui/Display.h"

#include <allegro5/allegro.h>

core::EventSourcePtr core::getDisplayEventSource(const Display & display)
{
	return gsl::make_not_null(al_get_display_event_source(display.handle()));
}
core::EventSourcePtr core::getKeyboardEventSource()
{
	return gsl::make_not_null(al_get_keyboard_event_source());
}
core::EventSourcePtr core::getMouseEventSource()
{
	return gsl::make_not_null(al_get_mouse_event_source());
}
