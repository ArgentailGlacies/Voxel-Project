#pragma once

#include "event/EventListener.h"
#include "editor/shape/Shape.h"
#include "editor/util/CameraHandler.h"
#include "editor/util/Cursor.h"
#include "editor/util/Grid.h"

namespace core
{
	class Camera;
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

		/**
			Assigns the currently active shape in the editor. If nullptr is passed in, the currently
			selected shape is deselected. Will also hide the previous tool and show the new, if
			applicable.

			@param shape The shape which should be selected, or nullptr to deselect all shapes.
		*/
		void setShape(Shape * shape);

		/**
			@return The current shape of the editor, or nullptr if no shape has been selected.
		*/
		inline Shape * getShape() { return m_shape; }

	private:
		core::Camera & m_camera;
		core::Listener m_mouseMove;

		CameraHandlerOrbital m_handler;
		Cursor m_cursor;
		Grid m_grid;

		ShapePoint m_shapePoint;
		ShapeRectangle m_shapeRectangle;
		Shape * m_shape = nullptr;
	};
}