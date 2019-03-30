#pragma once

#include "gui/Widget.h"

namespace core
{
	namespace gui
	{
		/**
			Executes all processors registered for the children of the specified widget. The
			processors of the children will be executed in the exact same order as the processors
			are registered.
		*/
		void updateChildren(Widget & widget);

		/**
			Computes and assigns the new position of the widget. The position will be relative to
			the link target, if the link is the parent the widget will be inside the parent,
			otherwise it will be outside the link target.

			The borders of both widgets will be taken into consideration, such that the distance
			between the widgets is the largest of the borders.

			@param widget The widget which should have its position recalculated.
		*/
		void updatePosition(Widget & widget);
		/**
			Computes and assigns the new size of the widget. The size will always be computed to be
			the smallest possible value. The user may define a smallest possible value, otherwise
			the size will be the smallest possible size such that all child widgets fits inside it.

			@param widget The widget which should have its size recalculated.
		*/
		void updateSize(Widget & widget);
	}
}