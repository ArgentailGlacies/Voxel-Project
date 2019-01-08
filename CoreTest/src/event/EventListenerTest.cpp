
#include "event/EventBus.h"
#include "event/EventListener.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::event
{
	TEST_CLASS(EventListenerTest)
	{
	public:
		TEST_METHOD(EventListener_move)
		{
			EventBus bus;
			Listener listenerA{ bus, typeid(int), 3u, 71, false };
			Listener listenerB = std::move(listenerA);

			Assert::AreEqual(3u, listenerB.id());
			Assert::AreEqual(71, listenerB.priority());
		}

		TEST_METHOD(EventListener_assignMove)
		{
			EventBus bus;
			Listener listenerA{ bus, typeid(int), 7u, 19, false };
			Listener listenerB{ bus, typeid(int), 4u, 50, false };
			listenerB = std::move(listenerA);

			Assert::AreEqual(7u, listenerB.id());
			Assert::AreEqual(19, listenerB.priority());
		}
	};
}