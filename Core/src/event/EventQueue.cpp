
#include "EventQueue.h"

#include "event/EventBus.h"
#include "event/Events.h"
#include "ui/Mouse.h"

#include <allegro5/display.h>
#include <allegro5/events.h>
#include <plog/Log.h>

namespace
{
	auto allegroMouseButtonToEnum(unsigned int button)
	{
		static const core::MouseButton ALLEGRO_TO_ENUM[] =
		{
			core::MouseButton::NONE,
			core::MouseButton::LEFT,
			core::MouseButton::RIGHT,
			core::MouseButton::MIDDLE,
			core::MouseButton::MIDDLE
		};
		return button < 5u ? ALLEGRO_TO_ENUM[button] : core::MouseButton::NONE;
	}
}

core::EventQueue::EventQueue(const EventBus & bus)
	: m_bus(bus), m_handle(al_create_event_queue())
{}
core::EventQueue::~EventQueue()
{
	al_destroy_event_queue(m_handle);
}

void core::EventQueue::add(gsl::not_null<ALLEGRO_EVENT_SOURCE*> source)
{
	al_register_event_source(m_handle, source);
}

void core::EventQueue::process() const
{
	ALLEGRO_EVENT event;
	while (al_get_next_event(m_handle, &event))
	{
		switch (event.type)
		{
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			m_bus.post(DisplayClose{});
			break;
		case ALLEGRO_EVENT_DISPLAY_RESIZE:
			al_acknowledge_resize(event.display.source);
			m_bus.post(DisplayResize{ { event.display.width, event.display.height } });
			break;
		case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
			m_bus.post(DisplaySwitchIn{});
			break;
		case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
			m_bus.post(DisplaySwitchOut{});
			break;

			// TODO: Handle joystick events
		case ALLEGRO_EVENT_JOYSTICK_AXIS:
			break;
		case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
			break;
		case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
			break;
		case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION:
			break;

		case ALLEGRO_EVENT_KEY_DOWN:
			m_bus.post(KeyPress{ event.keyboard.keycode, event.keyboard.modifiers });
			break;
		case ALLEGRO_EVENT_KEY_UP:
			m_bus.post(KeyRelease{ event.keyboard.keycode, event.keyboard.modifiers });
			break;
		case ALLEGRO_EVENT_KEY_CHAR:
			m_bus.post(KeyUnichar{ event.keyboard.unichar });
			break;

		case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
			m_bus.post(MouseEnterDisplay{});
			break;
		case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
			m_bus.post(MouseLeaveDisplay{});
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			m_bus.post(MousePress{
				allegroMouseButtonToEnum(event.mouse.button),
				{ event.mouse.x, event.mouse.y },
				{ event.mouse.z, event.mouse.w },
				event.mouse.pressure }
			);
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			m_bus.post(MouseRelease{
				allegroMouseButtonToEnum(event.mouse.button),
				{ event.mouse.x, event.mouse.y },
				{ event.mouse.z, event.mouse.w },
				event.mouse.pressure }
			);
			break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			m_bus.post(MouseMove{
				{ event.mouse.x, event.mouse.y }, { event.mouse.dx, event.mouse.dy },
				{ event.mouse.z, event.mouse.w }, { event.mouse.dz, event.mouse.dw }
			});
			break;
		case ALLEGRO_EVENT_MOUSE_WARPED:
			// TODO: Fire a mouse move event when the mouse is set to a certain position such that
			// the delta position is zero? Otherwise there might be problems with event listeners
			// that tracks the mouse movement, such as a camera handler
			break;

			// TODO: Handle timer events
		case ALLEGRO_EVENT_TIMER:
			break;

		default:
			LOG_WARNING << "Unhandled Allegro event type detected: " << event.type;
		}
	}
}
