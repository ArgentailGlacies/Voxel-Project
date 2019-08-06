#include "RendererButton.h"

#include "gui/Widget.h"

namespace
{
	/**
		Retrieves the frame for a normal sprite for the given widget. The sprite should be rendered
		with the specified frame to match the widget's state.

		@param widget The widget containing the state the frame should be retrieved from.
	*/
	std::string getSpriteFrame(const core::Widget & widget)
	{
		if (widget.m_value.m_bool)
		{
			if (widget.m_state.m_locked)
				return "active_locked";
			else if (widget.m_state.m_selected)
				return "active_clicked";
			else if (widget.m_state.m_hovered)
				return "active_hovered";
			else
				return "active";
		}
		else
		{
			if (widget.m_state.m_locked)
				return "inactive_locked";
			else if (widget.m_state.m_selected)
				return "inactive_clicked";
			else if (widget.m_state.m_hovered)
				return "inactive_hovered";
			else
				return "inactive";
		}
	}
}

// ...

void core::RendererButton::render(const Widget & widget, const glm::vec2 & offset) const
{
	if (!m_sprite.empty())
		m_sprite->draw(getSpriteFrame(widget), widget.m_bbox.m_pos + offset, widget.m_bbox.m_size);
}
