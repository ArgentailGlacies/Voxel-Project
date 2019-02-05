
#include "allegro/Bitmap.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::allegro
{
	TEST_CLASS(BitmapTest)
	{
	public:
		~BitmapTest()
		{
			m_fileA.erase();
			m_fileB.erase();
		}

		TEST_METHOD(Bitmap_ctor)
		{
			Bitmap temporary{ 32, 32 };

			Bitmap bitmapA;
			//Bitmap bitmapB = temporary;
			Bitmap bitmapC = std::move(temporary);
			Bitmap bitmapD = m_fileA;

			Assert::IsNull(bitmapA.handle());
			//Assert::IsNotNull(bitmapB.handle());
			Assert::IsNotNull(bitmapC.handle());
			Assert::IsNotNull(bitmapD.handle());
			//Assert::IsFalse(bitmapB.handle() == bitmapC.handle());
		}

		TEST_METHOD(Bitmap_child)
		{
			Bitmap parent{ 32, 32 };
			Bitmap child = parent.child({ 8, 8 }, { 12, 16 });

			Assert::IsNotNull(child.handle());
			Assert::IsFalse(child.handle() == parent.handle());
		}

		TEST_METHOD(Bitmap_create)
		{
			Bitmap bitmap;

			Assert::IsTrue(bitmap.create(32, 32));
		}
		TEST_METHOD(Bitmap_load)
		{
			Bitmap bitmap{ 32, 32 };
			bitmap.save(m_fileA);
			bitmap.clear();

			Assert::IsTrue(bitmap.load(m_fileA));
		}
		TEST_METHOD(Bitmap_save)
		{
			Bitmap bitmapA, bitmapB{ 32, 16 };
			bitmapA.save(m_fileA);
			bitmapB.save(m_fileB);

			Assert::IsFalse(m_fileA.exists());
			Assert::IsTrue(m_fileB.exists());
		}

		TEST_METHOD(Bitmap_clear)
		{
			Bitmap bitmap{ 32, 32 };
			bitmap.clear();

			Assert::IsNull(bitmap.handle());
		}

		// ...

		TEST_METHOD(Bitmap_lock)
		{
			Bitmap bitmapA, bitmapB{ 32, 24 };
			
			const auto regionA = bitmapA.lock(Bitmap::LockMode::READONLY);
			const auto regionB = bitmapB.lock(Bitmap::LockMode::READONLY);
			Assert::IsNull(regionA.data());
			Assert::IsNotNull(regionB.data());
		}

	private:
		const util::File m_fileA = "test_bitmap_file_a.png";
		const util::File m_fileB = "test_bitmap_file_b.png";
	};
}