
#include "world/util/Side.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::util
{
	TEST_CLASS(SideTest)
	{
	public:
		TEST_METHOD(Side_fromName)
		{
			Assert::IsTrue(Side::POS_X == Side::fromName("pos_x"));
			Assert::IsTrue(Side::POS_Y == Side::fromName("pos_y"));
			Assert::IsTrue(Side::POS_Z == Side::fromName("pos_z"));
			Assert::IsTrue(Side::NEG_X == Side::fromName("neg_x"));
			Assert::IsTrue(Side::NEG_Y == Side::fromName("neg_y"));
			Assert::IsTrue(Side::NEG_Z == Side::fromName("neg_z"));
			Assert::IsTrue(Side::OTHER == Side::fromName("invalid"));
		}
		TEST_METHOD(Side_fromList)
		{
			Assert::AreEqual(7u, Side::fromList("all").size());
			Assert::AreEqual(6u, Side::fromList("faces").size());
			Assert::AreEqual(4u, Side::fromList("sides").size());
			Assert::AreEqual(2u, Side::fromList("pos_x, pos_z").size());
			Assert::AreEqual(4u, Side::fromList("neg_y, sides").size());
		}

		TEST_METHOD(Side_positive)
		{
			Assert::IsTrue(Side::POS_X.positive());
			Assert::IsTrue(Side::POS_Y.positive());
			Assert::IsTrue(Side::POS_Z.positive());
			Assert::IsFalse(Side::NEG_X.positive());
			Assert::IsFalse(Side::NEG_Y.positive());
			Assert::IsFalse(Side::NEG_Z.positive());
			Assert::IsFalse(Side::OTHER.positive());
		}

		TEST_METHOD(Side_flip)
		{
			Assert::IsTrue(Side::POS_X.flip() == Side::NEG_X);
			Assert::IsTrue(Side::POS_Y.flip() == Side::NEG_Y);
			Assert::IsTrue(Side::POS_Z.flip() == Side::NEG_Z);
			Assert::IsTrue(Side::NEG_X.flip() == Side::POS_X);
			Assert::IsTrue(Side::NEG_Y.flip() == Side::POS_Y);
			Assert::IsTrue(Side::NEG_Z.flip() == Side::POS_Z);
			Assert::IsTrue(Side::OTHER.flip() == Side::OTHER);
		}
	};
}