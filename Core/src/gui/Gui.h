#pragma once

#include "gui/Widget.h"

#include <unordered_map>

namespace core
{
	class Gui
	{
	public:
		void setBool(const std::string& field, bool value);
		void setFloat(const std::string& field, float value);
		void setString(const std::string& field, const std::string& value);

		auto getBool(const std::string& field) const -> bool;
		auto getFloat(const std::string& field) const -> float;
		auto getString(const std::string& field) const-> std::string;

	private:
		std::unordered_map<std::string, bool> m_bools;
		std::unordered_map<std::string, float> m_floats;
		std::unordered_map<std::string, std::string> m_strings;

		Widget m_root;
	};
}
