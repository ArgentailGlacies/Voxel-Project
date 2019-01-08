
#include "allegro/Sprite.h"

#include "Common.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core
{
	TEST_CLASS(SpriteTest)
	{
	public:
		TEST_METHOD(Sprite_create)
		{
			Sprite sprite;
			Assert::IsTrue(sprite.create(32, 64));
			Assert::AreEqual({ 32, 64 }, sprite.getSize());
		}
	};
}