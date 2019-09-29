#pragma once

#include <functional>
#include <string>

namespace chaiscript
{
	namespace dispatch { class Dynamic_Object; }

	using Namespace = dispatch::Dynamic_Object;
}

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
			Applies the module to the specified script, binding the module with the provided
			parameters. The module will be bound to the script as the given name. If a module with
			the given name has already been registered, the old module will be overwritten.

			@param script The script the module should be applied to.
			@param name The name of the module when bound to the script.
			@param params The additional parameters which should be used when applying the module.
		*/
		void apply(Script & script, const std::string & name, const Params & ...params);

	protected:
		/**
			Populates a domain (namespace) with a series of functions, variables, and other
			constructs of relevance. All functionality which should be exposed in a script must be
			defined in one initialize block such as this.

			@param domain The namespace the functionality should be registered under.
			@param params The parameters which should be used when initializing the module.
		*/
		virtual void initialize(chaiscript::Namespace & domain, const Params & ...params) const = 0;
	};

	// ...

	namespace detail
	{
		using Initializer = std::function<void(chaiscript::Namespace &)>;

		/**
			Stores the given ChaiScript namespace initializer in the script, such that the script is
			capable of using the namespace. This function acts as a compilation firewall, such that
			compile times do not explode whenever this file is included.

			@param script The script the initializer should operate on.
			@param name The name of the namespace which will be available in the script.
			@param init The initializer which defines what the namespace will contain.
		*/
		void store(Script & script, const std::string & name, const Initializer & init);
	}
}

// ...

namespace core
{
	template<typename ...Params>
	inline void Module<Params...>::apply(Script & script, const std::string & name, const Params & ...params)
	{
		detail::store(script, name, [this, &params...](chaiscript::Namespace & domain) { initialize(domain, params...); });
	}
}