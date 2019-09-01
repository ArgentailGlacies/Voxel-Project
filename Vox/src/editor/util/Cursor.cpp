
#include "Cursor.h"

#include <plog/Log.h>

void vox::Cursor::setPosition(const glm::ivec3 & position)
{
	m_currentPosition = position;
}

glm::ivec3 vox::Cursor::getLockedPosition() const
{
	return m_lockedPosition ? m_clickedPosition : m_currentPosition;
}
glm::ivec3 vox::Cursor::getCurrentPosition() const
{
	return m_lockedPosition ? glm::ivec3{
		m_lockAxis.x ? m_clickedPosition.x : m_currentPosition.x,
		m_lockAxis.y ? m_clickedPosition.y : m_currentPosition.y,
		m_lockAxis.z ? m_clickedPosition.z : m_currentPosition.z
	} : m_currentPosition;
}

void vox::Cursor::lockAxis(util::Axis axis, bool locked)
{
	if (axis == util::Axis::X)
		m_lockAxis.x = locked;
	else if (axis == util::Axis::Y)
		m_lockAxis.y = locked;
	else if (axis == util::Axis::Z)
		m_lockAxis.z = locked;
	else
		LOG_WARNING << "Invalid axis " << static_cast<int>(axis);
}
void vox::Cursor::lockPosition(bool locked)
{
	m_lockedPosition = locked;
	m_clickedPosition = m_currentPosition;
}
