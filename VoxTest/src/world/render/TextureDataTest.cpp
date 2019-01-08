
#include "world/render/TextureData.h"
#include "world/util/Limits.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::render
{
	TEST_CLASS(TextureDataTest)
	{
	public:
		TEST_METHOD(TextureData_sizeof)
		{
			// Texture data must be guaranteed to take up exactly four bytes, which puts certain
			// constraints on what data the block can store.
			static_assert(sizeof(TextureData) == 4);
			static_assert(MAX_BLOCK_TEXTURE_ID < 65536);
			static_assert(MAX_BLOCK_TEXTURE_META < 256);
			static_assert(MAX_BLOCK_TEXTURE_HASH < 256);
		}

		TEST_METHOD(TextureData_equals)
		{
			TextureData dataA{ 12516 };
			TextureData dataB{ 1071, 241, 195 };
			TextureData dataC{ 1071, 241, 31 };

			// Equal hash values counts as otherwise equal
			Assert::IsFalse(dataA == dataB);
			Assert::IsTrue(dataB == dataC);
		}
		TEST_METHOD(TextureData_notEquals)
		{
			TextureData dataA{ 41671 };
			TextureData dataB{ 61, 2, 0 };
			TextureData dataC{ 61, 2, 7 };

			Assert::IsTrue(dataA != dataB);
			Assert::IsFalse(dataB != dataC);
		}

		TEST_METHOD(TextureData_ctor)
		{
			TextureData dataA{ 41671 };
			TextureData dataB{ 1071, 241, 195 };

			validate(dataA, 41671, 0, 0);
			validate(dataB, 1071, 241, 195);
		}

		TEST_METHOD(TextureData_setTexture)
		{
			TextureData dataA, dataB;
			dataA.setTexture(24616);
			dataB.setTexture(19617);

			validate(dataA, 24616, 0, 0);
			validate(dataB, 19617, 0, 0);
		}
		TEST_METHOD(TextureData_setMeta)
		{
			TextureData dataA, dataB;
			dataA.setMeta(255);
			dataB.setMeta(71);

			validate(dataA, 0, 255, 0);
			validate(dataB, 0, 71, 0);
		}
		TEST_METHOD(TextureData_setHash)
		{
			TextureData dataA, dataB;
			dataA.setHash(61);
			dataB.setHash(255);

			validate(dataA, 0, 0, 61);
			validate(dataB, 0, 0, 255);
		}
		TEST_METHOD(TextureData_setAll)
		{
			TextureData data;
			data.setTexture(31671);
			data.setMeta(67);
			data.setHash(31);

			validate(data, 31671, 67, 31);
		}

	private:
		void validate(
			TextureData data, unsigned int texture, unsigned int meta, unsigned int hash
		) const
		{
			Assert::AreEqual(texture, data.getTexture());
			Assert::AreEqual(meta, data.getMeta());
			Assert::AreEqual(hash, data.getHash());
		}
	};
}