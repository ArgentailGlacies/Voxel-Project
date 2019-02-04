
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
				[&](auto t, auto) { if (t > 0.25f - std::numeric_limits<float>::epsilon() || updates++ > 30) loop.stop(); },
				[&](auto) { renders++; }
			};
			loop.start(60.0, 60.0);

			Assert::AreEqual(15u, updates);
			Assert::AreNotEqual(0u, renders);
		}
	};
}