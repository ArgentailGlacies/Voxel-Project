#pragma once

#include "gui/GuiData.h"
#include "gui/Widget.h"

#include <glm/vec2.hpp>
#include <memory>

namespace core
{
	/**
		Creates a simple widget with a specified position and size. Does not assign the minimum size
		of the widget.
	*/
	inline auto mockWidget(const glm::vec2 & pos, const glm::vec2 & size)
	{
		Widget widget;
		widget.m_bbox.m_pos = pos;
		widget.m_bbox.m_size = size;
		return widget;
	}
	/**
		Creates a simple widget with a specified position and size. Does not assign the minimum size
		of the widget. The widget will be attached to the parent as a child of the parent.
	*/
	inline auto & mockWidget(Widget & parent, const glm::vec2 & pos, const glm::vec2 & size)
	{
		auto & ptr = parent.m_family.m_children.emplace_back(std::make_unique<Widget>());
		ptr->m_family.m_parent = &parent;
		ptr->m_bbox.m_pos = pos;
		ptr->m_bbox.m_size = size;
		return *ptr;
	}
	/**
		Creates a simple widget with a specified position and size. Does not assign the minimum size
		of the widget. The widget will be attached to the target as a link.
	*/
	inline auto mockWidget(const Widget & target, const glm::vec2 & ratio, const glm::vec2 & pos, const glm::vec2 & size)
	{
		auto widget = mockWidget(pos, size);
		widget.m_link.m_target = &target;
		widget.m_link.m_ratio = ratio;
		return widget;
	}
}