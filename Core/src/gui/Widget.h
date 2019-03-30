#pragma once

#include <functional>
#include <glm/vec2.hpp>
#include <string>
#include <vector>
#include <unordered_set>

namespace core
{
	class GuiData;

	/**
		All graphical user interface objects are represented as a widget. Unique functionality is
		appended during GUI loading, determined by the specified type of the widget.
	*/
	struct Widget
	{
		Widget() = delete;
		Widget(GuiData & data) : m_data(data) {}
		Widget(const Widget &) = delete;
		Widget(Widget &&) = delete;
		~Widget() = default;

		Widget & operator=(const Widget &) = delete;
		Widget & operator=(Widget &&) = delete;

		// ...

		/**
			All widgets are processed by one or more processors. Each processor perform one specific
			operation on the widget, writing data to it if necessary.
		*/
		using Processor = std::function<void(Widget &)>;
		/**
			Widgets may be rendered by one or more renderers. Each renderer may render different
			parts of the widget. The rendering order is the same order as the order the renderers
			were added, meaning the first renderer renders behind the last added renderer.
		*/
		using Renderer = std::function<void(Widget &, const glm::vec2 & offset)>;

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
		struct UserState
		{
			/* Whether the widget is visible to the user. Hides children if false. */
			bool m_visible = true;
			/* Whether the widget is locked for user interaction. Locks children if true. */
			bool m_locked = false;
			/* Whether a user is hovering the cursor over the widget. */
			bool m_hovered = false;
			/* Whether the user has clicked (but not released) the cursor on the widget. */
			bool m_selected = false;
		};

		/**
			Represents a relation between two or more widgets.
		*/
		struct Relation
		{
			Widget * m_leader = nullptr;
			std::unordered_set<Widget *> m_members;
		};

		// ...

		std::string m_type;
		std::vector<Processor> m_processors;
		std::vector<Renderer> m_renderers;
		GuiData & m_data;

		BoundingBox m_bbox;
		Border m_border;
		Relation m_family;
		Relation m_group;
		Link m_link;
		UserState m_state;
	};
}
