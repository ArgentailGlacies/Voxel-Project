
#include "script/Module.h"

#include "script/Script.h"

#include <chaiscript/chaiscript.hpp>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace
{
	class MockModule : public core::Module<int>
	{
	public:
		virtual void bind(core::Script & script, int param) const override final
		{
			script.handle().register_namespace([param](auto & domain)
			{
				domain["param"] = chaiscript::var(param);
			}, "module");
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
			MockModule{}.bind(script, 42);

			Assert::IsTrue(script.execute(R"(
				import("module")
				module.param
			)"));
		}
	};
}