
#include "Mouse.h"

#include "ui/Display.h"

#include <allegro5/mouse.h>

void core::setMousePosition(const Display & display, const glm::ivec2 & pos)
{
	al_set_mouse_xy(display.handle(), pos.x, pos.y);
}
