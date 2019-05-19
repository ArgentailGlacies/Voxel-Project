
#include "WidgetProcessor.h"

#include "util/MathOperations.h"

void core::gui::updateChildren(Widget & widget)
{
	for (auto & child : widget.m_family.m_children)
	{
		for (const auto & processor : child->m_processors)
			processor(*child);
	}
}

void core::gui::updatePosition(Widget & widget)
{
	// Positions outside of link targets
	if (const auto * target = widget.m_link.m_target)
	{
		const auto & size = widget.m_bbox.m_size;
		const auto border = util::max(widget.m_border.m_outer, target->m_border.m_outer);
		const auto offset = widget.m_link.m_ratio * (target->m_bbox.m_size + 2.0f * border + size) - border - size;
		widget.m_bbox.m_pos = offset + target->m_bbox.m_pos;
	}
	// Positions inside of parents
	else if (const auto * target = widget.m_family.m_parent)
	{
		const auto border = util::max(widget.m_border.m_outer, target->m_border.m_inner);
		const auto offset = widget.m_link.m_ratio * (target->m_bbox.m_size - 2.0f * border) + border;
		widget.m_bbox.m_pos = offset + target->m_bbox.m_pos;
	}
}
void core::gui::updateSize(Widget & widget)
{
	auto min = widget.m_bbox.m_minSize;
	for (const auto & child : widget.m_family.m_children)
	{
		// Children are inside parents
		const auto border = util::max(widget.m_border.m_inner, child->m_border.m_outer);
		const auto offset = child->m_bbox.m_pos - widget.m_bbox.m_pos;
		min = util::max(min, offset + child->m_bbox.m_size + border);
	}
	widget.m_bbox.m_size = min;
}
