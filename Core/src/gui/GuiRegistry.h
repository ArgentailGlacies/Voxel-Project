#pragma once

#include "event/EventBus.h"
#include "gui/Gui.h"
#include "io/File.h"
#include "scene/Scene.h"

#include <string>
#include <unordered_map>

namespace core
{
	/**
		The gui registry is responsible for creating, closing and generally managing the different
		graphical user interfaces. Each interface is created with a priority, where the interface
		with the highest priority is shown at the front.
	*/
	class GuiRegistry
	{
	public:
		GuiRegistry() = delete;
		GuiRegistry(EventBus & bus, Scene & scene);

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

	private:
		Scene & m_scene;

		std::unordered_map<std::string, Gui> m_guis;
		std::unordered_map<std::string, SceneEntry> m_nodes;
	};
}