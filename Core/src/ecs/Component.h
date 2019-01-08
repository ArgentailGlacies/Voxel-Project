#pragma once

#include <bitset>

namespace core
{
	constexpr unsigned int COMPONENT_MAX = 64;

	using ComponentMask = std::bitset<COMPONENT_MAX>;
}