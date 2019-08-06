
#include "HandlerSlider.h"

#include "event/EventBus.h"
#include "event/Events.h"
#include "gui/Widget.h"
#include "script/Script.h"
#include "util/MathOperations.h"

void core::HandlerSlider::action(Widget & widget)
{
	m_script.execute(m_code);
}

// ...

void core::HandlerSliderButton::action(Widget & widget)
{
	auto & value = widget.m_family.m_parent->m_value;
	auto & data = m_root.m_data;

	if (m_increment)
		value.m_float = util::min(data.m_max, value.m_float + data.m_step);
	else
		value.m_float = util::max(data.m_min, value.m_float - data.m_step);

	if (m_root.m_data.m_step > 0.0f)
		value.m_float = data.m_step * util::round(value.m_float / data.m_step);

	m_root.action(widget);
}

// ...

core::HandlerSliderBar::HandlerSliderBar(HandlerSlider & root, EventBus & bus)
	: m_root(root)
{
	m_listener = bus.add<MouseMove>([this](auto & event) { m_mousePosition = event.m_position; });
}

void core::HandlerSliderBar::process(Widget & widget)
{
	if (!widget.m_state.m_selected)
		return;
	auto & value = widget.m_family.m_parent->m_value;
	auto & data = m_root.m_data;

	const auto axis = 0;
	const auto factor = util::max(0.0f, util::min(1.0f,
		(m_mousePosition[axis] - widget.m_bbox.m_pos[axis]) / widget.m_bbox.m_size[axis]
	));

	if (factor <= 0.5f)
		value.m_float = util::lerp(data.m_min, data.m_center, 2.0f * factor);
	else
		value.m_float = util::lerp(data.m_center, data.m_max, 2.0f * factor - 1.0f);

	if (m_root.m_data.m_step > 0.0f)
		value.m_float = data.m_step * util::round(value.m_float / data.m_step);

	m_root.action(widget);
}
