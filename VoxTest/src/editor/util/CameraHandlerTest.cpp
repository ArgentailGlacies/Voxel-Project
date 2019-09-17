
#include "editor/util/CameraHandler.h"

#include "event/EventBus.h"
#include "event/Events.h"
#include "scene/Camera.h"

#include "Common.h"
#include "Context.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vox::editor
{
	TEST_CLASS(CameraHandlerTest)
	{
	public:
		TEST_METHOD(CameraHandlerOrbital_handleMovement)
		{
			CameraHandlerOrbital handler{ m_camera, m_bus };

			// Moving with a delta 1, 0 in primary mode moves the camera along the side vector
			m_camera.setPosition({});
			handler.handleMovement({ 1.0f, 0.0f });
			m_camera.process();

			Assert::AreEqual(0.0f, m_camera.getPosition().x, 0.001f);
			Assert::IsTrue(m_camera.getPosition().y > 0.0f);
			Assert::AreEqual(0.0f, m_camera.getPosition().z, 0.001f);

			// Moving with a delta 0, 1 in primary mode moves the camera along the up vector
			m_camera.setPosition({});
			handler.handleMovement({ 0.0f, 1.0f });
			m_camera.process();

			Assert::AreEqual(0.0f, m_camera.getPosition().x, 0.001f);
			Assert::AreEqual(0.0f, m_camera.getPosition().y, 0.001f);
			Assert::IsTrue(m_camera.getPosition().z > 0.0f);

			// ...

			simulateKeyPress(core::KeyboardKey::LCTRL);

			// Moving with a delta 1, 0 in secondary mode moves the camera along the side vector
			m_camera.setPosition({});
			handler.handleMovement({ 1.0f, 0.0f });
			m_camera.process();

			Assert::AreEqual(0.0f, m_camera.getPosition().x, 0.001f);
			Assert::IsTrue(m_camera.getPosition().y > 0.0f);
			Assert::AreEqual(0.0f, m_camera.getPosition().z, 0.001f);

			// Moving with a delta 0, 1 in secondary mode moves the camera along the forward vector
			m_camera.setPosition({});
			handler.handleMovement({ 0.0f, 1.0f });
			m_camera.process();

			Assert::IsTrue(m_camera.getPosition().x > 0.0f);
			Assert::AreEqual(0.0f, m_camera.getPosition().y, 0.001f);
			Assert::AreEqual(0.0f, m_camera.getPosition().z, 0.001f);
		}

		TEST_METHOD(CameraHandlerOrbital_handleRotation)
		{
			CameraHandlerOrbital handler{ m_camera, m_bus };
			simulateKeyPress(core::KeyboardKey::LCTRL);

			// Rotating with a delta 1, 0 in secondary mode rotates camera around in place
			m_camera.setPosition({});
			m_camera.setRotation({});
			handler.handleRotation({ 1.0f, 0.0f });
			m_camera.process();

			Assert::AreEqual({}, m_camera.getPosition());
			Assert::IsTrue(m_camera.getRotation().x > 0.0f);
			Assert::AreEqual(0.0f, m_camera.getRotation().y, 0.001f);

			// Rotating with a delta 0, 1 in secondary mode rotates camera around in place
			m_camera.setPosition({});
			m_camera.setRotation({});
			handler.handleRotation({ 0.0f, 1.0f });
			m_camera.process();

			Assert::AreEqual({}, m_camera.getPosition());
			Assert::AreEqual(0.0f, m_camera.getRotation().x, 0.001f);
			Assert::IsTrue(m_camera.getRotation().y > 0.0f);

			// ...

			simulateKeyRelease(core::KeyboardKey::LCTRL);

			// Rotating with a delta 1, 0 in standard mode rotates camera around center
			m_camera.setPosition({});
			m_camera.setRotation({});
			handler.handleRotation({ 1.0f, 0.0f });
			m_camera.process();

			Assert::AreNotEqual({}, m_camera.getPosition());
			Assert::IsTrue(m_camera.getRotation().x < 0.0f);
			Assert::AreEqual(0.0f, m_camera.getRotation().y, 0.001f);

			// Rotating with a delta 0, 1 in standard mode rotates camera around center
			m_camera.setPosition({});
			m_camera.setRotation({});
			handler.handleRotation({ 0.0f, 1.0f });
			m_camera.process();

			Assert::AreNotEqual({}, m_camera.getPosition());
			Assert::AreEqual(0.0f, m_camera.getRotation().x, 0.001f);
			Assert::IsTrue(m_camera.getRotation().y < 0.0f);
		}

		TEST_METHOD(CameraHandlerOrbital_handleDistance)
		{
			CameraHandlerOrbital handler{ m_camera, m_bus };

			// Zooming with a delta 1 moves camera closer to center
			m_camera.setPosition({});
			handler.handleDistance(1.0f);
			m_camera.process();

			Assert::IsTrue(m_camera.getPosition().x > 0.0f);
			Assert::AreEqual(0.0f, m_camera.getPosition().y, 0.001f);
			Assert::AreEqual(0.0f, m_camera.getPosition().z, 0.001f);

			// Zooming with a delta -1 moves camera away from center
			m_camera.setPosition({});
			handler.handleDistance(-1.0f);
			m_camera.process();

			Assert::IsTrue(m_camera.getPosition().x < 0.0f);
			Assert::AreEqual(0.0f, m_camera.getPosition().y, 0.001f);
			Assert::AreEqual(0.0f, m_camera.getPosition().z, 0.001f);
		}

		TEST_METHOD(CameraHandlerOrbital_handleMouseInput)
		{
			CameraHandlerOrbital handler{ m_camera, m_bus };

			// Moving mouse not holding mouse zooms
			simulateMouseMove({ 3.0f, 4.0f }, 1.0f);

			Assert::IsTrue(m_camera.getPosition().x > 0.0f);
			Assert::AreEqual(0.0f, m_camera.getPosition().y, 0.001f);
			Assert::AreEqual(0.0f, m_camera.getPosition().z, 0.001f);
			Assert::AreEqual({}, m_camera.getRotation());

			// ...

			simulateMousePress();

			// Moving mouse while holding mouse button rotates camera around center position
			m_camera.setPosition({});
			m_camera.setRotation({});
			simulateMouseMove({ 0.0f, 2.0f }, 1.0f);

			Assert::IsTrue(m_camera.getPosition().x > 0.0f);
			Assert::AreEqual(0.0f, m_camera.getPosition().y, 0.001f);
			Assert::IsTrue(m_camera.getPosition().z > 0.0f);
			Assert::AreEqual(0.0f, m_camera.getRotation().x, 0.001f);
			Assert::IsTrue(m_camera.getRotation().y < 0.0f);

			// Moving mouse while holding mouse button and primary moves camera
			m_camera.setPosition({});
			m_camera.setRotation({});
			simulateKeyPress(core::KeyboardKey::LSHIFT);
			simulateMouseMove({ -1.0f, 2.0f }, 1.0f);
			simulateKeyRelease(core::KeyboardKey::LSHIFT);

			Assert::AreEqual(0.0f, m_camera.getPosition().x, 0.001f);
			Assert::IsTrue(m_camera.getPosition().y < 0.0f);
			Assert::IsTrue(m_camera.getPosition().z > 0.0f);
			Assert::AreEqual({}, m_camera.getRotation());

			// Moving mouse while holding mouse button and secondary rotates camera in place
			m_camera.setPosition({});
			m_camera.setRotation({});
			simulateKeyPress(core::KeyboardKey::LCTRL);
			simulateMouseMove({ -1.0f, 2.0f }, 1.0f);
			simulateKeyRelease(core::KeyboardKey::LCTRL);

			Assert::AreEqual({}, m_camera.getPosition());
			Assert::IsTrue(m_camera.getRotation().x < 0.0f);
			Assert::IsTrue(m_camera.getRotation().y > 0.0f);

			// Moving mouse while holding mouse button, primary and secondary moves camera
			m_camera.setPosition({});
			m_camera.setRotation({});
			simulateKeyPress(core::KeyboardKey::LSHIFT);
			simulateKeyPress(core::KeyboardKey::LCTRL);
			simulateMouseMove({ -1.0f, 2.0f }, 1.0f);
			simulateKeyRelease(core::KeyboardKey::LCTRL);
			simulateKeyRelease(core::KeyboardKey::LSHIFT);

			Assert::IsTrue(m_camera.getPosition().x > 0.0f);
			Assert::IsTrue(m_camera.getPosition().y < 0.0f);
			Assert::AreEqual(0.0f, m_camera.getPosition().z, 0.001f);
			Assert::AreEqual({}, m_camera.getRotation());
		}

	private:
		void simulateMouseMove(const glm::vec2 & delta, float scroll)
		{
			m_bus.post<core::MouseMove>({ {}, delta, {}, { scroll, 0.0f } });
		}
		void simulateMousePress() { m_bus.post<core::MousePress>({ core::MouseButton::MIDDLE, {}, {}, 0.0f }); }
		void simulateMouseRelease() { m_bus.post<core::MouseRelease>({ core::MouseButton::MIDDLE, {}, {}, 0.0f }); }
		void simulateKeyPress(core::KeyboardKey key) { m_bus.post<core::KeyPress>({ key, 0 }); }
		void simulateKeyRelease(core::KeyboardKey key) { m_bus.post<core::KeyRelease>({ key, 0 }); }

		// ...

		core::Camera m_camera{ display() };
		core::EventBus m_bus;
	};
}