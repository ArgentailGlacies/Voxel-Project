
#include "allegro/SpriteFactory.h"

#include <allegro5/bitmap_io.h>

#include "Common.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::allegro
{
	TEST_CLASS(SpriteLoaderTest)
	{
	public:
		TEST_CLASS_INITIALIZE(initialize)
		{
			util::Folder{ "test" }.create();

			util::File{ "test/sprite.xml" }.write(R"(
				<bitmap path="test/bitmap.png" />
				<frames>
					<foo pos="4, 0" size="16, 16" center="8, 8" tint="0.25, 0.5, 0.75, 1" />
					<bar />
				</frames>
				)");

			ALLEGRO_BITMAP * bitmap;
			bitmap = al_create_bitmap(32, 16);
			al_save_bitmap("test/bitmap.png", bitmap);
			al_destroy_bitmap(bitmap);
		}
		TEST_CLASS_CLEANUP(cleanup)
		{
			util::Folder{ "test" }.erase(true);
		}

		TEST_METHOD(SpriteLoader_load)
		{
			SpriteLoader loader;
			auto sprite = loader.load("test/sprite.xml");

			Assert::IsNotNull(sprite->getHandle());
			Assert::AreEqual({ 32.0f, 16.0f }, sprite->getSize());

			Assert::IsTrue(sprite->hasFrame("foo"));
			Assert::AreEqual({ 16.0f, 16.0f }, sprite->getSize("foo"));
			Assert::AreEqual({ 4.0f, 0.0f }, sprite->getPos("foo"));
			Assert::AreEqual({ 8.0f, 8.0f }, sprite->getCenter("foo"));
			Assert::AreEqual(0.25f, sprite->getTint("foo").r, 0.001f);
			Assert::AreEqual(0.50f, sprite->getTint("foo").g, 0.001f);
			Assert::AreEqual(0.75f, sprite->getTint("foo").b, 0.001f);
			Assert::AreEqual(1.00f, sprite->getTint("foo").a, 0.001f);

			Assert::IsTrue(sprite->hasFrame("bar"));
			Assert::AreEqual({ 0.0f, 0.0f }, sprite->getSize("bar"));
			Assert::AreEqual({ 0.0f, 0.0f }, sprite->getPos("bar"));
			Assert::AreEqual({ 0.0f, 0.0f }, sprite->getCenter("bar"));
			Assert::AreEqual(1.0f, sprite->getTint("bar").r, 0.001f);
			Assert::AreEqual(1.0f, sprite->getTint("bar").g, 0.001f);
			Assert::AreEqual(1.0f, sprite->getTint("bar").b, 0.001f);
			Assert::AreEqual(1.0f, sprite->getTint("bar").a, 0.001f);
		}
	};
}