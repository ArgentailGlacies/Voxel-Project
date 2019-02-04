#pragma once

#include "gui/Widget.h"

#include <string>
#include <unordered_map>

namespace core
{
	class Gui
	{
	public:


	private:
		std::unordered_map<std::string, Widget> m_widgets;
	};
}