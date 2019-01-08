
#include "Allegro.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <functional>
#include <plog/Log.h>

namespace
{
	bool install(const std::function<void()> & func, const std::string & succeeded)
	{
		func();
		LOG_INFO << succeeded;
		return false;
	}
	bool install(const std::function<bool()> & func, const std::string & succeeded, const std::string & failed)
	{
		if (func())
		{
			LOG_INFO << succeeded;
			return false;
		}
		else
		{
			LOG_FATAL << failed;
			return true;
		}
	}
}

core::Allegro::Allegro()
{
	LOG_INFO << "Initializing Allegro...";

	if (!al_init())
		LOG_FATAL << "Failed to initialize Allegro";
	else
	{
		m_errors |= install(al_install_joystick, "Initialized joystick", "Failed to initialize joystick");
		m_errors |= install(al_install_keyboard, "Initialized keyboard", "Failed to initialize keyboard");
		m_errors |= install(al_install_mouse, "Initialized mouse", "Failed to initialize mouse");
		m_errors |= install(al_install_audio, "Initialized audio", "Failed to initialize audio");
		m_errors |= install(al_init_acodec_addon, "Initialized audio codec addon", "Failed to initialize audio codec addon");
		m_errors |= install(al_init_image_addon, "Initialized image addon", "Failed to initialize image addon");
		m_errors |= install(al_init_font_addon, "Initialized font addon");
		m_errors |= install(al_init_ttf_addon, "Initialized truetype font addon", "Failed to initialize truetype font addon");
		m_initialized = true;
	}
}
core::Allegro::~Allegro()
{
	if (!m_initialized)
		return;
	LOG_INFO << "Shutting down Allegro...";

	al_shutdown_font_addon();
	al_shutdown_image_addon();
	al_shutdown_ttf_addon();
	al_uninstall_joystick();
	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_uninstall_audio();
	al_uninstall_system();
}
