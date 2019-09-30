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

	class Gui
	{
	public:
		Gui() = delete;
		Gui(const AssetRegistry & assets, const util::File & file);

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

		/**
			Executes some arbitrary script within the gui. Should typically not be used, although
			in rare cases the developer may need to execute a script rather than use actual code.

			@param code The script which should be executed within the gui.
			@return True iff the script was executed without errors.
		*/
		inline bool executeScript(const std::string & code) { return m_script.execute(code); }

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

		/**
			Assigns a value to the speified widget, overwriting any previously specified value. If
			the widget does not exist, the method does nothing.

			@param widget The widget which should be updated.
			@param value The value which should be stored in the widget.
		*/
		void setBool(const std::string & widget, bool value);
		void setFloat(const std::string & widget, float value);
		void setString(const std::string & widget, const std::string & value);
		/**
			Attempts to read a value from the given widget, if it exists. If a value could not be
			retrieved, the default value false, 0.0f, or "", is returned instead.

			@param widget The widget the data should be retrieved from.
			@return The widget's value, or the default value if the widget could not be found.
		*/
		bool getBool(const std::string & widget) const;
		float getFloat(const std::string & widget) const;
		std::string getString(const std::string & widget) const;

		// ...

		// TODO: Attempt to elliminate these methods, internal data should not leak
		inline auto & getBus() { return m_bus; }
		inline auto & getBus() const { return m_bus; }

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

		// ...

		/**
			Retrieves a widget with the given name. If no such widget exists, nullptr is returned
			instead.

			@param name The name of the widget to retrieve.
			@return The widget, or nullptr if the widget could not be found.
		*/
		Widget * getWidget(const std::string & name) const;

		// ...

		EventBus m_bus;
		Script m_script;
		Widget m_root;

		std::unordered_map<std::string, Widget *> m_widgets;
	};
}
