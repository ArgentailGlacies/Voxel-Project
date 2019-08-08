
#include "allegro/TextElement.h"

#include "allegro/Font.h"

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::allegro
{
	TEST_CLASS(TextElementTest)
	{
	public:
		TEST_METHOD(ElementText_split)
		{
			ElementText elementA{ m_font.handle(), "Hello World!", glm::vec4{}, false, false };
			ElementText elementB{ m_font.handle(), "foo bar", glm::vec4{}, false, false };
			ElementText elementC{ m_font.handle(), "foo.bar", glm::vec4{}, false, false };

			const auto tasksA = elementA.split(0, std::numeric_limits<int>::max());
			const auto tasksB = elementA.split(0, 60);
			const auto tasksC = elementB.split(0, 40);
			const auto tasksD = elementB.split(0, 40);
			const auto tasksE = elementB.split(0, 25);
			
			// Single task when not constrained
			Assert::AreEqual(1u, tasksA.size());
			Assert::AreEqual({ 96, 8 }, tasksA[0].m_size);

			// Splitting when constrained to small space
			Assert::AreEqual(2u, tasksB.size());
			Assert::AreEqual({ 40, 8 }, tasksB[0].m_size);
			Assert::AreEqual({ 48, 8 }, tasksB[1].m_size);

			// Splitting at a space should not render space
			Assert::AreEqual(2u, tasksC.size());
			Assert::AreEqual({ 24, 8 }, tasksC[0].m_size);
			Assert::AreEqual({ 24, 8 }, tasksC[1].m_size);

			// Splitting at a non-space should render character
			Assert::AreEqual(2u, tasksD.size());
			Assert::AreEqual({ 32, 8 }, tasksD[0].m_size);
			Assert::AreEqual({ 24, 8 }, tasksD[1].m_size);

			// Splitting without linebreak character splits wherever
			Assert::AreEqual(3u, tasksE.size());
			Assert::AreEqual({ 24, 8 }, tasksE[0].m_size);
			Assert::AreEqual({ 8, 8 }, tasksE[1].m_size);
			Assert::AreEqual({ 24, 8 }, tasksE[2].m_size);
		}

	private:
		Font m_font;
	};
}