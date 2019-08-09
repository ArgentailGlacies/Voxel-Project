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
		RendererTextTest(const AssetRegistry & assets) : m_text(assets)
		{
			m_text.add({}, "Hello world! We are attempting to draw text!");
		}

		inline virtual void render(const Widget & widget, const glm::vec2 & offset) const override final
		{
			m_text.draw(offset, glm::vec2{ 100, 1000 });
		}

	private:
		Text m_text;
	};
}