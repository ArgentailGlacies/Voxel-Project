#pragma once

#include "allegro/Sprite.h"
#include "asset/Asset.h"
#include "gui/internal/Renderer.h"

namespace core
{
	/**
		Takes responsibility for rendering a background to the screen. Appears in the background,
		such that child widgets can render on top of the background, logically grouping widgets on
		the screen.
	*/
	class RendererPanel : public Renderer
	{
	public:
		RendererPanel(const Asset<Sprite>::Reference & sprite) : m_sprite(sprite) {}

		virtual void render(const Widget & widget, const glm::vec2 & offset) const override final;

	private:
		Asset<Sprite>::Reference m_sprite;
	};
}