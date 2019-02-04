#pragma once

#include <glm/vec2.hpp>
#include <string>
#include <unordered_set>

namespace core
{
	/**
		All graphical user interface objects are represented as a widget. Unique functionality is
		appended during GUI loading, determined by the specified type of the widget.
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
			float m_left = 0.0f;
			float m_right = 0.0f;
			float m_top = 0.0f;
			float m_bottom = 0.0f;
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
			Represents a relation between two or more widgets.
		*/
		struct Relation
		{
			Widget * m_leader = nullptr;
			std::unordered_set<Widget *> m_members;
		};

		// ...

		BoundingBox m_bbox;
		Border m_inner;
		Border m_outer;
		Link m_link;
		Relation m_family;
		Relation m_group;
	};
}