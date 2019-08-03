#pragma once

#include <string>

namespace core
{
	class Script;
	struct Widget;

	/**
		Whenever a button is activated, it will update the widget state in the script and execute
		some code. Different buttons may override the action to perform other tasks.
	*/
	class WidgetActionButton
	{
	public:
		WidgetActionButton() = delete;
		WidgetActionButton(const Script & script, const std::string & code) : m_script(script), m_code(code) {}

		virtual void operator()(Widget & widget);

	protected:
		const Script & m_script;
		const std::string m_code;
	};
	/**
		Checkbox buttons can be toggled on or off. In the on state, the widget status in the script
		will be set to 'true', and 'false' in the off state.
	*/
	class WidgetActionButtonCheckbox : public WidgetActionButton
	{
	public:
		WidgetActionButtonCheckbox(const Script & script, const std::string & code) : WidgetActionButton(script, code) {}

		virtual void operator()(Widget & widget) override final;
	};
	/**
		Radio buttons can only be activated, but whenever a radio button is activated all other
		members of its group are deactivated.
	*/
	class WidgetActionButtonRadio : public WidgetActionButton
	{
	public:
		WidgetActionButtonRadio(const Script & script, const std::string & code) : WidgetActionButton(script, code) {}

		virtual void operator()(Widget & widget) override final;
	};
}