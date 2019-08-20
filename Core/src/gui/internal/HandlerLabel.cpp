
#include "HandlerLabel.h"

#include "event/EventBus.h"
#include "event/Events.h"
#include "gui/Widget.h"

#include <allegro5/allegro.h>

core::HandlerLabel::HandlerLabel(const Callback & callback, EventBus & bus, Widget & widget)
	: m_callback(callback)
{
	m_unichar = bus.add<KeyUnichar>(0, [this, &widget](auto & event)
	{
		if (m_active && event.consume())
		{
			if (event.m_codepoint == '\b')
				pop(widget);
			else if (event.m_codepoint == '\r')
				push(widget, '\n');
			else
				push(widget, event.m_codepoint);
			m_callback(widget);
		}
	});
	m_keyPress = bus.add<KeyPress>(0, [this, &widget](auto & event)
	{
		if (event.m_key == KeyboardKey::ESCAPE)
			m_active = false;
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

	ALLEGRO_USTR_INFO info;
	const ALLEGRO_USTR * text = al_ref_cstr(&info, widget.m_value.m_string.c_str());
	text = al_ref_ustr(&info, text, 0, info.slen - 1);
	
	widget.m_value.m_string = std::string{ al_cstr(text), al_ustr_size(text) };
}
