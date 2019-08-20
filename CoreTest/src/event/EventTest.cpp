
#include "event/EventBus.h"
#include "event/Events.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::event
{
	TEST_CLASS(EventTest)
	{
	public:
		TEST_METHOD(Consumable_consume)
		{
			EventBus bus;
			const auto listenerA = bus.add<Consumable>(0, [](auto & event) { Assert::IsTrue(event.consume()); });
			const auto listenerB = bus.add<Consumable>(1, [](auto & event) { Assert::IsFalse(event.consume()); });
			Assert::IsTrue(bus.post(Consumable{}).isConsumed());
		}

		TEST_METHOD(MousePress_consume)
		{
			EventBus bus;
			const auto listenerA = bus.add<MousePress>(0, [](auto & event) { Assert::IsTrue(event.consume()); });
			const auto listenerB = bus.add<MousePress>(1, [](auto & event) { Assert::IsFalse(event.consume()); });
			Assert::IsTrue(bus.post(MousePress{ MouseButton::LEFT, {}, {}, 0.0f }).isConsumed());
		}
	};
}