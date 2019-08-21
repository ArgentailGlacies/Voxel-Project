#pragma once

#include "event/EventListener.h"

#include <glm/vec2.hpp>

namespace core
{
	class Camera;
	class EventBus;
}

namespace vox
{
	class CameraHandler
	{
	public:
		CameraHandler(core::Camera & camera) : m_camera(camera) {}
		virtual ~CameraHandler() = default;

	protected:
		core::Camera & m_camera;
	};

	/**
		The orbital camera will be centered on a specific position in the world. The user will be
		able to orbit the camera around this point at a certain distance from said position. The
		distance to the center position is also controllable by the user.
	*/
	class CameraHandlerOrbital : public CameraHandler
	{
	public:
		CameraHandlerOrbital(core::Camera & camera, core::EventBus & bus);

		// ...

		/**
			Assigns the sensitivity of the camera controls. A higher sensitivity leads to faster
			camera movement. The sensitivity is considered a multiplier, where a value of 1 is the
			default, 2 doubles movement, and 0.5 halves movement.

			@param sensitivity The sensitivity of the camera handler.
		*/
		void setSensitivity(float sensitivity);

		/**
			Moves the camera along the axis of motion, depending on which mode is currently active.
			In primary mode, the camera will move along the camera's up and side vectors. In the
			secondary mode, along the forward and side vectors.

			@param delta The distance the camera should be moved along the camera vectors.
		*/
		void handleMovement(const glm::vec2 & delta);
		/**
			Rotates the camera in the yaw and pitch direction. In the primary mode, the camera will
			rotate around the center position, whereas in the secondary mode, the camera will rotate
			in place.

			@param delta The amount of rotation which should be applied to the camera.
		*/
		void handleRotation(const glm::vec2 & delta);
		/**
			Moves the camera further away or close to the central location. For a negative delta,
			the camera will be moved closer. A positive delta moves the camera further away.

			@param delta The amount of distance which should be applied to the camera.
		*/
		void handleDistance(float delta);

	private:
		core::Listener m_mouseMove;
		core::Listener m_mousePress;
		core::Listener m_mouseRelease;
		core::Listener m_keyPress;
		core::Listener m_keyRelease;

		float m_sensitivity = 1.0f;
		float m_distance = 25.0f;

		bool m_holdMouse = false;
		bool m_holdPrimary = false;
		bool m_holdSecondary = false;
	};
}