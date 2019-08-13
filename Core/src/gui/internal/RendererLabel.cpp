
#include "RendererLabel.h"

#include "gui/Widget.h"

core::RendererLabel::RendererLabel(
	const AssetRegistry & assets, const Widget & widget, 
	const Text::Style & style, const std::string & pretext, const std::string & posttext
) : m_text(assets)
{
	if (!pretext.empty())
		m_text.addText(style, pretext);

	m_text.addValue(style, widget.m_value.m_string);

	if (!posttext.empty())
		m_text.addText(style, posttext);
}

void core::RendererLabel::render(const Widget & widget, const glm::vec2 & offset) const
{
	m_text.draw(widget.m_bbox.m_pos + offset, widget.m_bbox.m_size);
}
