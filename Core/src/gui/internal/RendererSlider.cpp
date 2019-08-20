
#include "RendererSlider.h"

#include "gui/internal/HandlerSlider.h"
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

	/**
		Calculates the position of the slider bar from 0.0f to 1.0f, where 0.0f is at the lower end
		of the slider, and 1.0f is at the upper end of the slider.
	*/
	float calculateRatio(float min, float center, float max, float value, bool horizontal)
	{
		const auto f = (value < center ? (value - min) / (center - min) : 1.0f + (value - center) / (max - center));
		return 0.5f * (horizontal ? f : 2.0f - f);
	}
	/**
		Retrieves a vector which is constructed from the given x and y coordinate. If the vector is
		horizontal, the vector's x-axis will be the x-component and the vector's y-axis will be the
		y-component, and vice versa if horizontal is false.
	*/
	glm::vec2 buildVec2(float x, float y, bool horizontal)
	{
		return horizontal ? glm::vec2{ x, y } : glm::vec2{ y, x };
	}
}

// ...

void core::RendererSlider::render(const Widget & widget, const glm::vec2 & offset) const
{
	if (m_sprite.empty())
		return;
	auto & value = widget.m_family.m_parent->m_value;

	const float ratio = calculateRatio(m_data.m_min, m_data.m_center, m_data.m_max, value.m_float, m_horizontal);

	// Discover which frames to use when rendering sprite
	const auto leftFrame = "left_" + getSpriteFrame(widget);
	const auto middleFrame = "middle_" + getSpriteFrame(widget);
	const auto rightFrame = "right_" + getSpriteFrame(widget);
	const auto barFrame = "bar_" + getSpriteFrame(widget);

	// Calculate segment locations and sizes
	const auto min = m_horizontal ? widget.m_bbox.m_size.y : widget.m_bbox.m_size.x;
	const auto max = m_horizontal ? widget.m_bbox.m_size.x : widget.m_bbox.m_size.y;

	const auto edgeSize = glm::vec2{ min, min };
	const auto edgePos = buildVec2(max - min, 0.0f, m_horizontal);

	const auto centerSize = buildVec2(util::max(0.0f, max - 2.0f * min), min, m_horizontal);
	const auto centerPos = buildVec2(min, 0.0f, m_horizontal);

	const auto barSize = buildVec2(0.25f * min, min, m_horizontal);
	const auto barPos = buildVec2(ratio * (max - (m_horizontal ? barSize.x : barSize.y)), 0.0f, m_horizontal);

	// Render segments
	m_sprite->draw(leftFrame, widget.m_bbox.m_pos, edgeSize);
	m_sprite->draw(middleFrame, widget.m_bbox.m_pos + centerPos, centerSize);
	m_sprite->draw(rightFrame, widget.m_bbox.m_pos + edgePos, edgeSize);
	m_sprite->draw(barFrame, widget.m_bbox.m_pos + barPos, barSize);
}
