#pragma once

namespace core
{
	struct Widget;

	namespace gui
	{
		/**
			Checks whether the given widget should be considered locked. It is locked it either it,
			or its parent, grandparents, etc., are locked.

			@param widget The widget to check if it is locked or not.
			@return true Iff the widget is considered locked.
		*/
		bool isLocked(const Widget & widget);
		/**
			Checks whether the given widget should be considered visible. It is visible it either
			it, and its parent, grandparents, etc., are all visible.

			@param widget The widget to check if it is visible or not.
			@return true Iff the widget is considered visible.
		*/
		bool isVisible(const Widget & widget);
	}
}