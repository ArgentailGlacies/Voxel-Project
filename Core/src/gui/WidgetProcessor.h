#pragma once

#include "event/EventListener.h"
#include "gui/GuiData.h"

#include <glm/vec2.hpp>

namespace core
{
	class EventBus;
	struct Widget;

	namespace gui
	{
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
		WidgetProcessorSlider(EventBus & bus, const gui::SliderData & data, bool horizontal) noexcept;

		void process(Widget & widget) const;

	private:
		Listener m_mouseMove;

		bool m_horizontal;
		gui::SliderData m_data;
		glm::vec2 m_mousePos = {};
	};
}