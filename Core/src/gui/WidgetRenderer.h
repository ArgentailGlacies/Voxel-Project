#pragma once

#include "allegro/Sprite.h"
#include "asset/Asset.h"
#include "gui/GuiData.h"
#include "gui/Widget.h"

namespace core
{
	namespace gui
	{
		/**
			Renders a simple sprite at the position of the widget. The sprite will be scaled to fill
			the entire space covered by the widget's bounding box.
		*/
		class WidgetRendererButton
		{
		public:
			WidgetRendererButton(Asset<Sprite>::Reference sprite) : m_sprite(sprite) {}

			inline void operator()(const Widget & widget, const glm::vec2 & offset) const { render(widget, offset); }

		private:
			/**
				Renders the sprite at the widget's position, taking into account the offset.

				@param widget The widget to render.
				@param offset The offset the sprite should be rendered at.
			*/
			void render(const Widget & widget, const glm::vec2 & offset) const;

			Asset<Sprite>::Reference m_sprite;
		};

		/**
			Renders a simple sprite at the position of the widget. The sprite will be scaled to fill
			the entire space covered by the widget's bounding box. The renderer will also render a
			bar indicating what the value of the slider is currently.
		*/
		class WidgetRendererSlider
		{
		public:
			WidgetRendererSlider(Asset<Sprite>::Reference sprite, const gui::SliderData & data)
				: m_sprite(sprite), m_data(data)
			{}

			inline void operator()(const Widget & widget, const glm::vec2 & offset) const { render(widget, offset); }

		private:
			/**
				Renders the sprite at the widget's position, taking into account the offset.

				@param widget The widget to render.
				@param offset The offset the sprite should be rendered at.
			*/
			void render(const Widget & widget, const glm::vec2 & offset) const;

			Asset<Sprite>::Reference m_sprite;
			gui::SliderData m_data;
		};
	}
}