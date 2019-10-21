#pragma once

#include "editor/shape/Shape.h"
#include "editor/util/CameraHandler.h"
#include "editor/util/Cursor.h"
#include "editor/util/Grid.h"

namespace core
{
	class EventBus;
	class Scene;
}

namespace vox
{
	/**
		The world editor is responsible for orchestrating all components required by the user to
		alter the world in meaningful ways. This includes moving cursor to the correct location,
		processing events to perform tasks, handle camera movement, and so on.
	*/
	class EditorWorld
	{
		friend class ModuleWorldEditor;

	public:
		EditorWorld(core::Scene & scene, core::EventBus & bus);

	private:
		CameraHandlerOrbital m_handler;
		Cursor m_cursor;
		Grid m_grid;

		ShapePoint m_shapePoint;
		ShapeRectangle m_shapeRectangle;
	};
}