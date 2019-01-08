
#include "allegro/Allegro.h"
#include "Context.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core
{		
	TEST_CLASS(AllegroTest)
	{
	public:
		TEST_METHOD(Allegro_initialize)
		{
			Assert::IsTrue(allegro().isInitialized());
			Assert::IsFalse(allegro().hasErrors());
		}
	};
}