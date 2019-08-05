#pragma once

#include <string>

namespace core
{
	class Script;
	struct Widget;

	/**
		Base class for all types of widget actions. Typically all actions will execute some script
		when activated.
	*/
	class WidgetAction
	{
	protected:
		WidgetAction(const Script & script, const std::string & code) : m_script(script), m_code(code) {}

		const Script & m_script;
		const std::string m_code;
	};

	// ...

	/**
		Whenever a button is activated, it will update the widget state in the script and execute
		some code. Different buttons may override the action to perform other tasks.
	*/
	class WidgetActionButton : private WidgetAction
	{
	public:
		WidgetActionButton(const Script & script, const std::string & code) : WidgetAction(script, code) {}

		void operator()(Widget & widget) const;
	};
	/**
		Checkbox buttons can be toggled on or off. In the on state, the widget status in the script
		will be set to 'true', and 'false' in the off state.
	*/
	class WidgetActionButtonCheckbox : private WidgetAction
	{
	public:
		WidgetActionButtonCheckbox(const Script & script, const std::string & code) : WidgetAction(script, code) {}

		void operator()(Widget & widget) const;
	};
	/**
		Radio buttons can only be activated, but whenever a radio button is activated all other
		members of its group are deactivated.
	*/
	class WidgetActionButtonRadio : private WidgetAction
	{
	public:
		WidgetActionButtonRadio(const Script & script, const std::string & code) : WidgetAction(script, code) {}

		void operator()(Widget & widget) const;
	};

	// ...

	/**
		Sliders are activated whenever the value they are storing is changed. This typically happens
		when the user is moving the glider or activating the 'increment' or 'decrement' buttons.
	*/
	class WidgetActionSlider : private WidgetAction
	{
	public:
		WidgetActionSlider(const Script & script, const std::string & code) : WidgetAction(script, code) {}

		void operator()(Widget & widget) const;
	};
}