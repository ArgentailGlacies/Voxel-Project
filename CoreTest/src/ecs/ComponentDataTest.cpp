
#include "ecs/ComponentData.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::ecs
{
	TEST_CLASS(ComponentDataTest)
	{
	public:
		TEST_METHOD(ComponentData_add)
		{
			ComponentData<int> data;
			data[7] = 3;
			data[2] = 0;

			Assert::IsFalse(data.has(0));
			Assert::IsTrue(data.has(2));
			Assert::IsTrue(data.has(7));
		}

		TEST_METHOD(ComponentData_get)
		{
			ComponentData<int> data;
			data[5] = 4;
			data[7] = -1;
			data[2] = 2;

			Assert::AreEqual(2, data[2]);
			Assert::AreEqual(4, data[5]);
			Assert::AreEqual(-1, data[7]);
		}
	};
}