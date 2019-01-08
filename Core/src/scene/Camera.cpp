
#include "Camera.h"

#include "ui/Display.h"
#include "util/MathOperations.h"
#include "util/MathTrigonometry.h"

#include <glm/gtc/matrix_transform.hpp>

void core::Camera::process()
{
	m_oldUp = m_up;
	m_oldPos = m_pos;
	m_oldRot = m_rot;
	m_oldFov = m_fov;
}

glm::mat4 core::Camera::calculateProjectionMatrix(float pt) const
{
	const auto fov = util::lerp(m_oldFov, m_fov, pt);

	return glm::perspective(glm::radians(fov), m_display.getAspectRatio(), m_znear, m_zfar);
}
glm::mat4 core::Camera::calculateViewMatrix(float pt) const
{
	const auto pos = util::lerp(m_oldPos, m_pos, pt);
	const auto rot = util::lerp(m_oldRot, m_rot, pt);
	const auto up = util::lerp(m_oldUp, m_up, pt);

	return glm::lookAt(pos, pos + util::cartesian(rot), up);
}

void core::Camera::lookTowards(const glm::vec3 & target, bool snap)
{
	setRotation(util::spherical(target - m_pos), snap);
}
void core::Camera::setPosition(const glm::vec3 & pos, bool snap)
{
	m_oldPos = snap ? pos : m_oldPos;
	m_pos = pos;
}
void core::Camera::setRotation(const glm::vec2 & rot, bool snap)
{
	m_oldRot = snap ? rot : m_oldRot;
	m_rot = rot;
}
void core::Camera::setRotation(float yaw, float pitch, bool snap)
{
	setRotation({ yaw, pitch }, snap);
}
void core::Camera::setUpVector(const glm::vec3 & up, bool snap)
{
	m_oldUp = snap ? up : m_oldUp;
	m_up = up;
}
void core::Camera::setFieldOfView(float fov, bool snap)
{
	m_oldFov = snap ? fov : m_oldFov;
	m_fov = fov;
}
void core::Camera::setViewDistance(float znear, float zfar)
{
	m_znear = znear;
	m_zfar = zfar;
}