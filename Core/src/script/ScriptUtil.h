#pragma once

#include "script/Script.h"

#include <chaiscript/chaiscript.hpp>
#include <string>

// ... because somebody decided that defining these was a good idea...
#undef min
#undef max
#undef OPAQUE
#undef TRANSPARENT

namespace core
{
	/**
		Adds an enum to the given script. The enum values will leak out into global namespace, ie.
		an enum 'enum class Enum { FOO, BAR }' may be used as 'var value = FOO'.
		
		@param script The scrip to add the enum to.
		@param name The name of the enum.
		@param pairs The list of pairs of enum values and names.

		Example:
		addEnum(script, "MyEnum", { { MyEnum::A, "A" }, { MyEnum::B, "B" } })
	*/
	template<typename Enum>
	void addEnum(Script & script, const std::string & name,
		const std::vector<std::pair<Enum, std::string>> & pairs
	);

	/**
		Adds the specified function to the given script. The function may be invoked through its
		registered name, provided the script function signature matches the registered function.

		@param script The script to add the function to.
		@param function The function to register.
		@param name The name of the function.

		Example:
		addFunction<float, int>(script, function, "function")
	*/
	template<typename Ret, typename ...Params>
	void addFunction(Script & script, Ret(*function)(Params...), const std::string & name);
	/**
		Adds the specified function to the given script. The function may be invoked through its
		registered name, provided the script function signature matches the registered function.

		@param script The script to add the function to.
		@param function The function to register.
		@param name The name of the function.

		Example:
		addFunction(script, [](int parameter) {}, "function")
	*/
	template<typename T>
	void addFunction(Script & script, const T & function, const std::string & name);

	/**
		Adds the specified variable to the given script. The variable may be accessed through its
		registered name.

		@param script The script to add the variable to.
		@param variable The variable to register.
		@param name The name of the variable.

		Example:
		addVariable(script, variable, "variable")
	*/
	template<typename Type>
	void addVariable(Script & script, Type && variable, const std::string & name);
	/**
		Adds the specified constant to the given script. The variable may be accessed through its
		registered name.

		@param script The script to add the constant to.
		@param constant The constant to register.
		@param name The name of the constant.

		Example:
		addConstant(script, constant, "constant")
	*/
	template<typename Type>
	void addConstant(Script & script, const Type & variable, const std::string & name);
	/**
		Adds the specified variable to the given script. The variable may be accessed through its
		registered name.

		@param script The script to add the variable to.
		@param variable The variable to register.
		@param name The name of the variable.

		Example:
		addVariable(script, variable, "variable")
	*/
	template<typename Type>
	void addGlobalVariable(Script & script, Type && variable, const std::string & name);
	/**
		Adds the specified constant to the given script. The variable may be accessed through its
		registered name.

		@param script The script to add the constant to.
		@param constant The constant to register.
		@param name The name of the constant.

		Example:
		addConstant(script, constant, "constant")
	*/
	template<typename Type>
	void addGlobalConstant(Script & script, const Type & variable, const std::string & name);

	// ...

	/**
		Adds the specific type to the given script.

		@param script The script to add the type to.
		@param name The name of the type.

		Example:
		addType<MyType>(script, "MyType")
	*/
	template<typename Type>
	void addType(Script & script, const std::string & name);
	/**
		Adds a specific relation between two types, such that the Chaiscript engine may correctly
		utilize polymorphism.

		@param script The script to add the relation to.

		Example:
		addRelation<Base, Derived>(script)
	*/
	template<typename Parent, typename Child>
	void addRelation(Script & script);

	/**
		Adds the specified constructor to the given script. The constructor may be invoked through
		its registered name, provided the script constructor signature matches the registered
		constructor.

		@param script The script to add the constructor to.
		@param name The name of the constructor.

		Example:
		addCtor<MyType()>(script, "MyType")
	*/
	template<typename Ctor>
	void addCtor(Script & script, const std::string & name);

	/**
		Adds the specified method to the given script. The method may be invoked through its
		registered name, provided the script method signature matches the registered method.

		@param script The script to add the method to.
		@param method The method to register.
		@param name The name of the method.

		Example:
		addMethod<MyType, float, int>(script, &MyType::method, "method")
	*/
	template<typename Class, typename Ret, typename ...Params>
	void addMethod(Script & script, Ret(Class::* method)(Params...), const std::string & name);
	/**
		Adds the specified method to the given script. The method may be invoked through its
		registered name, provided the script method signature matches the registered method.

		@param script The script to add the method to.
		@param method The method to register.
		@param name The name of the method.

		Example:
		addMethod<MyType, float, int>(script, &MyType::method, "method")
	*/
	template<typename Class, typename Ret, typename ...Params>
	void addMethod(Script & script, Ret(Class::* method)(Params...) const, const std::string & name);

	/**
		Adds the specified attribute to the given script. The method may be accessed through its
		registered name.

		@param script The script to add the attribute to.
		@param attribute The method to attribute.
		@param name The name of the attribute.

		Example:
		addAttribute<MyType, float, int>(script, &MyType::attribute, "attribute")
	*/
	template<typename Type>
	void addAttribute(Script & script, Type && attribute, const std::string & name);

	// ...

	/**
		Executes the specified shell in the given script. The resulting value from the shell is
		returned to the user.

		@param script The script the shell should be executed within.
		@param shell The shell to execute.
		@param params The parameters for the shell.
		@return The return value of the shell.
	*/
	template<typename Ret, typename ...Params>
	Ret execute(const Script & script, const std::function<Ret(Params...)> & shell,
		const Params & ...params);
	/**
		Executes the specified shell in the given script. Returns whether the shell executed without
		encountering any issues.

		@param script The script the shell should be executed within.
		@param shell The shell to execute.
		@param params The parameters for the shell.
		@return True iff the shell executed without any problems.
	*/
	template<typename ...Params>
	bool execute(const Script & script, const std::function<void(Params...)> & shell,
		const Params & ...params);
	/**
		Executes the specified shell in the given script. Returns whether the shell executed without
		encountering any issues.

		@param script The script the shell should be executed within.
		@param shell The shell to execute.
		@return True iff the shell executed without any problems.
	*/
	bool execute(const Script & script, const std::function<void()> & shell);
	/**
		Executes the specified code in the given script. Returns whether the code executed without
		encountering any issues.

		@param script The script the code should be executed within.
		@param code The code to execute.
		@return True iff the code executed without any problems.
	*/
	bool execute(const Script & script, const std::string & code);

	/**
		Attempts to retrieve a value from the provided expression from the script. If the expression
		forms a valid value for the provided type, that value is returned. Otherwise, the default
		value is returned.

		@param script The script to obtain the value from.
		@param expression The expression to reduce to a value.
		@param def The default value.
		@return The value of the expression, or default if the expression is invalid.
	*/
	template<typename Type>
	Type get(const Script & script, const std::string & expression, const Type & def = Type{});
}

#include "ScriptUtil.hpp"