
#include "event/EventBus.h"
#include "event/EventQueue.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::event
{
	TEST_CLASS(EventQueueTest)
	{
	public:
		TEST_METHOD(EventQueue_ctor)
		{
			EventBus bus;
			EventQueue queue{ bus };

			// TODO: Nothing to test...?
		}
	};
}