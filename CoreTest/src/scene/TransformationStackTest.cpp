
#include "scene/TransformationStack.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::scene
{
	const auto positionV = glm::vec3{ 3.0f, 1.0f, 4.0f };
	const auto rotationV = glm::vec3{ 0.0f, glm::radians(90.0f), 0.0f };
	const auto scaleV = glm::vec3{ -1.0f, 2.0f, 0.5f };

	const auto positionT = glm::translate(glm::mat4{ 1.0f }, positionV);
	const auto rotationT = glm::rotate(glm::mat4{ 1.0f }, glm::radians(90.0f), glm::vec3{ 0.0f, 1.0f, 0.0f });
	const auto scaleT = glm::scale(glm::mat4{ 1.0f }, scaleV);

	TEST_CLASS(TransformationStackTest)
	{
	public:
		TEST_METHOD(TransformationStack_ctor)
		{
			TransformationStack stack;

			Assert::AreEqual(glm::mat4{ 1.0f }, stack.top());
		}

		TEST_METHOD(TransformationStack_pushTranslation)
		{
			TransformationStack stack;
			stack.pushTranslation(positionV);

			Assert::AreEqual(positionT, stack.top());
		}
		TEST_METHOD(TransformationStack_pushRotation)
		{
			TransformationStack stack;
			stack.pushRotation(rotationV);

			Assert::AreEqual(rotationT, stack.top());
		}
		TEST_METHOD(TransformationStack_pushScale)
		{
			TransformationStack stack;
			stack.pushScale(scaleV);

			Assert::AreEqual(scaleT, stack.top());
		}
		TEST_METHOD(TransformationStack_push)
		{
			TransformationStack stack;
			stack.push(positionT);
			stack.push(rotationT);
			stack.push(scaleT);

			Assert::AreEqual(positionT * rotationT * scaleT, stack.top());
		}
		TEST_METHOD(TransformationStack_pop)
		{
			TransformationStack stack;
			stack.push(positionT);
			stack.push(rotationT);
			stack.push(scaleT);
			stack.pop();

			Assert::AreEqual(positionT * rotationT, stack.top());
		}
		TEST_METHOD(TransformationStack_popLast)
		{
			TransformationStack stack;
			stack.pop();

			Assert::AreEqual(glm::mat4{ 1.0f }, stack.top());
		}
	};
}