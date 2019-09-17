#pragma once

#include "io/Folder.h"

#include <string>

namespace core
{
	class AssetRegistry;
	class ECS;
	class Engine;
	class ModuleRegistry;
	class UBORegistry;

	/**
		Registers loaders and factories for all assets. All assets are managed through lazy loading,
		and will not consume memory unless they are explicitly required. Assets are automatically
		unloaded when they are no longer needed.

		Assets which are loaded:
		- Fonts (/fonts/*.xml)
		- Shader programs (/shaders/*.xml)
		- Sprites (/sprites/*.xml)

		@param assets The registry containing all assets available in the engine.
		@param data The root data folder containing all game data.
		@param ubos The registry containing all uniform buffer objects.
	*/
	void setupAssets(AssetRegistry & assets, const util::Folder & data, const UBORegistry & ubos);

	/**
		Registers all components in the entity component system structure. Any components not added
		cannot be used.

		@param ecs The entity component system structure the components should be added to.
	*/
	void setupComponents(ECS & ecs);
	/**
		Registers all systems in the entity component system structure. All systems added will
		automatically operate upon all data they have been configured to operate on.

		@param ecs The entity component system structure the systems should be added to.
	*/
	void setupSystems(ECS & ecs);

	/**
		Sets up the different uniform buffer objects which provides a global entry point to change
		shader program data.

		@param ubos The registry containing all uniform buffer objects.
	*/
	void setupUBOs(UBORegistry & ubos);

	/**
		Sets up the different script modules which adds various functionality to scripts.

		@param modules The module registry which should contain all script modules in the system.
		@param engine The game engine itself.
	*/
	void setupModules(ModuleRegistry & modules, Engine & engine);
}