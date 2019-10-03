
#include "CoreModules.h"

#include "gui/Gui.h"
#include "gui/GuiRegistry.h"
#include "gui/Widget.h"
#include "script/Script.h"
#include "script/ScriptUtil.h"
#include "io/File.h"
#include "io/Folder.h"

#include <string>

namespace
{
	std::string WIDGET;
}

void core::ModuleFileSystem::bind(Script & script) const
{
	addType<util::File>(script, "File");
	addCtor<util::File()>(script, "File");
	addCtor<util::File(const char *)>(script, "File");
	addCtor<util::File(const std::string &)>(script, "File");

	addType<util::Folder>(script, "Folder");
	addCtor<util::Folder(const char *)>(script, "Folder");
	addCtor<util::Folder(const std::string &)>(script, "Folder");
}

void core::ModuleGui::bind(Script & script, GuiRegistry & guis, Gui & gui) const
{
	addGlobalVariable(script, &guis, "GUI_REGISTRY");
	addGlobalVariable(script, &gui, "GUI");
	addGlobalVariable(script, &WIDGET, "WIDGET");

	addMethod(script, &Gui::has, "has");
	addMethod(script, &Gui::isVisible, "isVisible");
	addMethod(script, &Gui::setVisible, "setVisible");
	addMethod(script, &Gui::isLocked, "isLocked");
	addMethod(script, &Gui::setLocked, "setLocked");
	addMethod(script, &Gui::getBool, "getBool");
	addMethod(script, &Gui::getFloat, "getFloat");
	addMethod(script, &Gui::getString, "getString");
	addMethod(script, &Gui::setBool, "setBool");
	addMethod(script, &Gui::setFloat, "setFloat");
	addMethod(script, &Gui::setString, "setString");

	addMethod(script, &GuiRegistry::open, "open");
	addMethod(script, &GuiRegistry::close, "close");
}
