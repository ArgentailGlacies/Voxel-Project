
#include "gui/WidgetHelper.h"

#include "gui/Widget.h"

bool core::gui::isLocked(const Widget & widget)
{
	if (widget.m_state.m_locked)
		return true;
	if (widget.m_family.m_parent != nullptr)
		return isLocked(*widget.m_family.m_parent);
	return false;
}
bool core::gui::isVisible(const Widget & widget)
{
	if (!widget.m_state.m_visible)
		return false;
	if (widget.m_family.m_parent != nullptr)
		return isVisible(*widget.m_family.m_parent);
	return true;
}
