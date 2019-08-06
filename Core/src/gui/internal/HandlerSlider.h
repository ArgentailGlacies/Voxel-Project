#pragma once

#include "event/EventListener.h"
#include "gui/internal/Handler.h"

#include <glm/vec2.hpp>
#include <string>

namespace core
{
	class EventBus;
	class Script;

	/**
		Sliders are activated whenever the value they are storing is changed. This typically happens
		when the user is moving the glider or activating the 'increment' or 'decrement' buttons.
	*/
	class HandlerSlider : public Handler
	{
		friend class HandlerSliderButton;
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

		HandlerSlider(const Script & script, const std::string & code, const Data & data) 
			: m_script(script), m_code(code), m_data(data)
		{}

		virtual void action(Widget & widget) override final;
		virtual void process(Widget & widget) override final {}

	private:
		const Script & m_script;
		std::string m_code;

		Data m_data;
	};

	/**
		The slider button handler is responsible for changing the slider value by a single step in
		the relevant direction.
	*/
	class HandlerSliderButton : public Handler
	{
	public:
		HandlerSliderButton(HandlerSlider & root, bool increment) : m_root(root), m_increment(increment) {}

		virtual void action(Widget & widget) override final;
		virtual void process(Widget & widget) override final {}

	private:
		HandlerSlider & m_root;
		bool m_increment;
	};

	/**
		The slider button handler is responsible for changing the slider value by a single step in
		the relevant direction.
	*/
	class HandlerSliderBar : public Handler
	{
	public:
		HandlerSliderBar(HandlerSlider & root, EventBus & bus);

		virtual void action(Widget & widget) override final {}
		virtual void process(Widget & widget) override final;

	private:
		HandlerSlider & m_root;
		Listener m_listener;

		glm::vec2 m_mousePosition;
	};
}
