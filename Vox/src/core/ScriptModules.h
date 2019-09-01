#pragma once

namespace core
{
	class Engine;
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
	}
}