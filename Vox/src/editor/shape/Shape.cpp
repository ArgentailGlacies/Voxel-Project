
#include "Shape.h"

#include "core/Resources.h"
#include "scene/Scene.h"

vox::Shape::Shape(core::Scene & scene, ShapeMesher mesher)
	: m_scene(scene), m_mesher(mesher)
{
	m_renderer = m_scene.createRender(core::res::asset::WIREFRAME_SHADER_PROGRAM, "", [this]() { render(); }, core::Scene::DEFAULT_ENTRY);
}
vox::Shape::~Shape()
{
	m_scene.detach(core::Scene::DEFAULT_ENTRY, m_renderer);
}

void vox::Shape::render() const
{
	if (m_visible && m_mesh)
		m_mesh->render(core::RenderMode::LINES);
}