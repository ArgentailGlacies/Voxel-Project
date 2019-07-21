#pragma once

#include "gui/GuiData.h"
#include "gui/Widget.h"

namespace core
{
	class Gui
	{
	public:
		/**
			Renders all underlying widgets to the screen.
		*/
		void render() const;

	private:
		/**
			Renders the specific widget at the specified offset. The offset should be the position
			of the parent widget on the screen, unless the rendering explicitly requires rendering
			to an off-screen buffer/surface.

			@param widget The widget which should be rendered.
			@param offset The offset the widget should be rendered with.
		*/
		void render(const Widget & widget, const glm::vec2 & offset) const;

		GuiData m_data;
		Widget m_root;
	};
}
