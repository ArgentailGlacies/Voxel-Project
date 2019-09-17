
#include "CameraHandler.h"

#include "event/EventBus.h"
#include "event/Events.h"
#include "scene/Camera.h"
#include "util/MathOperations.h"
#include "util/MathTrigonometry.h"

namespace
{
	constexpr float MOVE_FACTOR = 0.01f;
	constexpr float ZOOM_FACTOR = 0.01f;
	constexpr glm::vec2 ROT_FACTOR = { 0.15f, 0.1f };

	constexpr float PITCH_LIMIT = 89.9f;
	constexpr float DISTANCE_MIN_LIMIT = 5.0f;
	constexpr float DISTANCE_MAX_LIMIT = 1500.0f;
}

vox::CameraHandlerOrbital::CameraHandlerOrbital(core::Camera & camera, core::EventBus & bus)
	: CameraHandler(camera)
{
	m_mouseMove = bus.add<core::MouseMove>([this](auto & event)
	{
		if (m_holdMouse)
		{
			if (m_holdPrimary)
				handleMovement(event.m_positionDelta);
			else
				handleRotation(event.m_positionDelta);
		}
		else
			handleDistance(event.m_scrollDelta.x);
	});

	m_mousePress = bus.add<core::MousePress>([this](auto & event)
	{
		if (event.m_button == core::MouseButton::MIDDLE)
			m_holdMouse = true;
	});
	m_mousePress = bus.add<core::MouseRelease>([this](auto & event)
	{
		if (event.m_button == core::MouseButton::MIDDLE)
			m_holdMouse = false;
	});
	
	m_keyPress = bus.add<core::KeyPress>([this](auto & event)
	{
		if (event.m_key == core::KeyboardKey::LSHIFT)
			m_holdPrimary = true;
		else if (event.m_key == core::KeyboardKey::LCTRL)
			m_holdSecondary = true;
	});
	m_keyRelease = bus.add<core::KeyRelease>([this](auto & event)
	{
		if (event.m_key == core::KeyboardKey::LSHIFT)
			m_holdPrimary = false;
		else if (event.m_key == core::KeyboardKey::LCTRL)
			m_holdSecondary = false;
	});
}

void vox::CameraHandlerOrbital::handleMovement(const glm::vec2 & delta)
{
	const auto sensitivity = m_sensitivity * util::sqrt(m_distance) * MOVE_FACTOR;
	const auto rotation = m_camera.getRotation();

	auto position = m_camera.getPosition();
	if (m_holdSecondary)
	{
		position += sensitivity * delta.x * util::cartesian(rotation.x + 90.0f, 0.0f);
		position += sensitivity * delta.y * util::cartesian(rotation.x, rotation.y);
	}
	else
	{
		position += sensitivity * delta.x * util::cartesian(rotation.x + 90.0f, 0.0f);
		position += sensitivity * delta.y * util::cartesian(rotation.x, rotation.y + 90.0f);
	}
	m_camera.setPosition(position);
}
void vox::CameraHandlerOrbital::handleRotation(const glm::vec2 & delta)
{
	const auto sensitivity = m_sensitivity * ROT_FACTOR;

	auto position = m_camera.getPosition();
	auto rotation = m_camera.getRotation();
	if (m_holdSecondary)
		rotation += sensitivity * delta;
	else
	{
		rotation -= sensitivity * delta;
		position += m_distance * (util::cartesian(m_camera.getRotation()) - util::cartesian(rotation));
	}
	rotation.y = util::max(-PITCH_LIMIT, util::min(PITCH_LIMIT, rotation.y));
	m_camera.setPosition(position);
	m_camera.setRotation(rotation);
}
void vox::CameraHandlerOrbital::handleDistance(float delta)
{
	const auto sensitivity = m_sensitivity * util::sqrt(m_distance) * ZOOM_FACTOR;
	const auto rotation = m_camera.getRotation();

	auto distance = m_distance;
	auto position = m_camera.getPosition();
	distance = util::max(DISTANCE_MIN_LIMIT, util::min(DISTANCE_MAX_LIMIT, m_distance * (1.0f - delta * sensitivity)));
	position += (m_distance - distance) * util::cartesian(rotation);
	m_distance = distance;
	m_camera.setPosition(position);
}
