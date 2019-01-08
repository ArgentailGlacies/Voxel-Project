
#include "io/Folder.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace
{
	const std::string path = "testfolder";
}

namespace util::io
{
	TEST_CLASS(FolderTest)
	{
	public:
		FolderTest()
		{
			Folder{ path }.create();
		}
		~FolderTest()
		{
			Folder{ path }.erase(true);
		}

		TEST_METHOD(Folder_exists)
		{
			Folder folder{ path + "/folder" };

			Assert::IsFalse(folder.exists());
			folder.create();
			Assert::IsTrue(folder.exists());
		}
		TEST_METHOD(Folder_create)
		{
			Folder folderA{ path + "/folder" };
			Folder folderB{ path + "/nested/deeply" };

			Assert::IsTrue(folderA.create());
			Assert::IsFalse(folderB.create(false));
			Assert::IsTrue(folderB.create(true));
		}
		TEST_METHOD(Folder_erase)
		{
			Folder folderA{ path + "/folder" };
			Folder folderB{ path + "/nested" };
			Folder folderC{ path + "/nested/deeply" };
			folderA.create();
			folderC.create(true);

			Assert::IsTrue(folderA.erase());
			Assert::IsFalse(folderB.erase(false));
			Assert::IsTrue(folderB.erase(true));
		}

		TEST_METHOD(Folder_path)
		{
			Folder folder{ "path/to/folder" };

			Assert::AreEqual({ "path/to/folder" }, folder.path());
		}
		TEST_METHOD(Folder_root)
		{
			Folder folder{ "path/to/folder" };

			Assert::AreEqual({ "path/to" }, folder.root());
		}
		TEST_METHOD(Folder_name)
		{
			Folder folderA{ "path/to/folder" };
			Folder folderB{ "folder" };

			Assert::AreEqual({ "folder" }, folderA.name());
			Assert::AreEqual({ "folder" }, folderB.name());
		}
		
		TEST_METHOD(Folder_file)
		{
			Folder folderA{ "folder" };
			Folder folderB{ "" };

			Assert::AreEqual({ "folder/file.txt" }, folderA.file("file.txt").path());
			Assert::AreEqual({ "folder" }, folderA.file("").path());
			Assert::AreEqual({ "file.txt" }, folderB.file("file.txt").path());
		}
		TEST_METHOD(Folder_files)
		{
			File{ path + "/fileC.txt" }.write("Hello World!");
			File{ path + "/fileA.txt" }.write("Foo");
			File{ path + "/fileB.txt" }.write("Bar");
			Folder folderA{ path };
			Folder folderB{ "invalid_folder" };

			const auto filesA = folderA.files();
			const auto filesB = folderB.files();
			Assert::AreEqual(3u, filesA.size());
			Assert::AreEqual(0u, filesB.size());
			Assert::AreEqual({ "fileA" }, filesA[0].name());
			Assert::AreEqual({ "fileB" }, filesA[1].name());
			Assert::AreEqual({ "fileC" }, filesA[2].name());
		}
		TEST_METHOD(Folder_folder)
		{
			Folder folderA{ "folder" };
			Folder folderB{ "" };

			Assert::AreEqual({ "folder/subfolder" }, folderA.folder("subfolder").path());
			Assert::AreEqual({ "folder" }, folderA.folder("").path());
			Assert::AreEqual({ "subfolder" }, folderB.folder("subfolder").path());
		}
		TEST_METHOD(Folder_folders)
		{
			Folder{ path + "/folderC" }.create();
			Folder{ path + "/folderA" }.create();
			Folder{ path + "/folderB" }.create();
			Folder folderA{ path };
			Folder folderB{ "invalid_folder" };

			const auto foldersA = folderA.folders();
			const auto foldersB = folderB.folders();
			Assert::AreEqual(3u, foldersA.size());
			Assert::AreEqual(0u, foldersB.size());
			Assert::AreEqual({ "folderA" }, foldersA[0].name());
			Assert::AreEqual({ "folderB" }, foldersA[1].name());
			Assert::AreEqual({ "folderC" }, foldersA[2].name());
		}
	};
}