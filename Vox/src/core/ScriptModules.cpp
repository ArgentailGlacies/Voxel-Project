
#include "ScriptModules.h"

#include "core/Engine.h"
#include "script/ScriptUtil.h"

void core::script::initializeEngine(Script & script, Engine & engine)
{
	util::addGlobalVariable(script, &engine, "ENGINE");
	util::addMethod(script, &Engine::stop, "stop");
}
