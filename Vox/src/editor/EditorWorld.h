#pragma once

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
	public:
		EditorWorld(core::Scene & scene, core::EventBus & bus);

		// ...

		inline void setCameraSensitivity(float sensitivity) { m_handler.setSensitivity(sensitivity); }

		inline void lockCursorAxis(util::Axis axis, bool locked) { m_cursor.lockAxis(axis, locked); }

		inline void setGridSize(int size, int spacing) { m_grid.setSize(size, spacing); }
		inline void setGridVisible(bool visible) { m_grid.setVisible(visible); }

	private:
		CameraHandlerOrbital m_handler;
		Cursor m_cursor;
		Grid m_grid;
	};
}