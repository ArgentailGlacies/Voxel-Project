
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
			util::execute(script,
				"def funA() {}"
				"def funB(int param) {}"
				"def funC(int param) { return 3 + param; }"
			);
			const auto & funA = util::get<std::function<void()>>(script, "funA");
			const auto & funB = util::get<std::function<void(int)>>(script, "funB");
			const auto & funC = util::get<std::function<int(int)>>(script, "funC");

			Assert::IsTrue(util::execute(script, funA));
			Assert::IsTrue(util::execute(script, funB, 5));
			Assert::AreEqual(4, util::execute(script, funC, 1));
			Assert::IsTrue(util::execute(script, "var foo = 1337;"));
		}

		TEST_METHOD(util_addEnum)
		{
			Script script{ "script" };
			util::addEnum<Enum>(script, "Enum", {
				{ Enum::FOO, "FOO" },
				{ Enum::BAR, "BAR" }
			});

			util::execute(script, 
				"var foo = FOO;"
				"var bar = BAR;"
			);

			Assert::IsTrue(Enum::FOO == util::get<Enum>(script, "foo"));
			Assert::IsTrue(Enum::BAR == util::get<Enum>(script, "bar"));
		}

		TEST_METHOD(util_addFunction)
		{
			Script script{ "script" };
			util::addFunction<void>(script, function, "function");
			util::addFunction<int, int>(script, function, "function");
			util::addFunction(script, [](float param) { return param; }, "function");

			util::execute(script,
				"function();"
				"var value = function(3);"
				"var other = function(4.1f);"
			);

			Assert::AreEqual(6, util::get<int>(script, "value"));
			Assert::AreEqual(4.1f, util::get<float>(script, "other"));
		}
		TEST_METHOD(util_addVariable)
		{
			Script script{ "script" };
			util::addVariable(script, 42, "variable");
			util::addConstant(script, 1337, "constant");
			util::addGlobalVariable(script, 9, "global_variable");
			util::addGlobalConstant(script, 3, "global_constant");

			Assert::AreEqual(42, util::get<int>(script, "variable"));
			Assert::AreEqual(1337, util::get<int>(script, "constant"));
			Assert::AreEqual(9, util::get<int>(script, "global_variable"));
			Assert::AreEqual(3, util::get<int>(script, "global_constant"));
		}

		TEST_METHOD(util_addClass)
		{
			Script script{ "script" };
			util::addType<Basic>(script, "Basic");
			util::addCtor<Basic()>(script, "Basic");
			util::addCtor<Basic(int)>(script, "Basic");
			util::addCtor<Basic(float)>(script, "Basic");
			util::addMethod(script, &Basic::data, "data");
			util::addMethod<Basic, void>(script, &Basic::work, "work");
			util::addMethod<Basic, void, int>(script, &Basic::work, "work");
			util::addAttribute(script, &Basic::attribute, "attribute");

			util::execute(script,
				"var basicA = Basic();"
				"var basicB = Basic(3);"
				"var basicC = Basic(0.5f);"
				"basicB.work();"
				"basicC.work(7);"
			);

			Assert::AreEqual(0, util::get<int>(script, "basicA.data()"));
			Assert::AreEqual(3, util::get<int>(script, "basicB.data()"));
			Assert::AreEqual(7, util::get<int>(script, "basicC.data()"));
			Assert::AreEqual(0.0f, util::get<float>(script, "basicA.attribute"));
			Assert::AreEqual(1.5f, util::get<float>(script, "basicB.attribute"));
			Assert::AreEqual(0.5f, util::get<float>(script, "basicC.attribute"));
		}
	};
}