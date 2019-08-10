#pragma once

#include "allegro/Text.h"
#include "gui/Widget.h"

namespace core
{
	/**
		Renders a simple sprite at the position of the widget. The sprite will be scaled to fill
		the entire space covered by the widget's bounding box. The renderer will also render a
		bar indicating what the value of the slider is currently.
	*/
	class RendererTextTest : public Renderer
	{
	public:
		RendererTextTest(const AssetRegistry & assets, const Widget & widget) : m_text(assets)
		{
			Text::Style style;
			style.m_font = "overlock";

			m_text.addValue(style, widget.m_value.m_float);

			style.m_size = 1.6f;
			m_text.addText(style, "Hello world! ");

			style.m_size = 1.0f;
			style.m_italic = true;
			m_text.addText(style, "We are ");

			style.m_size = 1.6;
			style.m_bold = true;
			style.m_color = { 1.0f, 0.0f, 0.0f, 1.0f };
			m_text.addText(style, "attempting ");

			style.m_italic = false;
			style.m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
			m_text.addText(style, "to draw text!");
		}

		inline virtual void render(const Widget & widget, const glm::vec2 & offset) const override final
		{
			m_text.draw(widget.m_bbox.m_pos + offset + glm::vec2{ 100.0f, 100.0f }, glm::vec2{ 500, 1000 });
		}

	private:
		Text m_text;
	};
}