
#include "EditorWorld.h"

#include "event/EventBus.h"
#include "event/EventListener.h"
#include "event/Events.h"
#include "editor/shape/ShapeMesher.h"
#include "editor/util/CameraHandler.h"
#include "editor/util/Cursor.h"
#include "editor/util/Grid.h"
#include "scene/Scene.h"

class vox::EditorWorld::Impl
{
public:
	Impl(core::Scene & scene, core::EventBus & bus);

	void setShape(Shape * shape);
	inline Shape * getShape() const { return m_shape; }

private:
	core::Camera & m_camera;
	core::Listener m_mouseMove;

	CameraHandlerOrbital m_handler;
	Cursor m_cursor;
	Grid m_grid;

	Shape * m_shape = nullptr;
};

vox::EditorWorld::Impl::Impl(core::Scene & scene, core::EventBus & bus) : 
	m_camera(scene.getCamera(core::Scene::DEFAULT_CAMERA)),
	m_handler(m_camera, bus),
	m_grid(scene)
{
	m_mouseMove = bus.add<core::MouseMove>([this](auto & event) {});
}

void vox::EditorWorld::Impl::setShape(Shape * shape)
{
	if (m_shape != nullptr)
		m_shape->setVisible(false);

	m_shape = shape;

	if (m_shape != nullptr)
		m_shape->setVisible(true);
}

// ...

vox::EditorWorld::EditorWorld(core::Scene & scene, core::EventBus & bus) :
	m_impl(std::make_unique<Impl>(scene, bus)),
	m_shapePoint(scene),
	m_shapeRectangle(scene)
{}
vox::EditorWorld::~EditorWorld() = default;

void vox::EditorWorld::setShape(Shape * shape) { m_impl->setShape(shape); }
vox::Shape * vox::EditorWorld::getShape() { return m_impl->getShape(); }
