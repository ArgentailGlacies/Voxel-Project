
#include "core/MainLoop.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core
{
	TEST_CLASS(MainLoopTest)
	{
	public:
		TEST_METHOD(MainLoop_start)
		{
			unsigned int updates = 0;
			unsigned int renders = 0;

			MainLoop loop{
				[&](auto t, auto) { if (t >= 1.0f || updates++ > 100) loop.stop(); },
				[&](auto) { renders++; }
			};
			loop.start(60.0, 60.0);

			Assert::AreEqual(60u, updates);
			Assert::AreNotEqual(0u, renders);
		}
	};
}