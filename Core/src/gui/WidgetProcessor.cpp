
#include "WidgetProcessor.h"

#include "event/EventBus.h"
#include "event/Events.h"
#include "gui/Widget.h"
#include "util/MathOperations.h"

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

// ...

core::WidgetProcessorSlider::WidgetProcessorSlider(EventBus & bus, const gui::SliderData & data, bool horizontal) noexcept
	: m_data(data), m_horizontal(horizontal)
{
	m_mouseMove = bus.add<MouseMove>([this](auto & event) { m_mousePos = event.m_position; });
}

void core::WidgetProcessorSlider::process(Widget & widget) const
{
	if (!widget.m_state.m_selected)
		return;

	const auto axis = m_horizontal ? 0 : 1;
	const auto factor = util::max(0.0f, util::min(1.0f,
		(m_mousePos[axis] - widget.m_bbox.m_pos[axis]) / widget.m_bbox.m_size[axis]
	));

	// Calculate the slider value
	if (factor <= 0.5f)
		widget.m_value.m_float = util::lerp(m_data.m_min, m_data.m_center, 2.0f * factor);
	else
		widget.m_value.m_float = util::lerp(m_data.m_center, m_data.m_max, 2.0f * factor - 1.0f);

	// Apply step size calculations
	if (m_data.m_step > 0.0f)
		widget.m_value.m_float = m_data.m_step * util::round(widget.m_value.m_float / m_data.m_step);
}
