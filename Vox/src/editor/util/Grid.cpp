
#include "Grid.h"

#include "core/Resources.h"
#include "scene/Scene.h"

#include <glm/gtc/matrix_transform.hpp>

vox::Grid::Grid(core::Scene & scene)
	: m_scene(scene)
{
	setSize(50, 1);

	m_transform = m_scene.createTransformation(core::Scene::DEFAULT_ENTRY);
	m_renderer = m_scene.createRender(core::res::asset::WIREFRAME_SHADER_PROGRAM, "", [this]() { render(); }, m_transform);
	m_scene.setTranslucency(m_renderer, core::Translucency::TRANSPARENT);
}
vox::Grid::~Grid()
{
	m_scene.detach(m_transform, core::Scene::DEFAULT_ENTRY);
}

void vox::Grid::setPosition(const glm::vec3 & pos)
{
	m_scene.setTransformation(m_transform, glm::translate(glm::mat4{ 1.0f }, pos));
}

void vox::Grid::setSize(int size, int spacing)
{
	m_mesh = std::make_unique<core::Mesh<glm::vec3>>();
	m_mesh->addAttribute(core::Attribute{ 0, 3, 0, 12, core::DataFormat::FLOAT });

	int index = 0;
	for (int i = -size; i <= size; ++i)
	{
		m_mesh->vertices().push_back(glm::vec3{ i, -size, 0 });
		m_mesh->vertices().push_back(glm::vec3{ i, size, 0 });
		m_mesh->vertices().push_back(glm::vec3{ -size, i, 0 });
		m_mesh->vertices().push_back(glm::vec3{ size, i, 0 });

		for (int j = 0; j < 4; ++j)
			m_mesh->indices().push_back(index++);
	}

	m_mesh->build();
}

void vox::Grid::render() const
{
	if (m_visible && m_mesh)
		m_mesh->render(core::RenderMode::LINES);
}
