
#include "gui/WidgetAction.h"

#include "gui/Widget.h"
#include "script/Script.h"

void core::WidgetActionButton::operator()(Widget & widget)
{
	widget.m_value.m_bool = true;
	m_script.execute(m_code);
}
void core::WidgetActionButtonCheckbox::operator()(Widget & widget)
{
	widget.m_value.m_bool = !widget.m_value.m_bool;
	m_script.execute(m_code);
}
void core::WidgetActionButtonRadio::operator()(Widget & widget)
{
	if (widget.m_value.m_bool)
		return;

	for (auto & member : widget.m_group.m_leader->m_group.m_members)
		member->m_value.m_bool = false;
	widget.m_value.m_bool = true;
	m_script.execute(m_code);
}