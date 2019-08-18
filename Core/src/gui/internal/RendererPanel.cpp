
#include "RendererPanel.h"

#include "gui/Widget.h"
#include "util/MathOperations.h"


void core::RendererPanel::render(const Widget & widget, const glm::vec2 & offset) const
{
	if (m_sprite.empty())
		return;

	// Calculate segment locations and sizes
	const auto cs = m_sprite->getSize("top_left");
	const auto es = util::max(glm::vec2{}, widget.m_bbox.m_size - 2.0f * cs);

	// Render segments
	m_sprite->draw("top_left", widget.m_bbox.m_pos, cs);
	m_sprite->draw("top", widget.m_bbox.m_pos + glm::vec2{ cs.x, 0.0f }, glm::vec2{ es.x, cs.y });
	m_sprite->draw("top_right", widget.m_bbox.m_pos + glm::vec2{ cs.x + es.x, 0.0f }, cs);
	m_sprite->draw("middle_left", widget.m_bbox.m_pos + glm::vec2{ 0.0f, cs.y }, glm::vec2{ cs.x, es.y });
	m_sprite->draw("middle", widget.m_bbox.m_pos + cs, es);
	m_sprite->draw("middle_right", widget.m_bbox.m_pos + cs + glm::vec2{ es.x, 0.0f }, glm::vec2{ cs.x, es.y });
	m_sprite->draw("bottom_left", widget.m_bbox.m_pos + glm::vec2{ 0.0f, cs.y + es.y }, cs);
	m_sprite->draw("bottom", widget.m_bbox.m_pos + cs + glm::vec2{ 0.0f, es.y }, glm::vec2{ es.x, cs.y });
	m_sprite->draw("bottom_right", widget.m_bbox.m_pos + cs + es, cs);
}
