
#include "MainLoop.h"

#include <allegro5/allegro.h>

core::MainLoop::MainLoop(const Processor & processor, const Renderer & renderer)
	: m_processor(processor), m_renderer(renderer)
{}

void core::MainLoop::start(double ups, double fps)
{
	double t = 0.0;
	double dt = 1.0 / ups;

	double currentTime = al_get_time();
	double accumulator = 0.0;

	m_running = true;
	while (m_running)
	{
		const auto newTime = al_get_time();
		const auto deltaTime = newTime - currentTime;
		currentTime = newTime;
		accumulator += deltaTime;
	
		while (accumulator >= dt)
		{
			if (m_running)
				m_processor(t, dt);
			accumulator -= dt;
			t += dt;
		}

		const float alpha = accumulator / dt;
		// const auto renderState = previousState * alpha + currentState * (1.0 - alpha);
		m_renderer(alpha);
	}
}
void core::MainLoop::stop()
{
	m_running = false;
}
