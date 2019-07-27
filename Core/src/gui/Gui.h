#pragma once

#include "gui/GuiData.h"
#include "gui/Widget.h"
#include "io/File.h"

namespace core
{
	class AssetRegistry;

	class Gui
	{
	public:
		Gui() = delete;
		Gui(const std::string & name) : m_data(name) {}

		/**
			Loads the gui data from the specified file.

			@param file The file containing the gui data.
			@param assets All registered assets within the system.
		*/
		void load(const util::File & file, const AssetRegistry & assets);

		/**
			Performs one tick on all widgets in the gui.
		*/
		void process();
		/**
			Renders all underlying widgets to the screen.
		*/
		void render() const;

	private:
		/**
			Updates the widget and all its underlying children. The widget's position and size will
			be recalculated based on the position and size of the parent and children.

			@param widget The widget which should be updated.
		*/
		void process(Widget & widget);
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
