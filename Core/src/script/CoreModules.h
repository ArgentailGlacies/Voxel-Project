#pragma once

#include "script/Module.h"

namespace core
{
	class Gui;
	class GuiRegistry;

	// ...

	/**
		The glm namespace is by default not exposed to scripts. By including this module, the glm
		containing vectors will be exposed. Only certain common functionality will be exposed, such
		as the different types of vectors.
	*/
	class ModuleGlm : public Module<>
	{
	public:
		virtual void bind(Script & script) const override final;
	};

	/**
		The file system script module allows a script to import various filesystem operations. The
		module contains features such as file and folder processing, locating files, and seeing if
		files exists, etc.
	*/
	class ModuleFileSystem : public Module<>
	{
	public:
		virtual void bind(Script & script) const override final;
	};

	/**
		The gui script module allows a script to import various gui functionality, such as opening
		and closing guis, accessing gui widget data, and modifying the state of widgets. It is
		required for more complicated gui scripting, and must be added to all gui scripts, since
		widgets changes the state of the script when they are activated.
	*/
	class ModuleGui : public Module<GuiRegistry &, Gui &>
	{
	public:
		virtual void bind(Script & script, GuiRegistry & guis, Gui & gui) const override final;
	};
}