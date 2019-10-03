#pragma once

#include "ScriptUtil.h"

namespace core
{
	template<typename Enum>
	void addEnum(Script & script, const std::string & name,
		const std::vector<std::pair<Enum, std::string>> & pairs)
	{
		auto ptr = std::make_shared<chaiscript::Module>();
		chaiscript::utility::add_class<Enum>(*ptr, name, pairs);
		script.handle().add(ptr);
	}

	template<typename Ret, typename ...Params>
	void addFunction(Script & script, Ret(*function)(Params...), const std::string & name)
	{
		script.handle().add(chaiscript::fun(function), name);
	}
	template<typename T>
	void addFunction(Script & script, const T & function, const std::string & name)
	{
		script.handle().add(chaiscript::fun(function), name);
	}

	template<typename Type>
	inline void addVariable(Script & script, Type && variable, const std::string & name)
	{
		script.handle().add(chaiscript::var(variable), name);
	}
	template<typename Type>
	inline void addConstant(Script & script, const Type & variable, const std::string & name)
	{
		script.handle().add(chaiscript::const_var(variable), name);
	}
	template<typename Type>
	void addGlobalVariable(Script & script, Type && variable, const std::string & name)
	{
		script.handle().add_global(chaiscript::var(variable), name);
	}
	template<typename Type>
	void addGlobalConstant(Script & script, const Type & variable, const std::string & name)
	{
		script.handle().add_global_const(chaiscript::const_var(variable), name);
	}

	// ...

	template<typename Type>
	inline void addType(Script & script, const std::string & name)
	{
		script.handle().add(chaiscript::user_type<Type>(), name);
	}
	template<typename Parent, typename Child>
	inline void addRelation(Script & script)
	{
		script.handle().add(chaiscript::base_class<Parent, Child>());
	}

	template<typename Ctor>
	inline void addCtor(Script & script, const std::string & name)
	{
		script.handle().add(chaiscript::constructor<Ctor>(), name);
	}
	template<typename Class, typename Ret, typename ...Params>
	inline void addMethod(Script & script, Ret(Class::* method)(Params...),
		const std::string & name)
	{
		script.handle().add(chaiscript::fun(method), name);
	}
	template<typename Class, typename Ret, typename ...Params>
	inline void addMethod(Script & script, Ret(Class::* method)(Params...) const,
		const std::string & name)
	{
		script.handle().add(chaiscript::fun(method), name);
	}

	template<typename Type>
	inline void addAttribute(Script & script, Type && attribute, const std::string & name)
	{
		script.handle().add(chaiscript::fun(attribute), name);
	}

	// ...

	template<typename Ret, typename ...Params>
	inline Ret execute(const Script & script, const std::function<Ret(Params...)> & shell,
		const Params & ...params)
	{
		Ret returned{};
		execute(script, [&shell, &returned, &params...]() { returned = shell(params...); });
		return returned;
	}
	template<typename ...Params>
	inline bool execute(const Script & script, const std::function<void(Params...)> & shell,
		const Params & ...params)
	{
		return execute(script, [&shell, &params...]() { shell(params...); });
	}
	inline bool execute(const Script & script, const std::function<void()> & shell)
	{
		return script.execute(shell);
	}
	inline bool execute(const Script & script, const std::string & code)
	{
		return script.execute(code);
	}

	template<typename Type>
	inline Type get(const Script & script, const std::string & expression, const Type & def)
	{
		try
		{
			return script.handle().eval<Type>(expression);
		}
		catch (const std::exception &)
		{
			return def;
		}
	}
}