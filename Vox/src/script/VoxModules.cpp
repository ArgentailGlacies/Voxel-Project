
#include "VoxModules.h"

#include "core/Engine.h"
#include "script/ScriptUtil.h"

void vox::ModuleEngine::bind(core::Script & script, core::Engine & engine) const
{
	core::util::addGlobalVariable(script, &engine, "ENGINE");

	core::util::addMethod(script, &core::Engine::stop, "stop");
}

void vox::ModuleUniverse::bind(core::Script & script, Universe & universe) const
{
}
