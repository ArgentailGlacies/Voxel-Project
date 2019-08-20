
#include "event/EventBus.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::event
{
	TEST_CLASS(EventBusTest)
	{
	public:
		TEST_METHOD(EventBus_add)
		{
			EventBus bus;
			const auto listenerA = bus.add<int>(5, [](auto & event) {});
			const auto listenerB = bus.add<int>([](auto & event) {});
			const auto listenerC = bus.add<float>([](auto & event) {});

			Assert::AreEqual(2u, bus.size<int>());
			Assert::AreEqual(1u, bus.size<float>());
			Assert::AreEqual(5, listenerA.priority());
		}
		TEST_METHOD(EventBus_remove)
		{
			EventBus bus;
			{ const auto listener = bus.add<int>(5, [](auto & event) {}); }
			{ const auto listener = bus.add<int>([](auto & event) {}); }
			{ const auto listener = bus.add<float>([](auto & event) {}); }

			Assert::AreEqual(0u, bus.size<int>());
			Assert::AreEqual(0u, bus.size<float>());
		}

		TEST_METHOD(EventBus_post)
		{
			int monitor = 0;

			EventBus bus;
			const auto listenerA = bus.add<int>([&monitor](auto & event) { monitor = event; });
			const auto listenerB = bus.add<int>(4, [](auto & event) { event += 5; });
			const auto listenerC = bus.add<int>(2, [](auto & event) { event *= 2; });

			const int result = bus.post(7);
			Assert::AreEqual(19, result);
			Assert::AreEqual(19, monitor);
		}
	};
}