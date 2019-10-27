#pragma once

#include "event/EventBus.h"
#include "event/EventListener.h"
#include "gui/Gui.h"
#include "io/File.h"
#include "scene/SceneNode.h"

#include <glm/vec2.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace core
{
	class AssetRegistry;
	class Display;
	class Scene;
	class Script;

	/**
		The gui registry is responsible for creating, closing and generally managing the different
		graphical user interfaces. Each interface is created with a priority, where the interface
		with the highest priority is shown at the front.
	*/
	class GuiRegistry
	{
	public:
		GuiRegistry() = delete;
		GuiRegistry(const AssetRegistry & assets, const Display & display, EventBus & bus, Scene & scene);

		/**
			Performs an update on all guis that have been opened thus far.
		*/
		void process();

		/**
			Opens the gui stored in the specified file if it is not already open. If the gui has
			already been opened, this method does nothing.

			@param file The file containing the gui to open.
			@return True iff the gui was opened.
		*/
		bool open(const util::File & file);
		/**
			Closes the gui stored in the given file if it has been loaded. If the gui has not been
			opened already, this method does nothing.

			@param file The file containing the gui to close.
			@return true iff the gui was closed.
		*/
		bool close(const util::File & file);

		// ...

		/**
			Registers a script module into the gui with the given file if it has been loaded. The
			registration of the module is performed in the function parameter.

			@param file The gui which should have the module registered to it.
			@param func The function which specifies how the module will be registered.
			@return true iff the module was registered to the file.
		*/
		bool registerModule(const util::File & file, const std::function<void(Script &)> & func);

	private:
		/**
			Resizes all guis to fill the given size.

			@param size The size all guis should have after resizing.
		*/
		void resizeGuis(const glm::vec2 & size);

		template<typename Event> void processEvent(Event & event);

		// ...

		const AssetRegistry & m_assets;
		const Display & m_display;
		Scene & m_scene;

		Listener m_displayResize;
		Listener m_mouseMove;
		Listener m_mousePress;
		Listener m_mouseRelease;
		Listener m_keyPress;
		Listener m_keyRelease;
		Listener m_keyUnichar;
		SceneEntry m_root;

		std::unordered_map<util::File, std::unique_ptr<Gui>> m_guis;
		std::unordered_map<util::File, SceneEntry> m_nodes;
	};
}

namespace core
{
	template<typename Event>
	inline void GuiRegistry::processEvent(Event & event)
	{
		for (const auto & [_, gui] : m_guis)
		{
			if (!event.isConsumed())
				gui->getBus().post(event);
		}
	}
}