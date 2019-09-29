
#include "Module.h"

#include "script/Script.h"

#include <chaiscript/chaiscript.hpp>

void core::detail::store(Script & script, const std::string & name, const Initializer & init)
{
	script.handle().register_namespace(init, name);
}
