#pragma once

#include "gui/Gui.h"

namespace core
{
	inline auto mockGui()
	{
		auto gui = std::make_unique<Gui>();
		return gui;
	}
}