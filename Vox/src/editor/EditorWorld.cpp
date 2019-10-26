
#include "EditorWorld.h"

#include "editor/shape/ShapeMesher.h"
#include "scene/Scene.h"

vox::EditorWorld::EditorWorld(core::Scene & scene, core::EventBus & bus) :
	m_handler(scene.getCamera(core::Scene::DEFAULT_CAMERA), bus),
	m_grid(scene),
	m_shapePoint(scene),
	m_shapeRectangle(scene)
{}

void vox::EditorWorld::setShape(Shape * shape)
{
	if (m_shape != nullptr)
		m_shape->setVisible(false);

	m_shape = shape;

	if (m_shape != nullptr)
		m_shape->setVisible(true);
}
