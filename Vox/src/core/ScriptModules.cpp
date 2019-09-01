
#include "ScriptModules.h"

#include "core/Engine.h"
#include "gui/Gui.h"
#include "gui/GuiRegistry.h"
#include "gui/Widget.h"
#include "script/ScriptUtil.h"
#include "io/File.h"
#include "io/Folder.h"

void core::script::initializeEngine(Script & script, Engine & engine)
{
	util::addGlobalVariable(script, &engine, "ENGINE");
	util::addMethod(script, &Engine::stop, "stop");
}

void core::script::initializeGui(Script & script, GuiRegistry & guis)
{
	util::addGlobalVariable(script, &guis, "GUI_REGISTRY");
	util::addMethod(script, &GuiRegistry::open, "open");
	util::addMethod(script, &GuiRegistry::close, "close");
}

void core::script::initializeFileSystem(Script & script)
{
	util::addType<::util::File>(script, "File");
	util::addCtor<::util::File()>(script, "File");
	util::addCtor<::util::File(const char *)>(script, "File");
	util::addCtor<::util::File(const std::string)>(script, "File");

	util::addType<::util::Folder>(script, "Folder");
	util::addCtor<::util::Folder(const char *)>(script, "Folder");
	util::addCtor<::util::Folder(const std::string)>(script, "Folder");
}
