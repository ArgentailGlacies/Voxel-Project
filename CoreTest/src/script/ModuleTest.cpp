
#include "script/Module.h"

#include "script/Script.h"

#include <chaiscript/chaiscript.hpp>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace
{
	class MockModule : public core::Module<int>
	{
	private:
		virtual void initialize(chaiscript::Namespace & domain, const int & param) const override final
		{
			domain["param"] = chaiscript::var(param);
		}
	};
}

namespace core::script
{
	TEST_CLASS(ModuleTest)
	{
	public:
		TEST_METHOD(Module_apply)
		{
			Script script{ "script" };
			MockModule{}.apply(script, "module", 42);

			Assert::IsTrue(script.execute(R"(
				import("module")
				module.param
			)"));
		}
	};
}