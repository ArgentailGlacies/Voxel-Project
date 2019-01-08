#pragma once

#include "ecs/ComponentRegistry.h"

namespace core
{
	inline ComponentRegistry mockComponentRegistry()
	{
		ComponentRegistry registry;

		registry.add<bool>();
		registry.add<double>();
		registry.add<float>();
		registry.add<int>();

		return registry;
	}
}