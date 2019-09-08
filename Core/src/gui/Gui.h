#pragma once

#include "event/EventBus.h"
#include "gui/Widget.h"
#include "io/File.h"
#include "script/Script.h"

#include <string>
#include <unordered_map>

namespace core
{
	class AssetRegistry;
	class ModuleRegistry;

	class Gui
	{
	public:
		Gui() = delete;
		Gui(const AssetRegistry & assets, const ModuleRegistry & modules, const util::File & file);

		/**
			Performs one tick on all widgets in the gui.
		*/
		inline void process() { process(m_root); }
		/**
			Renders all underlying widgets to the screen.
		*/
		inline void render() const { render(m_root, {}); }

		// ...

		/**
			Resizes the gui to fit the new given size. Should typically not be used, as whenever the
			user changes the size of the display, all gui sizes will automatically be updated.

			@param size The new size of the gui.
		*/
		void resize(const glm::vec2 & size);

		// ...

		/**
			Checks whether the gui contains a widget named the same as the given widget name.

			@param widget The name of the widget to check if exists in the gui.
			@return True iff the gui has a widget matching the given name exactly.
		*/
		bool has(const std::string & widget) const;

		/**
			Checks whether the given widget is visible or not. If the widget does not exist, false
			is returned.

			@param widget The widget which should be checked if it is visible or not.
			@return True iff the widget exists and is visible.
		*/
		bool isVisible(const std::string & widget) const;
		/**
			Assigns the visibility state of the given widget, if it exists.

			@param widget The widget which should be updated.
			@param visible Whether the widget should be visible or not.
		*/
		void setVisible(const std::string & widget, bool visible);

		/**
			Checks whether the given widget is locked or not. If the widget does not exist, true
			is returned.

			@param widget The widget which should be checked if it is locked or not.
			@return True iff the widget does not exist or is locked.
		*/
		bool isLocked(const std::string & widget) const;
		/**
			Assigns the locked state of the given widget, if it exists.

			@param widget The widget which should be updated.
			@param locked Whether the widget should be locked or not.
		*/
		void setLocked(const std::string & widget, bool locked);

		// ...

		// TODO: Attempt to elliminate these methods, internal data should not leak
		inline auto & getBus() { return m_bus; }
		inline auto & getBus() const { return m_bus; }
		inline auto & getScript() { return m_script; }
		inline auto & getScript() const { return m_script; }

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

		std::unordered_map<std::string, Widget *> m_widgets;
	};
}
