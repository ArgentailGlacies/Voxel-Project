#pragma once

#include "event/EventListener.h"
#include "gui/internal/Handler.h"

#include <glm/vec2.hpp>
#include <string>

namespace core
{
	class EventBus;

	/**
		Sliders are activated whenever the value they are storing is changed. This typically happens
		when the user is moving the glider or activating the 'increment' or 'decrement' buttons.
	*/
	class HandlerSlider : public Handler
	{
		friend class HandlerSliderBar;

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

		// ...

		HandlerSlider(const Callback & callback, const Data & data)  : m_callback(callback), m_data(data) {}

		virtual void action(Widget & widget) override final {}
		virtual void process(Widget & widget) override final {}

		// ...

		/**
			Generates an incrementer callback which can be applied to the incrementer button for the
			slider. Each invokation will increment the widget's value by one step.

			@param widget The widget the incrementer should operate on.
		*/
		Callback incrementer(Widget & widget);
		/**
			Generates a decrementer callback which can be applied to the incrementer button for the
			slider. Each invokation will decrement the widget's value by one step.

			@param widget The widget the decrementer should operate on.
		*/
		Callback decrementer(Widget & widget);

	private:
		Callback m_callback;
		Data m_data;
	};

	/**
		The slider bar handler is responsible allows the user to click and drag the bar to assign
		the actual value of the slider.
	*/
	class HandlerSliderBar : public Handler
	{
	public:
		HandlerSliderBar(HandlerSlider & root, EventBus & bus, bool horizontal);

		virtual void action(Widget & widget) override final {}
		virtual void process(Widget & widget) override final;

	private:
		HandlerSlider & m_root;
		Listener m_listener;
		bool m_horizontal;

		glm::vec2 m_mousePosition;
	};
}
