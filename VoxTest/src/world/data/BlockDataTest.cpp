
#include "world/data/BlockData.h"
#include "world/util/Limits.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::data
{
	TEST_CLASS(BlockDataTest)
	{
	public:
		TEST_METHOD(BlockData_sizeof)
		{
			// Block data must be guaranteed to take up exactly four bytes, which puts certain
			// constraints on what data the block can store.
			static_assert(sizeof(BlockData) == 4);
			static_assert(MAX_BLOCK_TYPE < 4096);
			static_assert(MAX_BLOCK_LIGHT < 32);
		}

		TEST_METHOD(BlockData_ctor)
		{
			BlockData dataA;
			BlockData dataB{ 0xDEADBEEF };
			BlockData dataC{ 42, { 3, 1, 4, 13 } };

			Assert::AreEqual(0u, dataA.getData());
			Assert::AreEqual(0xDEADBEEFu, dataB.getData());
			validate(dataC, 42, { 3, 1, 4, 13 });
		}

		TEST_METHOD(BlockData_hasLighting)
		{
			BlockData dataA, dataB;
			dataA.setType(42);
			dataB.setLight({ 3, 1, 4, 0 });

			Assert::IsFalse(dataA.hasLighting());
			Assert::IsTrue(dataB.hasLighting());
		}

		TEST_METHOD(BlockData_setData)
		{
			BlockData data;
			data.setData(0x12345678);

			Assert::AreEqual(0x12345678u, data.getData());
		}
		TEST_METHOD(BlockData_setType)
		{
			BlockData dataA, dataB;
			dataA.setType(31);
			dataB.setType(15);

			validate(dataA, 31, {});
			validate(dataB, 15, {});
		}
		TEST_METHOD(BlockData_setLight)
		{
			BlockData dataA, dataB;
			dataA.setLight({ 31, 31, 31, 31 });
			dataB.setLight({ 1, 2, 3, 5 });

			validate(dataA, 0, { 31, 31, 31, 31 });
			validate(dataB, 0, { 1, 2, 3, 5 });
		}
		TEST_METHOD(BlockData_setAll)
		{
			BlockData data;
			data.setType(17);
			data.setLight({ 3, 31, 26, 14 });

			validate(data, 17, { 3, 31, 26, 14 });
		}

	private:
		void validate(BlockData data, unsigned int type, const Light & light) const
		{
			Assert::AreEqual(type, data.getType());
			Assert::AreEqual<glm::uvec4>(light, data.getLight());
		}
	};
}