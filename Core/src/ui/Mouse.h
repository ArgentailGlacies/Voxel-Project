#pragma once

#include <glm/vec2.hpp>

namespace core
{
	class Display;

	/**
		All possible moust button states are found here.
	*/
	enum class MouseButton
	{
		LEFT,
		MIDDLE,
		NONE,
		RIGHT,
	};

	// ...

	/**
		Sets the mouse position on the specified display, to the specified position.

		@param display The display in which the mouse should be moved to.
		@param pos The position the mouse should have in the provided display.
	*/
	void setMousePosition(const Display & display, const glm::ivec2 & pos);
}