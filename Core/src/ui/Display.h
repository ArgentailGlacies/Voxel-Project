#pragma once

struct ALLEGRO_DISPLAY;

#include <glm/vec2.hpp>

namespace core
{
	class Display
	{
	public:
		Display() = delete;
		Display(const glm::uvec2 & size, bool fullscreen);
		Display(const Display &) = delete;
		Display(Display &&) = delete;
		~Display();

		Display & operator=(const Display &) = delete;
		Display & operator=(Display &&) = delete;

		inline auto * handle() const { return m_handle; }

		// ...

		bool isFullscreen() const;

		glm::uvec2 getSize() const;
		float getAspectRatio() const;

	private:
		ALLEGRO_DISPLAY * m_handle = nullptr;
	};
}