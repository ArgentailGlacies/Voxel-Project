
#include "asset/AssetUtil.h"
#include "mock/MockAsset.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::asset
{
	const util::Folder ASSET_FOLDER = "test";
	const util::Folder ASSET_NESTED_FOLDER = "test/nested";
	const util::File ASSET_FILE_A = "test/file.txt";
	const util::File ASSET_FILE_B = "test/nested/file.txt";
	const std::string ASSET_FILETYPE = "txt";

	TEST_CLASS(AssetBuilderTest)
	{
		TEST_METHOD(Builder_setupBuilder)
		{
			AssetRegistry registry;
			setupBuilder<AssetBuilder, int>(registry, "name", 1337);

			Assert::AreEqual<int>(1337, registry.get<int>("name"));
		}
	};

	TEST_CLASS(AssetLoaderTest)
	{
	public:
		AssetLoaderTest()
		{
			ASSET_NESTED_FOLDER.create(true);
			ASSET_FILE_A.write("42");
			ASSET_FILE_B.write("-1");
		}
		~AssetLoaderTest()
		{
			ASSET_FOLDER.erase(true);
		}

		TEST_METHOD(Loader_setupLoader)
		{
			AssetRegistry registry;
			setupLoader<AssetLoader, int>(registry, ASSET_FOLDER, ASSET_FILETYPE);

			Assert::AreEqual<int>(42, registry.get<int>("file"));
			Assert::AreEqual<int>(-1, registry.get<int>("nested/file"));
		}
	};
}