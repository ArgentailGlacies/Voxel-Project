#pragma once

#include "gui/internal/Handler.h"

#include <string>

namespace core
{
	class Script;

	/**
		Whenever a button is activated, it will update the widget state in the script and execute
		some code. Different buttons may override the action to perform other tasks.
	*/
	class HandlerButton : public Handler
	{
	public:
		HandlerButton(const Callback & callback) : m_callback(callback) {}

		virtual void action(Widget & widget) override final;
		virtual void process(Widget & widget) override final {}

	private:
		Callback m_callback;
	};

	/**
		Checkbox buttons can be toggled on or off. In the on state, the widget status in the script
		will be set to 'true', and 'false' in the off state.
	*/
	class HandlerButtonCheckbox : public Handler
	{
	public:
		HandlerButtonCheckbox(const Callback & callback) : m_callback(callback) {}

		virtual void action(Widget & widget) override final;
		virtual void process(Widget & widget) override final {}

	private:
		Callback m_callback;
	};

	/**
		Radio buttons can only be activated, but whenever a radio button is activated all other
		members of its group are deactivated.
	*/
	class HandlerButtonRadio : public Handler
	{
	public:
		HandlerButtonRadio(const Callback & callback) : m_callback(callback) {}

		virtual void action(Widget & widget) override final;
		virtual void process(Widget & widget) override final {}

	private:
		Callback m_callback;
	};
}