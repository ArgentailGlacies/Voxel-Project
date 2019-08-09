
#include "allegro/TextElement.h"

#include "allegro/Font.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::allegro
{
	TEST_CLASS(TextElementTest)
	{
	public:
		TEST_METHOD(givenNoLimits_whenSplitting_thenSingleString)
		{
			const auto tasks = split("Hello World!", 0, std::numeric_limits<int>::max());

			Assert::AreEqual(1u, tasks.size());
			Assert::AreEqual({ 96, 8 }, tasks[0].m_size);
		}

		TEST_METHOD(givenSimpleLimit_whenSplitting_thenTwoStrings)
		{
			const auto tasks = split("Hello World!", 0, 60);

			Assert::AreEqual(2u, tasks.size());
			Assert::AreEqual({ 40, 8 }, tasks[0].m_size);
			Assert::AreEqual({ 48, 8 }, tasks[1].m_size);
		}

		TEST_METHOD(givenLinebreakCharacter_whenSplitting_thenSplitOnCharacter)
		{
			const auto tasks = split("foo.bar", 0, 50);

			Assert::AreEqual(2u, tasks.size());
			Assert::AreEqual({ 32, 8 }, tasks[0].m_size);
			Assert::AreEqual({ 24, 8 }, tasks[1].m_size);
		}

		TEST_METHOD(givenStrictSplit_whenSplitting_thenSplitWherePossible)
		{
			const auto tasks = split("foo.bar", 0, 25);

			Assert::AreEqual(3u, tasks.size());
			Assert::AreEqual({ 24, 8 }, tasks[0].m_size);
			Assert::AreEqual({ 8, 8 }, tasks[1].m_size);
			Assert::AreEqual({ 24, 8 }, tasks[2].m_size);
		}

		TEST_METHOD(givenImpossibleSpace_whenSplitting_thenSplitOnAllCharacters)
		{
			const auto tasks = split("abc", 0, 0);

			Assert::AreEqual(3u, tasks.size());
			Assert::AreEqual({ 8, 8 }, tasks[0].m_size);
			Assert::AreEqual({ 8, 8 }, tasks[1].m_size);
			Assert::AreEqual({ 8, 8 }, tasks[2].m_size);
		}

		TEST_METHOD(givenWhitespaceSplit_whenSplitting_thenNoWhitespace)
		{
			const auto tasks = split("foo bar", 0, 25);

			Assert::AreEqual(2u, tasks.size());
			Assert::AreEqual({ 24, 8 }, tasks[0].m_size);
			Assert::AreEqual({ 24, 8 }, tasks[1].m_size);
		}

		TEST_METHOD(givenNewline_whenSplitting_thenMultipleLines)
		{
			const auto tasks = split("foo\nbar", 0, std::numeric_limits<int>::max());

			Assert::AreEqual(2u, tasks.size());
			Assert::AreEqual({ 24, 8 }, tasks[0].m_size);
			Assert::AreEqual({ 24, 8 }, tasks[1].m_size);
		}

		TEST_METHOD(givenMultipleNewlines_whenSplitting_thenMultipleLines)
		{
			const auto tasks = split("\n\n", 0, std::numeric_limits<int>::max());

			Assert::AreEqual(2u, tasks.size());
			Assert::AreEqual({ 0, 8 }, tasks[0].m_size);
			Assert::AreEqual({ 0, 8 }, tasks[1].m_size);
		}

		TEST_METHOD(givenStartingPosition_whenSplitting_thenCorrectLines)
		{
			const auto tasks = split("Hi foo and bar", 80, 100);

			Assert::AreEqual(2u, tasks.size());
			Assert::AreEqual({ 16, 8 }, tasks[0].m_size);
			Assert::AreEqual({ 88, 8 }, tasks[1].m_size);
		}

	private:
		inline std::vector<ElementText::Task> split(const std::string & string, int position, int width)
		{
			ElementText element{ m_font.handle(), string, glm::vec4{}, false, false };
			return element.split(position, width);
		}

		Font m_font;
	};
}