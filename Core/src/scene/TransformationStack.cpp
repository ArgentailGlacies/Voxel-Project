
#include "TransformationStack.h"

#include <glm/gtc/matrix_transform.hpp>

core::TransformationStack::TransformationStack()
{
	m_transformations.emplace(1.0f);
}

void core::TransformationStack::pushTranslation(const glm::vec3 & translation)
{
	push(glm::translate(glm::mat4{ 1.0f }, translation));
}
void core::TransformationStack::pushRotation(const glm::vec3 & rotation)
{
	glm::mat4 mat{ 1.0f };
	mat = glm::rotate(mat, rotation.x, glm::vec3{ 0.0f, 0.0f, 1.0f });
	mat = glm::rotate(mat, rotation.y, glm::vec3{ 0.0f, 1.0f, 0.0f });
	mat = glm::rotate(mat, rotation.z, glm::vec3{ 1.0f, 0.0f, 0.0f });
	push(mat);
}
void core::TransformationStack::pushScale(const glm::vec3 & scale)
{
	push(glm::scale(glm::mat4{ 1.0f }, scale));
}
void core::TransformationStack::push(const glm::mat4 & transformation)
{
	m_transformations.push(top() * transformation);
}
void core::TransformationStack::pop()
{
	if (m_transformations.size() == 1)
		return;
	m_transformations.pop();
}
