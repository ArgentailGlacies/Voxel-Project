
#include "WidgetRenderer.h"

#include "util/MathOperations.h"

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

void core::gui::WidgetRendererButton::render(const Widget & widget, const glm::vec2 & offset) const
{
	if (!m_sprite.empty())
		m_sprite->draw(getSpriteFrame(widget), widget.m_bbox.m_pos + offset, widget.m_bbox.m_size);
}

void core::gui::WidgetRendererSlider::render(const Widget & widget, const glm::vec2 & offset) const
{
	if (m_sprite.empty())
		return;

	const float ratio = widget.m_value.m_float < m_data.m_center
		? 0.5f * (widget.m_value.m_float - m_data.m_min) / (m_data.m_center - m_data.m_min)
		: 0.5f * (widget.m_value.m_float - m_data.m_center) / (m_data.m_max - m_data.m_center) + 0.5f;

	// Discover which frames to use when rendering sprite
	const auto leftFrame = "left_" + getSpriteFrame(widget);
	const auto middleFrame = "middle_" + getSpriteFrame(widget);
	const auto rightFrame = "right_" + getSpriteFrame(widget);
	const auto barFrame = "bar_" + getSpriteFrame(widget);

	// Calculate segment locations and sizes
	const auto min = util::min(widget.m_bbox.m_size.x, widget.m_bbox.m_size.y);
	const auto max = util::max(widget.m_bbox.m_size.x, widget.m_bbox.m_size.y);

	const glm::vec2 edge = { min, min };
	const glm::vec2 center = { max - 2.0f * min, min };
	const glm::vec2 bar = { 0.25f * min, min };

	const glm::vec2 centerPos = { min, 0.0f };
	const glm::vec2 edgePos = { max - min, 0.0f };
	const glm::vec2 barPos = { ratio * (max - bar.x), 0.0f };

	// Render segments
	m_sprite->draw(leftFrame, widget.m_bbox.m_pos, edge);
	m_sprite->draw(middleFrame, widget.m_bbox.m_pos + centerPos, center);
	m_sprite->draw(rightFrame, widget.m_bbox.m_pos + edgePos, edge);
	m_sprite->draw(barFrame, widget.m_bbox.m_pos + barPos, bar);
}
