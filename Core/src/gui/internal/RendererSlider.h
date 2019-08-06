#pragma once

#include "allegro/Sprite.h"
#include "asset/Asset.h"
#include "gui/internal/HandlerSlider.h"
#include "gui/Widget.h"

namespace core
{
	/**
		Renders a simple sprite at the position of the widget. The sprite will be scaled to fill
		the entire space covered by the widget's bounding box. The renderer will also render a
		bar indicating what the value of the slider is currently.
	*/
	class RendererSlider : public Renderer
	{
	public:
		RendererSlider(const HandlerSlider::Data & data, const Asset<Sprite>::Reference & sprite)
			: m_sprite(sprite), m_data(data)
		{}

		virtual void render(const Widget & widget, const glm::vec2 & offset) const override final;

	private:
		Asset<Sprite>::Reference m_sprite;
		HandlerSlider::Data m_data;
	};
}