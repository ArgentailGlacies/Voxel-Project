
#include "Shape.h"

#include "core/Resources.h"
#include "scene/Scene.h"

#include <glm/gtc/matrix_transform.hpp>

vox::Shape::Shape(core::Scene & scene)
	: m_scene(scene)
{
	m_transform = m_scene.createTransformation(core::Scene::DEFAULT_ENTRY);
	m_renderer = m_scene.createRender(core::res::asset::WIREFRAME_SHADER_PROGRAM, "", [this]() { render(); }, m_transform);
	m_scene.setTranslucency(m_renderer, core::Translucency::TRANSPARENT);
}
vox::Shape::~Shape()
{
	m_scene.detach(core::Scene::DEFAULT_ENTRY, m_transform);
}

void vox::Shape::stretch(const glm::ivec3 & from, const glm::ivec3 & to)
{
	const auto center = (to + from) >> 1;
	m_mesh = mesh(from - center, to - center);
	m_scene.setTransformation(m_transform, glm::translate(glm::mat4{ 1.0f }, glm::vec3{ center }));
}

void vox::Shape::render() const
{
	if (m_visible && m_mesh)
		m_mesh->render(core::RenderMode::LINES);
}

// ...

vox::ShapePoint::ShapePoint(core::Scene & scene) : Shape(scene)
{}

vox::WorldQuery vox::ShapePoint::query(const glm::ivec3 & from, const glm::ivec3 & to, const Block & block) const
{
	return WorldQuery();
}
vox::ShapeMeshPtr vox::ShapePoint::mesh(const glm::ivec3 & from, const glm::ivec3 & to) const
{
	return ShapeMeshPtr();
}

vox::ShapeRectangle::ShapeRectangle(core::Scene & scene) : Shape(scene)
{}

vox::WorldQuery vox::ShapeRectangle::query(const glm::ivec3 & from, const glm::ivec3 & to, const Block & block) const
{
	return WorldQuery();
}
vox::ShapeMeshPtr vox::ShapeRectangle::mesh(const glm::ivec3 & from, const glm::ivec3 & to) const
{
	return ShapeMeshPtr();
}
