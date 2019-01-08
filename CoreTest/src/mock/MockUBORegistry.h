#pragma once

#include "opengl/UBORegistry.h"

namespace core
{
	inline UBORegistry mockUBORegistry()
	{
		UBORegistry registry;

		registry.add<int>("", "");

		return registry;
	}
}