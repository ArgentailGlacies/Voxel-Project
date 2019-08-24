
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
