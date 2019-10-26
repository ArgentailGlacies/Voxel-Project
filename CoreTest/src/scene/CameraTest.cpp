
#include "scene/Camera.h"
#include "ui/Display.h"
#include "util/MathOperations.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Common.h"
#include "Context.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace core::scene
{
	TEST_CLASS(CameraTest)
	{
	public:
		TEST_METHOD(Camera_calculateMatrices)
		{
			Camera camera{ display() };
			camera.setFieldOfView(70.0f, true);
			camera.setPosition({ 3.0f, 1.0f, 4.0f }, true);
			camera.setRotation({ 0.0f, 0.0f }, true);

			const auto view = glm::lookAt(glm::vec3{ 3, 1, 4 }, { 4, 1, 4 }, { 0, 0, 1 });
			const auto proj = glm::perspective(glm::radians(70.0f), display().getAspectRatio(), 0.1f, 15000.0f);
			Assert::AreEqual(view, camera.calculateViewMatrix(0.0f));
			Assert::AreEqual(proj, camera.calculateProjectionMatrix(0.0f));
		}

		TEST_METHOD(Camera_setPosition)
		{
			Camera camera{ display() };
			camera.setPosition({ 3.0f, 1.0f, 4.0f });

			Assert::AreEqual({ 3.0f, 1.0f, 4.0f }, camera.getPosition());
		}
		TEST_METHOD(Camera_setRotation)
		{
			Camera camera{ display() };
			camera.setRotation(60.0f, 24.0f);

			Assert::AreEqual({ 60.0f, 24.0f }, camera.getRotation());
		}
		TEST_METHOD(Camera_setUpVector)
		{
			Camera camera{ display() };
			camera.setUpVector({ -0.6f, 0.6f, 0.2f });

			Assert::AreEqual({ -0.6f, 0.6f, 0.2f }, camera.getUpVector());
		}
		TEST_METHOD(Camera_setFOV)
		{
			Camera camera{ display() };
			camera.setFieldOfView(91.0f);

			Assert::AreEqual(91.0f, camera.getFieldOfView());
		}

		TEST_METHOD(Camera_lookTowards)
		{
			Camera cameraA{ display() };
			Camera cameraB{ display() };
			Camera cameraC{ display() };
			Camera cameraD{ display() };
			cameraD.setPosition({ 2.0f, 4.0f, 0.0f });

			cameraA.lookTowards({ 1.0f, 0.0f, 0.0f });
			cameraB.lookTowards({ 0.0f, 1.0f, 0.0f });
			cameraC.lookTowards({ 0.0f, 0.0f, 1.0f });
			cameraD.lookTowards({ -1.0f, 1.0f, 0.0f });

			Assert::AreEqual({ 0.0f, 0.0f }, cameraA.getRotation());
			Assert::AreEqual({ 90.0f, 0.0f }, cameraB.getRotation());
			Assert::AreEqual({ 0.0f, 90.0f }, cameraC.getRotation());
			Assert::AreEqual({ 225.0f, 0.0f }, cameraD.getRotation());
		}
	};
}