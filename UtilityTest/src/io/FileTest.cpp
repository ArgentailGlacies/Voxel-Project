
#include "io/File.h"

#include <unordered_map>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace
{
	const std::string filepath = "testfile.txt";
	const std::string alternative = "testotherfile.txt";
}

namespace util::io
{
	TEST_CLASS(FileTest)
	{
	public:
		~FileTest()
		{
			File{ filepath }.erase();
			File{ alternative }.erase();
		}

		TEST_METHOD(File_exists)
		{
			File file{ filepath };

			Assert::IsFalse(file.exists());
			file.write("Hello World!");
			Assert::IsTrue(file.exists());
		}
		TEST_METHOD(File_erase)
		{
			File file{ filepath };
			file.write("Hello World!");

			Assert::IsTrue(file.erase());
			Assert::IsFalse(file.exists());
		}

		TEST_METHOD(File_path)
		{
			File file{ "path/to/file.txt" };

			Assert::AreEqual({ "path/to/file.txt" }, file.path());
		}
		TEST_METHOD(File_root)
		{
			File file{ "path/to/file.txt" };

			Assert::AreEqual({ "path/to" }, file.root());
		}
		TEST_METHOD(File_name)
		{
			File fileA{ "path/to/file.txt" };
			File fileB{ "file.txt" };
			File fileC{ "path/to/file" };
			File fileD{ "file" };

			Assert::AreEqual({ "file" }, fileA.name());
			Assert::AreEqual({ "file" }, fileB.name());
			Assert::AreEqual({ "file" }, fileC.name());
			Assert::AreEqual({ "file" }, fileD.name());
		}
		TEST_METHOD(File_extension)
		{
			File file{ "path/to/file.txt" };

			Assert::AreEqual({ "txt" }, file.extension());
		}
		
		TEST_METHOD(File_write)
		{
			File file{ filepath };
			File other{ alternative };

			Assert::IsTrue(file.write("Hello World!"));
			Assert::IsTrue(other.write(reinterpret_cast<const unsigned char*>("Foo"), 3u));
		}
		TEST_METHOD(File_read)
		{
			File file{ filepath };
			file.write("Hello World!");
				
			Assert::AreEqual({ "Hello World!" }, file.read());
		}
		TEST_METHOD(File_parse)
		{
			const auto data = "#include \"" + alternative + "\"\nHi!";
			File{ alternative }.write("Foo\nBar");

			File file{ filepath };
			file.write(data + data);

			Assert::AreEqual({ "Foo\nBar\n\nHi!\n" }, file.parse());
		}

		// ...

		TEST_METHOD(File_hashmapKey)
		{
			const File fileA = "foo";
			const File fileB = "bar";

			std::unordered_map<File, int> map;
			map[fileA] = 1;
			map[fileB] = 2;

			Assert::AreEqual(2, map[fileB]);
			Assert::AreEqual(1, map[fileA]);
		}
	};
}
