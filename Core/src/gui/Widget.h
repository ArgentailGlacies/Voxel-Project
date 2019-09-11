#pragma once

#include "event/EventListener.h"
#include "gui/internal/Handler.h"
#include "gui/internal/Renderer.h"
#include "gui/internal/Processor.h"

#include <functional>
#include <glm/vec2.hpp>
#include <memory>
#include <string>
#include <vector>

namespace core
{
	/**
		All graphical user interface objects are represented as widgets. Unique functionality is
		appended during GUI loading, determined by the specified type of the widget. Widgets do not
		implement any logic on their own, and does nothing unless processors or listeners are added.
	*/
	struct Widget
	{
		/**
			The border determines how far away at minimum another widget must be to the current
			widget. If two widgets are connected, the actual border will be the greatest border
			value along the edge which they share.
		*/
		struct Border
		{
			float m_inner = 0.0f;
			float m_outer = 0.0f;
		};

		/**
			All widgets take up a specific amount of space on the screen. Their positions and sizes
			will automatically be updated to match whatever position they are linked to, based on
			where their parents and link targets are.
		*/
		struct BoundingBox
		{
			glm::vec2 m_pos = {};
			glm::vec2 m_size = {};
			glm::vec2 m_minSize = {};
		};

		/**
			One widget may own multiple child widgets, where each child widget is a normal widget in
			their own regards.
		*/
		struct Family
		{
			Widget * m_parent = nullptr;
			std::vector<std::unique_ptr<Widget>> m_children;
		};

		/**
			Represents a relation between a collection of widgets. The grouping may be interpreted
			in different manners from one widget type to another.
		*/
		struct Group
		{
			Widget * m_leader = nullptr;
			std::vector<Widget *> m_members;
		};

		/**
			Two widgets may be connected if they are in the same family (either parent-child, or
			sibling-sibling). One widget will then connect to the other at the specified location.
			If the connection is parent-child, the child will be connected inside the parent,
			otherwise it will be connected on the outside.
		*/
		struct Link
		{
			const Widget * m_target = nullptr;
			glm::vec2 m_ratio = {};
		};

		/**
			Users interacting with widgets may cause the widget to be highlighted or otherwise
			marked, but not yet committed to an action.
		*/
		struct State
		{
			/* Whether the widget is visible to the user. Hides children if false. */
			bool m_visible = true;
			/* Whether the widget is locked for user interaction. Locks children if true. */
			bool m_locked = false;

			/* Whether the user is hovering the cursor over the widget. */
			bool m_hovered = false;
			/* Whether the user has selected the widget or not. */
			bool m_selected = false;
		};

		/**
			When a widget is activated by the user, it may change its internal value to reflect that
			user input. A checkbox will toggle its boolean value, a slider its float value, a
			textbox its string value, etc.
		*/
		struct Value
		{
			/* The boolean value of the widget */
			bool m_bool = false;
			/* The floating point value of the widget */
			float m_float = 0.0f;
			/* The textual value of the widget */
			std::string m_string = "";
		};

		// ...

		std::string m_name = "root";

		std::unique_ptr<Handler> m_handler;
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<Processor> m_processor;

		BoundingBox m_bbox;
		Border m_border;
		Family m_family;
		Group m_group = { this, { this } };
		Link m_link;
		State m_state;
		Value m_value;
	};
}
