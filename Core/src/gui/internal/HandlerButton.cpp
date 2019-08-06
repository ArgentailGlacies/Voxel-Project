
#include "HandlerButton.h"

#include "gui/Widget.h"
#include "script/Script.h"

void core::HandlerButton::action(Widget & widget)
{
	widget.m_value.m_bool = true;
	m_script.execute(m_code);
}

void core::HandlerButtonCheckbox::action(Widget & widget)
{
	widget.m_value.m_bool = !widget.m_value.m_bool;
	m_script.execute(m_code);
}

void core::HandlerButtonRadio::action(Widget & widget)
{
	if (widget.m_value.m_bool)
		return;

	for (auto & member : widget.m_group.m_leader->m_group.m_members)
		member->m_value.m_bool = false;
	widget.m_value.m_bool = true;
	m_script.execute(m_code);
}
