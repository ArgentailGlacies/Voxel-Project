#pragma once

#include "editor/util/Cursor.h"

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


	private:
		Cursor m_cursor;
	};
}