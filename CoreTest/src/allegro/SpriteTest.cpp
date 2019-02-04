
#include "allegro/Sprite.h"
#include "io/Folder.h"

#include <allegro5/bitmap_io.h>

#include "Common.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::allegro
{
	TEST_CLASS(SpriteTest)
	{
	public:
		SpriteTest()
		{
			util::Folder{ "test" }.create();

			ALLEGRO_BITMAP * bitmap;
			bitmap = al_create_bitmap(32, 16);
			al_save_bitmap("test/bitmap.png", bitmap);
			al_destroy_bitmap(bitmap);
		}
		~SpriteTest()
		{
			util::Folder{ "test" }.erase(true);
		}

		TEST_METHOD(Sprite_create)
		{
			Sprite sprite;
			Assert::IsTrue(sprite.create(32, 64));
			Assert::AreEqual({ 32, 64 }, sprite.getSize());
		}
		TEST_METHOD(Sprite_load)
		{
			Sprite sprite;
			Assert::IsTrue(sprite.load("test/bitmap.png"));
			Assert::AreEqual({ 32, 16 }, sprite.getSize());
		}

		TEST_METHOD(Sprite_addFrame)
		{
			Sprite sprite;
			sprite.addFrame("foo", {});
			sprite.addFrame("bar", {});

			Assert::IsTrue(sprite.hasFrame("foo"));
			Assert::IsTrue(sprite.hasFrame("bar"));
			Assert::IsFalse(sprite.hasFrame("cin"));
		}
		TEST_METHOD(Sprite_getData)
		{
			Sprite::Frame frame;
			frame.m_center = glm::vec2{ 1.0f, 3.0f };
			frame.m_pos = glm::vec2{ 10.0f, 24.0f };
			frame.m_size = glm::vec2{ 16.0f, 8.0f };
			frame.m_tint = glm::vec4{ 0.25f, 0.5f, 0.75f, 1.0f };

			Sprite sprite;
			sprite.addFrame("foo", frame);

			Assert::AreEqual({ 1.0f, 3.0f }, sprite.getCenter("foo"));
			Assert::AreEqual({ 10.0f, 24.0f }, sprite.getPos("foo"));
			Assert::AreEqual({ 16.0f, 8.0f }, sprite.getSize("foo"));
			Assert::AreEqual({ 0.25f, 0.5f, 0.75f, 1.0f }, sprite.getTint("foo"));
		}
	};
}