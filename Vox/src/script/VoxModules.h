#pragma once

#include "script/Module.h"

namespace core
{
	class Engine;
}

namespace vox
{
	class Universe;

	/**
		The engine script module allows scripts to access the engine functionality, such as
		accessing the main loop for termination, the state manager for changing engine states, and
		similar functionality.
	*/
	class ModuleEngine : public core::Module<core::Engine &>
	{
	public:
		virtual void bind(core::Script & script, core::Engine & engine) const override final;
	};

	/**
		The universe script modules allows scripts access to the voxel data within the engine. The
		universe is split into worlds, which may or may not be loaded into memory at any given time.
		The module allows scripts to access worlds, by allowing reading and writing of blocks.
	*/
	class ModuleUniverse : public core::Module<Universe &>
	{
	public:
		virtual void bind(core::Script & script, Universe & universe) const override final;
	};
}