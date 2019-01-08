
#include "event/EventContainer.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::event
{
	TEST_CLASS(EventCallbackTest)
	{
	public:
		TEST_METHOD(EventCallback_add)
		{
			CallbackContainer<int> container;
			const auto id = container.add(5, 0);

			Assert::AreEqual(1u, container.size());
		}
		TEST_METHOD(EventCallback_remove)
		{
			CallbackContainer<int> container;
			const auto id = container.add(5, 0);

			container.remove(5, id);
			Assert::AreEqual(0u, container.size());
		}
	};
}