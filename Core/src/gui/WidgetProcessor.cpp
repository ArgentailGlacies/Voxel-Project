
#include "WidgetProcessor.h"

#include "gui/Widget.h"
#include "util/MathOperations.h"

namespace
{
	/**
		Calculates the positional offset for a widget with a given size, when attempting to link to
		a target widget with a given size. The position depends upon the positional ratio, and
		whether the widget is linked inside or outside the target widget.

		@param border The border which describes the relationship between the widgets.
		@param widgetSize The size of the widget being processed.
		@param targetSize The size of the target the current widget is placed against.
		@param ratio The ratio descripting how the widget should be positioned against its target.
		@param deambiguator The ratio along the second axis, used to determined whether the widget
		                    widget should be inside or outside the target widget.
	*/
	float calculateOffset(const core::Widget::Border border, float widgetSize, float targetSize, float ratio, float deambiguater)
	{
		if (ratio < 0.0f)
			return -border.m_outer - widgetSize;
		if (ratio > 1.0f)
			return border.m_outer + targetSize;
		if (deambiguater >= 0.0f && deambiguater <= 1.0f)
			return util::lerp(border.m_inner, targetSize - widgetSize - border.m_inner, ratio);
		return util::lerp(0.0f, targetSize - widgetSize, ratio);
	}
}

void core::gui::updatePosition(Widget & widget)
{
	// If the link has not been assigned, default to the parent
	const auto * target = widget.m_link.m_target ? widget.m_link.m_target : widget.m_family.m_parent;
	if (target == nullptr)
		return;

	const auto & ratio = widget.m_link.m_ratio;
	const auto & widgetSize = widget.m_bbox.m_size;
	const auto & targetSize = target->m_bbox.m_size;

	Widget::Border border;
	border.m_inner = util::max(widget.m_border.m_outer, target->m_border.m_inner);
	border.m_outer = util::max(widget.m_border.m_outer, target->m_border.m_outer);

	glm::vec2 offset;
	offset.x = calculateOffset(border, widgetSize.x, targetSize.x, ratio.x, ratio.y);
	offset.y = calculateOffset(border, widgetSize.y, targetSize.y, ratio.y, ratio.x);
	widget.m_bbox.m_pos = target->m_bbox.m_pos + offset;
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
