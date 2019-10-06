
#include "script/Script.h"
#include "script/ScriptUtil.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::script
{
	enum class Enum { FOO = 3, BAR = 7 };

	class Basic
	{
	public:
		Basic() = default;
		Basic(int data) : m_data(data) {}
		Basic(float data) : attribute(data) {}

		void work() { attribute += 1.5f; }
		void work(int parameter) { m_data = parameter; }

		inline auto data() const { return m_data; }

		float attribute = 0.0f;

	private:
		int m_data = 0;
	};

	void function() {}
	int function(int parameter) { return parameter + 3; }

	// ...

	TEST_CLASS(ScriptUtilTest)
	{
	public:
		TEST_METHOD(util_execute)
		{
			Script script{ "script" };
			execute(script,
				"def funA() {}"
				"def funB(int param) {}"
				"def funC(int param) { return 3 + param; }"
			);
			const auto & funA = get<std::function<void()>>(script, "funA");
			const auto & funB = get<std::function<void(int)>>(script, "funB");
			const auto & funC = get<std::function<int(int)>>(script, "funC");

			Assert::IsTrue(execute(script, funA));
			Assert::IsTrue(execute(script, funB, 5));
			Assert::AreEqual(4, execute(script, funC, 1));
			Assert::IsTrue(execute(script, "var foo = 1337;"));
		}

		TEST_METHOD(util_addEnum)
		{
			Script script{ "script" };
			addEnum<Enum>(script, "Enum", {
				{ Enum::FOO, "FOO" },
				{ Enum::BAR, "BAR" }
			});

			execute(script, 
				"var foo = FOO;"
				"var bar = BAR;"
			);

			Assert::IsTrue(Enum::FOO == get<Enum>(script, "foo"));
			Assert::IsTrue(Enum::BAR == get<Enum>(script, "bar"));
		}

		TEST_METHOD(util_addFunction)
		{
			Script script{ "script" };
			addFunction<void>(script, function, "function");
			addFunction<int, int>(script, function, "function");
			addFunction(script, [](float param) { return param; }, "function");

			execute(script,
				"function();"
				"var value = function(3);"
				"var other = function(4.1f);"
			);

			Assert::AreEqual(6, get<int>(script, "value"));
			Assert::AreEqual(4.1f, get<float>(script, "other"));
		}
		TEST_METHOD(util_addVariable)
		{
			Script script{ "script" };
			addVariable(script, 42, "variable");
			addConstant(script, 1337, "constant");
			addGlobalVariable(script, 9, "global_variable");
			addGlobalConstant(script, 3, "global_constant");

			Assert::AreEqual(42, get<int>(script, "variable"));
			Assert::AreEqual(1337, get<int>(script, "constant"));
			Assert::AreEqual(9, get<int>(script, "global_variable"));
			Assert::AreEqual(3, get<int>(script, "global_constant"));
		}

		TEST_METHOD(util_addClass)
		{
			Script script{ "script" };
			addType<Basic>(script, "Basic");
			addCtor<Basic()>(script, "Basic");
			addCtor<Basic(int)>(script, "Basic");
			addCtor<Basic(float)>(script, "Basic");
			addMethod(script, &Basic::data, "data");
			addMethod<Basic, void>(script, &Basic::work, "work");
			addMethod<Basic, void, int>(script, &Basic::work, "work");
			addAttribute(script, &Basic::attribute, "attribute");

			execute(script,
				"var basicA = Basic();"
				"var basicB = Basic(3);"
				"var basicC = Basic(0.5f);"
				"basicB.work();"
				"basicC.work(7);"
			);

			Assert::AreEqual(0, get<int>(script, "basicA.data()"));
			Assert::AreEqual(3, get<int>(script, "basicB.data()"));
			Assert::AreEqual(7, get<int>(script, "basicC.data()"));
			Assert::AreEqual(0.0f, get<float>(script, "basicA.attribute"));
			Assert::AreEqual(1.5f, get<float>(script, "basicB.attribute"));
			Assert::AreEqual(0.5f, get<float>(script, "basicC.attribute"));
		}
	};
}