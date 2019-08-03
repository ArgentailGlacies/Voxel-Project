#pragma once

#include "event/EventBus.h"
#include "event/EventListener.h"
#include "gui/Gui.h"
#include "io/File.h"
#include "scene/Scene.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace core
{
	class AssetRegistry;

	/**
		The gui registry is responsible for creating, closing and generally managing the different
		graphical user interfaces. Each interface is created with a priority, where the interface
		with the highest priority is shown at the front.
	*/
	class GuiRegistry
	{
	public:
		GuiRegistry() = delete;
		GuiRegistry(const AssetRegistry & assets, EventBus & bus, Scene & scene);

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

		/**
			Performs an update on all guis that have been opened thus far.
		*/
		void process();

	private:
		template<typename Event> void processEvent(Event & event);

		const AssetRegistry & m_assets;
		Scene & m_scene;

		Listener m_mouseMove;
		Listener m_mousePress;
		Listener m_mouseRelease;

		std::unordered_map<util::File, std::unique_ptr<Gui>> m_guis;
		std::unordered_map<util::File, SceneEntry> m_nodes;
		SceneEntry m_root;
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