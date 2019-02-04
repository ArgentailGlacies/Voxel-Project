
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
			const auto handleA = atlas.attach(core::Bitmap{ 32, 32 });
			const auto handleB = atlas.attach(core::Bitmap{ 96, 64 });
			const auto handleC = atlas.attach(core::Bitmap{ 64, 32 });

			Assert::AreEqual(0u, handleA);
			Assert::AreEqual(1u, handleB);
			Assert::AreEqual(7u, handleC);
			Assert::AreEqual(9u, atlas.size());
		}

		TEST_METHOD(BlockTextureAtlas_build)
		{
			BlockTextureAtlas atlas;
			atlas.attach(core::Bitmap{ 32, 32 });
			atlas.build();
		}

	private:
		void initialize()
		{
			util::Folder{ "test_files" }.create();
			
			core::Bitmap{ 32, 32 }.save("test_files/bitmapA.png");
			core::Bitmap{ 32, 32 }.save("test_files/bitmapB.png");
		}
		void deinitialize()
		{
			util::Folder{ "test_files" }.erase(true);
		}
	};
}