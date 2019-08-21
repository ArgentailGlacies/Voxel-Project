
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

			simulateKeyPress(core::KeyboardKey::LSHIFT);

			// Moving with a delta 0, 1 in secondary mode moves the camera along the side vector
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
			Assert::IsTrue(m_camera.getRotation().y < 0.0f);

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
			Assert::IsTrue(m_camera.getRotation().y > 0.0f);
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

	private:
		void simulateKeyPress(core::KeyboardKey key) { m_bus.post<core::KeyPress>({ key, 0 }); }
		void simulateKeyRelease(core::KeyboardKey key) { m_bus.post<core::KeyRelease>({ key, 0 }); }

		// ...

		core::Camera m_camera{ display() };
		core::EventBus m_bus;
	};
}