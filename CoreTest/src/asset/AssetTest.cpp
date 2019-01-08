
#include "asset/Asset.h"
#include "mock/MockAsset.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::asset
{
	TEST_CLASS(AssetTest)
	{
		TEST_METHOD(Reference_ctor)
		{
			Asset<int> asset;
			Asset<int>::Reference reference{ "asset", asset };

			Assert::AreEqual(1u, asset.references());
			Assert::IsFalse(asset.empty());
		}
		TEST_METHOD(Reference_dtor)
		{
			Asset<int> asset;
			{ Asset<int>::Reference reference{ "asset", asset }; }

			Assert::AreEqual(0u, asset.references());
			Assert::IsTrue(asset.empty());
		}

		TEST_METHOD(Reference_copy)
		{
			Asset<int> asset;
			Asset<int>::Reference referenceA{ "asset", asset };
			Asset<int>::Reference referenceB = referenceA;

			Assert::AreEqual(2u, asset.references());
		}
		TEST_METHOD(Reference_move)
		{
			Asset<int> asset;
			Asset<int>::Reference referenceA{ "asset", asset };
			Asset<int>::Reference referenceB = std::move(referenceA);

			Assert::AreEqual(1u, asset.references());
		}

		TEST_METHOD(Reference_assignCopy)
		{
			Asset<int> assetA, assetB;
			Asset<int>::Reference referenceA{ "assetA", assetA };
			Asset<int>::Reference referenceB{ "assetB", assetB };

			referenceB = referenceA;
			Assert::AreEqual(2u, assetA.references());
			Assert::AreEqual(0u, assetB.references());
		}
		TEST_METHOD(Reference_assignMove)
		{
			Asset<int> assetA, assetB;
			Asset<int>::Reference referenceA{ "assetA", assetA };
			Asset<int>::Reference referenceB{ "assetB", assetB };

			referenceB = std::move(referenceA);
			Assert::AreEqual(1u, assetA.references());
			Assert::AreEqual(0u, assetB.references());
		}

		TEST_METHOD(Reference_compare)
		{
			Asset<int> assetA;
			Asset<int> assetB;
			Asset<int>::Reference referenceA{ "assetA", assetA };
			Asset<int>::Reference referenceB{ "assetB", assetB };
			Asset<int>::Reference referenceC{ "assetC", assetA };

			Assert::IsFalse(referenceA == referenceB);
			Assert::IsTrue(referenceA != referenceB);
			Assert::IsTrue(referenceA == referenceC);
			Assert::IsFalse(referenceA != referenceC);
		}

		TEST_METHOD(Reference_access)
		{
			Asset<int> asset;
			Asset<int>::Reference reference{ "asset", asset };

			asset.resource() = 42;
			Assert::AreEqual<int>(42, reference);
		}
		TEST_METHOD(Reference_use)
		{
			Asset<MockAsset> asset;
			Asset<MockAsset>::Reference reference{ "asset", asset };

			asset.resource() = MockAsset{ 1337, 3.14f };
			Assert::AreEqual(1337, reference->m_int);
			Assert::AreEqual(3.14f, reference->m_float, 0.001f);
		}

		// ...

		TEST_METHOD(Asset_factory)
		{
			Asset<int> assetA;
			Asset<int> assetB{ []() { return std::make_unique<int>(42); } };
			Asset<int>::Reference referenceA{ "assetA", assetA };
			Asset<int>::Reference referenceB{ "assetB", assetB };

			Assert::AreEqual<int>(0, referenceA);
			Assert::AreEqual<int>(42, referenceB);
		}
	};
}