
#include "EditorWorld.h"

#include "event/EventBus.h"
#include "event/Events.h"
#include "editor/shape/ShapeMesher.h"
#include "scene/Scene.h"

vox::EditorWorld::EditorWorld(core::Scene & scene, core::EventBus & bus) :
	m_camera(scene.getCamera(core::Scene::DEFAULT_CAMERA)),
	m_handler(m_camera, bus),
	m_grid(scene),
	m_shapePoint(scene),
	m_shapeRectangle(scene)
{
	m_mouseMove = bus.add<core::MouseMove>([this](auto & event) {});
}

void vox::EditorWorld::setShape(Shape * shape)
{
	if (m_shape != nullptr)
		m_shape->setVisible(false);

	m_shape = shape;

	if (m_shape != nullptr)
		m_shape->setVisible(true);
}
