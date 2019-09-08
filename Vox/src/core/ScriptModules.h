#pragma once

namespace core
{
	class Engine;
	class Gui;
	class GuiRegistry;
	class Script;

	/**
		No script module will be applied to a script unless explicitly stated. In order to add a
		module to a script, ModuleRegistry should be used. It can apply any number of named modules
		to a given script.
	*/
	namespace script
	{
		/**
			Initializes the engine module of the scripting system. The engine module contains
			functionality which directly applies to the engine itself.

			@param script The script which should be initialized with the module.
			@param engine The game engine itself.
		*/
		void initializeEngine(Script & script, Engine & engine);

		/**
			Initializes the gui module of the scripting system. The gui module contains
			functionality which allows reading gui and widget data, as well as modifying the state.

			@param script The script which should be initialized with the module.
		*/
		void initializeGui(Script & script);
		/**
			Initializes the gui regsitry module of the scripting system. The module contains
			functionality which allows opening and closing guis.

			@param script The script which should be initialized with the module.
			@param guis The gui registry which is responsible for all guis.
		*/
		void initializeGuiRegistry(Script & script, GuiRegistry & guis);

		// ...

		/**
			Initializes the gui module with all file system data within the engine. The module
			contains features such as file and folder processing.

			@param script The script which should be initialized with the module.
		*/
		void initializeFileSystem(Script & script);

		// ...

		namespace detail
		{
			/**
				Binds the given script and gui together, such that the script is aware that there is
				a gui which is accessible to it.

				@param script The script which should be updated to contain the gui.
				@param gui The gui which should be made accessible to the script.
			*/
			void bindGui(Script & script, Gui & gui);
		}
	}
}