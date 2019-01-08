
#include "Raytrace.h"

#include "util/MathOperations.h"

#include <glm/geometric.hpp>

vox::Raytrace::Raytrace(const glm::ivec3 & start, const glm::ivec3 & end)
	: Raytrace(glm::vec3{ start } + 0.5f, glm::vec3{ end } + 0.5f)
{}
vox::Raytrace::Raytrace(const glm::vec3 & start, const glm::vec3 & end)
	: Raytrace(start, end - start, glm::length(end - start))
{}
vox::Raytrace::Raytrace(const glm::vec3 & start, const glm::vec3 & dir, float length)
	: m_pos(start), m_oldPos(start)
{
	m_dir = length * glm::normalize(dir);
	m_step = util::sign(m_dir);
	m_tMax = util::intbound(start, m_dir);
	m_tDelta = m_step / m_dir;

	for (unsigned int i = 0u; i < 3u; ++i)
	{
		if (!std::isfinite(m_tMax[i])) m_tMax[i] = 1.0f;
		if (!std::isfinite(m_tDelta[i])) m_tDelta[i] = 0.0f;
	}
}

bool vox::Raytrace::valid() const
{
	return m_tMax.x < 1.0f || m_tMax.y < 1.0f || m_tMax.z < 1.0f;
}
void vox::Raytrace::next()
{
	m_oldPos = m_pos;
	if (m_tMax.x < m_tMax.y && m_tMax.x < m_tMax.z)
	{
		m_pos.x += m_step.x;
		m_tMax.x += m_tDelta.x;
	}
	else if (m_tMax.y < m_tMax.z)
	{
		m_pos.y += m_step.y;
		m_tMax.y += m_tDelta.y;
	}
	else
	{
		m_pos.z += m_step.z;
		m_tMax.z += m_tDelta.z;
	}
}

glm::vec3 vox::Raytrace::getPos() const
{
	return m_pos;
}
glm::vec3 vox::Raytrace::getOldPos() const
{
	return m_oldPos;
}
glm::ivec3 vox::Raytrace::getBlockPos() const
{
	return util::floor(m_pos);
}
glm::ivec3 vox::Raytrace::getOldBlockPos() const
{
	return util::floor(m_oldPos);
}

// ...

vox::RaytraceBresenham::RaytraceBresenham(const glm::ivec3 & start, const glm::ivec3 & end)
	: m_pos(start), m_oldPos(start)
{
	m_step = util::sign(end - start);
	m_delta = util::abs(end - start);
	m_totalSteps =
		(m_delta.x > m_delta.y && m_delta.x > m_delta.z) ?
		m_delta.x :
		(m_delta.y > m_delta.z) ? m_delta.y : m_delta.z;
}
vox::RaytraceBresenham::RaytraceBresenham(const glm::vec3 & start, const glm::vec3 & end)
	: RaytraceBresenham(util::floor(start), util::floor(start))
{}
vox::RaytraceBresenham::RaytraceBresenham(const glm::vec3 & start, const glm::vec3 & dir, float length)
	: RaytraceBresenham(start, start + dir * length)
{}

bool vox::RaytraceBresenham::valid() const
{
	return m_currentStep <= m_totalSteps;
}
void vox::RaytraceBresenham::next()
{
	m_oldPos = m_pos;
	m_currentStep++;
	m_error += m_delta;
	for (unsigned int i = 0u; i < 3u; ++i)
	{
		if (2 * m_error[i] >= m_totalSteps)
		{
			m_pos[i] += m_step[i];
			m_error[i] -= m_totalSteps;
		}
	}
}

glm::ivec3 vox::RaytraceBresenham::getPos() const
{
	return m_pos;
}
glm::ivec3 vox::RaytraceBresenham::getOldPos() const
{
	return m_oldPos;
}