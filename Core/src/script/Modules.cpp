
#include "Modules.h"

#include "gui/Gui.h"
#include "gui/GuiRegistry.h"
#include "gui/Widget.h"
#include "script/Script.h"
#include "script/ScriptUtil.h"
#include "io/File.h"
#include "io/Folder.h"

#include <chaiscript/chaiscript.hpp>
#include <string>

namespace
{
	std::string WIDGET;
}

void core::ModuleFileSystem::bind(Script & script) const
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

void core::ModuleGui::bind(Script & script, GuiRegistry & guis, Gui & gui) const
{
	util::addGlobalVariable(script, &guis, "GUI_REGISTRY");
	util::addGlobalVariable(script, &gui, "GUI");
	util::addGlobalVariable(script, &WIDGET, "WIDGET");

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

	util::addMethod(script, &GuiRegistry::open, "open");
	util::addMethod(script, &GuiRegistry::close, "close");
}
