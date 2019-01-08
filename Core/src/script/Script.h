#pragma once

#include <functional>
#include <memory>
#include <string>

namespace chaiscript { class ChaiScript; }

namespace core
{
	class Script
	{
	public:
		Script() = delete;
		Script(const std::string & name);
		Script(const Script &) = delete;
		Script(Script &&) = delete;
		~Script();

		Script & operator=(const Script &) = delete;
		Script & operator=(Script &&) = delete;

		std::string name() const;

		bool execute(const std::function<void()> & shell) const;
		bool execute(const std::string & code) const;

		chaiscript::ChaiScript & handle() const;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}