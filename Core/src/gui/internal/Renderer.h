#pragma once

#include <glm/vec2.hpp>

namespace core
{
	struct Widget;

	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		/**
			Invoked every frame to render the given widget. Certain handlers may require a more
			complicated rendering routine that others.

			@param widget The widget which is to be rendered.
		*/
		virtual void render(const Widget & widget, const glm::vec2 & offset) const = 0;
	};
}