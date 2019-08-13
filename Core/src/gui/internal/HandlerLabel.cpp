
#include "HandlerLabel.h"

#include "event/EventBus.h"
#include "event/Events.h"
#include "gui/Widget.h"

#include <allegro5/allegro.h>

core::HandlerLabel::HandlerLabel(const Script & script, const std::string & code, EventBus & bus, Widget & widget)
	: m_script(script), m_code(code)
{
	m_unichar = bus.add<KeyUnichar>([this, &widget](auto & event)
	{
		if (m_active)
			push(widget, event.m_codepoint);
	});
	m_keyPress = bus.add<KeyPress>([this, &widget](auto & event)
	{
		if (m_active)
		{
			if (event.m_key == KeyboardKey::BACKSPACE)
				pop(widget);
			else if (event.m_key == KeyboardKey::ESCAPE)
				m_active = false;
		}
	});
	m_mousePress = bus.add<MousePress>([this](auto & event) { m_active = false; });
}

void core::HandlerLabel::push(Widget & widget, int codepoint)
{
	ALLEGRO_USTR_INFO info;
	ALLEGRO_USTR * text = al_ustr_dup(al_ref_cstr(&info, widget.m_value.m_string.c_str()));
	al_ustr_append_chr(text, codepoint);

	widget.m_value.m_string = al_cstr(text);

	al_ustr_free(text);
}
void core::HandlerLabel::pop(Widget & widget)
{
	if (widget.m_value.m_string.empty())
		return;

	widget.m_value.m_string.pop_back();
}
