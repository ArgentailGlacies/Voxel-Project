#pragma once

#include "event/EventListener.h"

namespace core
{
	class EventBus;
	struct Widget;

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

	// ...

	/**
		The slider must be able to detect when the user is attempting to drag the glider around.
		When the user is dragging the glider, the slider must update the widget's state to reflect
		the current values.
	*/
	class WidgetProcessorSlider
	{
	public:
		/**
			The slider's valid values are determined fully be the slider's data structure. The value
			must be within the range [min, max], where the glider is considered in the middle at the
			value 'center'. The value must be a multiple of the step size.
		*/
		struct Data
		{
			float m_min = 0.0f;
			float m_max = 1.0f;
			float m_center = 0.5f;
			float m_step = 0.1f;
		};

		WidgetProcessorSlider(EventBus & bus, const Data & data, bool horizontal);

		void operator()(Widget & widget) const;

	private:
		Listener m_mouseMove;
		Data m_data;
		bool m_horizontal;
	};
}