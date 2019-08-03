#pragma once

#include "event/EventBus.h"
#include "gui/Widget.h"
#include "io/File.h"
#include "script/Script.h"

namespace core
{
	class AssetRegistry;

	class Gui
	{
	public:
		Gui() = delete;
		Gui(const util::File & file, const AssetRegistry & assets);

		/**
			Performs one tick on all widgets in the gui.
		*/
		void process();
		/**
			Renders all underlying widgets to the screen.
		*/
		void render() const;

		// ...

		inline auto & getBus() { return m_bus; }
		inline const auto & getBus() const { return m_bus; }
		inline auto & getScript() { return m_script; }
		inline const auto & getScript() const { return m_script; }

	private:
		/**
			Loads the graphical interface data which is stored in the provided file.

			@param file The file containing all data to load.
			@param assets All assets registered within the system.
		*/
		void load(const util::File & file, const AssetRegistry & assets);
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

		EventBus m_bus;
		Script m_script;
		Widget m_root;
	};
}
