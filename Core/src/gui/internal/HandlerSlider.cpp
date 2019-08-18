
#include "HandlerSlider.h"

#include "event/EventBus.h"
#include "event/Events.h"
#include "gui/Widget.h"
#include "script/Script.h"
#include "util/MathOperations.h"

namespace
{
	float calculateFactor(const glm::vec2 & mouse, const glm::vec2 & pos, const glm::vec2 & size, bool horizontal)
	{
		const int axis = horizontal ? 0 : 1;
		const float f = util::max(0.0f, util::min(1.0f, (mouse[axis] - pos[axis]) / size[axis]));
		return horizontal ? f : 1.0f - f;
	}
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

	m_root.m_callback();
}

// ...

core::HandlerSliderBar::HandlerSliderBar(HandlerSlider & root, EventBus & bus, bool horizontal)
	: m_root(root), m_horizontal(horizontal)
{
	m_listener = bus.add<MouseMove>([this](auto & event) { m_mousePosition = event.m_position; });
}

void core::HandlerSliderBar::process(Widget & widget)
{
	if (!widget.m_state.m_selected)
		return;
	auto & value = widget.m_family.m_parent->m_value;
	auto & data = m_root.m_data;

	const auto factor = calculateFactor(m_mousePosition, widget.m_bbox.m_pos, widget.m_bbox.m_size, m_horizontal);

	if (factor <= 0.5f)
		value.m_float = util::lerp(data.m_min, data.m_center, 2.0f * factor);
	else
		value.m_float = util::lerp(data.m_center, data.m_max, 2.0f * factor - 1.0f);

	if (m_root.m_data.m_step > 0.0f)
		value.m_float = data.m_step * util::round(value.m_float / data.m_step);

	m_root.m_callback();
}
