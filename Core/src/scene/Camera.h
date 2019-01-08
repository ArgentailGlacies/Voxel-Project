#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace core
{
	class Display;
	
	class Camera
	{
	public:
		Camera() = delete;
		Camera(const Display & display) : m_display(display) {}
		Camera(const Camera &) = delete;
		Camera(Camera &&) = delete;
		~Camera() = default;

		Camera & operator=(const Camera &) = delete;
		Camera & operator=(Camera &&) = delete;

		void process();

		glm::mat4 calculateProjectionMatrix(float pt) const;
		glm::mat4 calculateViewMatrix(float pt) const;

		/**
			Turns the camera such that it faces towards the specified location.

			@param target The location the camera should face towards.
			@param snap Whether the camera should instantly perform the action or not.
		*/
		void lookTowards(const glm::vec3 & target, bool snap = false);
		/**
			Sets the current position of the camera.

			@param pos The position the camera should be moved to.
			@param snap Whether the camera should instantly perform the action or not.
		*/
		void setPosition(const glm::vec3 & pos, bool snap = false);
		/**
			Sets the current rotation of the camera.

			@param pos The rotation the camera should be rotated to.
			@param snap Whether the camera should instantly perform the action or not.
		*/
		void setRotation(const glm::vec2 & rot, bool snap = false);
		/**
			Sets the current rotation of the camera.

			@param yaw The yaw component of the rotation the camera should be rotated to.
			@param pitch The pitch component of the rotation the camera should be rotated to.
			@param snap Whether the camera should instantly perform the action or not.
		*/
		void setRotation(float yaw, float pitch, bool snap = false);
		/**
			Sets the vector the camera classifies as upwards.

			@param up The new direction to be considered up.
			@param snap Whether the camera should instantly perform the action or not.
		*/
		void setUpVector(const glm::vec3 & up, bool snap = false);
		/**
			Sets the current field of view of the camera. A larger field of view means the camera
			can see more of the world at any given time.

			@param fov The new field of view.
			@param snap Whether the camera should instantly perform the action or not.
		*/
		void setFieldOfView(float fov, bool snap = false);
		/**
			Sets the view distance for the camera. Anything closer than znear or further away than
			zfar will not be rendered. The ratio zfar/znear should not be too large, as this may
			cause rendering issues.

			@param znear The near clipping plane for the camera.
			@param zfar The far clipping plane for the camera.
		*/
		void setViewDistance(float znear, float zfar);

		/**
			@return The current position of the camera.
		*/
		inline auto getPosition() const { return m_pos; }
		/**
			@return The current rotation of the camera.
		*/
		inline auto getRotation() const { return m_rot; }
		/**
			@return The direction the camera considers to be up.
		*/
		inline auto getUpVector() const { return m_up; }
		/**
			@return The current field of view of the camera.
		*/
		inline auto getFieldOfView() const { return m_fov; }

	private:
		const Display & m_display;

		glm::vec3 m_pos{ 0.0f, 0.0f, 0.0f }, m_oldPos{ m_pos };
		glm::vec2 m_rot{ 0.0f, 0.0f }, m_oldRot{ m_rot };
		glm::vec3 m_up{ 0.0f, 0.0f, 1.0f }, m_oldUp{ m_up };

		float m_fov = 70.0f, m_oldFov = m_fov;
		float m_znear = 0.1f;
		float m_zfar = 15000.0f;
	};
}