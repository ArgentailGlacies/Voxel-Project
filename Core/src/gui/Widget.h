#pragma once

#include <functional>
#include <glm/vec2.hpp>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace core
{
	class GuiData;

	/**
		All graphical user interface objects are represented as widgets. Unique functionality is
		appended during GUI loading, determined by the specified type of the widget. Widgets do not
		implement any logic on their own, and does nothing unless processors or listeners are added.
	*/
	struct Widget
	{
		Widget() = default;
		Widget(const Widget &) = delete;
		Widget(Widget &&) = delete;
		~Widget() = default;

		Widget & operator=(const Widget &) = delete;
		Widget & operator=(Widget &&) = delete;

		// ...

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
			Represents a relation between a collection of widgets. The grouping may be interpreted
			in different manners from one widget type to another.
		*/
		struct Relation
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
			Widget * m_target = nullptr;
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
			/* Whether the widget is marked as active. Does not propagate to children. */
			bool m_active = false;

			/* Whether the user is hovering the cursor over the widget. */
			bool m_hovered = false;
			/* Whether the user has selected the widget or not. */
			bool m_selected = false;
		};

		// ...

		std::string m_name;
		std::string m_type;

		BoundingBox m_bbox;
		Border m_border;
		Relation m_family;
		Relation m_group;
		Link m_link;
		State m_state;
	};

	/**
		A collection of widgets require all widgets to be uniquely identified by name.
	*/
	using Widgets = std::unordered_map<std::string, Widget>;
}
