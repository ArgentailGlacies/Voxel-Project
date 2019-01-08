#pragma once

#include "script/Script.h"

#include <functional>
#include <string>
#include <unordered_map>

namespace core
{
	using Module = std::function<void(Script &)>;

	class ModuleRegistry
	{
	public:
		ModuleRegistry() = default;
		ModuleRegistry(const ModuleRegistry &) = delete;
		ModuleRegistry(ModuleRegistry &&) = delete;
		~ModuleRegistry() = default;

		ModuleRegistry & operator=(const ModuleRegistry &) = delete;
		ModuleRegistry & operator=(ModuleRegistry &&) = delete;

		/**
			Checks if the module with the given identifier exists within the registry.

			@param identifier The module to check if exists.
			@return True iff a module with the given identifier has been registered.
		*/
		bool has(const std::string & identifier) const;
		/**
			Registers the module with the given identifier into the registry. If a previous module
			has been registered under the specified identifier, that module wil be overwritten.

			@param identifier The identifier to register the module under.
			@param module The module to register.
		*/
		void add(const std::string & identifier, const Module & module);

		/**
			Applies the module under the given identifier to the given script.

			@param identifier The identifier for the module to apply to the script.
			@param script The script the module should be applied to.
		*/
		void apply(const std::string & identifier, Script & script);

	private:
		std::unordered_map<std::string, Module> m_modules;
	};
}