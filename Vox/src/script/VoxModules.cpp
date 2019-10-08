
#include "VoxModules.h"

#include "core/Engine.h"
#include "editor/EditorWorld.h"
#include "script/ScriptUtil.h"
#include "world/Query.h"
#include "world/Universe.h"

void vox::ModuleEngine::bind(core::Script & script, core::Engine & engine) const
{
	core::addGlobalVariable(script, &engine, "ENGINE");

	core::addMethod(script, &core::Engine::stop, "stop");
}

void vox::ModuleUniverse::bind(core::Script & script, Universe & universe) const
{
	core::addGlobalVariable(script, &universe, "UNIVERSE");

	core::addMethod(script, &Universe::getBlockRegistry, "getBlockRegistry");
	core::addMethod(script, &Universe::createWorld, "createWorld");
	core::addMethod(script, &Universe::destroyWorld, "destroyWorld");
	core::addMethod(script, &Universe::hasWorld, "hasWorld");
	core::addMethod(script, &Universe::getWorld, "getWorld");

	core::addMethod(script, &World::read, "read");
	core::addMethod(script, &World::write, "write");
	core::addMethod(script, &World::finish, "finish");

	core::addFunction(script, [](const BlockRegistry & registry, int id) { return registry[id]; }, "[]");
	core::addFunction(script, [](const BlockRegistry & registry, const std::string & name) { return registry[name]; }, "[]");

	core::addFunction(script, &readBlock, "readBlock");
	core::addFunction(script, &readCylinder, "readCylinder");
	core::addFunction(script, &readEllipse, "readEllipse");
	core::addFunction(script, &readRectangle, "readRectangle");
	core::addFunction(script, &readLine, "readLine");
	core::addFunction(script, &readSphere, "readSphere");
	core::addFunction(script, &writeBlock, "writeBlock");
	core::addFunction(script, &writeCylinder, "writeCylinder");
	core::addFunction(script, &writeEllipse, "writeEllipse");
	core::addFunction(script, &writeRectangle, "writeRectangle");
	core::addFunction(script, &writeLine, "writeLine");
	core::addFunction(script, &writeSphere, "writeSphere");

	core::addType<WorldQuery>(script, "WorldQuery");
	core::addCtor<WorldQuery()>(script, "WorldQuery");
	core::addFunction(script, &WorldQuery::count, "count");
	core::addFunction(script, &WorldQuery::empty, "empty");
	core::addFunction(script, &WorldQuery::has, "has");
}

// ...

void vox::ModuleWorldEditor::bind(core::Script & script, EditorWorld & editor) const
{
	core::addGlobalVariable(script, &editor, "EDITOR");
	
	core::addMethod(script, &EditorWorld::setCameraSensitivity, "setCameraSensitivity");
	core::addMethod(script, &EditorWorld::setGridSize, "setGridSize");
	core::addMethod(script, &EditorWorld::setGridVisible, "setGridVisible");
	core::addMethod(script, &EditorWorld::lockCursorAxis, "lockCursorAxis");
}
