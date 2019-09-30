#pragma once

namespace core
{
	class Script;

	/**
		All script modules include functionality to scripts in some manner. Each module exists in
		their own namespaces, to avoid name collisions. A script can be bound to any number of
		modules, but must never be bound more than once to any one module.
	*/
	template<typename ...Params>
	class Module
	{
	public:
		virtual ~Module() = default;

		/**
			Populates a domain (namespace) with a series of functions, variables, and other
			constructs of relevance. All functionality which should be exposed in a script must be
			defined in one initialize block such as this.

			The functionality does not need to be scoped to a namespace; free-standing functions
			and class functions may also be added. The exact details of the module is defined by the
			module providing the functionality.

			@param script The script which should have the module applied to itself.
			@param params The parameters which should be used when binding the module.
		*/
		virtual void bind(Script & script, Params && ...params) const = 0;
	};
}