
#include "WidgetListener.h"

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

void core::gui::mouseMove(MouseMove & event, Widget & widget)
{
	widget.m_state.m_hovered = isInside(event.m_position, widget.m_bbox);
	if (const auto * parent = widget.m_family.m_parent)
		widget.m_state.m_hovered &= parent->m_state.m_hovered;
}
void core::gui::mousePress(MousePress & event, Widget & widget)
{
	widget.m_state.m_selected = widget.m_state.m_hovered && event.consume();
}
void core::gui::mouseRelease(MouseRelease & event, Widget & widget)
{
	if (widget.m_state.m_selected && isInside(event.m_position, widget.m_bbox) && event.consume())
	{
		if (widget.m_handler)
			widget.m_handler->action(widget);
	}
	widget.m_state.m_selected = false;
}
