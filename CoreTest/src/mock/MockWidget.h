#pragma once

#include "gui/Widget.h"

#include <glm/vec2.hpp>
#include <memory>

namespace core
{
	/**
		Creates a simple widget with a specified position and size. Does not assign the minimum size
		of the widget.
	*/
	inline auto mockWidget(const glm::vec2& pos, const glm::vec2& size)
	{
		auto widget = std::make_unique<Widget>();
		widget->m_bbox.m_pos = pos;
		widget->m_bbox.m_size = size;
		return widget;
	}
	/**
		Creates a simple widget with a specified position and size. Does not assign the minimum size
		of the widget. The widget will be attached to the parent as a child of the parent.
	*/
	inline auto mockWidget(Widget& parent, const glm::vec2& pos, const glm::vec2& size)
	{
		auto widget = mockWidget(pos, size);
		parent.m_family.m_members.emplace(widget.get());
		widget->m_family.m_leader = &parent;
		return widget;
	}
	/**
		Creates a simple widget with a specified position and size. Does not assign the minimum size
		of the widget. The widget will be attached to the target as a link.
	*/
	inline auto mockWidget(Widget& target, const glm::vec2& ratio, const glm::vec2& pos, const glm::vec2& size)
	{
		auto widget = mockWidget(pos, size);
		widget->m_link.m_target = &target;
		widget->m_link.m_ratio = ratio;
		return widget;
	}
}