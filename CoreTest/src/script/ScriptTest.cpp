
#include "script/Script.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::script
{
	void shellValid() {}
	void shellInvalid() { throw std::exception(); }

	TEST_CLASS(ScriptTest)
	{
	public:
		TEST_METHOD(Script_execute)
		{
			Script script{ "script" };
			
			Assert::IsTrue(script.execute("var foo = 42"));
			Assert::IsFalse(script.execute("invalid script"));
			Assert::IsTrue(script.execute(shellValid));
			Assert::IsFalse(script.execute(shellInvalid));
		}
	};
}