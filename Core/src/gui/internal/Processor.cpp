
#include "Processor.h"

#include "event/EventBus.h"
#include "gui/Widget.h"
#include "gui/WidgetHelper.h"

namespace
{
	inline bool isInside(const glm::vec2 & pos, const core::Widget::BoundingBox & bbox)
	{
		return
			pos.x >= bbox.m_pos.x && pos.x < bbox.m_pos.x + bbox.m_size.x &&
			pos.y >= bbox.m_pos.y && pos.y < bbox.m_pos.y + bbox.m_size.y
			;
	}
}

// ...

core::Processor::Processor(Widget & widget, EventBus & bus)
	: m_widget(widget)
{
	m_mouseMove = bus.add<MouseMove>(0, [this](auto & event) { mouseMove(event, m_widget); });
	m_mousePress = bus.add<MousePress>(0, [this](auto & event) { mousePress(event, m_widget); });
	m_mouseRelease = bus.add<MouseRelease>(0, [this](auto & event) { mouseRelease(event, m_widget); });
}

void core::Processor::mouseMove(MouseMove & event, Widget & widget)
{
	widget.m_state.m_hovered = isInside(event.m_position, widget.m_bbox) && gui::isVisible(widget);
}
void core::Processor::mousePress(MousePress & event, Widget & widget)
{
	widget.m_state.m_selected = widget.m_state.m_hovered && event.consume();
}
void core::Processor::mouseRelease(MouseRelease & event, Widget & widget)
{
	if (widget.m_state.m_selected && isInside(event.m_position, widget.m_bbox) && event.consume())
	{
		if (widget.m_handler && !gui::isLocked(widget))
			widget.m_handler->action(widget);
	}
	widget.m_state.m_selected = false;
}
