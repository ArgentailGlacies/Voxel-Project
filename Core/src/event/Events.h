#pragma once

#include "ui/Keyboard.h"
#include "ui/Mouse.h"

#include <glm/vec2.hpp>

namespace core
{
	class Consumable
	{
	public:
		/**
			Marks the event as consumed. An event may only be consumed exactly once. If the event
			has not yet been consumed, true is returned.

			@return True iff the event has not yet been consumed.
		*/
		inline bool consume() { const auto state = m_consumed; m_consumed = true; return !state; }
		/**
			Checks whether the event has been consumed or not.

			@return True iff the event has not yet been consumed.
		*/
		inline bool isConsumed() const { return m_consumed; }

	private:
		bool m_consumed = false;
	};

	// ...

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
		// The new size of the display.
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
	struct KeyPress : public Consumable
	{
		KeyPress(KeyboardKey key, unsigned int modifiers) : m_key(key), m_modifiers(modifiers) {}

		// The key pressed by the user.
		const KeyboardKey m_key;
		// The key modifiers currently active (ie. shift, control, etc).
		const unsigned int m_modifiers;
	};
	/**
		Dispatched when the user released a key on the keyboard. See 'allegro5/keycodes.h'.
	*/
	struct KeyRelease : public Consumable
	{
		KeyRelease(KeyboardKey key, unsigned int modifiers) : m_key(key), m_modifiers(modifiers) {}

		// The key released by the user.
		const KeyboardKey m_key;
		// The key modifiers currently active (ie. shift, control, etc).
		const unsigned int m_modifiers;
	};
	/**
		Dispatched when the user inputs a character. Usually this happens when the user press a key
		on their keyboard.
	*/
	struct KeyUnichar : public Consumable
	{
		KeyUnichar(int codepoint) : m_codepoint(codepoint) {}

		// The character entered by the user.
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
	struct MousePress : public Consumable
	{
		MousePress(MouseButton button, const glm::vec2 & position, const glm::vec2 & scroll, float pressure)
			: m_button(button), m_position(position), m_scroll(scroll), m_pressure(pressure) {}

		// The mouse button which was clicked.
		const MouseButton m_button;
		// Where in the window the cursor was clicked.
		const glm::vec2 m_position;
		// The current scroll position on the mouse when the button was clicked.
		const glm::vec2 m_scroll;
		// How hard the user clicked the button (range [0, 1]).
		const float m_pressure;
	};
	/**
		Dispatched when a mouse button is released. Usually this happens after the user clicks
		somewhere within the current window.
	*/
	struct MouseRelease : public Consumable
	{
		MouseRelease(MouseButton button, const glm::vec2 & position, const glm::vec2 & scroll, float pressure)
			: m_button(button), m_position(position), m_scroll(scroll), m_pressure(pressure) {}

		// The mouse button which was released.
		const MouseButton m_button;
		// Where in the window the cursor was released.
		const glm::vec2 m_position;
		// The current scroll position on the mouse when the button was released.
		const glm::vec2 m_scroll;
		// How hard the user clicked the button (range [0, 1]).
		const float m_pressure;
	};
	/**
		Dispatched when the cursor moves. Usually this happens when the user physically moves their
		computer mouse.
	*/
	struct MouseMove : public Consumable
	{
		MouseMove(const glm::vec2 & position, const glm::vec2 & positionDelta, const glm::vec2 & scroll, const glm::vec2 & scrollDelta)
			: m_position(position), m_positionDelta(positionDelta), m_scroll(scroll), m_scrollDelta(scrollDelta) {}

		// The new position of the cursor in the window.
		const glm::vec2 m_position;
		// The change in position of the cursor in the window.
		const glm::vec2 m_positionDelta;
		// The new scroll position of the cursor in the window.
		const glm::vec2 m_scroll;
		// The change in scroll position of the cursor in the window.
		const glm::vec2 m_scrollDelta;
	};
}