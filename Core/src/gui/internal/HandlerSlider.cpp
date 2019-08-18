
#include "HandlerSlider.h"

#include "event/EventBus.h"
#include "event/Events.h"
#include "gui/Widget.h"
#include "util/MathOperations.h"
#include "util/StringParsing.h"

#include <iomanip>
#include <sstream>

namespace
{
	float calculateFactor(const glm::vec2 & mouse, const glm::vec2 & pos, const glm::vec2 & size, bool horizontal)
	{
		const int axis = horizontal ? 0 : 1;
		const float f = util::max(0.0f, util::min(1.0f, (mouse[axis] - pos[axis]) / size[axis]));
		return horizontal ? f : 1.0f - f;
	}

	std::string toString(float value)
	{
		std::stringstream stream;
		stream << std::fixed << std::setprecision(1) << value;
		return stream.str();
	}
}

// ...

core::Handler::Callback core::HandlerSlider::incrementer(Widget & widget)
{
	return [this, &widget](Widget & other)
	{
		widget.m_value.m_float = util::min(m_data.m_max, widget.m_value.m_float + m_data.m_step);
		widget.m_value.m_float = util::round(widget.m_value.m_float, m_data.m_step);
		widget.m_value.m_string = toString(widget.m_value.m_float);

		m_callback(widget);
	};
}
core::Handler::Callback core::HandlerSlider::decrementer(Widget & widget)
{
	return [this, &widget](Widget & other)
	{
		widget.m_value.m_float = util::max(m_data.m_min, widget.m_value.m_float - m_data.m_step);
		widget.m_value.m_float = util::round(widget.m_value.m_float, m_data.m_step);
		widget.m_value.m_string = toString(widget.m_value.m_float);

		m_callback(widget);
	};
}

core::Handler::Callback core::HandlerSlider::slider(Widget & widget)
{
	return [this, &widget](Widget & other)
	{
		if (other.m_value.m_float <= 0.5f)
			widget.m_value.m_float = util::lerp(m_data.m_min, m_data.m_center, 2.0f * other.m_value.m_float);
		else
			widget.m_value.m_float = util::lerp(m_data.m_center, m_data.m_max, 2.0f * other.m_value.m_float - 1.0f);
		widget.m_value.m_float = util::round(widget.m_value.m_float, m_data.m_step);
		widget.m_value.m_string = toString(widget.m_value.m_float);

		m_callback(widget);
	};
}

core::Handler::Callback core::HandlerSlider::translator(Widget & widget)
{
	return [this, &widget](Widget & other)
	{
		widget.m_value.m_float = util::Parser<float>{}.parse(widget.m_value.m_string);

		m_callback(widget);
	};
}

// ...

core::HandlerSliderBar::HandlerSliderBar(const Callback & callback, EventBus & bus, bool horizontal)
	: m_callback(callback), m_horizontal(horizontal)
{
	m_listener = bus.add<MouseMove>([this](auto & event) { m_mousePosition = event.m_position; });
}

void core::HandlerSliderBar::process(Widget & widget)
{
	if (!widget.m_state.m_selected)
		return;

	widget.m_value.m_float = calculateFactor(m_mousePosition, widget.m_bbox.m_pos, widget.m_bbox.m_size, m_horizontal);
	m_callback(widget);
}
