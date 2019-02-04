
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
			Bitmap temporary;
			temporary.create(32, 32);

			Bitmap bitmapA;
			Bitmap bitmapB = temporary;
			Bitmap bitmapC = std::move(temporary);

			Assert::IsNull(bitmapA.handle());
			Assert::IsNotNull(bitmapB.handle());
			Assert::IsNotNull(bitmapC.handle());
			Assert::IsFalse(bitmapB.handle() == bitmapC.handle());
		}

		TEST_METHOD(Bitmap_child)
		{
			Bitmap parent, child;
			parent.create(32, 32);
			child = parent.child({ 8, 8 }, { 12, 16 });

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
			Bitmap bitmap;
			bitmap.create(32, 32);
			bitmap.save(m_fileA);
			bitmap.clear();

			Assert::IsTrue(bitmap.load(m_fileA));
		}
		TEST_METHOD(Bitmap_save)
		{
			Bitmap bitmapA, bitmapB;
			bitmapA.create(32, 32);
			bitmapA.save(m_fileA);
			bitmapB.save(m_fileB);

			Assert::IsTrue(m_fileA.exists());
			Assert::IsFalse(m_fileB.exists());
		}

		TEST_METHOD(Bitmap_clear)
		{
			Bitmap bitmap;
			bitmap.create(32, 32);
			bitmap.clear();

			Assert::IsNull(bitmap.handle());
		}

		// ...

		TEST_METHOD(Bitmap_lock)
		{
			Bitmap bitmapA, bitmapB;
			bitmapA.create(32, 32);
			
			const auto regionA = bitmapA.lock(Bitmap::LockMode::READONLY);
			const auto regionB = bitmapB.lock(Bitmap::LockMode::READONLY);
			Assert::IsNotNull(regionA.data());
			Assert::IsNull(regionB.data());
		}

	private:
		const util::File m_fileA = "test_bitmap_file_a.png";
		const util::File m_fileB = "test_bitmap_file_b.png";
	};
}