#pragma once

#include "ui/Keyboard.h"
#include "ui/Mouse.h"

#include <glm/vec2.hpp>

namespace core
{
	/**
		Dispatched when the user closes the display. Usually this happens when the user clicks the
		close button on the window, or uses a keyboard shortcut to close the window.
	*/
	struct DisplayClose {};
	/**
		Dispatched when the window changes size. Usually this happens when the user alters the size
		of the window.
	*/
	struct DisplayResize
	{
		/* The new size of the display. */
		const glm::ivec2 m_size;
	};
	/**
		Dispatched when the user focuses the window. Usually this happens when the user switches
		from another window to this window.
	*/
	struct DisplaySwitchIn {};
	/**
		Dispatched when the user focuses another window. Usually this happens when the user switches
		from this window to another window.
	*/
	struct DisplaySwitchOut {};

	// ...

	// TODO: Joystick events

	// ...

	/**
		Dispatched when the user pressed a key on the keyboard. See 'allegro5/keycodes.h'.
	*/
	struct KeyPress
	{
		/* The key pressed by the user. */
		const KeyboardKey m_key;
		/* The key modifiers currently active (ie. shift, control, etc). */
		const unsigned int m_modifiers;
	};
	/**
		Dispatched when the user released a key on the keyboard. See 'allegro5/keycodes.h'.
	*/
	struct KeyRelease
	{
		/* The key released by the user. */
		const KeyboardKey m_key;
		/* The key modifiers currently active (ie. shift, control, etc). */
		const unsigned int m_modifiers;
	};
	/**
		Dispatched when the user inputs a character. Usually this happens when the user press a key
		on their keyboard.
	*/
	struct KeyUnichar
	{
		/* The character entered by the user. */
		const int m_codepoint;
	};

	// ...

	/**
		Dispatched when the cursor enters the current window. Usually this happens when the user
		moves the cursour around.
	*/
	struct MouseEnterDisplay {};
	/**
		Dispatched when the cursor leaves the current window. Usually this happens when the user
		moves the cursour around.
	*/
	struct MouseLeaveDisplay {};
	/**
		Dispatched when a mouse button is pressed. Usually this happens when the user clicks
		somewhere within the current window.
	*/
	struct MousePress
	{
		/* The mouse button which was clicked */
		const MouseButton m_button;
		/* Where in the window the cursor was clicked */
		const glm::vec2 m_position;
		/* The current scroll position on the mouse when the button was clicked */
		const glm::vec2 m_scroll;
		/* How hard the user clicked the button (range [0, 1]) */
		const float m_pressure;
	};
	/**
		Dispatched when a mouse button is released. Usually this happens after the user clicks
		somewhere within the current window.
	*/
	struct MouseRelease
	{
		/* The mouse button which was released */
		const MouseButton m_button;
		/* Where in the window the cursor was released */
		const glm::vec2 m_position;
		/* The current scroll position on the mouse when the button was released */
		const glm::vec2 m_scroll;
		/* How hard the user clicked the button (range [0, 1]) */
		const float m_pressure;
	};
	/**
		Dispatched when the cursor moves. Usually this happens when the user physically moves their
		computer mouse.
	*/
	struct MouseMove
	{
		/* The new position of the cursor in the window */
		const glm::vec2 m_position;
		/* The change in position of the cursor in the window */
		const glm::vec2 m_positionDelta;
		/* The new scroll position of the cursor in the window */
		const glm::vec2 m_scroll;
		/* The change in scroll position of the cursor in the window */
		const glm::vec2 m_scrollDelta;
	};
}