
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

void core::script::initializeGui(Script & script)
{
	util::addMethod(script, &Gui::has, "has");
	util::addMethod(script, &Gui::isVisible, "isVisible");
	util::addMethod(script, &Gui::setVisible, "setVisible");
	util::addMethod(script, &Gui::isLocked, "isLocked");
	util::addMethod(script, &Gui::setLocked, "setLocked");
	util::addMethod(script, &Gui::getBool, "getBool");
	util::addMethod(script, &Gui::getFloat, "getFloat");
	util::addMethod(script, &Gui::getString, "getString");
	util::addMethod(script, &Gui::setBool, "setBool");
	util::addMethod(script, &Gui::setFloat, "setFloat");
	util::addMethod(script, &Gui::setString, "setString");
}
void core::script::initializeGuiRegistry(Script & script, GuiRegistry & guis)
{
	util::addGlobalVariable(script, &guis, "GUI_REGISTRY");
	util::addMethod(script, &GuiRegistry::open, "open");
	util::addMethod(script, &GuiRegistry::close, "close");
}

void core::script::detail::bindGui(Script & script, Gui & gui)
{
	util::addGlobalVariable(script, &gui, "GUI");
	util::addGlobalVariable(script, "", "WIDGET");
}

// ...

void core::script::initializeFileSystem(Script & script)
{
	using ::util::File;
	using ::util::Folder;

	util::addType<File>(script, "File");
	util::addCtor<File()>(script, "File");
	util::addCtor<File(const char *)>(script, "File");
	util::addCtor<File(const std::string &)>(script, "File");

	util::addType<Folder>(script, "Folder");
	util::addCtor<Folder(const char *)>(script, "Folder");
	util::addCtor<Folder(const std::string &)>(script, "Folder");
}
