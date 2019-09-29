#pragma once

#include <string>

namespace chaiscript
{
	class Namespace;
}

namespace core
{
	class Script;

	/**
		All modules must be derived from a single base class.
	*/
	class ModuleBase {};
	/**
		All script modules include functionality to scripts in some manner. Each module exists in
		their own namespaces, to avoid name collisions. A script can be bound to any number of
		modules, but must never be bound more than once to any one module.
	*/
	template<typename ...Params>
	class Module : public ModuleBase
	{
	public:
		virtual ~Module() = default;

		/**
			Applies the module to the specified script, binding the module with the provided
			parameters. The module should never be applied more than once to any instance of a
			script.

			@param script The script the module should be applied to.
			@param params The additional parameters which should be used when applying the module.
		*/
		void apply(Script & script, const Params & ...params);

	protected:
		/**
			Populates a domain (namespace) with a series of functions, variables, and other
			constructs of relevance. All functionality which should be exposed in a script must be
			defined in one initialize block such as this.

			@param domain The namespace the functionality should be registered under.
			@param params The parameters which should be used when initializing the module.
		*/
		virtual void initialize(chaiscript::Namespace & domain, const Params & ...params) = 0;
	};
}