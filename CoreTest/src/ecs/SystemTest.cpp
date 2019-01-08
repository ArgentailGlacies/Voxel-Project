
#include "ecs/System.h"
#include "mock/MockComponentRegistry.h"
#include "mock/MockSystem.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::ecs
{
	TEST_CLASS(SystemTest)
	{
	public:
		TEST_METHOD(System_data)
		{
			MockSystemDoubleComponent system{ m_components };
			const auto & dataA = system.getData<float>();
			const auto & dataB = system.getData<int>();

			Assert::IsTrue(&m_components.get<float>() == &dataA);
			Assert::IsTrue(&m_components.get<int>() == &dataB);
			Assert::ExpectException<std::invalid_argument>([&]() { system.getData<double>(); });
		}

	private:
		ComponentRegistry m_components = mockComponentRegistry();
	};
}