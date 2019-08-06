
#include "gui/WidgetAction.h"

#include "gui/Widget.h"
#include "script/Script.h"
#include "util/MathOperations.h"

void core::WidgetActionSliderBar::operator()(Widget & widget) const
{
	m_script.execute(m_code);
}
void core::WidgetActionSliderButton::operator()(Widget & widget) const
{
	// Calculate the slider value
	if (m_increment)
		m_widget.m_value.m_float += m_data.m_step;
	else
		m_widget.m_value.m_float -= m_data.m_step;

	// Apply step size calculations
	if (m_data.m_step > 0.0f)
		m_widget.m_value.m_float = m_data.m_step * util::round(m_widget.m_value.m_float / m_data.m_step);

	m_script.execute(m_code);
}
