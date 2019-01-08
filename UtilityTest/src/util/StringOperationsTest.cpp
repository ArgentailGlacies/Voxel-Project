
#include "util/StringOperations.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace util::string
{
	TEST_CLASS(StringOperationsTest)
	{
	public:
		TEST_METHOD(string_split)
		{
			const auto dataA = split("Hello World! Foobar", " ");
			const auto dataB = split("Hello World! Foobar", "o");
			const auto dataC = split("Airplane, Car", ", ");

			Assert::AreEqual(3u, dataA.size());
			Assert::AreEqual({ "Hello" }, dataA[0u]);
			Assert::AreEqual({ "World!" }, dataA[1u]);
			Assert::AreEqual({ "Foobar" }, dataA[2u]);

			Assert::AreEqual(5u, dataB.size());
			Assert::AreEqual({ "Hell" }, dataB[0u]);
			Assert::AreEqual({ " W" }, dataB[1u]);
			Assert::AreEqual({ "rld! F" }, dataB[2u]);
			Assert::AreEqual({ "" }, dataB[3u]);
			Assert::AreEqual({ "bar" }, dataB[4u]);

			Assert::AreEqual(2u, dataC.size());
			Assert::AreEqual({ "Airplane" }, dataC[0u]);
			Assert::AreEqual({ "Car" }, dataC[1u]);
		}

		TEST_METHOD(string_replace)
		{
			Assert::AreEqual({ "That is a string" }, replace("This is a string", "is", "at"));
			Assert::AreEqual({ "HeFoolo World!" }, replace("Hello World!", "l", "Foo"));
			Assert::AreEqual({ "Value: 42/%d" }, replace("Value: %d/%d", "%d", "42"));
			Assert::AreEqual({ "data/sprites/sprite_name" }, replace("data/sprites/sprite_name.xml", ".xml", ""));
		}
		TEST_METHOD(string_replaceAll)
		{
			Assert::AreEqual({ "String" }, replaceAll("String", "", "foo"));
			Assert::AreEqual({ "HeFooFooo WorFood!" }, replaceAll("Hello World!", "l", "Foo"));
			Assert::AreEqual({ "Value: 42/42" }, replaceAll("Value: %d/%d", "%d", "42"));
		}
	};
}