
#include "util/StringParsing.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace util::string
{
	TEST_CLASS(StringParsingTest)
	{
	public:
		TEST_METHOD(String_parseBool)
		{
			Parser<bool> parser;
			Assert::IsTrue(parser.parse("true"));
			Assert::IsFalse(parser.parse("false"));
			Assert::IsFalse(parser.parse(""));
			Assert::IsTrue(parser.parse("invalid", true));
		}
		TEST_METHOD(String_parseDouble)
		{
			Parser<double> parser;
			Assert::AreEqual(3.14, parser.parse("3.14"), 0.0001);
			Assert::AreEqual(-42.001, parser.parse("-42.001"), 0.0001);
			Assert::AreEqual(5.3, parser.parse("invalid", 5.3), 0.0001);
		}
		TEST_METHOD(String_parseFloat)
		{
			Parser<float> parser;
			Assert::AreEqual(3.14f, parser.parse("3.14f"), 0.0001f);
			Assert::AreEqual(-42.001f, parser.parse("-42.001"), 0.0001f);
			Assert::AreEqual(5.3f, parser.parse("invalid", 5.3f), 0.0001f);
		}
		TEST_METHOD(String_parseInt)
		{
			Parser<int> parser;
			Assert::AreEqual(42, parser.parse("42"));
			Assert::AreEqual(-15, parser.parse("-0xF"));
			Assert::AreEqual(1337, parser.parse("invalid", 1337));
		}
		TEST_METHOD(String_parseUnsignedInt)
		{
			Parser<unsigned int> parser;
			Assert::AreEqual(42u, parser.parse("42u"));
			Assert::AreEqual(15u, parser.parse("0xF"));
			Assert::AreEqual(1337u, parser.parse("invalid", 1337u));
		}
		TEST_METHOD(String_parseString)
		{
			Parser<std::string> parser;
			Assert::AreEqual({ "Hello World!" }, parser.parse("Hello World!"));
		}

		TEST_METHOD(String_parseVector)
		{
			Parser<std::vector<int>> parser;

			const auto vector = parser.parse("3, 1, 4");
			Assert::AreEqual(3u, vector.size());
			Assert::AreEqual(3, vector[0]);
			Assert::AreEqual(1, vector[1]);
			Assert::AreEqual(4, vector[2]);
		}

		TEST_METHOD(String_parseVec2)
		{
			Parser<glm::vec2> parser;
			Assert::AreEqual({ 3.1f, 0.75f }, parser.parse("3.1, 0.75"));
			Assert::AreEqual(glm::vec2{ 1.0f }, parser.parse("invalid", glm::vec2{ 1.0f }));
		}
		TEST_METHOD(String_parseVec3)
		{
			Parser<glm::vec3> parser;
			Assert::AreEqual({ 3.1f, 0.75f, -0.333f }, parser.parse("3.1, 0.75, -.333"));
			Assert::AreEqual(glm::vec3{ 1.0f }, parser.parse("invalid", glm::vec3{ 1.0f }));
		}
		TEST_METHOD(String_parseVec4)
		{
			Parser<glm::vec4> parser;
			Assert::AreEqual({ 3.1f, 0.75f, -0.333f, 0.0f }, parser.parse("3.1, 0.75, -.333, 0"));
			Assert::AreEqual(glm::vec4{ 1.0f }, parser.parse("invalid", glm::vec4{ 1.0f }));
		}
	};
}