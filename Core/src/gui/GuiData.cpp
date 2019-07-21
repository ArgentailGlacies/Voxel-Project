
#include "GuiData.h"

const std::string core::GuiData::STATE_BOOL = "STATE_BOOL";
const std::string core::GuiData::STATE_FLOAT = "STATE_FLOAT";
const std::string core::GuiData::STATE_STRING = "STATE_STRING";

// ...

core::GuiData::GuiData(const std::string & name) : m_script(name)
{
	m_script.execute("global " + STATE_BOOL + " = false;");
	m_script.execute("global " + STATE_FLOAT + " = 0.0f;");
	m_script.execute("global " + STATE_STRING + " = \"\";");
}

auto core::GuiData::getBool(const std::string & field) const -> bool
{
	const auto it = m_bools.find(field);
	return it == m_bools.end() ? false : it->second;
}
auto core::GuiData::getFloat(const std::string & field) const -> float
{
	const auto it = m_floats.find(field);
	return it == m_floats.end() ? 0.0f : it->second;
}
auto core::GuiData::getString(const std::string & field) const -> std::string
{
	const auto it = m_strings.find(field);
	return it == m_strings.end() ? "" : it->second;
}
