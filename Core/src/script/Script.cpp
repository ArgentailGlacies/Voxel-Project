
#include "Script.h"

#include <chaiscript/chaiscript.hpp>
#include <plog/Log.h>

struct core::Script::Impl
{
	std::string m_name;

	chaiscript::ChaiScript m_chaiscript;
};

core::Script::Script(const std::string & name)
	: m_impl(std::make_unique<Impl>())
{
	m_impl->m_name = name;
}
core::Script::~Script() = default;

std::string core::Script::name() const
{
	return m_impl->m_name;
}

bool core::Script::execute(const std::function<void()>& shell) const
{
	try
	{
		shell();
		return true;
	}
	catch (const std::exception & e)
	{
		LOG_WARNING << "Error in '" << name() << "':" << std::endl << e.what();
		return false;
	}
}
bool core::Script::execute(const std::string & code) const
{
	try
	{
		m_impl->m_chaiscript.eval(code);
		return true;
	}
	catch (const std::exception & e)
	{
		LOG_WARNING << "Error in '" << name() << "':" << std::endl << e.what();
		return false;
	}
}

chaiscript::ChaiScript & core::Script::handle() const
{
	return m_impl->m_chaiscript;
}