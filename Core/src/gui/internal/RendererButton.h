#pragma once

#include "allegro/Sprite.h"
#include "asset/Asset.h"
#include "gui/internal/Renderer.h"

namespace core
{
	/**
		Takes responsibility for rendering a button to the screen. The button's state determines
		how the sprite will be rendered. If the button is selected, hovered or otherwise interacted
		with, it will reflect so on the screen.
	*/
	class RendererButton : public Renderer
	{
	public:
		RendererButton(const Asset<Sprite>::Reference & sprite) : m_sprite(sprite) {}

		virtual void render(const Widget & widget, const glm::vec2 & offset) const override final;

	private:
		Asset<Sprite>::Reference m_sprite;
	};
}