
#include "io/Folder.h"
#include "world/render/BlockTextureAtlas.h"

#include <allegro5/bitmap_io.h>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::world::render
{
	TEST_CLASS(BlockTextureAtlasTest)
	{
	public:
		BlockTextureAtlasTest() { initialize(); }
		~BlockTextureAtlasTest() { deinitialize(); }

		TEST_METHOD(BlockTextureAtlas_attachFile)
		{
			BlockTextureAtlas atlas;
			const auto handleA = atlas.attach("test_files/bitmapA.png");
			const auto handleB = atlas.attach("test_files/bitmapB.png");
			const auto handleC = atlas.attach("test_files/bitmapB.png");
			const auto handleD = atlas.attach("invalid");

			Assert::AreEqual(0u, handleA);
			Assert::AreEqual(1u, handleB);
			Assert::AreEqual(1u, handleC);
			Assert::AreEqual(0u, handleD);
			Assert::AreEqual(2u, atlas.size());
		}
		TEST_METHOD(BlockTextureAtlas_attachBitmap)
		{
			BlockTextureAtlas atlas;
			const auto handleA = atlas.attach(gsl::make_not_null(al_create_bitmap(32, 32)));
			const auto handleB = atlas.attach(gsl::make_not_null(al_create_bitmap(96, 64)));
			const auto handleC = atlas.attach(gsl::make_not_null(al_create_bitmap(64, 32)));

			Assert::AreEqual(0u, handleA);
			Assert::AreEqual(1u, handleB);
			Assert::AreEqual(7u, handleC);
			Assert::AreEqual(9u, atlas.size());
		}

		TEST_METHOD(BlockTextureAtlas_build)
		{
			BlockTextureAtlas atlas;
			atlas.attach(gsl::make_not_null(al_create_bitmap(32, 32)));
			atlas.build();
		}

	private:
		void initialize()
		{
			util::Folder{ "test_files" }.create();
			
			ALLEGRO_BITMAP * bitmap;
			bitmap = al_create_bitmap(32, 32);
			al_save_bitmap("test_files/bitmapA.png", bitmap);
			al_destroy_bitmap(bitmap);
			bitmap = al_create_bitmap(32, 32);
			al_save_bitmap("test_files/bitmapB.png", bitmap);
			al_destroy_bitmap(bitmap);
		}
		void deinitialize()
		{
			util::Folder{ "test_files" }.erase(true);
		}
	};
}